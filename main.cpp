#include "graph.cpp"
#include "graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct AirportData{
    string code;
    string state;
    string city;
    int inboundcount = 0;
    int outboundcount = 0;
    AirportData(string code, string city, string state) : 
        code(code), city(city), state(state) {}
};

void readfile(Graph<string>& flightGraph, vector<AirportData>& airportInfo){
    fstream fin;

    fin.open("airports.csv", ios::in);

}


int main() {
    



    return 0;
}
