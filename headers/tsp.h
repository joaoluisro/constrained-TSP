typedef int (* Boundval)(graph *G, int l, int *partial_solution);

int cost(graph *G, int *path);

int cost(graph *G, int *partial_path, int l);

int MinCostBound(graph *G, int l, int *partial_solution);

int Reduce(int *M, int size);

int ReducedBound(graph *G, int l, int *partial_solution);

int get_index(std::vector<int> array, int elm);

void solve_tsp_restriction_branch_and_bound(graph *G,
                                            int *path,
                                            int l,
                                            std::vector<int> possible,
                                            int &best,
                                            int optimal_path[],
                                            Boundval B,
                                            int &nodes);
