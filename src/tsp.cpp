#include <iostream>
#include <vector>
#include <string.h>
#include "..\headers\graph.h"
using namespace std;

typedef int (* Boundval)(int *x);

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

/*
void solve_tsp(graph *G,
              int *path,
              int l,
              vector<int> possible,
              int best,
              int optimal_path[]){
  for(int i = 0; i < possible.size(); i++){
    vector<int> tmp;
    tmp = possible;
    tmp.erase(tmp.begin() + i);
    path[l] = possible[i];
    solve_tsp(G, path, l+1, tmp, best, optimal_path);
  }
  if(l == G->n){
    int c = cost(G, path);
    if(c < best){
      best = c;
      for(int i = 0; i < G->n; i++){
        optimal_path[i] = path[i];
      }
    }
  }
}
*/


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


void sort(int *array){

}

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

  int k = 0;

  // cria um vector<int> temporario para armazenar as possibilidades
  // tmp guarda todos os nodos que ainda podem ser explorados fora o nodo atual
  vector<int> tmp;
  if(l == 0){
    tmp.push_back(0);
  }
  else if(l == 1){
    for(int i = 0; i < G->n ; i++){
      tmp.push_back(i);
    }
  }
  else{
    tmp = possible;
    tmp.erase(tmp.begin() + xlast);
  }

  // explora todas as possibilidades de sub-árvores
  for(int i = 0; i < tmp.size(); i++){

    bool restricted = false;
    int current = tmp[i];

    // verifica se o caminho é viável
    if(!is_feasible(G, path, l, current)){
      continue;
    }

    // atualiza o camminho para que o nodo atual esteja incluso no caminho escolhido
    path[l] = current;

    // calcula o valor do limite inferior
    choices[k] = current;
    bounds[k] = B(path);
    k++;
  }

  sort(choices);
  sort(bounds);

  for(int i = 0; i < k - 1; i++){
    if(bounds[i] >= best) return;
    path[l] = choices[i];
    solve_tsp_restriction(G, path, l+1, tmp, best, optimal_path, i);
  }
}


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
    for(int i = 1; i < G->n ; i++){
      tmp.push_back(i);
    }
  }
  else{
    tmp = possible;
    tmp.erase(tmp.begin() + xlast);
  }

  // explora todas as possibilidades de sub-árvores
  for(int i = 0; i < tmp.size(); i++){
    bool restricted = false;
    int current = tmp[i];

    // verifica se o caminho é viável
    if(!is_feasible(G, path, l, current)){
      continue;
    }

    // atualiza o camminho para que o nodo atual esteja incluso no caminho escolhido
    path[l] = current;
    solve_tsp_restriction_branch_and_bound_test(G, path, l+1, tmp, best, optimal_path, i);
  }
}
