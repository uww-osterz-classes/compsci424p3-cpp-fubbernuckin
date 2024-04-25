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
    int** allocation;

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
    max = new int*[n];
    for (int i = 0; i < n; i++) {
        max[i] = new int[m];
        // for (int j = 0; j < m; j++) {
        //     max[i][j] = 0;
        // }
    }

    allocation = new int*[n];
    for (int i = 0; i < n; i++) {
        allocation[i] = new int[m];
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
}

#endif