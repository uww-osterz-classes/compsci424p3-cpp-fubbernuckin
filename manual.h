#ifndef MANUAL
#define MANUAL

#include <iostream>
#include <vector>

#include "banker.h"

using namespace std;

class manual
{
private:
    /* data */

public:
    manual(/* args */);
    ~manual();

    void run(banker* bank) {
        while (true) { //menu loop
            vector<string> tokens;
            cout << "Enter a command" << endl;
            string command;
            getline(cin, command);

            //split command tokens
            int pos = 0;
            while (true) {
                int n_pos = command.find(" ", pos);
                if (n_pos == -1) {
                    tokens.push_back(command.substr(pos, command.length() - pos));
                    break;
                }
                else {
                    tokens.push_back(command.substr(pos, n_pos - pos));
                }
                pos = n_pos + 1;
            }
            
            // for(int i = 0; i < tokens.size(); i++) {
            //     cout << ":" << tokens[i] << ":" << endl;
            // }

            if (tokens[0] == "end") {
                break;
            }
            else if (tokens[0] == "request") {
                // request
                bank->addRequest(stoi(tokens[5]), stoi(tokens[3]), stoi(tokens[1]));
            }
            else if (tokens[0] == "release") {
                bank->releaseRequest(stoi(tokens[5]), stoi(tokens[3]), stoi(tokens[1]));
                // release
            }
            else if (tokens[0] == "infodump") {
                bank->infodump();
            }
            else {
                cerr << "Unknown command: " << tokens[0] << endl;
            }
        }
    }
};

manual::manual(/* args */)
{
}

manual::~manual()
{
}



#endif