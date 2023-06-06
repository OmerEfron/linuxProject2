#include "utility.h"
//here we read the data from the file.
void airport::load_arv() {
    string filePath = "./flightDB/" + icoa + "/" + icoa + ".arv";
    ifstream file(filePath);

    if (!file.is_open()) {
        throw icoa;// Throw the airport icoa that not found
        cout << "Error: could not open file" << endl;
    }
    else {
        string line;

        vector<string> row;
        string field;
        int table_index;
        Flight arv_;

        getline(file, line);//ignore the first line

        while (getline(file, line)) {
            stringstream ss(line);
            table_index = 0;//table index
            while (getline(ss, field, ',')) {
                switch (table_index)
                {
                case 0: arv_.icoa24 = field; break;
                case 1: arv_.dpt_time = (field == "null") ? -1 :stoi(field); break;
                case 2: arv_.dpt = field; break;
                case 3: arv_.arv_time = (field == "null") ? -1 :stoi(field); break;
                case 4: arv_.arv = field; break;
                case 5: arv_.callsign = field; break;
                }
                ++table_index;
            }
            arv_flights.push_back(arv_);
            table_index = 0;
        }
    }
    file.close();
}
void airport::load_dpt() {
    string filePath = "./flightDB/" + icoa + "/" + icoa + ".dpt";
    ifstream file(filePath);

    if (!file.is_open()) {
        throw icoa;// Throw the airport icoa that not found
        cout << "Error: could not open file" << endl;
    }
    else {
        string line;

        vector<string> row;
        string field;
        int table_index;
        Flight dpt_;

        getline(file, line);//ignore the first line

        while (getline(file, line)) {
            stringstream ss(line);
            table_index = 0;//table index
            while (getline(ss, field, ',')) {
                switch (table_index)
                {
                case 0: dpt_.icoa24 = field; break;
                case 1: dpt_.dpt_time = stoi(field); break;
                case 2: dpt_.dpt = field; break;
                case 3: dpt_.arv_time = stoi(field); break;
                case 4: dpt_.arv = field; break;
                case 5: dpt_.callsign = field; break;
                }
                ++table_index;
            }
            dpt_flights.push_back(dpt_);
            table_index = 0;
        }
    }
    file.close();
}


const string airport::convertLocalTime(const int timeInSeconds){
    if(timeInSeconds == -1){
        return "unknown";
    }
    // convert to time_t
    time_t time = timeInSeconds;

    // convert to local time
    struct tm* tm = localtime(&time);

    // format the time as a string
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", tm);
    return string(buffer);
}
//func that comper time and flight.
bool comp(const pair<const Flight,bool>& flight1, const pair<const Flight,bool>& flight2) {

    if (flight1.second == IS_ARV && flight2.second == IS_ARV)
        return (flight1.first.arv_time < flight2.first.arv_time);

    else if (flight1.second == IS_DPT && flight2.second == IS_DPT)
        return (flight1.first.dpt_time < flight2.first.dpt_time);

    else if (flight1.second == IS_ARV && flight2.second == IS_DPT)
        return (flight1.first.arv_time <  flight2.first.dpt_time);

    else
        return (flight1.first.dpt_time < flight2.first.arv_time);

}
//sort with the func we build before.
vector<pair<Flight,bool>> airport::sort_full_sche() {
 
    vector<pair<Flight,bool>> sortSche;

    for(const Flight& f : arv_flights)
        sortSche.push_back({f,IS_ARV});
    
    for(const Flight& f : dpt_flights)
        sortSche.push_back({f,IS_DPT});

    sort(sortSche.begin(), sortSche.end(), comp);

    return sortSche;

}

vector<Flight> airport::getFlightsByicoa24(string icoa24)
{
    vector<Flight> flights;
    for(Flight& f : arv_flights)
    {
        if(f.icoa24 == icoa24)
            flights.push_back(f);
    }
    for(Flight& f : dpt_flights)
    {
        if(f.icoa24 == icoa24)
            flights.push_back(f);
    }
    return flights;
}
