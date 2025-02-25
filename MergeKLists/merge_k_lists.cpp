/*
    Gozman-Pop Maria-Eliza
    Group 30424
    Merging k lists
    --------------------------------------------
       In acest program ni se dau k liste sortate cu un total de n elemente (le generam noi de fapt), care trebuie imbinate, astfel ca si
    rezultatul lor sa fie soratat. Pentru generarea lor, nu avem o distributie in totalitate random, listele au un numar aproximativ egal 
    de elemente Am generat un array sortat crescator si am luat fiecare element pe rand si l-am introdus in cate o lista pe rand. Lucram cu 
    un "array de liste" de marime k. Pentru a imbina listele, avem nevoie de o metoda cat mai eficienta, solutia fiind sa folosim un 
    min-heap. Am ales modalitatea de a pune pe nodurile heapului, primul element din fiecare lista pentru a face mai usoara accesarea
    elementelor urmatoare si pentru a economisi memorie (altfel am fi avut nevoie de o structura suplimentara). 
       Pentru implemetare am avut nevoie de functia 'heapify' si 'build-heap'. In 'heapify' au fost necesare niste modificari pentru a putea 
    lucra cu array-ul de liste si le-am interschimbat in functie de primul element din fiecare lista. Practoc am comstruit un heap de liste cu
    minimurile curente in prim-plan. 
       In functia 'merge' am contruit heap-ul, dupa care am luat fiecare minim pe rand, l-am adaugat in solutie, l-am scos din lista din care 
    facea parte si am reatriubuit proprietatile de heap pentru a avea mereu elementul minim pe pozitia radacinii. In cazul in care lista din
    care am luat elementul a ramas goala, ea trebuie eliminata din heap: o mutam la finalul heapului si scandem size-ul acestuia.
    Pentru a testa performanta algoritmului, numaram operatiile. Din cauza ca avem doua variabile care ne pot influenta performanta (n, 
    numarul de elemente totale, si k, numarul listelor => O(nlogk)), avem nevoie de doua cazuri: cazul in care variaza n si cazul in care 
    variaza k. Am facut media a valorilor in 5 cazuri diferite. In cazul in care variaza n, avem 3 constante pentru k (5, 10 si 100), iar 
    graficul va trebui sa fie liniar (O(n)). Pentru a face posibila compararea celor 3 cazuri in acelasi grafic, am declarat trei 
    operatii diferite si le-am pasat ca parametrii in functii. In cazul in care variaza k, initializam n ca 10000 si complexitatea 
    graficului va fi O(logk), asa cum reiese din figura.
*/

#include <iostream>
#include <list>

#include "Profiler.h"

Profiler prof("AVERAGE_N_VAR");

using namespace std;

void heapify(list <int> mylist[], int nrlists, int i, Operation op)
{
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int min = i;

    if (left < nrlists && (mylist[min].front() > mylist[left].front()))
        min = left;
    if (left < nrlists)
        op.count();

    if (right < nrlists && (mylist[min].front() > mylist[right].front()))
        min = right;
    if (right < nrlists)
        op.count();
 
    if (min != i)
    {
        op.count(3);
        swap(mylist[i], mylist[min]);
        heapify(mylist, nrlists, min, op);
    }
}

void buildHeap(list <int> mylist[], int nrlists, Operation op)
{
    int i;
    for (i = nrlists / 2 - 1; i >= 0; i--)
        heapify(mylist, nrlists, i, op);
}

void mergeLists(list <int> &finlist, list <int> mylist[], int nrlists, Operation op)
{
    buildHeap(mylist, nrlists, op);

    while (nrlists != 0)
    {
        int min = mylist[0].front();
        op.count();
        finlist.push_back(min);
        op.count();
        mylist[0].pop_front();
        op.count();

        op.count();
        if (mylist[0].empty())
        {
            op.count(3);
            swap(mylist[0], mylist[nrlists - 1]);
            nrlists--;
        }
        heapify(mylist, nrlists, 0, op);
    }
}

void printList(list <int> &finlist)
{
    for (auto v : finlist)
        cout << v << " ";
    cout << endl;
}

void printList2(list <int> finlist)
{
    for (auto v : finlist)
        cout << v << " ";
    cout << endl;
}

void generateLists(int n, int k, list <int>mylist[])
{
    int array[10001];
    int i;
    int j = 0;
    FillRandomArray(array, n, 1, 50000, true, ASCENDING);
    for (i = 0; i < n; i++)
    {
        if (j == k)
            j = 0;
        mylist[j].push_back(array[i]);
        j++;
    }
}

void demo()
{
    list <int> mylist[3];
    list <int> finlist;
    int n = 15;
    int k = 3;

    Operation op = prof.createOperation("operations", k);

    generateLists(n, k, mylist);

    printList2(mylist[0]);
    printList2(mylist[1]);
    printList2(mylist[2]);

    mergeLists(finlist, mylist, k, op);

    printList(finlist);
}

void perf_k_var()
{
    list <int> mylist[1001];
    list <int> finlist;

    int k;
    int start = 10;
    int end = 500;
    int increment = 10;
    int NR_EXP = 5;
    int n = 10000;

    for (int experiments = 0; experiments < NR_EXP; experiments++)
    {
        for (k = start; k <= end; k += increment)
        {
            Operation op = prof.createOperation("operations", k);
            generateLists(n, k, mylist);
            mergeLists(finlist, mylist, k, op);
        }
    }
    prof.divideValues("operations", NR_EXP);

    prof.showReport();
}

void perf_n_var()
{
    list <int> mylist[101];
    list <int> finlist;

    int start = 100;
    int end = 10000;
    int increment = 100;
    int NR_EXP = 5;
    int n;

    int k = 5;
    for (int experiments = 0; experiments < NR_EXP; experiments++)
    {
        for (n = start; n <= end; n += increment)
        {
            Operation op1 = prof.createOperation("operationsk5", n);
            generateLists(n, k, mylist);
            mergeLists(finlist, mylist, k, op1);
        }
    }
    prof.divideValues("operationsk5", NR_EXP);

    k = 10;
    for (int experiments = 0; experiments < NR_EXP; experiments++)
    {
        for (n = start; n <= end; n += increment)
        {
            Operation op2 = prof.createOperation("operationsk10", n);
            generateLists(n, k, mylist);
            mergeLists(finlist, mylist, k, op2);
        }
    }
    prof.divideValues("operationsk10", NR_EXP);

    k = 100;
    for (int experiments = 0; experiments < NR_EXP; experiments++)
    {
        for (n = start; n <= end; n += increment)
        {
            Operation op3 = prof.createOperation("operationsk100", n);
            generateLists(n, k, mylist);
            mergeLists(finlist, mylist, k, op3);
        }
    }
    prof.divideValues("operationsk100", NR_EXP);

    prof.createGroup("operations", "operationsk5", "operationsk10", "operationsk100");
    prof.showReport();
}

int main()
{
    demo();
    //perf_k_var();
    //perf_n_var();
}

