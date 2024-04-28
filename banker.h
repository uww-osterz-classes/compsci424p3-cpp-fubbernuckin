#ifndef BANKER
#define BANKER
#include <iostream>
#include <vector> //basically for debugging
#include <fstream>
#include <string>

using namespace std;

class banker {
private:
    int m; //num resources
    int n; //num processes
    int* available; //available units of each resource
    int** max;//maximum claims by each process (n)
             //for each resource (m)
    int** allocation; //how many instances of a resource are allocated to each process
    int** request; //request[i][j] records number of resources R_j that process p_i is requesting

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

    bool processFile(ifstream& file) { //return false if issue with data
        //read in data
        //read available section
        string line;
        getline(file, line); //discard the "available" line
        for (int i = 0; i < m; ++i) {
            file >> available[i];
        }
        getline(file, line); //discard the new line after available section

        //read max section
        getline(file, line);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                file >> max[i][j];
            }
        }
        getline(file, line); //discard the new line after max section

        //read allocation section
        getline(file, line); 
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; ++j) {
                file >> allocation[i][j];
            }
        }
        //checking initial conditions
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (allocation[i][j] > max[i][j]) {
                    cout << "Allocated more resources than stated maximum for " << i << ", " << j << endl;
                    return false;
                }
            }
        }
        bool safe = checkSafety();
        return safe; //all good
    }
    
    bool checkSafety() { //return false if unsafe
        //initialize variables
        for (int i = 0; i < n; i++) {
            finish[i] = false;
            for (int j = 0; j < m; j++) {
                need[i][j] = max[i][j] - allocation[i][j];
                work[j] = available[j];
            }
        }
        
        //check for safety
        bool finished = false; 
        while (finished == false) {
            bool workDone = false; //whether any processes have been finished
            
            for (int i = 0; i < n; i++) {
                if (finish[i] == true) {
                    continue;
                }
                bool safe = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        safe = false;
                        break;
                    }
                }
                if (safe == true) {
                    finish[i] = true;
                    for (int j = 0; j < m; j++) {
                        work[j] += allocation[i][j];
                    }
                }
            }
            
            //check if we're done
            finished = true;
            for (int i = 0; i < n; i++) {
                if (finish[i] == false) {
                    finished = false;
                }
            }
        }
        
        return true;
    }
    
    /*
    //heck, it's all wrong
    bool submitRequests() { //false if any requests were denied
        //initialize necessary variables
        for (int i = 0; i < m; i++) {
            work[i] = available[i];
        }
        for (int i = 0; i < n; i++) {
            finish[i] = false;
            for (int j = 0; j < m; j++) {
                need[i][j] = max[i][j] - allocation[i][j];
            }
        }
        //simulate requests
        bool finished = false;
        while (!finished) { //have to  change condition
            finished = true;
            for(int i = 0; i < n; i++) { //for each process
                //housekeeping
                bool requestFound = false;
                if (finish[i] == true) {
                    continue;
                }
                finished = false;
                
                finish[i] = true; //temp assume resource is finished
                for (int j = 0; j < m; j++) { //for each request
                    if (request[i][j] > 0) { //if request exists by P_i for R_j
                        requestFound = true;
                        int curr_req = request[i][j]; //num units being requested
                        if (need[i][j] > work[j]) { //where should this go?
                            finish[i] = false;
                        }
                    }
                }
                
                
                
                if(requestFound == false) {
                    finish[i] = true;
                }
            }
        }
    }
    */
};

banker::banker(int num_resources, int num_processes) {
    m = num_resources;
    n = num_processes;
    available = new int[m];
    work = new int[m];

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

    request = new int*[n];
    for (int i = 0; i < n; i++) {
        request[i] = new int[m];
    }

    need = new int*[n];
    for (int i = 0; i < n; i++) {
        need[i] = new int[m];
    }
    
    finish = new bool[n];
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