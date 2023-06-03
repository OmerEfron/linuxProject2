
#include "header.h"

void parentProccess(int* parentToChild,  int* childToParent, pid_t& pid)
{
        signal(SIGINT, signalHandler);
        signal(SIGUSR1, signalHandler);
        signal(SIGUSR2, signalHandler);
        signal(SIGCONT, signalHandler);
        close(parentToChild[READ_END]);
        close(childToParent[WRITE_END]);
        int choice;
        std::string input;
        std::vector<std::string> strings;

        pause();//Wait from the child that 
        while (true) {
            printMenu();
            std::cin >> choice;
            if(choice < 1 || choice > 7){
                cout << "Invalid choice, please try again." << endl;
                continue;
            }
            write(parentToChild[WRITE_END], &choice, sizeof(choice));

            if (choice == 7) {
                break;
            }

            switch (choice) {
                case 1: {
                    std::cout << "Enter the number of airports you want get details: " << endl;
                    int numStrings;
                    std::cin >> numStrings;
                    checkNumStringsInput(numStrings);
                    write(parentToChild[WRITE_END], &numStrings, sizeof(numStrings));    
                    getInputsAndWriteToPipe(numStrings, input, parentToChild[WRITE_END]);
                    break;
                }
                case 2:{
                    std::cout << "Enter the number of airports you want get details: " << endl;
                    int numStrings;
                    std::cin >> numStrings;
                    checkNumStringsInput(numStrings);
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
                case 7:{
                    kill(pid, SIGINT);
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

void childProccess(int* parentToChild,  int* childToParent){
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
        cleanUp(parentToChild[WRITE_END],childToParent[READ_END]);
        try{
            execution mainObject;
            kill(getppid(), SIGUSR1);
        int choice;
        string result;
        while (true) {
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
                case 5:{
                    bool isZipped = mainObject.zipDB();
                    if(isZipped)
                        result = "Data Base ZIP archive created successfully";
                    else
                        result = "Data Base ZIP archive not created due error while zipping the files";
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
        }catch(string no_db){
            kill(getppid(), SIGUSR2);
        }
        close(parentToChild[READ_END]);
        close(childToParent[WRITE_END]);
        std::exit(0);
        
}

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
void checkNumStringsInput(int& numStrings){
    while(numStrings <= 0){
        cout << "Invalid input, please try again enter positive number: " << endl;
        cin >> numStrings;
    }
}
void printMenu(){
    std::cout << "Menu:\n";
        std::cout << "1. Fetch airports incoming flights.\n";
        std::cout << "2. Fetch airports full flights schedule.\n";
        std::cout << "3. Fetch aircraft full flights schedule.\n";
        std::cout << "4. Update DB\n";
        std::cout << "5. Zip DB files\n";
        std::cout << "6. GEt child process PID\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";
}

void cleanUp(int pipe1, int pipe2){
    close(pipe1);
    close(pipe2);
}

void signalHandler(int signumber){
    if(signumber == SIGINT)
    {   
        kill(pid,SIGINT);  
        std::exit(0);
    }
    else if(signumber == SIGUSR2){
        cout <<"No data base." << endl;
        
        std::exit(0);
    }
    else if(signumber == SIGCONT){
        //data base load succefully
    }
}