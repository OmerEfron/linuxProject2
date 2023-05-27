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


string mainClass::getAirplaneFlight(string icoas24){
      
    string res;
    ostringstream oss;
    string path = "./flightDB/";
    for (const auto & entry : filesystem::directory_iterator(path)) {
        airports.push_back(airport(entry.path().filename().string()));
    }
    vector<Flight> flights;
    vector<Flight> temp_flights;
    vector<Flight> temp;
    //Here we check for evrey arg in evrey icoa if found we print.
    for(int i =1; i < argc; i++)
    {
            for(int j = 0; j < (int)airports.size(); j++)
            {
                 temp = airports[j].getFlightsByicoa24(argv[i]);  
                 temp_flights.insert(temp_flights.end(),temp.begin(),temp.end());              
            }
            if(temp_flights.size() == 0)
                    cout << "Airplane \"" << argv[i] << "\" have no data" << endl;
            flights.insert(flights.end(),temp_flights.begin(),temp_flights.end());  
            temp_flights.clear();
    }
    for(Flight& f : flights)
    {
   	 std::ostringstream oss;
        oss << f.icoa24 << " departed from " << f.dpt << " at " << airport::convertLocalTime(f.dpt_time) << " arrived in " << f.arv << " at " << airport::convertLocalTime(f.arv_time) << endl;
    }
    
    for (const std::string& line : lines) {
        //cout<<line <<endl;
        res += line; 
    }
    return res;
}
  

string mainClass::getFull_schedule(vector<string> icoas){
    string res;
    ostringstream oss;
    vector<string> lines;
     for (int i = 1; i < icoas.size(); i++)
    {
    vector<pair<Flight,bool>> sortSche;
    string name = argv[i];
//loop sorted by time using func form utilty
        try {
            airport air(name);
            sortSche = air.sort_full_sche();
            for (const auto& f : sortSche) {
               std::ostringstream oss;
                if(f.second == IS_ARV)
                     oss << "Flight #" << f.first.callsign << " arriving from " << f.first.dpt << ", at " << air.convertLocalTime(f.first.arv_time) << endl;
                else
                     oss << "Flight #" << f.first.callsign << " departing to " << f.first.arv << ", at " << air.convertLocalTime(f.first.dpt_time) <<  endl;

            }
        }
        catch (string exepction_icoa)
        {
            cout << exepction_icoa << " not exist in the data base" << endl;
        }
        cout << "----------------------------------------------------------------------------------------------" << endl;
	
	  for (const std::string& line : lines) {
        //cout<<line <<endl;
        res += line; 
    }
    }
    return res;
}
