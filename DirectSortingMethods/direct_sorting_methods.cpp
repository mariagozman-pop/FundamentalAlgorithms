/* Gozman - Pop Maria - Eliza
   Group 30424
   Direct Sorting Methods

   -------------------------------
    Am implementat 3 tipuri de sortari directe ale unui array (inserion sort, selection sort si bubble sort).

    Primul tip de sortare (insertion) "imparte" sirul in 2 parti:una sortata si una nesortata. Prin asta intelegem ca luam
   fiecare element din partea stanga pe rand si sa ii gasim locul respunzator din partea dreapta. Atata timp cat elementul
   cu care comparam bufferul este mai mare, il vom shifta la dreapta pentru a elibera o pozitie pentru "noul" termen.
    Al doilea tip de sortare (selection) contine, de asemenea, un zid imaginar si constituie opusul primei sortari: cautam
   de fiecare data minimul "de dupa zid" si il mutam la finalul sirului din dreapta. Vom lucra cu pozitiile elementelor
   pentru a face algoritmul mai eficient. Dupa ce gasim pozitia minimului verifcam daca nu este pozitia elementului de la 
   care am inceput, caz in care nu trebuie schimbata, si interschimbam elementele.
    Implementarea ultimului tip de sortare (bubble) este una mai eficienta decat cea clasica (cu doua for-uri), folosind 
   comanda do while in felul urmator: conditia de continuare a buclei este ca algoritmul sa nu fie sortat, si asftel 
   verificam daca este macar necesara sortarea. Atata timp cat gasim in sir un element mai mare decat cel urmator, 
   variabila sorted devine falsa si inteschimbam elementele. Vom scadea size-ul dupa fiecare for pentru ca maximul se va
   afla pe ultima pozitie dupa realizarea swap-urilor.

    Am impartit graficele pe cazuri: BEST, AVERAGE si WORST, si le-am ales corespunzator pentru tipul de rezolvare. La cazul
   BEST este destul de clar ca vectorul trebuie sa fie sortat, astfel nu trebuie sa modificam nimic. Pentru cazul AVERAGE am 
   facut media a 5 exprimente diferite pentru a avea un rezultat cat mai clar, si array-ul este generat random de fiecare
   data, cu conditia de a fi nesortat. La WORST case a fost nevoie de un caz particular pentru selection sort: ascending cu 
   conditia ca primul termen este maxim pentru a face cat mai multe swap-uri.

    Din grafice observam ca pentru BEST case, cea mai eficienta metoda este bubble sort, pentru AVERAGE case insertion sort 
   si selection sort sunt cele mai bune si aproximativ la fel de eficiente, iar la WORST case selection sort este cea optima.
*/

#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler prof("BEST");

void insertionSort(int *a, int size) {

    Operation comp_ins = prof.createOperation("ins_comp", size);
    Operation assign_ins = prof.createOperation("ins_assign", size);
    
    int i, j, buffer;
    for (i = 1; i < size; i++) {
        buffer = a[i];
        assign_ins.count();
        j = i - 1;
        while (j > -1 && a[j] > buffer){
            comp_ins.count();
            a[j + 1] = a[j];
            assign_ins.count();
            j--;
        }
        comp_ins.count();
        a[j + 1] = buffer;
        assign_ins.count();
    }
}

void selectionSort(int *a, int size) {

    Operation comp_sel = prof.createOperation("sel_comp", size);
    Operation assign_sel = prof.createOperation("sel_assign", size);

    int i, j, pmin;
    for (i = 0; i < size; i++) {
        pmin = i;
        for (j = i + 1; j < size; j++) {
            comp_sel.count();
            if (a[j] < a[pmin])
                pmin = j;
        }
        if (pmin != i) {
            swap(a[pmin], a[i]);
            assign_sel.count(3);
        }
    }

}

void bubbleSort(int *a, int size) {

    Operation comp_bub = prof.createOperation("bub_comp", size);
    Operation assign_bub = prof.createOperation("bub_assign", size);

    int i;
    bool sorted;
    do {
        sorted = true;
        for (i = 0; i < size - 1; i++){
            comp_bub.count();
            if (a[i] > a[i + 1]) {
                sorted = false;
                swap(a[i], a[i + 1]);
                assign_bub.count(3);
            }
        }
        size--;
    } while (!sorted);
}

void printArray(int *a, int size) {

    for (int i = 0; i < size; i++)
        cout << a[i] << " ";
    cout << endl;
}

void demo() {

    int a[] = { 4, 7, 2, 5, 9, 1 , 3, 4};
    int size = sizeof(a) / sizeof(a[0]);
    int dummy[100];

    CopyArray(dummy, a, size);
    insertionSort(dummy, size);
    printArray(dummy, size);

    CopyArray(dummy, a, size);
    selectionSort(dummy, size);
    printArray(dummy, size);

    CopyArray(dummy, a, size);
    bubbleSort(dummy, size);
    printArray(dummy, size);
}

enum CASES{ AVERAGE, BEST, WORST };

void choose(CASES var) {

    int a[50001];
    int size;
    int start = 100;
    int end = 10000;
    int increment = 100;
    int dummy[50001];
    int NR_EXP = 1;
    if (var == AVERAGE)
        NR_EXP = 5;
    for (int experiments = 0; experiments < NR_EXP; experiments++)
    {
        for (size = start; size <= end; size += increment) {
            if (var == BEST) {
                //insertion sort
                FillRandomArray(a, size, 1, 50000, false, ASCENDING);
                insertionSort(a, size);
                //selection sort
                FillRandomArray(a, size, 1, 50000, false, ASCENDING);
                selectionSort(a, size);
                //bubble sort
                FillRandomArray(a, size, 1, 50000, false, ASCENDING);
                bubbleSort(a, size);
            }
            else if (var == WORST) {
                //insertion sort
                FillRandomArray(a, size, 1, 50000, false, DESCENDING);
                insertionSort(a, size);
                //selection sort
                FillRandomArray(a, size, 1, 49999, false, ASCENDING);
                a[0] = 50000;
                selectionSort(a, size);
                //bubble sort
                FillRandomArray(a, size, 1, 50000, false, DESCENDING);
                bubbleSort(a, size);
            }
            else {
                //insertion sort
                FillRandomArray(a, size, 1, 50000, false, UNSORTED);
                CopyArray(dummy, a, size);
                insertionSort(dummy, size);
                //selection sort
                CopyArray(dummy, a, size);
                selectionSort(dummy, size);
                //bubble sort
                CopyArray(dummy, a, size);
                bubbleSort(dummy, size);
            }
        }
    }
    prof.divideValues("ins_assign", NR_EXP);
    prof.divideValues("ins_comp", NR_EXP);

    prof.divideValues("sel_assign", NR_EXP);
    prof.divideValues("sel_comp", NR_EXP);

    prof.divideValues("bub_assign", NR_EXP);
    prof.divideValues("bub_comp", NR_EXP);

    prof.addSeries("ins_operation", "ins_assign", "ins_comp");
    prof.addSeries("sel_operation", "sel_assign", "sel_comp");
    prof.addSeries("bub_operation", "bub_assign", "bub_comp");

    prof.createGroup("assign", "ins_assign", "sel_assign", "bub_assign");
    prof.createGroup("comp", "ins_comp", "sel_comp", "bub_comp");
    prof.createGroup("operation", "ins_operation", "sel_operation", "bub_operation");
}

void perf_all(){

    choose(BEST);
    prof.reset("AVERAGE");
    choose(AVERAGE);
    prof.reset("WORST");
    choose(WORST);
    prof.showReport();
}

int main()
{
    demo();
    //perf_all();
}