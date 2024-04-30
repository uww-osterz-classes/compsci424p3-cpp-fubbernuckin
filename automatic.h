#ifndef AUTOMATIC
#define AUTOMATIC

#include <iostream>
#include <thread>
#include <random>
#include <ctime>
#include <chrono>

#include "banker.h"

using namespace std;

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
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numRequests; j++) {
                bank->addRequest(i, distribution(generator), 1);
                bank->releaseRequest(i, distribution(generator), 1);
            }
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