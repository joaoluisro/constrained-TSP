#include <iostream>
#include <string.h>
#include <vector>
#include "..\headers\graph.h"
#include "..\headers\tsp.h"
using namespace std;

int main(){

  graph *G = new graph();
  G->fill_graph();
  //G->show_graph();

  int n = G->n;
  int *path = new int[n];
  vector<int> possible{1,2,3,4};
  int *optimal_path = new int[n];
  int Optbest = 1000000;
  for(int i = 0; i < n; i++){
    path[i] = 0;
    optimal_path[i] = 1;
  }

  solve_tsp_restriction(G, path, 1, possible, Optbest, optimal_path);

  for(int  i = 0; i < n; i++){
    cout << optimal_path[i] << " ";
  }
  cout << "\n";
  cout << cost(G, optimal_path) << "\n";
}
