/*
     Gozman-Pop Maria-Eliza
     Group 30424
     Selection sort iterative vs selection sort recursive

     In acest program am implementat functia de sortare selection in doua moduri: iterativ si recursiv. Functia de selection sort 
    functioneaza astfel: imparte array-ul in doua parti (una sortata si una nesortata), si alege mereu minimul din partea nesortata si il
    adauga la finalul partii sortate, interschimband elementele. Am comparat cele doua motive pentru cazul AVERAGE (media a 5 experimente), 
    si se poate observa din grafic ca sunt aproape identice. De asemenea, am mai comparat si timpul de rulare ale celor doua programe, si 
    din valori reiese ca modalitatea iterativa este putin mai eficienta decat cea recursiva.
*/

#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler prof("AVERAGE");

void SelectionSortIterative(int *array, int size)
{
    //Operation comp_it = prof.createOperation("iterative_sort_comp", size);
    //Operation assign_it = prof.createOperation("iterative_sort_assign", size);

    int i, j, pmin;
    for (i = 0; i < size; i++) 
    {
        pmin = i;
        for (j = i + 1; j < size; j++) 
        {
            //comp_it.count();
            if (array[j] < array[pmin])
                pmin = j;
        }
        if (pmin != i) 
        {
            //assign_it.count(3);
            swap(array[pmin], array[i]);
        }
    }
}

int FindMin(int* array, int pivot, int size, int min_ind, Operation comp_rec)
{
    if (pivot == (size - 1))
        return min_ind;
    comp_rec.count();
    if (array[min_ind] > array[pivot])
        min_ind = pivot;
    FindMin(array, pivot + 1, size, min_ind, comp_rec);
}

int FindMin2(int* array, int pivot, int size, int min_ind)
{
    if (pivot == (size - 1))
        return min_ind;
    if (array[min_ind] > array[pivot])
        min_ind = pivot;
    FindMin2(array, pivot + 1, size, min_ind);
}

void SelectionSortRecursive(int *array, int pivot, int size)
{
    Operation assign_rec = prof.createOperation("recursive_sort_assign", size);
    Operation comp_rec = prof.createOperation("recursive_sort_comp", size);

    if (pivot > (size-1))
        return;
    int min_ind = pivot;
    min_ind = FindMin(array, pivot, size, min_ind, comp_rec);
    if(pivot!=min_ind)
    {
        swap(array[pivot], array[min_ind]);
        assign_rec.count(3);
    }
    SelectionSortRecursive(array, pivot + 1, size); 
}

void SelectionSortRecursive2(int* array, int pivot, int size)
{
    if (pivot > (size - 1))
        return;
    int min_ind = pivot;
    min_ind = FindMin2(array, pivot, size, min_ind);
    if (pivot != min_ind)
        swap(array[pivot], array[min_ind]);
    SelectionSortRecursive2(array, pivot + 1, size);
}

void perf()
{
    int array[50001];
    int size;
    int start = 100;
    int end = 10000;
    int increment = 100;
    int dummy[50001];
    int NR_EXP = 5;

    for (int experiments = 0; experiments < NR_EXP; experiments++)
    {
        for (size = start; size <= end; size += increment)
        {
            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            //ITERATIVE SORT
            CopyArray(dummy, array, size);
            SelectionSortIterative(dummy, size);
            //RECURSIVE SORT
            SelectionSortRecursive(array, 0, size);
        }
    }

    prof.divideValues("iterative_sort_assign", NR_EXP);
    prof.divideValues("iterative_sort_comp", NR_EXP);

    prof.divideValues("recursive_sort_assign", NR_EXP);
    prof.divideValues("recursive_sort_comp", NR_EXP);

    prof.addSeries("iterative_sort_operation", "iterative_sort_assign", "iterative_sort_comp");
    prof.addSeries("recursive_sort_operation", "recursive_sort_assign", "recursive_sort_comp");

    prof.createGroup("assign", "iterative_sort_assign", "recursive_sort_assign");
    prof.createGroup("comp", "iterative_sort_comp", "recursive_sort_comp");
    prof.createGroup("operation", "iterative_sort_operation", "recursive_sort_operation");

    prof.showReport();
}

void perf_time()
{
    int nr_tests = 30;
    int array[10001];
    int size = 10000;
    int start = 100;
    int end = 10000;
    int increment = 100;
    int dummy[10001];

    FillRandomArray(array, size, 10, 50000, false, UNSORTED);
    CopyArray(dummy, array, size);
   
    for (size = start; size <= end; size += increment)
    {
        prof.startTimer("recursive",  size);
        for (int test = 0; test < nr_tests; ++test) 
        {
            SelectionSortRecursive2(array, 0, size);
        }
        prof.stopTimer("recursive", size);
    }

    for (size = start; size <= end; size += increment)
    {
        prof.startTimer("iterative", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            SelectionSortRecursive2(dummy, 0, size);
        }
        prof.stopTimer("iterative", size);
    }

    prof.showReport();
}

void printArray(int array[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        cout << array[i] << " ";
    cout << endl;
}

void demoRecursive()
{
    int array[] = { 4, 7, 3, 9, 5, 1, 6, 8 };
    int size = sizeof(array) / sizeof(array[0]);
    SelectionSortRecursive2(array, 0, size);
    cout << "The recursive result is: ";
    printArray(array, size);
}

void demoIterative()
{
    int array[] = { 4, 7, 3, 9, 5, 1, 6, 8 };
    int size = sizeof(array) / sizeof(array[0]);
    cout << "The iterative result is: ";
    SelectionSortIterative(array, size);
    printArray(array, size);
}

int main()
{
    demoRecursive();
    demoIterative();
    //perf();
    //perf_time();
}
