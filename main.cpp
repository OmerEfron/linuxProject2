#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include "mainClass.h"
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int flag = 0;

void signalHandler(int signalNumber){
    flag = 1;
}

int main() {
    int parentToChild[2];
    int childToParent[2];
    
    if (pipe(parentToChild) == -1 || pipe(childToParent) == -1) {
        std::cerr << "Failed to create pipe.\n";
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed.\n";
        return 1;
    }

    if (pid == 0) {
        // Child process
        signal(SIGCONT, signalHandler);
        close(parentToChild[WRITE_END]);
        close(childToParent[READ_END]);
        
        mainClass mainObject;
        int choice;
        string result;
        int sizeOfResult;
        while (true) {
            while(flag == 0){
                //cout<<"still 0"<<endl;
                sleep(1);
            }
            flag = 0;
            read(parentToChild[READ_END], &choice, sizeof(choice));
            if (choice == 7) {
                break;
            }
            switch (choice) {
                case 1: {
                    int numStrings;
                    read(parentToChild[READ_END], &numStrings, sizeof(numStrings));
                    cout<<numStrings;

                    std::vector<std::string> strings(numStrings);
                    for (int i = 0; i < numStrings; ++i) {
                        char buffer[256];
                        read(parentToChild[READ_END], buffer, sizeof(buffer));
                        strings[i] = buffer;
                    }
                    result = mainObject.getAllArrivalFlightsDetails(strings);
                    break;
                }
                case 2:{
                    int numStrings;
                    read(parentToChild[READ_END], &numStrings, sizeof(numStrings));

                    std::vector<std::string> strings(numStrings);
                    for (int i = 0; i < numStrings; ++i) {
                        char buffer[256];
                        read(parentToChild[READ_END], buffer, sizeof(buffer));
                        strings[i] = buffer;
                    }
                    result = mainObject.getFull_schedule(strings);
                    break;
                }
                default:
                    result = "method " + std::to_string(choice);
            }
            cout<<"child size before write: "<<result.size()<< endl;
            write(childToParent[WRITE_END], result.c_str(), result.size() + 1);
            kill(getppid(), SIGCONT);

        }

        close(parentToChild[READ_END]);
        close(childToParent[WRITE_END]);
        exit(0);
    } else {
        // Parent process
        close(parentToChild[READ_END]);
        close(childToParent[WRITE_END]);
        signal(SIGCONT, signalHandler);
        int choice;
        std::string input;
        std::vector<std::string> strings;

        while (true) {
            std::cout << "Menu:\n";
            std::cout << "1. Enter strings\n";
            std::cout << "2. Option 2\n";
            std::cout << "3. Option 3\n";
            std::cout << "4. Option 4\n";
            std::cout << "5. Option 5\n";
            std::cout << "6. Option 6\n";
            std::cout << "7. Quit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            write(parentToChild[WRITE_END], &choice, sizeof(choice));

            if (choice == 7) {
                break;
            }

            switch (choice) {
                case 1: {
                    std::cout << "Enter the number of strings: ";
                    int numStrings;
                    std::cin >> numStrings;
                    write(parentToChild[WRITE_END], &numStrings, sizeof(numStrings));
                

                    std::cin.ignore(); // Ignore the newline character from previous input
                    for (int i = 0; i < numStrings; ++i) {
                        std::cout << "Enter string " << i + 1 << ": ";
                        std::getline(std::cin, input);
                        write(parentToChild[WRITE_END], input.c_str(), input.size() + 1);
                        cout<<i<< " out of "<< numStrings<< endl;
                    }

                    break;
                }
                case 2:{
                    std::cout << "Enter the number of strings: ";
                    int numStrings;
                    std::cin >> numStrings;
                    write(parentToChild[WRITE_END], &numStrings, sizeof(numStrings));

                    std::cin.ignore(); // Ignore the newline character from previous input
                    for (int i = 0; i < numStrings; ++i) {
                        std::cout << "Enter string " << i + 1 << ": ";
                        std::getline(std::cin, input);
                        write(parentToChild[WRITE_END], input.c_str(), input.size() + 1);
                    }
                    break;
                }
                default:
                    // Other cases don't require additional input from the user
                    break;
            }

            kill(pid, SIGCONT);
            char buffer[1200000];
            int status;
            while(flag == 0){
                sleep(1);
            }
            flag = 0;
            read(childToParent[READ_END], buffer, sizeof(buffer));
            cout<<"buffer size before print: "<< strlen(buffer) << endl;
            //std::cout << "Response from child: " << buffer << std::endl;
        }

        close(parentToChild[WRITE_END]);
        close(childToParent[READ_END]);
        wait(NULL); // Wait for the child process to terminate
    }

    return 0;
}
