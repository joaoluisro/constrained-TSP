#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include "..\headers\graph.h"
#include "..\headers\tsp.h"
using namespace std;

int main(){

  graph *G = new graph();
  G->fill_graph();

  // inicializa as variáveis "globais"
  int n = G->n;

  int *path = new int[n];
  int *optimal_path = new int[n];
  int *choices = new int[n];
  int *bounds = new int[n];

  vector<int> possible;

  int Optbest = 100000000;

  for(int i = 0; i < n; i++){
    path[i] = 0;
    optimal_path[i] = 1;
    choices[i] = 1;
    bounds[i] = 0;
  }


  solve_tsp_restriction_branch_and_bound(G, path, 0, possible, Optbest, optimal_path, choices, bounds, 0, MinCostBound);
  // invoca o solver
  cout << "------------------------------------" << "\n";
  for(int i = 0; i < G->n; i++){
    cout << optimal_path[i] << "\n";
  }
  for(int i = 0; i < n; i++){
    path[i] = 0;
    optimal_path[i] = 1;
    choices[i] = 0;
    bounds[i] = 0;
  }
  cout << "------------------------------------" << "\n";
  Optbest = 10000000;
  solve_tsp_restriction_branch_and_bound_test(G, path, 0, possible, Optbest, optimal_path, 0);

  for(int i = 0; i < G->n; i++){
    cout << optimal_path[i] << "\n";
  }
}
