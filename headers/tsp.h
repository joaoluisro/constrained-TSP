typedef int (* Boundval)(graph *G, int l, int *partial_solution);

int cost(graph *G, int *path);

int cost(graph *G, int *partial_path, int l);

void solve_tsp_restriction(graph *G,
                          int *path,
                          int l,
                          std::vector<int> possible,
                          int &best,
                          int optimal_path[]);

int MinCostBound(graph *G, int l, int *partial_solution);

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
