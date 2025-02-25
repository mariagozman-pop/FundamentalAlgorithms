/*  
    Gozman-Pop Maria-Eliza
    Group 30424\
    Multi-way trees
    -------------------------------
       In this program I have used 3 different modes of representing multi-way trees, made transitions between them and also implemented a pretty print for
    each. The first representation consist of a simple array, in which the element is the parent of the node corresponding to its index. I found the root, 
    and passed it to the recursive pretty print function. To print the right amount of spaces (" "), I introduced a "level" variable, and printed each 
    element in its right place, at a time. The second representation is using a node structure that stores the node value, an array of the children nodes and 
    the number of children (that will make the printing easier). Basically, each parent points to all its children. The printing is similar to the first one, with 
    the difference being in the last loop, where we use the number of children as the stopping point. The third representation is a sort of binary tree that
    has the left pointer as its first child, and the right pointer as its first brother. I used a recursive function that starts from the root and adds all 
    its children, then back to its first child and does the same thing, and the the second, and so on, until the whole tree is iterated. The pretty print is 
    different, and much easier to implement, because of the structure of the tree, and it is done by two recursive calls. (Additionally, I have implemented 
    the first representation in a way which is a lot like the second representation - using a matrix of children for each node).
*/

#include <iostream>
#define MAX_SIZE 30

using namespace std;

typedef struct R2 {
    int value;
    int count;
    struct R2* children[MAX_SIZE];
} R2;

typedef struct R3 {
    int value;
    struct R3* left;
    struct R3* right;
};

R2* root2 = NULL;
R3* root3 = NULL;

void PrettyPrint1(int** children, int new_root, int size, int count[], int level)
{
    if (new_root == 0)
        return;

    for (int i = 0; i < level; i++)
        cout << "  ";
    cout << new_root << endl;

    for (int i = 0; i <= count[new_root-1]; i++)
    {
        if (children[new_root - 1][i] != 0)
            PrettyPrint1(children, children[new_root - 1][i], size, count, level + 1);
    }
}

void PrettyPrint1_2(int new_root, int parent[], int size, int level)
{
    for (int i = 0; i < level; i++)
        cout << "  ";
    cout << new_root << endl;

    for (int i = 0; i < size; i++)
        if (parent[i] == new_root)
            PrettyPrint1_2(i + 1, parent, size, level + 1);
}

void PrettyPrint2(R2* new_root, int level)
{
    if (new_root == NULL)
        return;

    for (int i = 0; i < level; i++)
        cout << "  ";
    cout << new_root->value << endl;

    for (int i = 0; i < new_root->count; i++)
        PrettyPrint2(new_root->children[i], level + 1);
}

void PrettyPrint3(R3* new_root, int level)
{
    if (new_root == NULL)
        return;

    for (int i = 0; i < level; i++)
        cout << "  ";
    cout << new_root->value << endl;

    PrettyPrint3(new_root->left, level + 1);
    PrettyPrint3(new_root->right, level);
}

R2* createNode(int value)
{
    R2* node = (R2*)malloc(sizeof(R2));
    node->value = value;
    node->count = 0;
    return node;
}

void addChild(R2* child, R2* parent)
{
    if (parent->count < MAX_SIZE)
        parent->children[parent->count++] = child;
    else
        cout << "Parent node cannot have more children" << endl;
}

void T1()
{
    int parents[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
    int size = sizeof(parents) / sizeof(parents[0]);
    R2* tree2[MAX_SIZE];

    for (int i = 1; i <= size; ++i)
        tree2[i] = createNode(i);

    for (int i = 1; i <= size; ++i)
    {
        if (parents[i - 1] == -1)
            root2 = tree2[i];
        else
            addChild(tree2[i], tree2[parents[i - 1]]);
    }

    cout << "R2: " << endl;
    PrettyPrint2(root2, 0);
    cout << endl;
}

struct R3* transform(R2* current)
{
    if (current == NULL)
        return NULL;

    R3* node = (R3*)malloc(sizeof(R3));

    node->value = current->value;
    node->left = NULL;
    node->right = NULL;

    if (current->count > 0)
    {
        node->left = transform(current->children[0]);
        R3* ptr = node->left;
        for (int i = 1; i < current->count; i++)
        {
            ptr->right = transform(current->children[i]);
            ptr = ptr->right;
        }
    }
    return node;
}

void T2()
{
    root3 = transform(root2);

    cout << "R3:" << endl;
    PrettyPrint3(root3, 0);
}

void R1_2()
{
    int parents[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
    int size = sizeof(parents) / sizeof(parents[0]);
    int root = 0;

    for (int i = 0; i < size; i++)
        if (parents[i] == -1)
            root = i + 1;

    cout << "R1_2:" << endl;
    PrettyPrint1_2(root, parents, size, 0);
    cout << endl;
}

void R1()
{
    int parents[] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
    int size = sizeof(parents) / sizeof(parents[0]);
    int** children = new int* [size];
    int* count = new int[size] {};
    int root;

    for (int i = 0; i < size; ++i)
        children[i] = new int[MAX_SIZE] {};

    for (int i = 0; i < size; i++)
    {
        int parent = parents[i];

        if (parent == -1)
            root = i + 1;
        else
        {
            int k = count[parent - 1];
            children[parent - 1][k] = i + 1;
            count[parent - 1]++;
        }
    }

    cout << "R1:" << endl;
    PrettyPrint1(children, root, size, count, 0);
    cout << endl;
}

int main()
{
    R1();
    R1_2();
    T1();
    T2();

    return 0;
}