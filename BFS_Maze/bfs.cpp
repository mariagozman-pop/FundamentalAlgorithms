#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <stack>
#include <random>
#include <ctime>
#include <math.h>
#include "bfs.h"

using namespace std;

/*int get_neighbors(const Grid* grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    /*10 18
        1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
        1 0 0 0 1 0 1 0 0 1 0 0 0 1 0 1 0 1
        1 0 0 0 1 0 0 0 0 1 0 0 0 1 0 0 0 1
        1 0 0 0 1 1 0 0 1 1 0 0 0 1 1 0 0 1
        1 1 1 1 1 1 1 1 1 1 1 1 0 1 1 1 0 1
        1 0 1 0 1 0 0 0 1 1 0 1 0 1 0 0 0 1
        1 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1
        1 1 0 1 0 1 1 0 1 1 1 0 1 0 1 1 0 1
        1 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 1
        1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1

    int dx[] = { -1, 0, 1, 0 };
    int dy[] = { 0, 1, 0, -1 };

    int nr = 0;
    for (int i = 0; i < 4; i++)
    {
        int x = p.row + dx[i];
        int y = p.col + dy[i];
        if ((x < grid->rows && x >= 0) && (y < grid->cols && y >= 0) && grid->mat[x][y] != 1)
        {
            neighb[nr].row = x;
            neighb[nr].col = y;
            nr++;
        }
    }
    return nr;
}*/

int get_neighbors(const Grid* grid, Point p, Point neighb[])
{
    /*10 18
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1*/

    int nr = 0;

    if (p.row + 2 < grid->rows && p.col + 1 < grid->cols && grid->mat[p.row + 2][p.col + 1] != 1) {
        neighb[nr].row = p.row + 2;
        neighb[nr].col = p.col + 1;
        ++nr;
    }

    if (p.row + 2 < grid->rows && p.col - 1 >= 0 && grid->mat[p.row + 2][p.col - 1] != 1) {
        neighb[nr].row = p.row + 2;
        neighb[nr].col = p.col - 1;
        ++nr;
    }

    if (p.row - 2 >= 0 && p.col + 1 < grid->cols && grid->mat[p.row - 2][p.col + 1] != 1) {
        neighb[nr].row = p.row - 2;
        neighb[nr].col = p.col + 1;
        ++nr;
    }

    if (p.row - 2 >= 0 && p.col - 1 >= 0 && grid->mat[p.row - 2][p.col - 1] != 1) {
        neighb[nr].row = p.row - 2;
        neighb[nr].col = p.col - 1;
        ++nr;
    }

    if (p.row + 1 < grid->rows && p.col + 2 < grid->cols && grid->mat[p.row + 1][p.col + 2] != 1) {
        neighb[nr].row = p.row + 1;
        neighb[nr].col = p.col + 2;
        ++nr;
    }

    if (p.row + 1 < grid->rows && p.col - 2 >= 0 && grid->mat[p.row + 1][p.col - 2] != 1) {
        neighb[nr].row = p.row + 1;
        neighb[nr].col = p.col - 2;
        ++nr;
    }

    if (p.row - 1 >= 0 && p.col + 2 < grid->cols && grid->mat[p.row - 1][p.col + 2] != 1) {
        neighb[nr].row = p.row - 1;
        neighb[nr].col = p.col + 2;
        ++nr;
    }

    if (p.row - 1 >= 0 && p.col - 2 >= 0 && grid->mat[p.row - 1][p.col - 2] != 1) {
        neighb[nr].row = p.row - 1;
        neighb[nr].col = p.col - 2;
        ++nr;
    }

    return nr;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    queue<Node*> Queue;

    for (int i = 0; i < graph->nrNodes; i++)
    {
        graph->v[i]->dist = 0;
        graph->v[i]->parent = nullptr;
        graph->v[i]->color = COLOR_WHITE;
    }

    if (op != NULL)
        op->count();
    Queue.push(s);

    if (op != NULL)
        op->count();
    s->color = COLOR_GRAY; 

    while (!Queue.empty())
    {
        if (op != NULL)
            op->count();
        Node* node = Queue.front();

        for (int i = 0; i < node->adjSize; i++)
        {
            if (op != NULL)
                op->count();
            Node* neighb = node->adj[i];

            if (op != NULL)
                op->count();
            if (neighb->color == COLOR_WHITE)
            {
                neighb->parent = node;
                neighb->dist = node->dist + 1;
                neighb->color = COLOR_GRAY;
                Queue.push(neighb);
                if (op != NULL)
                    op->count(4);
            }
        }
        Queue.pop();
        node->color = COLOR_BLACK;
        if (op != NULL)
            op->count();
    }
}

void PrettyPrint(Graph* graph, int* p, int size, int root, int level)
{
    for (int i = 0; i < level; i++)
        cout << "      ";
    cout << "(" << graph->v[root]->position.row << "," << graph->v[root]->position.col << ")" << endl;

    for (int i = 0; i < size; i++)
        if (p[i] == root)
            PrettyPrint(graph, p, size, i, level + 1);
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs

        for (int i = 0; i < n; i++)
            if (p[i] == -1)
                PrettyPrint(graph, p, n, i, 0);
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000

    bfs(graph, start, NULL);
     
    stack<Node*>ReversePath; 

    Node* node = end;

    while (node != start)
    {
        ReversePath.push(node);
        node = node->parent;
    }

    ReversePath.push(start);

    int count = 0;
    while (!ReversePath.empty())
    {
        Node* node = ReversePath.top();
        ReversePath.pop();
        path[count] = node;
        count++;
        node = node->parent;
    }

    return end->dist;
}

Graph generate_edges(Graph graph, int numNodes, int numEdges)
{
    mt19937 gen(time(nullptr));

    int numx = sqrt(numNodes);
    int numy = numNodes - numx;

    bool** nodeExists = new bool* [numy];
    for (int i = 0; i < numy; ++i) {
        nodeExists[i] = new bool[numy];
        for (int j = 0; j < numy; ++j)
            nodeExists[i][j] = false;
    }

    for (int i = 0; i < numNodes; ++i) {
        graph.v[i] = new Node();
        graph.v[i]->adj = new Node * [numNodes] {};
        graph.v[i]->adjSize = 0;
        graph.v[i]->color = COLOR_WHITE;
    }

    int count1 = 0;
    while (count1 < numNodes)
    {
        uniform_int_distribution<int> nodeDistributionx(0, numx - 1);
        uniform_int_distribution<int> nodeDistributiony(0, numy - 1);

        int posx = nodeDistributionx(gen);
        int posy = nodeDistributiony(gen);

        if (!nodeExists[posx][posy])
        {
            Point point;
            point.col = posy;
            point.row = posx;
            graph.v[count1]->position = point;
            nodeExists[posx][posy] = true;

            count1++;
        }
    }

    bool** edgeExists = new bool* [numNodes];
    for (int i = 0; i < numNodes; ++i)
    {
        edgeExists[i] = new bool[numNodes];
        for (int j = 0; j < numNodes; ++j)
            edgeExists[i][j] = false;
    }

    for (int i = 0; i < numNodes - 1; i++)
    {
        int x = i;
        int y = i + 1;

        int sizex = graph.v[x]->adjSize;
        graph.v[x]->adj[sizex] = graph.v[y];
        graph.v[x]->adjSize++;

        int sizey = graph.v[y]->adjSize;
        graph.v[y]->adj[sizey] = graph.v[x];
        graph.v[y]->adjSize++;

        edgeExists[x][y] = edgeExists[y][x] = true;
    }

    int count2 = numNodes-1;
    while (count2 <= numEdges) 
    {
        uniform_int_distribution<int> nodeDistribution2(0, count2 - 1);

        int x = nodeDistribution2(gen);
        int y = nodeDistribution2(gen);

        if (x != y && x < numNodes && y < numNodes && !edgeExists[x][y]) {
            int sizex = graph.v[x]->adjSize;
            graph.v[x]->adj[sizex] = graph.v[y];
            graph.v[x]->adjSize++;

            int sizey = graph.v[y]->adjSize;
            graph.v[y]->adj[sizey] = graph.v[x];
            graph.v[y]->adjSize++;

            edgeExists[x][y] = edgeExists[y][x] = true;

            count2++;
        }
    }

    for (int i = 0; i < numy; ++i)
        delete[] nodeExists[i];
    delete[] nodeExists;

    for (int i = 0; i < numNodes; ++i)
        delete[] edgeExists[i];
    delete[] edgeExists;

    return graph;
}

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        graph = generate_edges(graph, graph.nrNodes, n);
        cout << "stop " << n << " ";

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        graph = generate_edges(graph, n, 4500); 
        cout << "stop " << n << " ";

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
