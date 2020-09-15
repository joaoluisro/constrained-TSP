int cost(graph *G, int *path);

void solve_tsp(graph *G,
              int *path,
              int l,
              std::vector<int> possible,
              int best,
              int *optimal_path);

void solve_tsp_restriction(graph *G,
                          int *path,
                          int l,
                          std::vector<int> possible,
                          int &best,
                          int optimal_path[]);
