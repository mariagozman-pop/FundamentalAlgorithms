/*
   Gozman-Pop Maria-eliza
   Group 30424
   QuickSort vs HeapSort

    In acest program am implementat heap sort si quick sort si le-am comparat. Heap-sort foloseste functiile heapify si build heap 
   (implementate in tema anterioara), si retinem elementul cu index 0 (elementul maxim) punandu-l la coada. Dupa aceea, aplicam functia 
   heapify pentru a nu pierde proprietatea heap-ului (de a avea elementul maxim ca radacina), dar pentru size--, deoarece pe ultima 
   pozitie am retinut maximul. Procedura se repeta pana ajungem la un array sortat crescator. Quick-Sort ul alege un pivot pe care il
   compara cu fiecare element din array si le vom inteschimba astfel incat in stanga vom avea numerele mici decat pivotul si in dreapta le
   vom aveam pe cele mai mari => pivotul va fi mereu in pozitia corecta. Va trebui sa facem asta in mod recursiv pe partitiile formate de
   fiecare data, pana ajungem la array-ul sortat. 
    Am comparat cele doua moduri de sortatre pe toate cazurile (BEST, AVERAGE, WORST), si rezultatele sunt: pentru cazul BEST, heap-sort este
   mai eficient, pentru cazul WORST, la fel, iar in cazul AVERAGE, quick-sort este mai eficient. 
*/

#include <iostream>
#include <cstdlib> 
#include <ctime> 

#include "Profiler.h"

using namespace std;

Profiler prof("BEST"); 

int Partition(int *a, int size, int left, int right)
{
    Operation comp_part = prof.createOperation("quick_sort_comp", size);
    Operation assign_part = prof.createOperation("quick_sort_assign", size);

    int pivot = right;
    int i = left-1;
    for (int j = left; j < right; j++)
    {
        comp_part.count();
        if (a[j] <= a[pivot])
        {
            i++;
            assign_part.count(3);
            swap(a[i], a[j]);
        }
    }
    assign_part.count(3);
    swap(a[i + 1], a[right]);
    return i + 1;
}

int PartitionBest(int* a, int size, int left, int right)
{
    Operation comp_part = prof.createOperation("quick_sort_comp", size);
    Operation assign_part = prof.createOperation("quick_sort_assign", size);

    int pivot = right;
    swap(a[(left+right)/2], a[pivot]);
    int i = left - 1;
    for (int j = left; j < right; j++)
    {
        comp_part.count();
        if (a[j] <= a[pivot])
        {
            i++;
            assign_part.count(3);
            swap(a[i], a[j]);
        }
    }
    assign_part.count(3);
    swap(a[i + 1], a[right]);
    return i + 1;
}

void QuickSort(int *a, int size, int left, int right)
{
    int pivot;
    if (left < right)
    {
        pivot = Partition(a, size, left, right);
        QuickSort(a, size,left, pivot - 1);
        QuickSort(a, size, pivot + 1, right);
    }
}

void QuickSortBest(int* a, int size, int left, int right)
{
    int pivot;
    if (left < right)
    {
        pivot = PartitionBest(a, size, left, right);
        QuickSortBest(a, size, left, pivot - 1);
        QuickSortBest(a, size, pivot + 1, right);
    }
}

void Heapify(int* h, int heap_size, int i, Operation comp_bott, Operation assign_bott)
{
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int max = i;
    if (left < heap_size && h[i] < h[left])
        max = left;
    if (left < heap_size)
        comp_bott.count();
    if (right < heap_size && h[max] < h[right])
        max = right;
    if (right < heap_size)
        comp_bott.count();
    if (max != i)
    {
        assign_bott.count(3);
        swap(h[i], h[max]);
        Heapify(h, heap_size, max, comp_bott, assign_bott);
    }
}

void BuildHeap(int* h, int heap_size, Operation comp_bott, Operation assign_bott)
{
    int i;
    for (i = heap_size / 2 - 1; i >= 0; i--)
        Heapify(h, heap_size, i, comp_bott, assign_bott);
}

void HeapSort(int* array, int array_size)
{
    Operation comp_bott = prof.createOperation("heap_sort_comp", array_size);
    Operation assign_bott = prof.createOperation("heap_sort_assign", array_size);
    BuildHeap(array, array_size, comp_bott, assign_bott);
    for (int i = array_size - 1; i >= 1; i--) {
        assign_bott.count(3);
        swap(array[0], array[i]);
        Heapify(array, i, 0, comp_bott, assign_bott);
    }
}

void printArray(int array[], int size)
{
    for (int i = 0; i < size; i++)
        cout << array[i]<<" ";
    cout << endl;
}

void demoQuickSort()
{
    int array[] = { 4, 7, 3, 1, 2, 5, 6, 9 };
    int size = sizeof(array) / sizeof(array[0]);
    QuickSort(array, size, 0, size - 1);
    cout << "Quick sort result: ";
    printArray(array, size);
}

void demoHeapSort()
{
    int array[] = { 4, 7, 3, 1, 2, 5, 6, 9 };
    int size = sizeof(array) / sizeof(array[0]);
    HeapSort(array, size);
    cout << "Heap sort result: ";
    printArray(array, size);
}

enum CASES { BEST, AVERAGE, WORST };

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
                //HEAP SORT
                FillRandomArray(a, size, 10, 50000, false, ASCENDING);
                HeapSort(a, size);
                //QUICK SORT
                FillRandomArray(a, size, 10, 50000, false, DESCENDING);
                QuickSort(a, size, 0, size - 1);
            }
            else if(var == BEST){
                //HEAP SORT
                FillRandomArray(a, size, 1, 2, false, ASCENDING);
                for (int i = 0; i <= size; i++)
                    if (a[i] == 2)
                        a[i] = 1;
                HeapSort(a, size);
                //QUICK SORT
                FillRandomArray(a, size, 10, 50000, true, ASCENDING);
                QuickSortBest(a, size, 0, size - 1);
            }
            else {
                FillRandomArray(a, size, 1, 50000, false, UNSORTED);
                //HEAP SORT
                CopyArray(dummy, a, size);
                HeapSort(dummy, size);
                //QUICK SORT
                CopyArray(dummy, a, size);
                QuickSort(dummy, size, 0, size);
            }
        }
    }

    prof.divideValues("quick_sort_assign", NR_EXP);
    prof.divideValues("quick_sort_comp", NR_EXP);

    prof.divideValues("heap_sort_assign", NR_EXP);
    prof.divideValues("heap_sort_comp", NR_EXP);

    prof.addSeries("quick_sort_operation", "quick_sort_assign", "quick_sort_comp");
    prof.addSeries("heap_sort_operation", "heap_sort_assign", "heap_sort_comp");

    prof.createGroup("assign", "quick_sort_assign", "heap_sort_assign");
    prof.createGroup("comp", "quick_sort_comp", "heap_sort_comp");
    prof.createGroup("operation", "quick_sort_operation", "heap_sort_operation");
}

void perf_all()
{
    perf(BEST);
    prof.reset("WORST");
    perf(WORST);
    prof.reset("AVERAGE");
    perf(AVERAGE);
    prof.showReport();
}

int main()
{
    demoQuickSort();
    demoHeapSort();
    //perf_all();
}
