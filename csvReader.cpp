#include "csvReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

void CSVReader::trim(string& s) {
    size_t a = s.find_first_not_of(" \t\r\n\"");
    size_t b = s.find_last_not_of(" \t\r\n\"");
    s = (a == string::npos) ? "" : s.substr(a, b-a+1);
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
            cerr << "CSVReader: Skipping malformed row at line " << lineNum << "\n";
            continue;
        }

        string oCode   = fields[0];
        string dCode   = fields[1];
        string oCity   = fields[2];
        string dCity   = fields[3];
        string distStr = fields[4];
        string costStr = fields[5];

        trim(oCode); trim(dCode); trim(oCity);
        trim(dCity); trim(distStr); trim(costStr);

        if (oCode.empty() || dCode.empty()) {
            cerr << "CSVReader: Skipping malformed row at line " << lineNum << "\n";
            continue;
        }

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