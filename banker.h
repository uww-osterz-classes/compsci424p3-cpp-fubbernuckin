#ifndef BANKER
#define BANKER
#include <iostream>
#include <vector> //basically for debugging

using namespace std;

class banker {
private:
    int m; //num resources
    int n; //num processes
    int* available; //available units of each resource
    int** max;//maximum possible claim by each process (n)
             //for each resource (m)
    int** allocation; //how many instances of a resource are allocated to each process
    int** request; //records number of units R_j that process p_i is requesting

    int** need; //how many potential request edges each process could ask for
    int* work; //in-process simulation of available resources
    bool* finish; //which processes have been reduced
    // int* total; //total number of each resource

public:
    banker(int m, int n);
    ~banker();

    void infodump() { //debug thing
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << "ij" << i << j << endl;
                cout << max[i][j] << endl;
            }
        }
    }
};

banker::banker(int num_resources, int num_processes) {
    m = num_resources;
    n = num_processes;
    available = new int[m];
    //populate available
    work = new int[m];
    for (int i = 0; i < m; i++) {
        work[i] = available[i];
    }

    max = new int*[n];
    for (int i = 0; i < n; i++) {
        max[i] = new int[m];
        for (int j = 0; j < m; j++) {
            max[i][j] = 0;
        }
    }

    allocation = new int*[n];
    for (int i = 0; i < n; i++) {
        allocation[i] = new int[m];
    }

    request = new int*[n];
    for (int i = 0; i < n; i++) {
        request[i] = new int[m];
    }

    need = new int*[n];
    for (int i = 0; i < n; i++) {
        need[i] = new int[m];
    }
}

banker::~banker() {
    delete[] available;
    
    for (int i = 0; i < n; i++) {
        delete[] max[i];
    }
    delete[] max;

    for (int i = 0; i < n; i++) {
        delete[] allocation[i];
    }
    delete[] allocation;

    for (int i = 0; i < n; i++) {
        delete[] request[i];
    }
    delete[] request;

    for (int i = 0; i < n; i++) {
        delete[] need[i];
    }
    delete[] need;
}

#endif