#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <limits>
#include "..\headers\graph.h"
#include "..\headers\tsp.h"
using namespace std;
using namespace std::chrono;
#define INF 10000000;
int main(int argc, char **argv){

  graph *G = new graph();
  G->fill_graph();

  // inicializa as variáveis "globais"
  int n = G->n;

  int *path = new int[n];
  int *optimal_path = new int[n];
  int *choices = new int[n];
  int *bounds = new int[n];
  vector<int> possible;

  // preenche com zeros/INF
  int Optbest = INF;
  for(int i = 0; i < n; i++){
    path[i] = 0;
    optimal_path[i] = 1;
  }
  int nodes = 0;

  Boundval B = ReducedBound;
  if(argc > 1) B = MinCostBound;

  auto start = high_resolution_clock::now();
  solve_tsp_restriction_branch_and_bound(G, path, 0, possible, Optbest, optimal_path, B, nodes);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);

  cout << cost(G, optimal_path) << "\n";
  for(int i = 1; i < G->n; i++){
    cout << optimal_path[i] << "\n";
  }
  cout << nodes << " nodes \n";
  cout << duration.count()/1000 << " ms" << endl;
}
