#include "header.h"


int main() {
    int parentToChild[2];
    int childToParent[2];
    
    if (pipe(parentToChild) == -1 || pipe(childToParent) == -1) {//TODO
        std::cerr << "Failed to create pipe.\n";
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed.\n";
        return 1;//TODO
    }

    if (pid == 0) {
        // Child process
        childProccess(parentToChild,childToParent);

    } 
    else {
        // Parent process
        parentProccess(parentToChild,childToParent,pid);
    }

    return 0;
}