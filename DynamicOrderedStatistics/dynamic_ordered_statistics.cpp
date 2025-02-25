/*
    Gozman-Pop Maria-Eliza
    Group 30424
    Dynamic Ordered Statistics
    -----------------------------------
        In this code, I have implemented operations (build tree, select and delete) on balanced augumented binary search trees. Firstly, I needed a function 
    to create each node of the tree by allocating memory for it and initializing it. Then I implemented the BuildTree function that calls another (divide),
    which is a recursive one and which uses divide_and_conquer logic. It always looks for the middle of the indexes, and assigns each smaller element on the
    left, and each larger one on the right. It is important to mention that I compute the size of each node in the same function. The OS_Select function 
    returns the ith smallest element in the tree, using its rank. It compares the calculated rank for each node with the variable i, and depending on the 
    result, it follows the correct branch. The OS_Delete function deletes a node also based on its rank. In this function we have many cases: if the node is
    the root (if it has no children, if it has 1), if the node has 0 children, if it has 1 child or if it has 2. To make this work, I needed to update the 
    size after each deletion, and to do that, I created the UpdateSize function, that starts from the deleted node's parent all the way up to the root 
    (following a path of parents), changing all their sizes accordingly. 
        To measure the efficiency, I performed 5 experiments with a various number of elements for the tree, and called the functions BuildTree, OS_Select 
    and OS_Delete, and for the last two I chose the ranks randomly, using two different functions: one that keeps record of the ranks chosen before, to 
    avoid repetitions (for OS_Select), and one that only chooses a random eleemnt based on the number of elements in the tree (for OS_Delete). The complexity
    of BuildTree is O(n), and for the OS_Select and OS_Delete it is O(h), even though on the graph it will show up as O(n) (this is because we add up the
    results of all the n operations performed to search and delete all the elements).

*/

#include <iostream>
#include <random>
#include <ctime>
#include "Profiler.h"

Profiler prof("Dynamic Order Statistics");

using namespace std;

typedef struct tree_node {
    int value;
    int size;
    struct tree_node* parent;
    struct tree_node* left;
    struct tree_node* right;
};

tree_node* root;

tree_node* createTreeNode(int value)
{
    tree_node* node = (tree_node*)malloc(sizeof(tree_node));
    node->value = value;
    node->size = 1;
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;
    return node;
}

struct tree_node* divide(int l, int r, tree_node* p, Operation op)
{
    if (l > r)
        return nullptr;

    int mid = (l + r) / 2;

    tree_node* node = createTreeNode(mid);
    node->parent = p;

    node->left = divide(l, mid - 1, node, op);
    node->right = divide(mid + 1, r, node, op);

    if (node->left != nullptr)
    {
        op.count();
        node->size += node->left->size;
    }
    if (node->right != nullptr)
    {
        op.count();
        node->size += node->right->size;
    }

    return node;
}

void PrettyPrint(tree_node* new_root, int level = 0, char c = 'C')
{
    if (root == nullptr)
        cout << "There are no more nodes in the tree.";

    if (new_root == nullptr)
        return;

    PrettyPrint(new_root->right, level + 1, 'R');

    for (int i = 0; i < level; i++)
        cout << "  ";
    cout << new_root->value << "(" << c << ")" << endl;

    PrettyPrint(new_root->left, level + 1, 'L');
}

void BuildTree(int n, Operation op)
{
    root = divide(1, n, nullptr, op); 
}

tree_node* OS_Select(tree_node* new_root, int i, Operation op)
{
    if (new_root == nullptr)
        return nullptr;

    int rank = 1;
    if (new_root->left != nullptr)
    {
        rank = new_root->left->size + 1;
        op.count();
    }

    if (rank == i)
        return new_root;
    else if (i < rank)
        return OS_Select(new_root->left, i, op);
    else
        return OS_Select(new_root->right, i - rank, op);
}

void UpdateSize(tree_node* node, Operation op)
{
    if (node == nullptr)
        return;

    int new_size = 1; 

    if (node->left != nullptr)
    {
        new_size += node->left->size;
        op.count();
    }

    if (node->right != nullptr)
    {
        new_size += node->right->size;
        op.count();
    }

    node->size = new_size;
    op.count();

    UpdateSize(node->parent, op);
}

tree_node* Successor(tree_node* node)
{
    if (node->left == nullptr)
        return node;
    return Successor(node->left);
}

void DeleteBST(tree_node* node, Operation op)
{
    //for node == root
    if (node->parent == nullptr)
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            root = nullptr;
            free(node);
            return;
        }
        else if (node->left == nullptr && node->right != nullptr)
        {
            root = node->right;
            root->parent = nullptr;
            free(node);
            UpdateSize(root, op);
            return;
        }
        else if (node->left != nullptr && node->right == nullptr)
        {
            root = node->left;
            root->parent = nullptr;
            free(node);
            UpdateSize(root, op);
            return;
        }
    }

    tree_node* l = node->left;
    tree_node* r = node->right; 
    tree_node* p = node->parent;

    //for 0 children
    op.count();
    if (node->left == nullptr && node->right == nullptr)
    {
        if (p->left == node)
        {
            p->left = nullptr;
            free(node);
            UpdateSize(p, op);
            return;
        }
        else if (p->right == node)
        {
            p->right = nullptr;
            free(node);
            UpdateSize(p, op);
            return;
        }
    }

    //for 1 child
    if (l == nullptr && r != nullptr)
    {
        if (p->left == node)
        {
            p->left = r;
            r->parent = p;
            free(node);
            UpdateSize(p, op);
            return;
        }
        else if (p->right == node)
        {
            p->right = r;
            r->parent = p;
            free(node);
            UpdateSize(p, op);
            return;
        }
    }
    else if (l != nullptr && r == nullptr)
    {
        if (p->left == node)
        {
            p->left = l;
            l->parent = p;
            free(node);
            UpdateSize(p, op);
            return;
        }
        else if (p->right == node)
        {
            p->right = l;
            l->parent = p;
            free(node);
            UpdateSize(p, op);
            return;
        }
    }

    //for 2 children
    if (r != nullptr && l != nullptr)
    {
        tree_node* leaf = Successor(node->right);

        int temp = node->value;
        node->value = leaf->value;
        leaf->value = temp;

        DeleteBST(leaf, op); 
        return;
    }
}

void OS_Delete(tree_node* root, int i, Operation op)
{
    tree_node* deleted = OS_Select(root, i, op);
    if (deleted != nullptr)
        DeleteBST(deleted, op);
}

int select_num_sel(int n, bool* chosen)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, n - 1);

    int randomNumber;
    do {
        randomNumber = distribution(gen);
    } while (chosen[randomNumber]);

    chosen[randomNumber] = true;

    return randomNumber;
}

int select_num_delete(int n)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(1, n);

    int randomNumber = distribution(gen);

    return randomNumber;
}

void demo()
{
    int n = 11;
    Operation op = prof.createOperation("build_tree", 0);
    BuildTree(n, op);
    PrettyPrint(root);
    cout << endl;

    int count = n;
    for (int i = 0; i < n; i++)
    {
        int rand_index = select_num_delete(count);
        cout << "The random index is: " << rand_index;
        tree_node* node = OS_Select(root, rand_index, op);
        if (node != nullptr)
            cout << endl << "the " << rand_index << "th smallest element is: " << node->value << endl;
        else
            cout << endl << "A node with such a rank does not exist." << endl;
        cout << endl; 

        OS_Delete(root, rand_index, op);
        PrettyPrint(root);
        cout << endl;
        count--;
    }
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
            Operation op_b = prof.createOperation("build_tree", i);
            BuildTree(i, op_b);

            Operation op_s = prof.createOperation("os_select", i);
            bool* chosen = new bool[i]();
            for (int j = 0; j < i; j++)
            {
                int rand_index = select_num_sel(i, chosen);
                OS_Select(root, rand_index, op_s);
            }
           
            Operation op_d = prof.createOperation("os_delete", i);
            int count = i;
            for (int j = 0; j < i; j++)
            {
                int rand_index = select_num_delete(count);
                OS_Delete(root, rand_index, op_d);
                count--;
            }
        }
    }

    prof.divideValues("os_select", nr_exp);
    prof.divideValues("os_delete", nr_exp);
    prof.divideValues("build_tree", nr_exp);

    prof.createGroup("OS", "os_select", "os_delete");

    prof.showReport();
}

int main()
{
    demo();
    //perf();

    return 0;
}