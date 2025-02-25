/*
    Gozman-Pop Maria-Eliza
    Group 30424
    Disjoint sets
    --------------------
        In this code, I have implemented base operations on disjoint sets, namely, Make_Set, Find_Set and Union. I have worked with nodes, so the operation that makes the set 
    simply allocates memory for the node, it gives it the value of the initial set, sets the rank as 1 and makes the parent of the node nullptr. The operation Find_Set looks 
    for the "head" of the set, the representative value, going up the branch of parents, while also using path compression (it links each node in the set directly to the head 
    to make the search easier for further calls). The operation Union links two sets together, based on the rank of each set's head: the one with the larger height will recieve 
    the one with the smaller one. 
        In the second part of the code, I have implememnted Krs=uskal's algorithm for a graph, using the operations I created before. Initially, the nodes are not connected
    literally, only by a new struct "edge" that specifies the nodes and the weight of the connection. Firstly, we sort the edges and then, we take each one at a time and verify 
    if it will make a cycle in the minimum spanning tree or not (if the two nodes have the same set "head", it means they are connected so we cannot add the edge to the 
    solution). In case they are not connected, we add the edge and perform union on the two nodes to mark their connection.
        For analysing the performance, I generated 4*n random edges (with random weights), where n is the number of vertices. To make sure I don't have any loop edges or 
    repeated ones, I used an adjacency matrix to store the connections. I randomly generate the nodes x and y and check in the adjacency matrix if the edge already exists. In 
    the case that it does, it will generate again until it finds a good combination. 
    */

#include <iostream>
#include <random>
#include "Profiler.h"

Profiler prof("Kruskal Operations");

using namespace std;

typedef struct node{
    struct node* parent;
    int value;
    int rank;
};

typedef struct edge {
    struct node* x;
    struct node* y;
    int weight;
};

node* MakeSetPerf(int value, Operation kruskal, Operation make_set)
{
    node* current = (node*)malloc(sizeof(node));

    current->parent = nullptr;
    current->value = value;
    current->rank = 1;

    kruskal.count(3);
    make_set.count(3); 

    return current;
}

node* MakeSet(int value)
{
    node* current = (node*)malloc(sizeof(node));

    current->parent = nullptr;
    current->value = value;
    current->rank = 1;

    return current;
}

node* FindSetPerf1(node* current, Operation kruskal, Operation uni, Operation findset) 
{
    kruskal.count();
    uni.count();
    findset.count();
    if (current->parent == nullptr)
        return current;

    node* repr = FindSetPerf1(current->parent, kruskal, uni, findset); 
    current->parent = repr;
    uni.count();
    kruskal.count();
    findset.count();

    return repr;
}

node* FindSetPerf2(node* current, Operation kruskal, Operation findset)
{
    kruskal.count();
    findset.count();
    if (current->parent == nullptr)
        return current;

    node* repr = FindSetPerf2(current->parent, kruskal, findset);
    current->parent = repr;
    kruskal.count();
    findset.count();

    return repr;
}

node* FindSet(node* current) 
{
    if (current->parent == nullptr) 
        return current;

    node* repr = FindSet(current->parent); 
    current->parent = repr;
    
    return repr;
}

void LinkPerf(node* x, node* y, Operation kruskal, Operation uni)
{
    kruskal.count();
    uni.count();
    if (x->rank > y->rank)
    {
        kruskal.count();
        uni.count();
        y->parent = x;
    }
    else
    {
        uni.count();
        kruskal.count();
        if (x->rank < y->rank)
        {
            uni.count();
            kruskal.count();
            x->parent = y;
        }
        else
        {
            uni.count(2);
            kruskal.count(2);
            y->parent = x; 
            x->rank++; 
        }
    }
}

void Link(node* x, node* y)
{
    if (x->rank > y->rank)
        y->parent = x;
    else if (x->rank < y->rank)
        x->parent = y;
    else
    {
        y->parent = x;
        x->rank++;
    }
}

void UnionPerf(node* x, node* y, Operation kruskal, Operation uni, Operation findset)
{
    LinkPerf(FindSetPerf1(x, kruskal, uni, findset), FindSetPerf1(y, kruskal, uni, findset), kruskal, uni);    
}

void Union(node* x, node* y)
{
    Link(FindSet(x), FindSet(y));
}

edge* Make_Edge(node* x, node* y, int weight) 
{
    edge* e = (edge*)malloc(sizeof(edge)); 

    e->x = x;
    e->y = y; 
    e->weight = weight;

    return e;
}

int Partition(edge* e[], int left, int right)
{
    int pivot = right;
    int i = left - 1;
    for (int j = left; j < right; j++)
    {
        if (e[j]->weight <= e[pivot]->weight)
        {
            i++;
            swap(e[i], e[j]);
        }
    }
    swap(e[i + 1], e[right]);
    return i + 1;
}

void SortEdges(edge* edges[], int left, int right) 
{
    int pivot;
    if (left < right)
    {
        pivot = Partition(edges, left, right);
        SortEdges(edges, left, pivot - 1);
        SortEdges(edges, pivot + 1, right); 
    }
}

edge** KruskalPerf(edge* edges[], int size, int nr_vertices, Operation kruskal, Operation uni, Operation findset) 
{
    edge** sol = (edge**)malloc(size * sizeof(edge));

    SortEdges(edges, 0, size - 1);

    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (count > nr_vertices - 1)
            break;

        if (FindSetPerf2(edges[i]->x, kruskal, findset) != FindSetPerf2(edges[i]->y, kruskal, findset)) 
        {
            sol[count] = edges[i];
            count++;
            UnionPerf(edges[i]->x, edges[i]->y, kruskal, uni, findset);
        }
    }
    return sol;
}

edge** Kruskal(edge* edges[],  int size)
{
    edge** sol = (edge**)malloc(size*sizeof(edge));

    SortEdges(edges, 0, size - 1);

    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (FindSet(edges[i]->x) != FindSet(edges[i]->y)) 
        {
            sol[count] = edges[i]; 
            count++; 
            Union(edges[i]->x, edges[i]->y); 
        }
    }
    return sol;
}

void printSets(node* sets[], int size)
{
    bool printed[10] = { false };

    for (int i = 0; i < size; i++)
    {
        node* representative = FindSet(sets[i]);
        if (!printed[representative->value])
        {
            cout << "{";
            for (int j = i; j < size; j++)
            {
                if (FindSet(sets[j]) == representative)
                {
                    cout << sets[j]->value << " ";
                    printed[sets[j]->value] = true;
                }
            }
            cout << "}, ";
        }
    }
    cout << endl;
}

void Print_Solution(edge** solution, int size)
{
    cout << "Kruskal solution: " << endl;
    for (int i = 0; i < size; i++)
    {
        if (solution[i] != nullptr)
        {
            cout << "edge: " << solution[i]->x->value << " - " << solution[i]->y->value << ", weight: " << solution[i]->weight << endl;
        }
    }
}

void demo_union()
{
    node* sets[10] = { nullptr };
    for (int i = 0; i < 10; i++)
        sets[i] = MakeSet(i);

    cout << "Sets before union: " << endl;
    printSets(sets, 10);
    cout << endl;

    Union(sets[1], sets[3]);
    Union(sets[3], sets[5]); 
    Union(sets[6], sets[2]); 
    Union(sets[2], sets[7]); 
    Union(sets[0], sets[8]); 

    cout << "Sets after union: " << endl;
    printSets(sets, 10); 
    cout << endl;
}

void demo_kruskal()
{
    node* sets[6] = { nullptr };
    edge* edges[11] = { nullptr };
    edge** sol;

    for (int i = 0; i < 6; i++)
        sets[i] = MakeSet(i); 

    edges[0] = Make_Edge(sets[0], sets[1], 5);    
    edges[1] = Make_Edge(sets[0], sets[2], 3);   
    edges[2] = Make_Edge(sets[0], sets[3], 1);   
    edges[3] = Make_Edge(sets[0], sets[4], 10);
    edges[4] = Make_Edge(sets[1], sets[2], 6);
    edges[5] = Make_Edge(sets[1], sets[3], 2);
    edges[6] = Make_Edge(sets[1], sets[4], 9);   
    edges[7] = Make_Edge(sets[2], sets[3], 4);
    edges[8] = Make_Edge(sets[3], sets[4], 8);

    sol = Kruskal(edges, 9); 

    Print_Solution(sol, 4);
}

edge** randomEdges(node** nodes, edge** edges, int numNodes, int numEdges)
{
    mt19937 gen(time(nullptr)); 
    uniform_int_distribution<int> nodeDistribution(0, numNodes - 1); 
    uniform_int_distribution<int> weightDistribution(1, 10 * numNodes); 

    bool** edgeExists = new bool* [numNodes]; 
    for (int i = 0; i < numNodes; ++i)
    {
        edgeExists[i] = new bool[numNodes];
        for (int j = 0; j < numNodes; ++j)
            edgeExists[i][j] = false;
    }

    int count = 0;

    for (int i = 1; i < numNodes - 1; i++)
    {
        int x = i; 
        int y = i + 1;

        if (!edgeExists[x][y]) 
        {
            int weight = weightDistribution(gen); 
            edges[count] = Make_Edge(nodes[x], nodes[y], weight); 
            edgeExists[x][y] = edgeExists[y][x] = true; 
            ++count; 
        }
    }

    while (count < numEdges)
    {
        int x = nodeDistribution(gen);
        int y = nodeDistribution(gen);

        if (x != y && !edgeExists[x][y])
        {
            int weight = weightDistribution(gen);
            edges[count] = Make_Edge(nodes[x], nodes[y], weight); 
            edgeExists[x][y] = edgeExists[y][x] = true;
            ++count;
        }
    }

    for (int i = 0; i < numNodes; ++i) {
        delete[] edgeExists[i];
    }
    delete[] edgeExists;

    return edges;
}


void perf()
{
    int nr_exp = 1;
    int start = 100;
    int end = 10000;
    int increment = 100;

    for (int experiments = 0; experiments < nr_exp; experiments++)
    {
        for (int size = start; size <= end; size += increment)
        {
            Operation kruskal = prof.createOperation("Kruskal", size);
            Operation make_set = prof.createOperation("Make_Set", size);
            Operation uni = prof.createOperation("Union", size);
            Operation find_set = prof.createOperation("Find_Set", size);

            node** sets = new node * [size];
            for (int i = 0; i < size; i++)
                sets[i] = MakeSetPerf(i, kruskal, make_set); 

            int numEdges = 4 * size;
            edge** edges = new edge * [numEdges]; 
            edges = randomEdges(sets, edges, size, numEdges); 

            KruskalPerf(edges, numEdges, size, kruskal, uni, find_set);   
        }
    }

    prof.divideValues("Kruskal", nr_exp);
    prof.divideValues("Union", nr_exp);
    prof.divideValues("Make_Set", nr_exp);
    prof.divideValues("Find_Set", nr_exp);

    prof.createGroup("Operations", "Kruskal", "Union", "Make_Set", "Find_Set");

    prof.showReport();
}

int main()
{
    demo_union();
    demo_kruskal();
    //perf();   
}
