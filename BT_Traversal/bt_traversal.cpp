/*
    Gozman-Pop Maria-Elzia
    Group 30414
    BT Traversal
    ---------------------------
        In this code, I have implemented an iterative and a recursive binary search traversal. For the recursive part, I explore the left-most part, and them move onto the right one at a time. For the iterative part, 
    I counted on temporarily changing the structure of the tree: the process begins at the root and for each node, if it has a left child, a link to the rightmost node in the left subtree (predecessor) is created. 
    If this link already exists, it's a signal that the left subtree has been processed, so it is removed. The traversal continues until all nodes have been visited. Both those tarversals have the complexity of O(n).
        To analyze the peformance, I built the tree from a random root each time, to make the result more accurate. Because the result is the same, I also added a time peformance comparison to make observe the 
    difference from this point of view as well. 
    
*/

#include <iostream>
#include <cstdlib>
#include <random>
#include "Profiler.h"

Profiler prof("BT Traversal");

using namespace std;

typedef struct BT_node {
    int key;
    struct BT_node* left;
    struct BT_node* right;
};

BT_node* root = (BT_node*)malloc(sizeof(BT_node));

void Recursive_T(BT_node* current, Operation *op)
{
    if (current == nullptr)
        return;

    Recursive_T(current->left, op);

    //cout << current->key << " ";
    if (op != NULL)
        op->count();

    Recursive_T(current->right, op);
}

void Iterative_T(BT_node* current, Operation *op)
{
    while (current != nullptr)
    {
        if (current->left == nullptr)
        {
            //cout << current->key << " "; 
            if (op != NULL) 
                op->count(); 
            current = current->right;
        }
        else
        {
            BT_node* predecessor = current->left;

            while (predecessor->right != nullptr && predecessor->right != current) 
                predecessor = predecessor->right;

            if (predecessor->right == nullptr)
            {
                predecessor->right = current;
                current = current->left; 
            }
            else 
            {
                predecessor->right = nullptr; 
                //cout << current->key << " ";
                if (op != NULL) 
                    op->count(); 
                current = current->right;
            }
        }
    }
}

void PrettyPrint(BT_node* new_root, int level = 0, char c = 'C')
{
    if (root == nullptr)
        cout << "There are no more nodes in the tree.";

    if (new_root == nullptr)
        return;

    PrettyPrint(new_root->right, level + 1, 'R');

    for (int i = 0; i < level; i++)
        cout << "  ";
    cout << new_root->key << "(" << c << ")" << endl;

    PrettyPrint(new_root->left, level + 1, 'L');
}

BT_node* CreateNode(int value)
{
    BT_node* node = (BT_node*)malloc(sizeof(BT_node));

    node->key = value;
    node->left = nullptr;
    node->right = nullptr;

    return node;
}

void AddNode(BT_node* current, BT_node* new_parent)
{
    if (new_parent->key > current->key)
    {
        if (new_parent->left == nullptr)
        {
            new_parent->left = current;
            return;
        }
        else
            AddNode(current, new_parent->left);
    }
    else
    {
        if (new_parent->right == nullptr)
        {
            new_parent->right = current;
            return;
        }
        else
            AddNode(current, new_parent->right);
    }
}

void BuildTree(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (array[i] != root->key)
        {
            BT_node* temp = CreateNode(array[i]);
            AddNode(temp, root);
        }
    }
}

void demo()
{
    int array[] = { 4, 5, 7, 1, 3, 9, 2, 8, 6};
    int size = sizeof(array) / sizeof(array[0]);

    Operation op = prof.createOperation("demo", size);

    root->key = array[0];
    root->left = nullptr;
    root->right = nullptr;

    BuildTree(array, size);
    PrettyPrint(root);
    cout << endl;
    Recursive_T(root, &op);
    cout << endl;
    Iterative_T(root, &op);
}

int select_num(int n)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, n-1);

    int randomNumber = distribution(gen);

    return randomNumber;
}

void perf()
{
    int nr_exp = 5; 
    int start = 100; 
    int end = 10000; 
    int increment = 100;

    for (int experiments = 0; experiments < nr_exp; experiments++)
    {
        for (int i = start; i <= end; i += increment)
        {
            int* array = new int[i+1];
            FillRandomArray(array, i, 1, 50000, true, UNSORTED);

            int rand_index = select_num(i);

            root->key = array[rand_index];
            root->left = nullptr;
            root->right = nullptr;

            BuildTree(array, i);

            Operation op_r = prof.createOperation("recursive", i);
            Operation op_i = prof.createOperation("iterative", i);

            Recursive_T(root, &op_r);
            Iterative_T(root, &op_i);
        }
    }

    prof.divideValues("recursive", nr_exp);
    prof.divideValues("iterative", nr_exp);

    //prof.createGroup("Operations", "recursive", "iterative");

    prof.showReport();
}

void perf_time()
{
    int nr_tests = 100;
    int start = 100;
    int end = 10000;
    int increment = 100;

    prof.reset("TIME");

    for (int size = start; size <= end; size += increment)
    {
        int* array = new int[size + 1];
        int* dummy = new int[size + 1];
        FillRandomArray(array, size, 1, 50000, true, UNSORTED); 

        int rand_index = select_num(size);

        root->key = array[rand_index]; 
        root->left = nullptr; 
        root->right = nullptr; 

        BuildTree(array, size); 

        prof.startTimer("Recursive", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            Recursive_T(root, NULL);
        }
        prof.stopTimer("Recursive", size);
    }

    for (int size = start; size <= end; size += increment)
    {
        int* array = new int[size + 1];
        int* dummy = new int[size + 1];
        FillRandomArray(array, size, 1, 50000, true, UNSORTED);

        int rand_index = select_num(size);

        root->key = array[rand_index];
        root->left = nullptr;
        root->right = nullptr;

        BuildTree(array, size);

        prof.startTimer("Iterative", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            Iterative_T(root, NULL);
        }
        prof.stopTimer("Iterative", size);
    }

    prof.createGroup("TIME", "Recursive", "Iterative");

    prof.showReport();

}

int main()
{
    //demo();
    //perf();
    perf_time();
}


