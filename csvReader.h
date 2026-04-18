#ifndef CSVREADER_H
#define CSVREADER_H
#include "graph.h"
#include <string>
using namespace std;

class CSVReader {
public:
    static bool load(const string& filename, Graph& graph);

private:
    static void trim(string& s);
};

#endif