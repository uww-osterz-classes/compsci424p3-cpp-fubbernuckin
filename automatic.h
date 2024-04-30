#ifndef AUTOMATIC
#define AUTOMATIC

#include <iostream>
#include <thread>
#include <random>
#include <ctime>
#include <chrono>
#include <vector>

#include "banker.h"

using namespace std;

void request(banker* bank, int process, int resource, int amount) {
    bank->addRequest(process, resource, amount);
}
void release(banker* bank, int process, int resource, int amount) {
    bank->releaseRequest(process, resource, amount);
}

class automatic
{
private:
    
    
public:
    automatic();
    ~automatic();
    
    void run(banker* bank, int numProcesses, int numResources, int numRequests) {
        time_t ct = time(nullptr);
        default_random_engine generator(ct);
        uniform_int_distribution distribution(0, numResources - 1);
        
        //create threads
        vector<thread> threads;
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numRequests; j++) {
                threads.push_back(thread(request, bank, i, distribution(generator), 1));
                threads.push_back(thread(release, bank, i, distribution(generator), 1));
                //bank->addRequest(i, distribution(generator), 1);
                //bank->releaseRequest(i, distribution(generator), 1);
            }
        }
        
        for(int i = 0; i < threads.size(); i++) {
            threads[i].join();
        }
    }
};

automatic::automatic() 
{
}

automatic::~automatic() 
{
}

#endif