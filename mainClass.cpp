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

        for(const auto& icoa:icoas){
            try{
                for (const auto& f : airports[icoa]->get_arv_flights())
                {
                    std::ostringstream oss;
                    oss << "Flight #" << f.callsign << " arriving from " << f.dpt << ", took off at " << airport::convertLocalTime(f.dpt_time) << " landed at " << airport::convertLocalTime(f.arv_time) << std::endl;
                    lines.push_back(oss.str());
                }
            }
            catch(string exepction_icoa)
            {
                res += exepction_icoa;
            }
        }
        
    for (const std::string& line : lines) {
        res += line; 
    }
    return res;
}


string mainClass::getFull_schedule(vector<string> icoas){
    string res;
    vector<string> lines;
    
    for (string icoa:icoas)
    {
        vector<pair<Flight,bool>> sortSche;
        string name = icoa;
    //loop sorted by time using func form utilty
        try {
            
            sortSche = airports[name]->sort_full_sche();
            for (const auto& f : sortSche) {
                std::ostringstream oss;
                if(f.second == IS_ARV)
                    oss << "Flight #" << f.first.callsign << " arriving from " << f.first.dpt << ", at " << airport::convertLocalTime(f.first.arv_time) << endl;
                else
                    oss << "Flight #" << f.first.callsign << " departing to " << f.first.arv << ", at " << airport::convertLocalTime(f.first.dpt_time) <<  endl;
                lines.push_back(oss.str());
            }
        }
        catch (string exepction_icoa)
        {
            res+=exepction_icoa;
        }
    }
	
	  for (const std::string& line : lines) 
      {
        res += line; 
      }

    return res;
}


string mainClass::getAirplaneFlight(string icoa24){
      
    string res;
    ostringstream oss;
    
    vector<Flight> flights;
    vector<Flight> temp_flights;
    vector<Flight> temp;
    vector<string> lines;
    //Here we check for evrey arg in evrey icoa if found we print.

        for(auto const& airp : airports)
        {
                temp = airp.second->getFlightsByicoa24(icoa24);  
                temp_flights.insert(temp_flights.end(),temp.begin(),temp.end());              
        }
        if(temp_flights.size() == 0)
                {
                    oss << "Airplane \"" << icoa24 << "\" have no data" << endl;
                    lines.push_back(oss.str());
                }
        flights.insert(flights.end(),temp_flights.begin(),temp_flights.end());  
        temp_flights.clear();

    for(Flight& f : flights)
    {
        oss << f.icoa24 << " departed from " << f.dpt << " at " << airport::convertLocalTime(f.dpt_time) << " arrived in " << f.arv << " at " << airport::convertLocalTime(f.arv_time) << endl;
        lines.push_back(oss.str());
    }
    
    for (const std::string& line : lines) {
        res += line; 
    }
    return res;
}

string mainClass::updateDB(){
    int counter = 0;
    string res;
 //Here we check if there is DB to update
    if(!(filesystem::exists(path) && filesystem::is_directory(path)))
        return "No data base dir \"flightDB\" found";
    else if(!(filesystem::exists(cmd) && !filesystem::is_directory(cmd)))
        return "No bash script \"flightScanner.sh\" found";
    else{
        for (const auto & entry : filesystem::directory_iterator(path)) {
            cmd += " " + entry.path().filename().string();
            counter++;
        }
    }
    if(counter > 0)
    {
        system("rm -r flightDB");
        int result = system(cmd.c_str());
        if (result != 0) 
            return"Error occured";
    }
    else{
        res += "Flight data base is empty.\n";
    }
    res += "DB updated succesfuly";
    return res;
}
