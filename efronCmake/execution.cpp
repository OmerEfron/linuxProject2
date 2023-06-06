#include "execution.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>  
#include <fstream>
#include <zip.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include "utility.h"

using namespace std;

execution::execution() {
    unzipDB();
    if (!(std::filesystem::exists(path) && std::filesystem::is_directory(path))) {
        throw string ("No data base to load data.");
    } 
    for (const auto& entry : filesystem::directory_iterator(path)) {
        string name = entry.path().filename().string();
        airport* ap = new airport(name);
        airports[name] = ap;
    }
}
execution::~execution() {
    zipDB();
    for (const auto& pair : airports) {
        delete pair.second;
    }

}


string execution::getAllArrivalFlightsDetails(vector<string> icoas){
    string res;
    ostringstream oss;
    vector<string> lines;

        for(const auto& icoa:icoas){
            try{
                if(airports.count(icoa) <= 0)
                    throw string("The airpot " + icoa + " does not exist in the data base.\n");
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


string execution::getFull_schedule(vector<string> icoas){
    string res;
    vector<string> lines;
    
    for (string icoa:icoas)
    {
        vector<pair<Flight,bool>> sortSche;
        string name = icoa;
    //loop sorted by time using func form utilty
        try {
            if(airports.count(icoa) <= 0)
                    throw string("The airpot " + icoa + " does not exist in the data base.\n");

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


string execution::getAirplaneFlight(string icoa24){
      
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

string execution::updateDB(){
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

        res += "DB updated succesfuly";
    }
    else{
        res += "Flight data base is empty.\n";
    }
    return res;
}
bool execution::zipDB(){
    bool res = true;

    const string libraryPath = path;
    const string zipFilename = "DB.zip";

    int error;
    zip* archive = zip_open(zipFilename.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &error);
    if (!archive) {
        //std::cerr << "Failed to create ZIP archive: " << zip_strerror(archive) << std::endl;
        return false;
    }
    filesystem::path libraryDirectory(libraryPath);
    if (filesystem::exists(libraryDirectory) && filesystem::is_directory(libraryDirectory)) {
        addDirectoryToZip(archive, libraryDirectory, filesystem::path(""));
    } else {
        cerr << "Invalid library path: " << libraryPath << endl;
        zip_close(archive);
        return 1;
    }

    if (zip_close(archive) == -1) {
        //std::cerr << "Failed to close ZIP archive: " << zip_strerror(archive) << std::endl;
        return false;
    }
    return res;
}
void execution::addFileToZip(zip* archive, const filesystem::path& filePath, const filesystem::path& relativePath) {
    std::string fileStr = filePath.string();

    zip_source* source = zip_source_file(archive, fileStr.c_str(), 0, -1);
    if (!source) {
        std::cerr << "Failed to add file to ZIP archive: " << zip_strerror(archive) << std::endl;
        return;
    }

    std::string relativePathStr = relativePath.string();
    zip_int64_t index = zip_file_add(archive, relativePathStr.c_str(), source, ZIP_FL_OVERWRITE);
    if (index == -1) {
        //std::cerr << "Failed to add file to ZIP archive: " << zip_strerror(archive) << std::endl;
        zip_source_free(source);
        return;
    }
}

void execution::addDirectoryToZip(zip* archive, const filesystem::path& directoryPath, const filesystem::path& relativePath) {
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        const auto& path = entry.path();
        const auto& relativeSubPath = relativePath / path.filename();

        if (filesystem::is_directory(path)) {
            addDirectoryToZip(archive, path, relativeSubPath);
        } else if (filesystem::is_regular_file(path)) {
            addFileToZip(archive, path, relativeSubPath);
        }
    }
}

bool execution::createDirectory(const std::string& directoryPath) {
    int status = mkdir(directoryPath.c_str(), 0755);
    if (status == 0) {
        return true;
    } else {
        return errno == EEXIST; // Directory already exists
    }
}

bool execution::extractEntry(zip* archive, int index, const std::string& outputDirectory) {
    zip_stat_t stat;
    if (zip_stat_index(archive, index, 0, &stat) != 0) {
        return false;
    }

    std::string entryName = stat.name;
    std::string outputPath = outputDirectory + "/" + entryName;

    if (stat.name[strlen(stat.name) - 1] == '/') {
        // Entry is a directory
        if (!createDirectory(outputPath)) {
            return false;
        }
    } else {
        // Entry is a file
        std::size_t found = entryName.find_last_of("/");
        if (found != std::string::npos) {
            // Extract the directory path from the entry name
            std::string directoryPath = outputDirectory + "/" + entryName.substr(0, found);
            if (!createDirectory(directoryPath)) {
                return false;
            }
        }

        zip_file_t* file = zip_fopen_index(archive, index, 0);
        if (!file) {
            return false;
        }

        std::ofstream outputFile(outputPath, std::ios::binary);
        if (!outputFile) {
            zip_fclose(file);
            return false;
        }

        static const size_t bufferSize = 8192;
        char buffer[bufferSize];
        zip_int64_t bytesRead = 0;
        while ((bytesRead = zip_fread(file, buffer, bufferSize)) > 0) {
            outputFile.write(buffer, bytesRead);
        }

        zip_fclose(file);
    }

    return true;
}

bool execution::unzipFile(const std::string& zipFilename, const std::string& outputDirectory) {
    zip* archive = zip_open(zipFilename.c_str(), 0, NULL);
    if (!archive) {
        return false;
    }

    int numEntries = zip_get_num_entries(archive, 0);
    for (int i = 0; i < numEntries; ++i) {
        if (!extractEntry(archive, i, outputDirectory)) {
            zip_close(archive);
            return false;
        }
    }
    zip_close(archive);
    return true;
}


bool execution::unzipDB(){
    bool hasDir = true;
    const std::string zipFilename = "DB.zip";
    const std::string outputDirectory = "flightDB";
    if (!(std::filesystem::exists(path) && std::filesystem::is_directory(path))) {
        mkdir(path.c_str(), 0755);
        hasDir = false;
    }
    mkdir(path.c_str(), 0755);
    if (unzipFile(zipFilename, outputDirectory)) {
        return true;
    } else {
        if(!hasDir)
            std::filesystem::remove(path);
       return false;
    }
}