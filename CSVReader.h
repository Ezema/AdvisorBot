#pragma once
#include "OrderBookEntry.h"
#include <vector>
#include <string>

class CSVReader
{
    //declare the functions implemented in CSVReader.cpp
    public:
     CSVReader();
     static std::vector<OrderBookEntry> readCSV(std::string csvFile);
     static std::vector<std::string> tokenise(std::string csvLine, char separator);     
};