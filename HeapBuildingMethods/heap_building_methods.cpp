/*
    Gozman-Pop Maria-Eliza
    Group 30424
    Heap Building Methods

    ------------------------------

     Am implementat 2 tipuri de construire a unui heap (bottom-up si top-down) si un algoritm de sortare al elementelor 
    acestuia (heapsort). 
     Pentru implementarea functiei 'bottom-up' m-am folosit de functia 'sink', care in esenta compara
    copii unui nod cu index i (2i+1 si 2i+2) cu respectivul nod, astfel determinand maximul dintre ele pentru a stabili care 
    trebuie sa se afle pe pozitia de parinte. Functia este apelata recursiv de la urmatorul nod pana nu mai este nevoie de 
    interschimbari pe ramura respectiva. In functia 'bottom-up' avem un for care porneste de la n+1/2 care implicit foloseste 
    doar nodurile care nu sunt frunze, lucrand pe subarbori pana la contruirea completa a heap-ului. 
     Implementarea functiei 'top-down' am realizt-o cu ajutorul functiei 'swim', care urca de la copil spre parintele sau, 
    comparandu-i si interschimbandu-i cand nodul parintele este mai mic decat cel al copilului. Este, de asemenea, o functie 
    recursiva care se reapeleaza pana in momentul in care gasim un parinte mai mare decat copilul sau, fapt ce demonstreaza
    ca ramura este una corect atibuita. In functia 'top-down' gasim din nou un for care, de data aceasta, porneste de la 
    primul copil al radacinei si ajunge pana la frunze (inclusiv).
     In functia 'heapsort', intai apelez functia 'bottom-up' pentru a construi un heap cu datele din array-ul primit, apoi,
    prin intermediului unui for care merge de la frunze spre penultimul element, parcurgem heap-ul astfel: retinem elementul
    cu index 0 (elementul maxim) si il punem la coada. Dupa aceea, aplicam functia sink pentru a nu pierde proprietatea 
    heap-ului (de a avea elementul maxim ca radacina), dar pentru size--, deoarece pe ultima pozitie am retinut maximul.
    Procedura se repeta pana ajungem la un array sortat crescator.

     Am impartit graficele pe cazurile AVERAGE si WORST. Pentru cazul AVERAGE, numarul experimentelor este de 5, iar array-ul
    dat este nesortat. Din grafice, putem observa ca functia 'bottom-up' este mai eficienta decat functia 'top-down' pentru
    acest caz. Pentru cazul WORST, array-ul este crescator, deoarece elementele mari care trebuie sa ajunga in primele noduri
    sunt la final, iar astfel necesita un numar mare de operatii. Din grafice, reiese clar ca functia 'bottom-up' este mult 
    mai eficienta decat functia 'top-down'.
*/

#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler prof("AVERAGE");

void sink(int* h, int heap_size, int i)
{
    Operation comp_bott = prof.createOperation("bottom_up_comp", heap_size);
    Operation assign_bott = prof.createOperation("bottom_up_assign", heap_size);

    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int max = i;
    if (left < heap_size && h[i]<h[left])
        max = left;
    if (left < heap_size)
        comp_bott.count();
    if (right < heap_size && h[max]<h[right])
        max = right;
    if (right < heap_size)
        comp_bott.count();
    if (max != i)
    {
        assign_bott.count(3);
        swap(h[i], h[max]);
        sink(h, heap_size, max);
    }
}

void swim(int* h, int heap_size, int i)
{
    Operation comp_top = prof.createOperation("top_down_comp", heap_size);
    Operation assign_top = prof.createOperation("top_down_assign", heap_size);

    int parent = (i - 1) / 2;
    comp_top.count();
    if (h[i] > h[parent])
    {
        assign_top.count(3);
        swap(h[i], h[parent]);
        swim(h, heap_size, parent);
    }
}

void bottom_up(int* h, int heap_size)
{
    int i;
    for (i = heap_size / 2 - 1; i >= 0; i--)
        sink(h, heap_size, i);
}

void top_down(int* h, int heap_size)
{
    int i;
    for (i = 1; i < heap_size; i++)
        swim(h, heap_size, i);
}

void heap_sort(int* array, int array_size)
{
    bottom_up(array, array_size);
    for (int i = array_size-1; i >= 1; i--) {
        swap(array[0], array[i]);
        sink(array, i, 0);
    }
}

void demo_bottom_up()
{
    int array[] = { 4, 7, 2, 5, 9, 1 , 3, 6 };
    int array_size = sizeof(array)/ sizeof(array[0]);
    bottom_up(array, array_size);

    cout << "The bottom up result is: ";
    for (int i = 0; i < array_size; i++)
        cout << array[i]<<" ";
    cout << endl;
}

void demo_top_down()
{
    int array[] = { 4, 7, 2, 5, 9, 1 , 3, 6 };
    int array_size = sizeof(array) / sizeof(array[0]);
    top_down(array, array_size);

    cout << "The top down result is: ";
    for (int i = 0; i < array_size; i++)
        cout << array[i]<<" ";
    cout << endl;
}

void demo_heap_sort()
{
    int array[] = { 4, 7, 2, 5, 9, 1 , 3, 6 };
    int array_size = sizeof(array) / sizeof(array[0]);
    heap_sort(array, array_size);

    cout << "The heap sort result is: ";
    for (int i = 0; i < array_size; i++)
        cout << array[i]<<" ";
    cout << endl;
}

enum CASES { AVERAGE, WORST };

void perf(CASES var)
{
    int a[50001];
    int size;
    int start = 100;
    int end = 10000;
    int increment = 100;
    int dummy[50001];
    int NR_EXP = 1;
    if (var == AVERAGE)
        NR_EXP = 5;
    for (int experiments = 0; experiments < NR_EXP; experiments++) {
        for (size = start; size <= end; size += increment) {
            if (var == WORST) {
                //BOTTOM_UP
                FillRandomArray(a, size, 1, 50000, false, ASCENDING);
                bottom_up(a, size);
                //TOP_DOWN
                FillRandomArray(a, size, 1, 50000, false, ASCENDING);
                top_down(a, size);
            }
            else {
                FillRandomArray(a, size, 1, 50000, false, UNSORTED);
                //BOTTOM_UP
                CopyArray(dummy, a, size);
                bottom_up(dummy, size);
                //TOP_DOWN
                CopyArray(dummy, a, size);
                top_down(dummy, size);
            }
        }
    }

    prof.divideValues("bottom_up_assign", NR_EXP);
    prof.divideValues("bottom_up_comp", NR_EXP);

    prof.divideValues("top_down_assign", NR_EXP);
    prof.divideValues("top_down_comp", NR_EXP);

    prof.addSeries("bottom_up_operation", "bottom_up_assign", "bottom_up_comp");
    prof.addSeries("top_down_operation", "top_down_assign", "top_down_comp");

    prof.createGroup("assign", "bottom_up_assign", "top_down_assign");
    prof.createGroup("comp", "bottom_up_comp", "top_down_comp");
    prof.createGroup("operation", "bottom_up_operation", "top_down_operation");
}

void perf_all()
{
    perf(AVERAGE);
    prof.reset("WORST");
    perf(WORST);
    prof.showReport();
}

int main()
{
    demo_bottom_up();
    demo_top_down();
    demo_heap_sort();
    //perf_all();
}
