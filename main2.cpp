#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include "execution.h"
#include <string.h>

#define READ_END 0
#define WRITE_END 1
using namespace std;

std::vector<std::string> readInputs(int pipeReadEnd){
    int numStrings;
    read(pipeReadEnd, &numStrings, sizeof(numStrings));

    std::vector<std::string> strings(numStrings);
    for (int i = 0; i < numStrings; ++i) {
        char buffer[256];
        read(pipeReadEnd, buffer, sizeof(buffer));
        strings[i] = buffer;
    }
    return strings;
}

void getInputsAndWriteToPipe(int numStrings, string input, int pipeWriteEnd){
    std::cin.ignore(); // Ignore the newline character from previous input
    for (int i = 0; i < numStrings; ++i) {
        std::cout << "Enter input " << i + 1 << ": ";
        std::getline(std::cin, input);
        write(pipeWriteEnd, input.c_str(), input.size() + 1);
    }
}

void printMenu(){
    std::cout << "Menu:\n";
        std::cout << "1. Enter strings\n";
        std::cout << "2. Option 2\n";
        std::cout << "3. Option 3\n";
        std::cout << "4. Option 4\n";
        std::cout << "5. Option 5\n";
        std::cout << "6. Option 6\n";
        std::cout << "7. Quit\n";
        std::cout << "Enter your choice: ";
}

void cleanUp(int pipe1, int pipe2){
    close(pipe1);
    close(pipe2);
}


int flag = 1;
int childFlag = 1;
void signalHandler(int signumber){
    if(signumber == SIGINT || signumber == SIGUSR1)
        {
            cout<<"Im here main signal " << signumber<<endl;
            flag = 0;
            exit(0);
        }
    else if(signumber == SIGTERM){
        childFlag = 0;
        cout<<"Im here chile signal " << signumber<<endl;
    }
    cout<<"recived signal: " << signumber<<endl;
}



int main() {
    int parentToChild[2];
    int childToParent[2];
    
    if (pipe(parentToChild) == -1 || pipe(childToParent) == -1) {//TODO
        std::cerr << "Failed to create pipe.\n";
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed.\n";
        return 1;//TODO
    }

    if (pid == 0) {
        // Child process
        signal(SIGTERM, signalHandler);
        cleanUp(parentToChild[WRITE_END],childToParent[READ_END]);
        execution mainObject;
        int choice;
        string result;
        while (childFlag) {
            read(parentToChild[READ_END], &choice, sizeof(choice));
            if (choice == 7) {
                break;
            }
            switch (choice) {
                case 1: {
                    std::vector<std::string> strings = readInputs(parentToChild[READ_END]);
                    result = mainObject.getAllArrivalFlightsDetails(strings);
                    break;
                }
                case 2:{
                    std::vector<std::string> strings = readInputs(parentToChild[READ_END]);
                    result = mainObject.getFull_schedule(strings);
                    break;
                }
                case 3:{
                    std::vector<std::string> strings = readInputs(parentToChild[READ_END]);
                    result = mainObject.getAirplaneFlight(strings[0]);
                    break;
                }
                case 4:{
                    result = mainObject.updateDB();
                    break;
                }
                case 6:{
                    string str = "Child pid is: ";
                    result = str + to_string(getpid());
                    break;
                }
                default:
                    result = "method " + std::to_string(choice);
            }
            int sizeOfResult = result.size() + 1;
            write(childToParent[WRITE_END], &sizeOfResult, sizeof(sizeOfResult));
            write(childToParent[WRITE_END], result.c_str(), result.size() + 1);
        }

        close(parentToChild[READ_END]);
        close(childToParent[WRITE_END]);
        exit(0);
    } else {
        // Parent process
        signal(SIGINT, signalHandler);
        signal(SIGUSR1, signalHandler);
        close(parentToChild[READ_END]);
        close(childToParent[WRITE_END]);
        int choice;
        std::string input;
        std::vector<std::string> strings;

        while (flag) {
            printMenu();
            std::cout << getpid();
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
                    getInputsAndWriteToPipe(numStrings, input, parentToChild[WRITE_END]);
                    break;
                }
                case 2:{
                    std::cout << "Enter the number of strings: ";
                    int numStrings;
                    std::cin >> numStrings;
                    write(parentToChild[WRITE_END], &numStrings, sizeof(numStrings));
                    getInputsAndWriteToPipe(numStrings, input, parentToChild[WRITE_END]);
                    break;
                }
                case 3:{
                    int numStrings = 1;
                    write(parentToChild[WRITE_END], &numStrings, sizeof(numStrings));
                    getInputsAndWriteToPipe(numStrings, input, parentToChild[WRITE_END]);
                    break;
                }
                default:
                    // Other cases don't require additional input from the user
                    break;
            }
            vector<string> bufferVector;
            int sizeOfResult;
            read(childToParent[READ_END], &sizeOfResult, sizeof(sizeOfResult));

            while(sizeOfResult > 0){
                char buffer[1200000];
                int bytesRead = read(childToParent[READ_END], buffer, sizeof(buffer));
                bufferVector.push_back(buffer);
                sizeOfResult -= bytesRead;
            }
            for(string& str:bufferVector){
                cout<< str;
            }
            cout<<"\n";
        }
        kill(pid, SIGTERM);
        close(parentToChild[WRITE_END]);
        close(childToParent[READ_END]);
        wait(NULL); // Wait for the child process to terminate
    }

    return 0;
}