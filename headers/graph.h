typedef struct graph{
  // conjunto de arestas
  int *edge_set;
  // tamanho do problema
  int n, m;
  int *restrictions;
  // Inicializa a matriz -> vetor de n^2 posições
  graph(){
    //A[i][j] = x[i + (j * # cols)]
    std::cin >> n;
    n++;
    edge_set = new int[n * n];
    std::cin >> m;
    restrictions = new int[2 * m];
  }

  // preenche a matriz
  fill_graph(){
    for(int i = 0; i < n * n; i++){
      std::cin >> edge_set[i];
    }
    for(int i = 0; i < 2 * m; i++){
      std::cin >> restrictions[i];
    }
  }

  // mostra a matriz
  show_graph(){
    for(int i = 0; i < n * n; i++){
      std::cout << edge_set[i] << " " ;
      if((i + 1) % n  == 0 && i > 0) std::cout << "\n";
    }
  }
}graph;
