#ifndef HEADER_H
#define HEADER_H

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

pid_t pid;

void parentProccess(int* parentToChild,  int* childToParent, pid_t& pid);
void childProccess(int* parentToChild,  int* childToParent);
std::vector<std::string> readInputs(int pipeReadEnd);
void getInputsAndWriteToPipe(int numStrings, string input, int pipeWriteEnd);
void checkNumStringsInput(int& numStrings);
void printMenu();
void cleanUp(int pipe1, int pipe2);
void signalHandler(int signumber);

#endif