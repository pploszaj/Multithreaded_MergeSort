#include <pthread.h>
#include <iostream>
#include <vector>

#define LIST_SIZE 20
#define SUBLIST_SIZE 5
#define NUM_THREADS 6

using namespace std;

pthread_t threads[NUM_THREADS];
vector<int> list = {1, 2, 9, 22, 7, 54, 13, 9, 10, 44, 59, 29, 50, 20, 17, 18, 0, 91, 78, 69};

// function that merges two sublists
void merge(vector<int>& arr, int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temp arrays
    vector<int> L(n1), R(n2);

    // Copy data to temp arrays
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[middle + 1 + j];

    // Merge the temp arrays back
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Function to implement MergeSort
void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// Function to sort sublist
void* sortSubList(void* arg) {
    long index = (long)arg;
    int start = index * SUBLIST_SIZE;
    int end = (index + 1) * SUBLIST_SIZE - 1;
    mergeSort(list, start, end);
    return nullptr;
}

// Function to merge sublists
void* mergeSubLists(void* arg) {
    long index = (long)arg;
    int start1 = index * SUBLIST_SIZE * 2;
    int end1 = start1 + SUBLIST_SIZE - 1;
    int start2 = end1 + 1;
    int end2 = start2 + SUBLIST_SIZE - 1;

    merge(list, start1, end1, end2);
    return nullptr;
}

int main() {
    // Create threads for sorting and call sortSubList on them
    for (long i = 0; i < 4; i++) {
        pthread_create(&threads[i], nullptr, sortSubList, (void*)i);
        cout << "Created thread\n";
    }

    // Wait for threads to complete
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], nullptr);
        cout << "Joined thread\n";
    }

    // Create threads to merge the sublists
    pthread_create(&threads[4], nullptr, mergeSubLists, (void*)0);
    pthread_create(&threads[5], nullptr, mergeSubLists, (void*)1);

    // Wait for merging threads to complete
    pthread_join(threads[4], nullptr);
    pthread_join(threads[5], nullptr);

    // Final merge of the two sorted sublists
    merge(list, 0, SUBLIST_SIZE * 2 - 1, LIST_SIZE - 1);

    // Print the sorted list
    for (int val : list) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}