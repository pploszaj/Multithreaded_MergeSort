#include<pthread.h>
#include<iostream>
#include <vector>

#define LIST_SIZE 20
#define SUBLIST_SIZE 5
#define NUM_THREADS 7

using namespace std;

pthread_t threads[NUM_THREADS];
vector<int> list;



void* sortSubList(void* arg){
    long index = (long)arg;
    auto start = list.begin() + index * SUBLIST_SIZE;
    auto end = list.begin() + (index + 1) * SUBLIST_SIZE;
    sort(start, end);
    return nullptr;
}

void* mergeSubLists(void* arg){
    long index = (long)arg;
    vector<int> temp(SUBLIST_SIZE * 2);

    auto firstSublistStart = list.begin() + id * SUBLIST_SIZE * 2;
    auto firstSublistEnd = list.begin() + (id + 1) * SUBLIST_SIZE * 2;
    auto secondSublistStart = firstSublistEnd;
    auto secondSublistEnd = list.begin() + (id + 2) * SUBLIST_SIZE * 2;

    merge(firstSublistStart, firstSublistEnd, secondSublistStart, secondSublistEnd, temp.begin());
    copy(temp.begin(), temp.end(), firstSublistStart);

    return nullptr;
}

int main (int argc, char *argv[]){

    //push all the command line arguments into a vector and parse them into integers
    for(int i = 0; i < LIST_SIZE; i++){
        list.push_back(atoi(argv[i]));
    }

    //create threads to sort sublists
    for(long i = 0; i < 4; i++){
        pthread_create(&threads[i], nullptr, sortSubList, (void*)i);
    }

    //wait for threads to complete
    for(int i = 0; i < 4; i++){
        pthread_join(threads[i], nullptr);
    }

    //create threads to merge sublists
    pthread_create(&threads[4], nullptr, mergeSubLists, (void*)0);
    pthread_create(&threads[5], nullptr, mergeSubLists, (void*)1);

    //wait for merging threads to complete
    pthread_join(threads[4], nullptr);
    pthread_join(threads[5], nullptr);

    //merge the final two sublists
    long finalMergeIndex = 0;
    mergeSubLists((void*)finalMergeIndex);

    //print the sorted list
    for (int val : list){
        cout << val << " ";
    }

    return 0;
}
