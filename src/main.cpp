#include <iostream>
#include <string.h>
#include <vector>
#include "..\headers\graph.h"
#include "..\headers\tsp.h"
using namespace std;

int main(){

  graph *G = new graph();
  G->fill_graph();

  // inicializa as variáveis "globais"
  int n = G->n;
  int *path = new int[n];
  vector<int> possible;
  for(int i = 1; i < n; i++){
    possible.push_back(i);
  }
  int *optimal_path = new int[n];
  int Optbest = 100000000;
  for(int i = 0; i < n; i++){
    path[i] = 0;
    optimal_path[i] = 1;
  }

  // invoca o solver

  solve_tsp_restriction(G, path, 1, possible, Optbest, optimal_path);

  // exibe a saída
  cout << cost(G, optimal_path) << "\n";
  for(int i = 1; i < n; i++){
    cout << optimal_path[i];
    if(i != n - 1) cout << "\n";
  }
}
