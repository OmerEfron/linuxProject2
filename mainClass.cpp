#include "mainClass.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include "utility.h"

using namespace std;
mainClass::mainClass() {
    string path = "./flightDB/";
    for (const auto& entry : filesystem::directory_iterator(path)) {
        string name = entry.path().filename().string();
        airport* ap = new airport(name);
        airports[name] = ap;
    }
}


string mainClass::getAllArrivalFlightsDetails(vector<string> icoas){
    string res;
    ostringstream oss;
    vector<string> lines;
    try{
        for(const auto& icoa:icoas)
            for (const auto& f : airports[icoa]->get_arv_flights())
            {
                std::ostringstream oss;
                oss << "Flight #" << f.callsign << " arriving from " << f.dpt << ", took off at " << airport::convertLocalTime(f.dpt_time) << " landed at " << airport::convertLocalTime(f.arv_time) << std::endl;
                lines.push_back(oss.str());
            }
        }
        catch(string exepction_icoa)
        {
            return exepction_icoa;
        }
    for (const std::string& line : lines) {
        //cout<<line <<endl;
        res += line; 
    }
    return res;
}
