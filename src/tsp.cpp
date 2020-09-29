#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <limits>
#include "..\headers\graph.h"
using namespace std;
#define INF 10000000;
typedef int (* Boundval)(graph *G, int l, int *partial_solution);

int cost(graph *G, int *path){
  int cost_f = 0;
  int index_i, index_j;

  // faz o somatório de cada trajeto
  for(int i = 0; i < G->n-1; i++){
    index_i = path[i];
    index_j = path[i + 1];
    cost_f += G->edge_set[index_j + (index_i * (G->n))];
  }

  // soma com o custo de voltar para a origem
  cost_f += G->edge_set[path[G->n - 1] * (G->n)];
  return cost_f;
}

int cost(graph *G, int *partial_path, int l){
  int cost_f = 0;
  int index_i, index_j;

  // faz o somatório de cada trajeto
  for(int i = 1; i < l; i++){
    index_i = partial_path[i- 1];
    index_j = partial_path[i];
    cost_f += G->edge_set[index_j + (index_i * (G->n))];
  }
  return cost_f;
}

bool is_feasible(graph *G, int *path, int pos, int current){
  bool restricted = false;
  int dependency;
  // procura um lugar que esteja na restrição
  for(int j = 1; j < G->m * 2; j += 2){
    if(current == G->restrictions[j]){
      restricted = true;
      dependency = G->restrictions[j-1];
    }
  }

  // se não achou, todos os caminhos podem ser explorados
  if (!restricted ){
    return true;
  }

  // se achou, verifica se já se passou pelo lugar dependente
  for(int i = 0; i < pos; i++){
    if(dependency == path[i]){
      return true;
    }
  }

  // a sub-árvore a ser explorada é inviável.
  return false;
}

int get_index(vector<int> array, int elm){
  for(int i = 0; i < array.size(); i++){
    if(array[i] == elm) return i;
  }
}

int MinCostBound(graph *G, int l, int *partial_solution){

  int MCB = 0;

  // primeira parte
  MCB += cost(G, partial_solution, l);

  if(l == 0 ) return MCB;
  // segunda parte
  vector<int> Y;
  for(int i = 1; i < G->n ; i++) Y.push_back(i);
  for(int i = 0; i < l; i++) Y.erase(Y.begin() + get_index(Y, partial_solution[i]) );

  int smallest = G->edge_set[(partial_solution[l - 1] * G->n) + Y[0]];
  for(int i = 1; i < l; i++){
    int current = G->edge_set[(partial_solution[l - 1] * G->n) + Y[i]];
    if(current < smallest) smallest = current;
  }
  MCB += smallest;

  // terceira parte
  Y.push_back(0);
  int sum = 0;
  for(int i = 0; i < Y.size(); i++){
    smallest = G->edge_set[(Y[i] * G->n) + Y[0]];
    for(int j = 1; j < Y.size(); j++){
      int current = G->edge_set[(Y[i] * G->n) + Y[j]];
      if(current < smallest) smallest = current;
    }
    sum += smallest;
  }

  MCB += sum;
  return MCB;
}

int Reduce(int *M, int size){
  int val = 0;
  int min_val;


  for(int i = 0; i < size; i++){
    min_val = M[i * (size)];
    // acesso a matriz: M[i * (n) + j] = M[i][j]
    for(int j = 1; j < size; j++){
      if(M[(i * size) + j] < min_val && i != j && M[(i * size) + j] >= 0) min_val = M[(i * size) + j];
    }
    for(int j = 0; j < size; j++){
      M[(i * size) + j] -= min_val;
    }
    val += min_val;
  }

  for(int j = 0; j < size; j++){
    min_val = M[j];
    for(int i = 1; i < size; i++){
      if(M[(i * size) + j] < min_val && i != j && M[(i * size) + j] >= 0) min_val = M[(i * size) + j];
    }
    for(int i = 0; i < size; i++){
      M[(i * size) + j] -= min_val;
    }
    val += min_val;
  }

  return val;

}

int ReducedBound(graph *G, int l, int *partial_solution){


  int dim = G->n - l + 1;
  int *M = new int[dim * dim];
  M[0] = INF;
  int j = 1;

  vector<int> Y;
  for(int i = 0; i < G->n ; i++) Y.push_back(i);
  for(int i = 0; i < l; i++) Y.erase(Y.begin() + get_index(Y, partial_solution[i]));


  for(int i = 0; i < Y.size(); i++){
    M[j] = G->edge_set[(partial_solution[l] * G->n) + Y[i]];
    j++;
  }

  j = 1;
  for(int i = 0; i < Y.size(); i++){
    M[(j * dim)] = G->edge_set[(Y[i] * G->n)];
    j++;
  }

  int i = 1;
  for(int k = 0; k < Y.size(); k++){
    j = 1;
    for(int z = 0; z < Y.size(); z++){
      M[(i * dim) + j] = G->edge_set[(Y[k] * G->n) + Y[z]];
      j++;
    }
    i++;
  }


  int ans = Reduce(M, dim);
  ans += cost(G, partial_solution, l);
  return ans;
}

void sort_choice_and_bound(int *choices, int *bounds, int k){

  int min;
  int temp, aux, temp2;

  for(int i = 0; i < k; i++){
    min = bounds[i];
    aux = i;
    for(int j = i; j < k; j++){
      if(bounds[j] < min){
        min = bounds[j];
        aux = j;
      }
    }

    temp = bounds[i];
    bounds[i] = min;
    bounds[aux] = temp;

    temp2 = choices[i];
    choices[i] = choices[aux];
    choices[aux] = temp2;
  }
}

void solve_tsp_restriction_branch_and_bound(graph *G,
                                          int *path,
                                          int l,
                                          vector<int> possible,
                                          int &best,
                                          int optimal_path[],
                                          Boundval B,
                                          int &nodes){

  // verifica se o caminho chegou no tamanho máximo

  if(l == G->n){
    // calcula o custo total do caminho

    int c = cost(G, path);

    // se o caminho é melhor, atualize o melhor caminho
    if(best > c){
      best = c;
      for(int i = 0; i < G->n; i++){
        optimal_path[i] = path[i];
      }
    }
    return;
  }

  // cria um vector<int> temporario para armazenar as possibilidades
  // tmp guarda todos os nodos que ainda podem ser explorados fora o nodo atual

  vector<int> tmp;
  if(l == 0){
    tmp.push_back(0);
  } else if(l == 1){
    for(int i = 1; i < G->n ; i++) tmp.push_back(i);
  }
  else{
    tmp = possible;
    tmp.erase(tmp.begin() + get_index(tmp, path[l - 1]));
  }

  int k = 0;
  int *choices = new int[tmp.size()];
  int *bounds = new int[tmp.size()];
  // explora todas as possibilidades de sub-árvores
  for(int i = 0; i < tmp.size(); i++){

    int current = tmp[i];

    // atualiza o camminho para que o nodo atual esteja incluso no caminho escolhido
    path[l] = current;

    // calcula o valor do limite inferior e atualiza as possibilidades
    choices[k] = current;
    bounds[k] = B(G, l, path);
    k++;
  }

  // ordena os vetores de escolha e de bounds

  sort_choice_and_bound(choices, bounds, k);

  for(int i = 0; i < k; i++){
    // verifica se vale a pena seguir a sub-arvore
    if(bounds[i] >= best) return;

    // atualiza o caminho
    path[l] = choices[i];

    // verifica se o caminho é viável
    if(!is_feasible(G, path, l, choices[i])) continue;

    nodes++;

    solve_tsp_restriction_branch_and_bound(G, path, l+1, tmp, best, optimal_path, B, nodes);
  }

}
