#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include "..\headers\graph.h"
#include "..\headers\tsp.h"
using namespace std;
using namespace std::chrono;

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


  auto start = high_resolution_clock::now();
  solve_tsp_restriction_branch_and_bound(G, path, 0, possible, Optbest, optimal_path, choices, bounds, MinCostBound);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);

  cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
  cout << cost(G, optimal_path) << "\n";
  for(int i = 1; i < G->n; i++){
    cout << optimal_path[i] << "\n";
  }
}
