#include "header.h"


int main() {
    int parentToChild[2];
    int childToParent[2];
    
    if (pipe(parentToChild) == -1 || pipe(childToParent) == -1) {//TODO
        std::cerr << "Failed to create pipe.\n";
        return 1;
    }

    pid_t& p_pid = getPid();
    p_pid = fork();

    if (p_pid < 0) {
        std::cerr << "Fork failed.\n";
        return 1;//TODO
    }

    if (p_pid == 0) {
        // Child process
        childProccess(parentToChild,childToParent);

    } 
    else {
        // Parent process
        parentProccess(parentToChild,childToParent,p_pid);
    }

    return 0;
}