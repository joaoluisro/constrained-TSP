#include <iostream>
#include <vector>
#include <string.h>
#include "..\headers\graph.h"
using namespace std;

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

  // não achou, a sub-árvore a ser explorada é inviável.
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
