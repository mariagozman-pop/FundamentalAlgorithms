/*
    Gozman-Pop Maria-Eliza
    Group 30424
    DFS algorithm
    ----------------------------------------
        In this code, I have implemented the DFS algorithm, Topological Sort and Tarjan's algorithm. DFS is a graph traversal method that starts at an initial node, explores as deeply as possible along each branch before
    backtracking, and marks nodes as visited. To implement this, I have used variables such as discovery and finish time, as well as assign each vertex a color (white - unvisited, grey - visited or black - fully 
    explored). The Topological sort is used to linearly order the nodes in a directed acyclic graph based on their dependencies, using a modified version of DFS (one that also uses a stack, where the nodes are being 
    pushed once all their dependancies are explored). Tarjan's algorithm is used to find strongly connected components (subgraphs where every node is reachable from every other node within the same component) in a 
    directed graph. The alorithm uses a modified DFS (we are no longer working with colors, but discovery time and finish time) and a stack to store the values. When a strongly connected component is identified (all 
    nodes in the component are on the stack), the component is extracted and printed.
*/

#include <iostream>
#include <stack>
#include <random>
#include "Profiler.h"

using namespace std;

Profiler prof("dfs");

typedef struct _Node {
    int value;
    int color;
    int disc;
    int fin;
    int index;
    int lowlink;
    bool on_stack;
    struct _Node* parent;
    int adjSize;
    struct _Node** adj;
}Node;

typedef struct Graph {
    int nrNodes;
    Node** v;
}Graph;

enum {
    COLOR_WHITE = 0,
    COLOR_GRAY,
    COLOR_BLACK
};

Node* createNode(Node* current, int val)
{
    current = (Node*)malloc(sizeof(Node));

    current->value = val;
    current->color = COLOR_WHITE;
    current->parent = nullptr;
    current->adjSize = 0;
    current->adj = (Node**)malloc(sizeof(Node*));
    current->disc = -1;
    current->fin = -1;
    
    return current; 
}

void addEdge(Node* a, Node* b)  
{
    int size_a = a->adjSize;  
    a->adj = (Node**)realloc(a->adj, (size_a + 1) * sizeof(Node*));
    a->adj[size_a] = b; 
    a->adjSize = size_a + 1; 
}

int time_dfs;

void DFS_visit(Graph* g, Node* current, Operation* op)
{
    time_dfs++;
    current->disc = time_dfs;
    if (op != NULL)
        op->count();

    cout << " " << current->value;

    current->color = COLOR_GRAY;
    if (op != NULL)
        op->count();

    for (int i = 0; i < current->adjSize; i++)
    {
        if (op != NULL) 
            op->count();
        if (current->adj[i]->color == COLOR_WHITE)
        {
            if (op != NULL)
                op->count();

            current->adj[i]->parent = current;
            DFS_visit(g, current->adj[i], op);
        }
    }

    current->color = COLOR_BLACK;
    time_dfs++;
    current->fin = time_dfs;

    if (op != NULL)
        op->count(2);
}

void DFS(Graph* g, Operation* op)
{
    time_dfs = 0;
    for (int i = 0; i < g->nrNodes; i++)
    {
        if (op != NULL)
            op->count();

        if (g->v[i]->color == COLOR_WHITE)
            DFS_visit(g, g->v[i], op);
    }
}

void DFS_tsort(Graph* g, Node* current, stack<Node*>& nodes)
{
    time_dfs++;
    current->disc = time_dfs;

    current->color = COLOR_GRAY;
    for (int i = 0; i < current->adjSize; i++)
        if (current->adj[i]->color == COLOR_WHITE)
        {
            current->adj[i]->parent = current;
            DFS_tsort(g, current->adj[i], nodes);
        }

    current->color = COLOR_BLACK;
    time_dfs++;
    current->fin = time_dfs;
    nodes.push(current);
}

Node** topologicalSort(Graph* g) 
{
    stack<Node*>nodes;
    Node** sol = (Node**)malloc((g->nrNodes) * sizeof(Node*));

    time_dfs = 0; 
    for (int i = 0; i < g->nrNodes; i++) 
        if (g->v[i]->color == COLOR_WHITE) 
            DFS_tsort(g, g->v[i], nodes); 

    for (int i = 0; i < g->nrNodes; i++)
    {
        sol[i] = nodes.top();
        nodes.pop();
    }
      
    return sol;
}

void DFS_Tarjan(Graph* g, Node* current, stack<Node*>& nodes)
{
    time_dfs++;
    current->disc = time_dfs;
    current->lowlink = time_dfs;

    nodes.push(current);
    current->on_stack = true;

    for (int i = 0; i < current->adjSize; i++)
    {
        if (current->adj[i]->disc == -1)
        {
            current->adj[i]->parent = current;
            DFS_Tarjan(g, current->adj[i], nodes);
            current->lowlink = min(current->lowlink, current->adj[i]->lowlink);
        }
        else if (current->adj[i]->on_stack)
        {
            current->lowlink = min(current->lowlink, current->adj[i]->disc);
        }
    }

    if (current->disc == current->lowlink)
    {
        stack<Node*>sol;
        int count = 0;

        while (nodes.top() != current)
        {
            Node* topNode = nodes.top();
            topNode->on_stack = false;
            sol.push(topNode);
            count++;
            nodes.pop();
        }
        current->on_stack = false;
        nodes.pop();
        cout << "One of the components is: " << current->value;
        for (int i = 0; i < count; i++)
        {
            Node* top = sol.top(); 
            cout << " " << top->value;
            sol.pop();
        }
        cout << endl;
    }

    current->color = COLOR_BLACK;
    time_dfs++;
    current->fin = time_dfs;
}

void Tarjan(Graph* g)
{
    stack<Node*>nodes;

    time_dfs = 0;
    for (int i = 0; i < g->nrNodes; i++) 
        if (g->v[i]->disc == -1)
        {
            g->v[i]->index = 1;
            g->v[i]->lowlink = 1;
     
            DFS_Tarjan(g, g->v[i], nodes);
        }
}

void printGraph(Graph* g)
{
    int i, j;
    for (i = 0; i < g->nrNodes; i++)
    {
        cout << g->v[i]->value << " -> ";
        for (j = 0; j < g->v[i]->adjSize; j++)
            cout << g->v[i]->adj[j]->value << " ";
        cout << endl;
    }
}

void printTree(Node* root, int level = 0) {
    if (root == nullptr) return;

    for (int i = 0; i < level; ++i) {
        cout << "    "; 
    }

    cout << root->value << endl; 

    for (int i = 0; i < root->adjSize; ++i) {
        printTree(root->adj[i], level + 1); 
    }
}

Graph* createGraph(int n)
{
    Graph* g = (Graph*)malloc(sizeof(Graph));

    g->v = (Node**)malloc(n * sizeof(Node*));
    g->nrNodes = n;

    return g;
}

void demo_DFS()
{
    Graph* g = createGraph(10); 
   
    for (int i = 0; i < g->nrNodes; i++)
        g->v[i] = createNode(g->v[i], i); 

    addEdge(g->v[0], g->v[1]);
    addEdge(g->v[0], g->v[2]);
    addEdge(g->v[1], g->v[3]);
    addEdge(g->v[1], g->v[4]);
    addEdge(g->v[2], g->v[5]); 
    addEdge(g->v[3], g->v[6]);
    addEdge(g->v[3], g->v[7]); 
    addEdge(g->v[4], g->v[8]); 
    addEdge(g->v[4], g->v[9]);

    printGraph(g);
    cout << endl;
    
    cout << "The solution for the DFS algorithm is:";
    DFS(g, NULL); 
    cout << endl;

    for (int i = 0; i < g->nrNodes; i++) {
        if (g->v[i]->parent == nullptr) {
            printTree(g->v[i]);
        }
    }

    cout << endl << endl;
}

void demo_TSort()
{
    Graph* g = createGraph(8);
    Node** solution;

    for (int i = 0; i < g->nrNodes; i++)  
        g->v[i] = createNode(g->v[i], i); 

    addEdge(g->v[0], g->v[1]);  //undershorts->pants
    addEdge(g->v[1], g->v[3]);  //pants->belt
    addEdge(g->v[1], g->v[2]);  //pants->shoes
    addEdge(g->v[4], g->v[2]);  //socks->shoes
    addEdge(g->v[3], g->v[5]);  //belt->jacket
    addEdge(g->v[6], g->v[3]);  //shirt->belt
    addEdge(g->v[6], g->v[7]);  //shirt->tie
    addEdge(g->v[7], g->v[5]);  //tie->jacket

    printGraph(g); 
    cout << endl;  

    solution = topologicalSort(g); 
    cout << "The solution of the topological sort is: "; 
    for (int i = 0; i < g->nrNodes; i++)
        cout << solution[i]->value << " ";

    cout << endl;
}

void demo_Tarjan()
{
    Graph* g = createGraph(9);

    for (int i = 0; i < g->nrNodes; i++)
        g->v[i] = createNode(g->v[i], i);

    addEdge(g->v[0], g->v[2]);
    addEdge(g->v[2], g->v[4]);  
    addEdge(g->v[4], g->v[6]);  
    addEdge(g->v[6], g->v[0]);
    addEdge(g->v[1], g->v[5]);  
    addEdge(g->v[5], g->v[7]);  
    addEdge(g->v[7], g->v[1]);  
    addEdge(g->v[3], g->v[8]);  
    addEdge(g->v[8], g->v[3]);  
    addEdge(g->v[2], g->v[1]);
    addEdge(g->v[5], g->v[3]);

    printGraph(g);
    cout << endl;

    cout << "The solution is: " << endl;
    Tarjan(g);
}

Graph generate_edges(Graph graph, int numNodes, int numEdges)
{
    mt19937 gen(time(nullptr));
    uniform_int_distribution<int> nodeDistribution2(0, numEdges - 1); 

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
        addEdge(graph.v[x], graph.v[y]);
        edgeExists[x][y] = true;
    }

    int count2 = numNodes - 1;
    while (count2 < numEdges)
    {
        int x = nodeDistribution2(gen);
        int y = nodeDistribution2(gen);

        if (x != y && x < numNodes && y < numNodes && !edgeExists[x][y])
        {
            addEdge(graph.v[x], graph.v[y]);
            edgeExists[x][y] = true;
            count2++;
        }
    }

    return graph;
}

void free_graph(Graph* graph)
{
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
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

void perf()
{
    int n, i;

    for (n = 1000; n <= 4500; n += 100)
    {
        Operation op = prof.createOperation("dfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;

        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i)
        {
            graph.v[i] = createNode(graph.v[i], i); 
            op.count(5);
        }

        graph = generate_edges(graph, 100, n);
        cout << "stop " << n << " ";

        DFS(&graph, &op);
        free_graph(&graph);
    }

    for (n = 100; n <= 200; n += 10)
    {
        Operation op = prof.createOperation("dfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;

        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i)
        {
            graph.v[i] = createNode(graph.v[i], i);
            op.count(5);
        }

        graph = generate_edges(graph, n, 4500);
        cout << "stop " << n << " ";

        DFS(&graph, &op);
        free_graph(&graph);
    }

    prof.showReport();
}

int main()
{
    demo_DFS(); 
    demo_TSort();
    demo_Tarjan();
    //perf();
}