#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <set>
#include <algorithm>

using namespace std;

#define IS_ARV false
#define IS_DPT true


typedef struct flight {
    string icoa24;
    string arv;
    string dpt;
    string callsign;
    int dpt_time;
    int arv_time;
}Flight;

class airport {

    string icoa;

    vector<Flight> arv_flights;
    vector<Flight> dpt_flights;

public:
    airport(string airport_name) noexcept(false) : icoa(airport_name) { load_dpt(); load_arv(); }
    
    void load_arv() noexcept(false);
    void load_dpt() noexcept(false);

    static const string convertLocalTime(const int timeInSeconds);

    friend bool comp(const pair<const Flight,bool>& flight1, const pair<const Flight,bool>& flight2);

    vector<pair<Flight,bool>> sort_full_sche();
      
    vector<Flight> getFlightsByicoa24(string icoa24);
    
     const inline string geticao() { return this->icoa;};
     const inline vector<Flight>& get_arv_flights() const { return arv_flights;};
     const inline vector<Flight>& get_dpt_flights() const { return dpt_flights;};

     

};



#endif