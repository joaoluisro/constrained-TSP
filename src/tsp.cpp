#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include "..\headers\graph.h"
using namespace std;

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
  for(int i = 0; i < l; i++){
    index_i = partial_path[i];
    index_j = partial_path[i + 1];
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

bool is_feasible2(graph *G, int *path){
  bool feasible = true;
  for(int i = 1; i < G->n; i++){
    feasible = feasible && is_feasible(G, path, G->n, path[i]);
  }
  return feasible;
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
  for(int i = 1; i < l; i++) Y.erase(Y.begin() + get_index(Y, partial_solution[l]) );

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

int ReducedBound(int *x){

}

// funcionando
void solve_tsp_restriction(graph *G,
                          int *path,
                          int l,
                          vector<int> possible,
                          int &best,
                          int optimal_path[]){

  // explora todas as possibilidades de sub-árvores
  for(int i = 0; i < possible.size(); i++){
    vector<int> tmp;
    bool restricted = false;
    int current = possible[i];

    // verifica se o caminho é viável
    if(!is_feasible(G, path, l, current)){
      continue;
    }

    // cria um vector<int> temporario para armazenar as possibilidades
    // tmp guarda todos os nodos que ainda podem ser explorados fora o nodo atual
    tmp = possible;
    tmp.erase(tmp.begin() + i);

    // atualiza o camminho para que o nodo atual esteja incluso no caminho escolhido
    path[l] = current;

    // chama a recursão
    solve_tsp_restriction(G, path, l+1, tmp, best, optimal_path);
  }

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
  }
}

// unknown
void solve_tsp_restriction_branch_and_bound(graph *G,
                                          int *path,
                                          int l,
                                          vector<int> possible,
                                          int &best,
                                          int optimal_path[],
                                          int *choices,
                                          int *bounds,
                                          int xlast,
                                          Boundval B){

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

  // explora todas as possibilidades de sub-árvores
  for(int i = 0; i < tmp.size(); i++){

    bool restricted = false;
    int current = tmp[i];

    // atualiza o camminho para que o nodo atual esteja incluso no caminho escolhido
    path[l] = current;

    // calcula o valor do limite inferior

    choices[k] = current;
    bounds[k] = B(G, l, path);
    k++;
  }

  sort(choices, choices + k);
  sort(bounds, bounds + k);

  for(int i = 0; i < k; i++){
    if(bounds[i] > best) return;
    path[l] = choices[i];
    // verifica se o caminho é viável
    if(!is_feasible(G, path, l, choices[i])) continue;
    solve_tsp_restriction_branch_and_bound(G, path, l+1, tmp, best, optimal_path, choices, bounds, i, B);
  }

}

// funcionando
void solve_tsp_restriction_branch_and_bound_test(graph *G,
                                          int *path,
                                          int l,
                                          vector<int> possible,
                                          int &best,
                                          int optimal_path[],
                                          int xlast){

  // verifica se o caminho chegou no tamanho máximo
  if(l == G->n){
    // calcula o custo total do caminho
    int c = cost(G, path);

    // se o caminho é melhor, atualize o melhor caminho
    if(best > c){
      best = c;
      for(int i = 0; i < G->n ; i++){
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

  // explora todas as possibilidades de sub-árvores
  for(int i = 0; i < tmp.size(); i++){
    bool restricted = false;
    int current = tmp[i];
    // atualiza o camminho para que o nodo atual esteja incluso no caminho escolhido
    path[l] = current;
    // verifica se o caminho é viável
    if(!is_feasible(G, path, l, current)) continue;

    solve_tsp_restriction_branch_and_bound_test(G, path, l+1, tmp, best, optimal_path, i);
  }
}
