#include "csvReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void CSVReader::trim(string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    s = (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

bool CSVReader::load(const string& filename, Graph& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "CSVReader: Cannot open file '" << filename << "'\n";
        return false;
    }

    string line;
    getline(file, line);

    int lineNumber = 1;
    while (getline(file, line)) {
        ++lineNumber;
        if (line.empty()) continue;

        stringstream ss(line);
        string originCode, destCode, originCity, destCity, distStr, costStr;

        getline(ss, originCode, ',');
        getline(ss, destCode, ',');
        getline(ss, originCity, ',');
        getline(ss, destCity, ',');
        getline(ss, distStr, ',');
        getline(ss, costStr, ',');

        trim(originCode); trim(destCode);
        trim(originCity); trim(destCity);
        trim(distStr); trim(costStr);

        if (originCode.empty() || destCode.empty()) {
            cerr << "CSVReader: Skipping malformed row at line " << lineNumber << "\n";
            continue;
        }

        double dist = distStr.empty() ? 0.0 : stod(distStr);
        double cost = costStr.empty() ? 0.0 : stod(costStr);

        graph.addAirportAndEdge(originCode, originCity, destCode, destCity, dist, cost);
    }

    file.close();
    return true;
}