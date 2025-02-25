/*
    Gozman-Pop Maria-Eliza
    Group 30414
    Hashtables
    ----------------------------------------
       In acest cod am implementat functiile INSERT, SEARCH si DELETE care lucreaza pe hashtables. In primul rand am creat structura unui
    entry (care contine id-ul si numele), si am declarat hashtable-ul ca dublu pointer. Pentru a crea un entry, am folosit functia
    "create_ent" care primeste ca parametrii id-ul si numele si retuneaza entry-ul. Pentru a insera elementele in tabel am avut nevoie de
    o functie de "hash" (quadratic hashing), care sa le distribuie cat mai distantate in tabel, pentru a evita gruparile mari de elemente
    (clusterings), care duc la o eficienta mai scazuta. Functia primeste ca parametrii numarul maxim de elemente din hashtable, id-ul pe
    care vrem sa il inseram si un contor cu ajutorul caruia vom rezolva coloziunile. Cand index-ul gasit este deja ocupat in tabel,
    contorul va creste si va recalcula hash-ul (asta se va intampla pana gasim un loc liber).
       Functia de INSERT preia un entry si ii calculeaza hash-ul si verifica daca locul respectiv este ocupat in tabel: daca este, il
    insereaza, iar daca nu, counter-ul rec va creste si va recalcula hash-ul (pana gaseste un loc liber sau pana afla ca nu mai are loc
    in tabel).
       Functia de SEARCH este foarte asemenatoare celei de INSERT. Primeste ca parametru id-ul entry-ului, ii calculeaza hash-ul si daca
    locul gasit este null, inseamna ca elementul nu se afla in tabel (inseamna ca niciun element cu hsh-ul initial = index-ul nu a fost
    inserat, asa ca nu are sens sa continuam cautarea) si functia returneaza -1. Daca id-ul este gasit, returnam pozitia lui in tabel,
    iar daca gasim alt id, recalculam hash-ul asa cum am procedat in INSERT.
       Functia de DELETE se foloseste de cea de SEARCH, cauta index-ul corespunzator, si verifica daca acesta exista in tabel (daca
    functia a returnat ceva diferit de -1). Daca acesta este cazul, ii atribuie id-ului entry-ului valoarea -1 (si sterge numele), care
    marcheaza ca locul este gol, dar a fost ocupat la un moment dat, pentru a nu strica functia de SEARCH (care se bazeaza pe
    continuitatea elementelor), insa in functia de insert vom tine cont ca locul este eliberat si poate fi ocupat.
       Evaluarea eficientei programului este legata de numarul de calculari al hash-ului unui index, pe care l-am privit din doua puncte
    de vedere: cautand elemente care se afla in tabel (100%), si cautand elemente la care avem certitudinea ca nu se afla in tabel.
    Pentru acestea am aflat numarul mediu si numarul maxim de recalculari. La aceste cerinte se adauga filling factor-ul care trebuie
    luat in considerare, si l-am schimbat pe parcursul programului, luand valorile: 0.80, 0.85, 0.90, 0.95 si 0.99. 
       Pentru elementele gasite, am generat un vector in mod random, din intervalul [20000000, 45000000] si de lungime N*filling_factor 
    (numarul de elemente necesare care trebuiesc inserate), iar valorile acestuia vor reprezenta id-urile entry-urilor. Dupa ce le-am 
    inserat, urmeaza sa le cautam, retinand numarul de recalculari, pe care l-am salvat intr-un vector (pentru fiecare element cautat). 
    Pentru a ma asigura ca acele 1500 de entry-uri cautate sunt uniform distribuite in tabel, am impartit hashtable-ul in 1500 de parti 
    aproximativ egale, si am ales un index random din fiecare partitie. Am adaugat o bucla de cautare, din care nu se iese pana cand 
    index-ul generat are un element care ii corespunde, si care nu e null sau sters (sau pana cand am trecut de range*range ori prin 
    acel interval, fapt ce ar sugera ca intervalul este gol - evitam bucla infinita). 
       Pentru elementele pe care nu se pot afla, am generat un alt array random, dar cu elemente in intervalul [45000001, 50000000], 
    care, cu siguranta, nu se afla in tabel, deoarece nu le-am inserat. In acelasi fel, le vom cauta si le vom retine numarul de 
    recalculari. La final am aflat media celor doi vectori si maximul sau, elemente care se vor regasi in tabelul final de performanta.
       In mod asemanator am analizat si performanta pentru functia DELETED, am sters elemente random din hashtable, incepand de la un 
    filling factor de 0.99, pana la unul de 0.80, apoi am cautat din nou elemente care se afla in el si elemente care nu. La final, 
    valorie sunt aproximatov la fel ca cele de la filling factorul de 0.99 (deoarece chiar daca au fost sterse, funtcia de search inca 
    le ia in considerare).
*/

#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
#include "Profiler.h"

using namespace std;

Profiler prof("HASHTABLES");

void fillRandomArray(int* array, int size, int minValue, int maxValue) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(minValue, maxValue);

    for (int i = 0; i < size; ++i) {
        array[i] = distribution(gen);
    }
}

typedef struct
{
    int id;
    char name[30];
}h_entry;

h_entry** hashtable;

h_entry* create_ent(int id, const char* name)
{
    h_entry* ent = (h_entry*)malloc(sizeof(h_entry));
    ent->id = id;
    snprintf(ent->name, sizeof(ent->name), "%s", name);

    return ent;
}

int hashfct(int N, int id, int rec)
{
    int index;
    int rec_2 = rec * rec;
    index = (id + rec * 3 + 5 * rec_2) % N;
    return index;
}

void insert(h_entry* ent, int N)
{
    int rec = 0;
    int index;
    do
    {
        index = hashfct(N, ent->id, rec);
        if (hashtable[index] == NULL || hashtable[index]->id == -1)
        {
            hashtable[index] = ent;
            return;
        }
        rec++;
    } while (rec < N);
}

int search(int id, int N)
{
    int rec = 0;
    int index;
    int not_found = -1;
    do
    {
        index = hashfct(N, id, rec);
        if (hashtable[index] == NULL)
            return not_found;
        if (hashtable[index]->id == id)
            return index;
        rec++;
    } while (rec < N);
    return not_found;
}

int search_perf(int id, int N)
{
    int rec = 0;
    int index;
    int not_found = -1;
    do
    {
        index = hashfct(N, id, rec);
        if (hashtable[index] == NULL)
            return rec;
        if (hashtable[index]->id == id)
            return rec;
        rec++;
    } while (rec < N);
    return rec;
}

void delete_e(int id, int N)
{
    int index = search(id, N);
    if (index != -1)
    {
        hashtable[index]->id = -1;
        memset(hashtable[index]->name, 0, sizeof(hashtable[index]->name));
    }
}

void print_table(int N)
{
    for (int i = 0; i < N; i++)
    {
        if (hashtable[i] == NULL)
            cout << i << "   ---   ---" << endl;
        else if (hashtable[i]->id == -1)
            cout << i << " deleted deleted" << endl;
        else
            cout << i << "   " << hashtable[i]->id << "   " << hashtable[i]->name << endl;
    }
    cout << endl;
}

void search_print(int id, int N)
{
    int index;
    index = search(id, N);
    if (index == -1)
        cout << "The entry " << id << " could not be found in the hash table." << endl;
    else
        cout << "The entry " << id << " is at position " << index << endl;
    cout << endl;
}

void demo()
{
    int N = 23;
    hashtable = (h_entry**)malloc(sizeof(h_entry*) * N);

    for (int i = 0; i < N; i++)
        hashtable[i] = NULL;

    h_entry* myentry;
    myentry = create_ent(13, "Maria");
    insert(myentry, N);

    myentry = create_ent(189, "Ana");
    insert(myentry, N);

    myentry = create_ent(25, "Alina");
    insert(myentry, N);

    myentry = create_ent(68, "Elena");
    insert(myentry, N);

    myentry = create_ent(36, "Andrei");
    insert(myentry, N);

    myentry = create_ent(18, "Costin");
    insert(myentry, N);

    myentry = create_ent(75, "Lucian");
    insert(myentry, N);

    myentry = create_ent(33, "Selin");
    insert(myentry, N);

    print_table(N);

    search_print(33, N);

    delete_e(189, N);
    delete_e(13, N);

    print_table(N);

    search_print(36, N);

    cout << "------------------------------------------------------" << endl;
    cout << endl;
}

float average_found[7] = { 0 };
float average_notfound[7] = { 0 };
int max_found[7] = { 0 };
int max_notfound[7] = { 0 };

int select_elem(int n, float range, int partition)
{
    int min = partition * range + 1;
    int max = (partition + 1) * range;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribution(min, max);
    if (distribution(gen) < n)
        return distribution(gen);
    return n - 1;
}

void perf(float fill, int ind)
{
    int N = 10007;
    int numEntries = fill * N;
    int recF[1501] = { 0 };
    int recNF[1501] = { 0 };

    hashtable = (h_entry**)malloc(sizeof(h_entry*) * N);
    for (int i = 0; i < N; i++)
        hashtable[i] = NULL;

    int* found = new int[numEntries];
    //FillRandomArray(found, numEntries, 20000000, 45000000, true, ASCENDING);
    fillRandomArray(found, numEntries, 20000000, 45000000);

    for (int i = 0; i < numEntries; i++)
    {
        h_entry* myentry;
        myentry = create_ent(found[i], "Maria");
        insert(myentry, N);
    }

    int randIndex;
    int find;
    float range = static_cast<float>(N) / 1500.0f;
    for (int i = 0; i <= 1498; i++)
    {
        int count = 0;
        int max_count = range * range * range;
        randIndex = select_elem(N, range, i);
        while (hashtable[randIndex] == NULL || hashtable[randIndex]->id == -1)
        {
            if (count > max_count)
                break;
            randIndex = select_elem(N, range, i);
            count++;
        }
        if (count < max_count)
        {
            int fakeid = hashtable[randIndex]->id;
            find = search_perf(fakeid, N);
            recF[i] = find + 1;
            //cout << i << " " << randIndex << " " << fakeid << endl;
        }
    }

    int* nfound = new int[1501];
    //FillRandomArray(nfound, 1500, 45000001, 50000000, true, UNSORTED);
    fillRandomArray(nfound, 1500, 45000001, 50000000);
    for (int i = 0; i < 1498; i++)
    {
        find = search_perf(nfound[i], N);
        recNF[i] = find + 1;
    }

    int maxF = -1;
    int maxNF = -1;
    for (int i = 0; i < 1500; i++)
    {
        average_notfound[ind] = average_notfound[ind] + recNF[i];
        average_found[ind] = average_found[ind] + recF[i];
        if (recNF[i] > maxNF)
            maxNF = recNF[i];
        if (recF[i] > maxF)
            maxF = recF[i];
    }
    if (maxNF > max_notfound[ind])
        max_notfound[ind] = maxNF;
    if (maxF > max_found[ind])
        max_found[ind] = maxF;
}

void perf_delete()
{
    int N = 10007;
    int numEntries1 = 0.99 * N;
    int numEntries2 = 0.80 * N;
    int recF[1501] = { 0 };
    int recNF[1501] = { 0 };

    hashtable = (h_entry**)malloc(sizeof(h_entry*) * N);
    for (int i = 0; i < N; i++)
        hashtable[i] = NULL;

    int* found = new int[numEntries1];
    //FillRandomArray(found, numEntries1, 20000000, 45000000, UNSORTED);
    fillRandomArray(found, numEntries1, 20000000, 45000000);

    for (int i = 0; i < numEntries1; i++)
    {
        h_entry* myentry;
        myentry = create_ent(found[i], "Maria");
        insert(myentry, N);
    }

    int num = numEntries1 - numEntries2;
    for (int i = 0; i < num; i++)
    {
        int index = select_elem(N, N - 1, 0);
        while (hashtable[index] == NULL || hashtable[index]->id == -1)
            index = select_elem(N, N - 1, 0);
        int fakeid = hashtable[index]->id;
        delete_e(hashtable[index]->id, N);
    }
    //cout << "DELETED" << endl;
    int randIndex;
    int j = 0;
    int find;
    float range = static_cast<float>(N) / 1500.0f;
    for (int i = 0; i <= 1498; i++)
    {
        int count = 0;
        int max_count = range * range * range;
        randIndex = select_elem(N, range, i);
        while (hashtable[randIndex] == NULL || hashtable[randIndex]->id == -1)
        {
            if (count > max_count)
                break;
            randIndex = select_elem(N, range, i);
            count++;
        }
        if (count < max_count)
        {
            int fakeid = hashtable[randIndex]->id;
            find = search_perf(fakeid, N);
            recF[i] = find + 1;
            //cout << i << " " << randIndex << " " << fakeid << endl;
        }
    }

    int* nfound = new int[1501];
    //FillRandomArray(nfound, 1500, 45000001, 50000000, true, UNSORTED);
    fillRandomArray(nfound, 1500, 45000001, 50000000);
    for (int i = 0; i < 1500; i++)
    {
        find = search_perf(nfound[i], N);
        recNF[i] = find + 1;
    }

    int maxF = -1;
    int maxNF = -1;
    for (int i = 0; i < 1500; i++)
    {
        average_notfound[5] = average_notfound[5] + recNF[i];
        average_found[5] = average_found[5] + recF[i];
        if (recNF[i] > maxNF)
            maxNF = recNF[i];
        if (recF[i] > maxF)
            maxF = recF[i];
    }
    if (maxNF > max_notfound[5])
        max_notfound[5] = maxNF;
    if (maxF > max_found[5])
        max_found[5] = maxF;
}

void perf_all()
{
    int nrExp = 50;
    int index = 0;
    for (int i = 0; i < nrExp; i++)
    {
        index = 0;
        float fill = 0.80;
        perf(fill, index);

        index++;
        fill = 0.85;
        perf(fill, index);

        index++;
        fill = 0.90;
        perf(fill, index);

        index++;
        fill = 0.95;
        perf(fill, index);

        index++;
        fill = 0.99;
        perf(fill, index);

        perf_delete();
    }
    for (int i = 0; i < 6; i++)
    {
        average_notfound[i] = average_notfound[i] / (nrExp * 1500);
        average_found[i] = average_found[i] / (nrExp * 1500);
    }

    cout << "          AvgF       MaxF      AvgNF       MaxNF" << endl;
    cout << endl;

    cout << "0.80      ";
    cout << fixed << setprecision(2) << average_found[0] << "        ";
    cout << max_found[0] << "       ";
    cout << fixed << setprecision(2) << average_notfound[0] << "         ";
    cout << max_notfound[0] << endl;

    cout << "0.85      ";
    cout << fixed << setprecision(2) << average_found[1] << "        ";
    cout << max_found[1] << "       ";
    cout << fixed << setprecision(2) << average_notfound[1] << "         ";
    cout << max_notfound[1] << endl;

    cout << "0.90      ";
    cout << fixed << setprecision(2) << average_found[2] << "        ";
    cout << max_found[2] << "       ";
    cout << fixed << setprecision(2) << average_notfound[2] << "        ";
    cout << max_notfound[2] << endl;

    cout << "0.95      ";
    cout << fixed << setprecision(2) << average_found[3] << "        ";
    cout << max_found[3] << "      ";
    cout << fixed << setprecision(2) << average_notfound[3] << "        ";
    cout << max_notfound[3] << endl;

    cout << "0.99      ";
    cout << fixed << setprecision(2) << average_found[4] << "        ";
    cout << max_found[4] << "      ";
    cout << fixed << setprecision(2) << average_notfound[4] << "        ";
    cout << max_notfound[4] << endl;

    cout << "Deleted   ";
    cout << fixed << setprecision(2) << average_found[5] << "        ";
    cout << max_found[5] << "      ";
    cout << fixed << setprecision(2) << average_notfound[5] << "        ";
    cout << max_notfound[5] << endl;
}

int main()
{
    demo();
    perf_all();
}