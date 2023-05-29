#include "utility.h"
#include <string>
#include <vector>
#include <map>

class mainClass{
    private:
    std::map<string,airport*> airports;
    string path = "./flightDB/";
    string cmd = "./flightScanner.sh";
    
    public:
    mainClass();
    // ~mainClass(); //free airport memory, zips DB;
    std::string getAllArrivalFlightsDetails(std::vector<std::string> icoas); // 1
    std::string getFull_schedule(std::vector<std::string> icoas); // 2
    std::string getAirplaneFlight(string icoa24);// 3
    std::string updateDB();// 4
    // bool zipDB(); // 5
};