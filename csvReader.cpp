#include "csvReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
// The STL <algorithm> is only used in the trim function for the "remove" feature
// It was added because the csv file has a line that is {"item, item"}
// And the reader kept reading that as 2 seperate items instead of 1
// Also I hope that the stream libraries ar allowed...

using namespace std;

// Trims leading and trailing whitespaces
void CSVReader::trim(string& s) {
    size_t a = s.find_first_not_of(" \t\r\n\"");
    size_t b = s.find_last_not_of(" \t\r\n\"");
    s = (a == string::npos) ? "" : s.substr(a, b - a + 1);

    // Remove any remaining carriage returns
    s.erase(remove(s.begin(), s.end(), '\r'), s.end());
}

// Parses one CSV line respecting quoted fields e.g. "Allentown, PA"
static vector<string> parseLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (int i = 0; i < (int)line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            // Tells the reader if there are quotes and when they end
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field); // last field
    return fields;
}

// Loads data into graph file to construct trees
bool CSVReader::load(const string& filename, Graph& graph) {
    ifstream file(filename);
    if (!file.is_open()) { cerr << "CSVReader: Cannot open '" << filename << "'\n"; return false; }

    string line;
    getline(file, line); // skip header

    int lineNum = 1;
    while (getline(file, line)) {
        ++lineNum;
        if (line.empty()) continue;

        vector<string> fields = parseLine(line);
        if ((int)fields.size() < 6) {
            // Throws error when expected number of data per line is not met
            // (this line will never do anything, it is soley part of my insanity trying to figure out why there are quotes in a csv)
            cerr << "CSVReader: Skipping malformed row at line " << lineNum << "\n";
            continue;
        }

        // Tracks data by column position
        string oCode = fields[0];
        string dCode = fields[1];
        string oCity = fields[2];
        string dCity = fields[3];
        string distStr = fields[4];
        string costStr = fields[5];

        // Trims data before sending (removes whitespaces and such)
        trim(oCode); trim(dCode); trim(oCity);
        trim(dCity); trim(distStr); trim(costStr);

        if (oCode.empty() || dCode.empty()) {
            // Throws an error if one of the codes is missing
            // (will do nothing in practice, only here to see why, again, data is seperated by commans inside quotes)
            // (I dont like csv files, please be txt)
            cerr << "CSVReader: Skipping malformed row at line " << lineNum << "\n";
            continue;
        }

        // To emphesize my dislike for reading csv files, here are some more debbuging error handlers
        double dist = 0.0, cost = 0.0;
        try { dist = distStr.empty() ? 0.0 : stod(distStr); }
        catch (...) { cerr << "Bad distance at line " << lineNum << ": [" << distStr << "]\n"; continue; }
        try { cost = costStr.empty() ? 0.0 : stod(costStr); }
        catch (...) { cerr << "Bad cost at line " << lineNum << ": [" << costStr << "]\n"; continue; }
        graph.addAirportAndEdge(oCode, oCity, dCode, dCity, dist, cost);
    }
    
    file.close();
    return true;
}