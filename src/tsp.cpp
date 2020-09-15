#include <iostream>
#include <vector>
#include <string.h>
#include "..\headers\graph.h"
using namespace std;

int cost(graph *G, int *path){
  int cost_f = 0;
  int index_i, index_j;
  for(int i = 0; i < G->n-1; i++){
    index_i = path[i];
    index_j = path[i + 1];
    cost_f += G->edge_set[index_j + (index_i * (G->n))];

    //cout << "\n" << index_i << " " << index_j << " -> " << G->edge_set[index_j + (index_i * (G->n))] << "\n";
  }

  cost_f += G->edge_set[path[G->n - 1] * (G->n)];

  cout << "\n";
  return cost_f;
}

int find_index(vector<int> vec, int elem){
  for(int i = 0; i < vec.size(); i ++){
    if(vec[i] == elem){
      return i;
    }
  }
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
  for(int j = 1; j < G->m * 2; j += 2){
    if(current == G->restrictions[j]){
      restricted = true;
      dependency = G->restrictions[j-1];
    }
  }
  if (!restricted ){
    return true;
  }
  for(int i = 0; i < pos; i++){
    if(dependency == path[i]){
      return true;
    }
  }

  return false;
}

void solve_tsp_restriction(graph *G,
                          int *path,
                          int l,
                          vector<int> possible,
                          int &best,
                          int optimal_path[]){

  for(int i = 0; i < possible.size(); i++){
    vector<int> tmp;
    bool restricted = false;

    int current = possible[i];

    if(!is_feasible(G, path, l, current)){
      continue;
    }

    tmp = possible;
    tmp.erase(tmp.begin() + i);
    path[l] = current;
    solve_tsp_restriction(G, path, l+1, tmp, best, optimal_path);
  }
  if(l == G->n){
    int c = cost(G, path);
    if(best > c){
      best = c;
      for(int i = 0; i < G->n; i++){
        optimal_path[i] = path[i];
      }
    }
  }
}
