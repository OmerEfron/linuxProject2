#include "utility.h"
#include <string>
#include <vector>
#include <map>

class mainClass{
    private:
    std::map<string,airport*> airports;
    
    public:
    mainClass();
    // ~mainClass(); //free airport memory, zips DB;
    std::string getAllArrivalFlightsDetails(std::vector<std::string> icoas); // 1
    // std::string getFUllAirportScheduleOrderedByTime(std::vector<std::string> airportName); // 2
    // std::string getAllFlightsOfAircraft(std::vector<string> aircrafts);// 3
    // bool updateDB();// 4
    // bool zipDB(); // 5
};