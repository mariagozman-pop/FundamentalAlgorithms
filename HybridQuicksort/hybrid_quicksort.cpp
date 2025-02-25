/*
    Gozman-Pop Maria-Eliza
    Group 30424
    Hybrid OuickSort
    --------------------------------
        In this code, I have implemented a HybridQuickSort, that uses QuickSort and InsertionSort to make a more efficient type of sort. The classic QuickSort algorithm is implemented with a partition function to sort 
    the array recursively, sorting the subarrays divided around a pivot element chosen from the partition function. The insertion sort is included for the small subarrays that are under the tested threshold that we 
    found by experimentig. The optimal threshold I observed from the graphic is 12-13 if we are talking about the number of operations, but time-wise, the larger the threshold, the better.
*/

#include <iostream>
#include "Profiler.h"

Profiler prof("OPERATIONS");

using namespace std;

int Partition(int* a, int left, int right, Operation assign1, Operation comp1)
{
    int pivot = right;
    int i = left - 1;
    for (int j = left; j < right; j++)
    {
        comp1.count();
        if (a[j] <= a[pivot])
        {
            i++;
            assign1.count(3);
            swap(a[i], a[j]);
        }
    }
    assign1.count(3);
    swap(a[i + 1], a[right]);
    return i + 1;
}

int PartitionDemo(int* a, int left, int right)
{
    int pivot = right;
    int i = left - 1;
    for (int j = left; j < right; j++)
    {
        if (a[j] <= a[pivot])
        {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[right]);
    return i + 1;
}

void QuickSort(int* a, int left, int right, Operation QS_assign, Operation QS_comp)
{
    int pivot;
    if (left < right)
    {
        pivot = Partition(a, left, right, QS_assign, QS_comp); 
        QuickSort(a, left, pivot - 1, QS_assign, QS_comp); 
        QuickSort(a, pivot + 1, right, QS_assign, QS_comp); 
    }
}

void QuickSortDemo(int* a, int left, int right)
{
    int pivot;
    if (left < right)
    {
        pivot = PartitionDemo(a, left, right);
        QuickSortDemo(a, left, pivot - 1);
        QuickSortDemo(a, pivot + 1, right);
    }
}

void insertionSort(int* a, int left, int right, Operation assign2, Operation comp2) 
{
    int i, j, buffer;
    for (i = left; i <= right; i++) {
        buffer = a[i];
        assign2.count();
        j = i - 1;
        while (j >= 0 && a[j] > buffer) {
            comp2.count();
            a[j + 1] = a[j];
            assign2.count();
            j--;
        }
        a[j + 1] = buffer;
        assign2.count();
    }
}

void insertionSortDemo(int* a, int left, int right)
{
    int i, j, buffer;
    for (i = left; i <= right; i++) {
        buffer = a[i];
        j = i - 1;
        while (j > -1 && a[j] > buffer) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = buffer;
    }
}

void HybridQuickSort(int* a, int left, int right, int threshold, Operation HQS_assign, Operation HQS_comp)
{
    if (left < right)
    {
        if (right - left + 1 < threshold)
        {
            insertionSort(a, left, right, HQS_assign, HQS_comp);
            return;
        }
        else
        {
            int pivot = Partition(a, left, right, HQS_assign, HQS_comp);
            HybridQuickSort(a, left, pivot - 1, threshold, HQS_assign, HQS_comp);
            HybridQuickSort(a, pivot + 1, right, threshold, HQS_assign, HQS_comp);
        }
    }
}

void HybridQuickSortDemo(int* a, int left, int right, int threshold)
{
    if (left < right)
    {
        if (right - left + 1 < threshold)
        {
            insertionSortDemo(a, left, right);
            return;
        }
        else
        {
            int pivot = PartitionDemo(a, left, right);
            HybridQuickSortDemo(a, left, pivot - 1, threshold);
            HybridQuickSortDemo(a, pivot + 1, right, threshold);
        }
    }
}

void printArray(int array[], int size)
{
    for (int i = 0; i < size; i++)
        cout << array[i] << " ";
    cout << endl;
}

void demo()
{
    int array[] = { 23, 42, 81, 57, 15, 36, 92, 9, 62, 91, 54, 73, 44, 10, 71, 76, 35, 13, 94, 3, 86, 65, 74, 96, 31, 8, 88, 97, 47, 28, 66, 58, 99, 82, 69, 11, 49, 89, 7,
        52, 26, 19, 39, 80, 95, 16, 30, 77, 68, 51, 34, 98, 46, 85, 20, 4, 61, 72, 22, 50, 90, 12, 83, 63, 32, 38, 75, 59, 21, 93, 67, 60, 25, 18, 48, 2, 79, 84, 14, 87, 5,
        70, 24, 64, 40, 1, 6, 78, 53, 33, 45, 55, 41, 29, 56, 37, 27, 43, 17, 100 };
    int dummy[] = { 23, 42, 81, 57, 15, 36, 92, 9, 62, 91, 54, 73, 44, 10, 71, 76, 35, 13, 94, 3, 86, 65, 74, 96, 31, 8, 88, 97, 47, 28, 66, 58, 99, 82, 69, 11, 49, 89, 7, 
        52, 26, 19, 39, 80, 95, 16, 30, 77, 68, 51, 34, 98, 46, 85, 20, 4, 61, 72, 22, 50, 90, 12, 83, 63, 32, 38, 75, 59, 21, 93, 67, 60, 25, 18, 48, 2, 79, 84, 14, 87, 5,
        70, 24, 64, 40, 1, 6, 78, 53, 33, 45, 55, 41, 29, 56, 37, 27, 43, 17, 100 };
    int size = sizeof(array) / sizeof(array[0]);

    QuickSortDemo(array, 0, size - 1); 
    cout << "Quick sort result: ";
    printArray(array, size);

    HybridQuickSortDemo(dummy, 0, 99, 13);
    cout << "Hybrid quick sort result: ";
    printArray(dummy, 100);
}

void perf()
{
    int nr_exp = 5;
    int start = 100;
    int end = 10000;
    int increment = 100;
    int array[10001] = { 0 };
    int dummy[10001] = { 0 };
   
    for (int i = start; i <= end; i += increment)
    {
        Operation QS_assign = prof.createOperation("QuickSort assign", i);
        Operation QS_comp = prof.createOperation("QuickSort comp", i);

        Operation HQS_assign = prof.createOperation("HybridQuickSort assign", i);
        Operation HQS_comp = prof.createOperation("HybridQuickSort comp", i);

        for (int experiments = 0; experiments < nr_exp; experiments++)
        {
            FillRandomArray(array, i, 1, 50000, false, UNSORTED);
            CopyArray(dummy, array, i);

            QuickSort(array, 0, i - 1, QS_assign, QS_comp);

            HybridQuickSort(dummy, 0, i - 1, 13, HQS_assign, HQS_comp);
        }
    }

    prof.divideValues("QuickSort assign", nr_exp);
    prof.divideValues("QuickSort comp", nr_exp);
    prof.addSeries("QuickSort operations", "QuickSort assign", "QuickSort comp");

    prof.divideValues("HybridQuickSort assign", nr_exp);
    prof.divideValues("HybridQuickSort comp", nr_exp);
    prof.addSeries("HybridQuickSort operations", "HybridQuickSort assign", "HybridQuickSort comp");

    prof.createGroup("Operations", "QuickSort operations", "HybridQuickSort operations");
    prof.createGroup("Assignments", "QuickSort assign", "HybridQuickSort assign");
    prof.createGroup("Comparisons", "QuickSort comp", "HybridQuickSort comp");

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
        prof.startTimer("QuickSort", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];
            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            QuickSortDemo(array, 0, size - 1);
        }
        prof.stopTimer("QuickSort", size);
    }

    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("HybridQuickSort", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];
            FillRandomArray(array, size, 10, 50000, false, UNSORTED); 
            HybridQuickSortDemo(array, 0, size - 1, 30);
        }
        prof.stopTimer("HybridQuickSort", size);
    }

    prof.createGroup("TIME", "QuickSort", "HybridQuickSort");

    prof.showReport();
}

void perf_threshold()
{
    int start = 3;
    int end = 40;
    int array[10001];
    int dummy[10001];
    int nr_exp = 10;
    int go = 100;
    int stop = 10000;
    int increment = 100;

    prof.reset("THRESHOLD");

    for (int experiments = 0; experiments < nr_exp; experiments++)
    {
        for (int size = go; size <= stop; size += increment)
        {
            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            for (int threshold= start; threshold <= end; threshold++)
            {
                CopyArray(dummy, array, size);

                Operation assign = prof.createOperation("HybridQuickSort assign", threshold); 
                Operation comp = prof.createOperation("HybridQuickSort comp", threshold); 

                HybridQuickSort(dummy, 0, size - 1, threshold, assign, comp);  
            }
        }
    }

    prof.divideValues("HybridQuickSort assign", nr_exp);
    prof.divideValues("HybridQuickSort comp", nr_exp);

    prof.addSeries("HybridQuickSort operations", "HybridQuickSort assign", "HybridQuickSort comp");

    prof.showReport();
}

void perf_threshold_time()
{
    int start = 3;
    int end = 40;
    int nr_exp = 10;
    int go = 100;
    int stop = 10000;
    int increment = 100;

    prof.reset("THRESHOLD_TIME");
    for (int size = go; size <= stop; size += increment) 
    {
        for (int threshold = start; threshold <= end; threshold++)
        {
            prof.startTimer("thr_time", threshold); 
            for (int test = 0; test < nr_exp; ++test)
            {
                int* array = new int[size];

                FillRandomArray(array, size, 10, 50000, false, UNSORTED);
                HybridQuickSortDemo(array, 0, size - 1, threshold);

            }
            prof.stopTimer("thr_time", threshold);
        }
    }

    prof.divideValues("HybridQuickSort assign", nr_exp);
    prof.divideValues("HybridQuickSort comp", nr_exp);

    prof.addSeries("HybridQuickSort operations", "HybridQuickSort assign", "HybridQuickSort comp");

    prof.showReport();
}

void perf_threshold_time2()
{
    int nr_tests = 100;
    int start = 100;
    int end = 10000;
    int increment = 100;

    prof.reset("THRESHOLD TIME");

    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("10", size); 
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];

            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            HybridQuickSortDemo(array, 0, size - 1, 10);

        }
        prof.stopTimer("10", size);
    }

    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("12", size); 
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];

            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            HybridQuickSortDemo(array, 0, size - 1, 12);

        }
        prof.stopTimer("12", size);
    }

    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("14", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];

            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            HybridQuickSortDemo(array, 0, size - 1, 14);

        }
        prof.stopTimer("14", size);
    }

    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("16", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];

            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            HybridQuickSortDemo(array, 0, size - 1, 16);

        }
        prof.stopTimer("16", size);
    }
    
    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("18", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];

            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            HybridQuickSortDemo(array, 0, size - 1, 18);

        }
        prof.stopTimer("18", size);
    }

    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("20", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];

            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            HybridQuickSortDemo(array, 0, size - 1, 20);

        }
        prof.stopTimer("20", size);
    }

    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("22", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];

            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            HybridQuickSortDemo(array, 0, size - 1, 22);

        }
        prof.stopTimer("22", size);
    }

    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("24", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];

            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            HybridQuickSortDemo(array, 0, size - 1, 24);

        }
        prof.stopTimer("24", size);
    }

    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("26", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];

            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            HybridQuickSortDemo(array, 0, size - 1, 26);

        }
        prof.stopTimer("26", size);
    }

    for (int size = start; size <= end; size += increment)
    {
        prof.startTimer("28", size);
        for (int test = 0; test < nr_tests; ++test)
        {
            int* array = new int[size];

            FillRandomArray(array, size, 10, 50000, false, UNSORTED);
            HybridQuickSortDemo(array, 0, size - 1, 28);

        }
        prof.stopTimer("28", size);
    }

    prof.createGroup("THRESHOLD TIME", "10", "12", "14", "16", "18", "20", "22", "24", "26", "28");

    prof.showReport();
}

int main()
{
    demo();
    //perf();
    //perf_time();
    //perf_threshold();
    //perf_threshold_time(); 
}
