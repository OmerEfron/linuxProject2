#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include "mainClass.h"
#include <limits>




#define READ_END 0
#define WRITE_END 1
using namespace std;




std::vector<std::string> getStringsFromUser() {
    std::vector<std::string> result;
    std::string currentString;
    string input;

    cin>>input;
    cout<<input;
    for(char c: input){
        if(c == ' '){
            if(!currentString.empty()){
                result.push_back(currentString);
                currentString.clear();
            }
        }else{
            currentString += c;
        }
    }
    if(!currentString.empty()){
        result.push_back(currentString);
    }
    return result;
}



int childDoneWriting = 0;
int parentDoneWriting = 0;

void signalHandler(int signalNumber){
    if(signalNumber == SIGCONT)
        childDoneWriting = 1;
    else if(signalNumber == SIGUSR1)
        parentDoneWriting = 1;
}

int main() {
    int parentToChild[2];
    int childToParent[2];
    mainClass mainObject;
    signal(SIGCONT, signalHandler);
    // Create the pipes
    if (pipe(parentToChild) == -1 || pipe(childToParent) == -1) {
        std::cerr << "Failed to create pipes." << std::endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed." << std::endl;
        return 1;
    } else if (pid == 0) {
        // Child process
        close(parentToChild[WRITE_END]);
        close(childToParent[READ_END]);

        int choice;
        

        while (true) {
            read(parentToChild[READ_END], &choice, sizeof(choice));
            cout<<choice;
            char buffer[256];
            vector<string> input;
            std::string output;
            ssize_t bytes_read;
            switch(choice){
                case 1:
                    {
                        read(parentToChild[READ_END], &input, sizeof(input));
                        while(bytes_read = read(parentToChild[READ_END], buffer, sizeof(buffer) > 0)){
                            input.emplace_back(buffer, bytes_read);
                        }
                        output = mainObject.getAllArrivalFlightsDetails(input);
                    }
            }
            write(childToParent[WRITE_END], output.c_str(), output.size() + 1);
            kill(getppid(), SIGCONT);
        }

        close(parentToChild[READ_END]);
        close(childToParent[WRITE_END]);
        exit(0);
    } else {
        // Parent process
        close(parentToChild[READ_END]);
        close(childToParent[WRITE_END]);

        int choice, status;
        char output[120000];

        while (true) {
            cout << "1 - Fetch Airports incoming Flights.\n2 - Fetch airpots full flights schedule\n3 - Fetch aircraft full flights schedule\n4 - Update DB\n5 - Zip DB\n6 - Get child process PID.\n7 - EXIT." << endl;
            cout<< "Please make your choice <1, 2, 3, 4, 5, 6, 7: ";
            std::cin >> choice;
            vector<string> icoas;
            switch(choice){
                case 1:
                {
                    cout<<"Enter the ICOA code names: ";
                    icoas = getStringsFromUser();
                    write(parentToChild[WRITE_END], &choice, sizeof(choice));
                    for(const auto& icoa:icoas){
                        cout<<icoas[0];
                        write(parentToChild[WRITE_END], icoa.c_str(), icoa.size());
                    }
                    
                    break;
                }
                case 2:
                {
                    cout<<"Enter the ICOA code names: ";
                    break;
                }
                case 3:
                {
                    cout<<"Enter the ICOA24 code name: ";
                    break;
                }
                case 4:
                {
                    break;
                }
                case 5:
                {
                    break;
                }
                case 6:
                {
                    break;
                }
                case 7:
                {
                    break;
                }
            }
            // while(childDoneWriting == 0){
            //     sleep(4);
            // }
            childDoneWriting = 0;
            read(childToParent[READ_END], output, sizeof(output));
            std::cout << "Output from child: " << output << std::endl;
        }

        close(parentToChild[WRITE_END]);
        close(childToParent[READ_END]);

        wait(NULL); // Wait for the child process to terminate
    }

    return 0;
}