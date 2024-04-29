#ifndef BANKER
#define BANKER
#include <iostream>
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
        cout << "max values: " << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << "ij" << i << j << " ";
                cout << max[i][j] << "; ";
            }
            cout << endl;
        }
        cout << "allocation values:" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << "ij" << i << j << " ";
                cout << allocation[i][j] << "; ";
            }
            cout << endl;
        }
        cout << "available values:" << endl;
        for (int i = 0; i < m; i++) {
            cout << "j" << i << " "; //says j to line up with previous loops where j is resource index
            cout << available[i] << "; ";
        }
        cout << endl;
    }

    bool processFile(ifstream& file) { //return false if issue with data
        //read in data
        //read available section
        string line;
        getline(file, line); //discard the "available" line
        for (int i = 0; i < m; i++) {
            file >> available[i];
        }
        getline(file, line); //discard the new line after available section

        //read max section
        getline(file, line);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                file >> max[i][j];
            }
        }
        getline(file, line); //discard the new line after max section

        //read allocation section
        getline(file, line); 
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                file >> allocation[i][j];
            }
        }
        //checking initial conditions
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (allocation[i][j] > max[i][j]) {
                    cerr << "Allocated more resources than stated maximum for " << i << ", " << j << endl;
                    return false;
                }
            }
        }
        bool safe = checkSafety();
        if (!safe) {
            cerr << "Initial state is unsafe" << endl;
        }
        return safe;
    }
    
    bool checkSafety() { //return false if unsafe
        //initialize variables
        for (int j = 0; j < m; j++) {
            work[j] = available[j];
        }
        for (int i = 0; i < n; i++) {
            finish[i] = false;
            for (int j = 0; j < m; j++) {
                need[i][j] = max[i][j] - allocation[i][j];
            }
        }
        
        //check for safety
        bool finished = false;
        while (finished == false) {
            bool workDone = false; //whether any processes were finished this cycle
            
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
                    workDone = true;
                    finish[i] = true;
                    for (int j = 0; j < m; j++) {
                        work[j] += allocation[i][j];
                    }
                }
            }
            
            //check if we're done
            if (!workDone) {
                return false;
            }
            finished = true;
            for (int i = 0; i < n; i++) {
                if (finish[i] == false) {
                    finished = false;
                    break;
                }
            }
        }
        
        return true;
    }
    
    bool addRequest(int process, int resource, int amount) { //return false if request denied
        cout << "Process " << process << " is requesting " << amount << " units of resource " << resource << endl;
        //copying variables
        int available_bk[m];
        int allocation_bk[n][m];
        //int request_bk[n][m];
        for (int i = 0; i < m; i++) {
            available_bk[i] = available[i];
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                allocation_bk[i][j] = allocation[i][j];
                //request_bk[i][j] = request[i][j];
            }
        }
        
        //checking request
        bool granted = false;
        allocation[process][resource] += amount;
        available[resource] -= amount;
        if(!checkSafety()) { //if request is unsafe
            cout << "Resource request denied." << endl;
            granted = false;
        }
        else if (amount > available[resource]) { //checks for over-allocation of resource
            cout << "Resource request denied." << endl;
            granted = false;
        }
        else {
            cout << "Resource request granted." << endl;
            granted = true;
            //variables were changed earlier, no need to edit them here
        }
        
        if (!granted) {
            //reset variables
            for (int i = 0; i < m; i++) {
                available[i] = available_bk[i];
            }
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    allocation[i][j] = allocation_bk[i][j];
                }
            }
        }
        
        return granted;
    }
    
    bool releaseRequest(int process, int resource, int amount) {
        if (amount <= 0) {
            cout << "Must release greater than zero units of a resource." << endl;
            return false;
        }
        else if(amount > allocation[process][resource]) {
            cout << "Can't release more units of a resource than are allocated to the process." << endl;
            return false;
        }
        else {
            cout << "Release granted." << endl;
            allocation[process][resource] -= amount;
            available[resource] += amount;
            return true;
        }
    }
};

banker::banker(int num_resources, int num_processes) {
    m = num_resources;
    n = num_processes;
    available = new int[m];
    work = new int[m];

    max = new int*[n];
    for (int i = 0; i < n; i++) {
        max[i] = new int[m];
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