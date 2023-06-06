#include "utility.h"
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>
#include <zip.h>

namespace fs = std::filesystem;

class execution{
    private:
    std::map<string,airport*> airports;
    std::string path = "./flightDB/";
    std::string cmd = "./flightScanner.sh";
    
    void addFileToZip(zip* archive, const fs::path& filePath, const fs::path& relativePath);
    void addDirectoryToZip(zip* archive, const fs::path& directoryPath, const fs::path& relativePath);
    bool unzipFile(const std::string& zipFilename, const std::string& outputDirectory);
    bool extractEntry(zip* archive, int index, const std::string& outputDirectory);
    bool createDirectory(const std::string& directoryPath);
    

    public:
    execution();
    ~execution(); //free airport memory, zips DB;
    std::string getAllArrivalFlightsDetails(std::vector<std::string> icoas); // 1
    std::string getFull_schedule(std::vector<std::string> icoas); // 2
    std::string getAirplaneFlight(string icoa24);// 3
    std::string updateDB();// 4
    bool zipDB(); // 5
    bool unzipDB();
};