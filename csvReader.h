#ifndef CSVREADER_H
#define CSVREADER_H
#include "graph.h"
#include <string>
#include <vector>

class CSVReader {
public:
    static bool load(const std::string& filename, Graph& graph);
private:
    static void trim(std::string& s);
};

#endif