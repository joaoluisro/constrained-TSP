typedef int (* Boundval)(int *x);

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


void sort(int * array);

void solve_tsp_restriction_branch_and_bound(graph *G,
                                            int *path,
                                            int l,
                                            std::vector<int> possible,
                                            int &best,
                                            int optimal_path[],
                                            int *choices,
                                            int *bounds,
                                            int xlast,
                                            Boundval B);

void solve_tsp_restriction_branch_and_bound_test(graph *G,
                                                int *path,
                                                int l,
                                                std::vector<int> possible,
                                                int &best,
                                                int optimal_path[],
                                                int xlast);
