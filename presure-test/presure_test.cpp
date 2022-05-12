


#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include "../skiplist.h"

using namespace std;

#define NUM_THREADS 2
#define TEST_COUNT 100000
SkipList<int, string> skipList(17);
int success_nums = 0;

void* insert_element(void*) {

    int nums = TEST_COUNT / NUM_THREADS;
    for (int i = 0; i < nums; ++i) {
        int ret = skipList.insert_element(rand() % TEST_COUNT, "a");
        if (ret == 0) {
            ++success_nums;
        }
    }
    pthread_exit(NULL);

}

void* search_element(void*)  {
    int nums = TEST_COUNT / NUM_THREADS;
    for (int i = 0; i < nums; ++i) {
        bool ret = skipList.search_element(rand() % TEST_COUNT);
        if (ret) {
            ++success_nums;
        }
    }
    pthread_exit(NULL);
}

void presure_test_insert() {

    pthread_t threads[NUM_THREADS];
    auto startTime = chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; ++i) {
        cout << "main: creating thread " << i << endl;
        int ret = pthread_create(&threads[i], NULL, insert_element, NULL);

        if (ret) {
            cout<< "Error: unable to create thread" << endl;
            exit(-1);
        }

    }

    void* retval;
    for (int i = 0; i < NUM_THREADS; ++i) {
        int ret = pthread_join(threads[i], &retval);
        if (ret != 0) {
            cout<< "Error: unable to recycle thread" << endl;
            exit(-1);
        }

    }

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> usedTime = endTime - startTime;
    cout << "insert time: " << usedTime.count()  << "s" << endl;
    cout << "insert numbers: " << success_nums << endl;
}

void presure_test_search() {
    pthread_t threads[NUM_THREADS];
    auto startTime = chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_THREADS; ++i) {
        cout << "main: creating thread " << i << endl;
        int ret = pthread_create(&threads[i], NULL, search_element, NULL);

        if (ret) {
            cout<< "Error: unable to create thread" << endl;
            exit(-1);
        }

    }

    void* retval;
    for (int i = 0; i < NUM_THREADS; ++i) {
        int ret = pthread_join(threads[i], &retval);
        if (ret != 0) {
            cout<< "Error: unable to recycle thread" << endl;
            exit(-1);
        }

    }

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> usedTime = endTime - startTime;
    cout << "search time: " << usedTime.count()  << "s" << endl;
    cout << "search numbers: " << success_nums << endl;
}

int main() {
    srand (time(NULL)); 
    {
      presure_test_insert();
      success_nums = 0;
      //skipList.display_list();
      //presure_test_search();
      pthread_exit(NULL);
      return 0;
    }
}


