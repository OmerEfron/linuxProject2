#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "mainClass.h"

#define READ_END 0
#define WRITE_END 1
using namespace std;

int doneWriting = 0;

void signalHandler(int signalNumber){
    if(signalNumber == SIGCONT)
        doneWriting = 1;
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
        std::string output;

        while (true) {
            read(parentToChild[READ_END], &choice, sizeof(choice));

            if (choice == 1) {   
                output = mainObject.getAllArrivalFlightsDetails({"LLBG"});
            } else if (choice == 2) {
                output = "method 2";
            } else if (choice == 3) {
                output = "method 3";
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
            std::cout << "Enter your choice (1-3): ";
            std::cin >> choice;

            write(parentToChild[WRITE_END], &choice, sizeof(choice));

            // if (choice == 3) {
            //     break;
            // }
            while(doneWriting == 0){
                sleep(1);
            }
            doneWriting = 0;
            read(childToParent[READ_END], output, sizeof(output));
            std::cout << "Output from child: " << output << std::endl;
        }

        close(parentToChild[WRITE_END]);
        close(childToParent[READ_END]);

        wait(NULL); // Wait for the child process to terminate
    }

    return 0;
}