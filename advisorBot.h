#include <iostream>
#include <vector>
#include <algorithm>
#include "OrderBook.h"
#include "CSVReader.h"

class advisorBot{
    public:        
        void init();
    private:                    
                        
        //midterm functions declaration implemented in the advisorBot.cpp file
        void printMainMenu(int& menuPrintCount);        
        void processUserOption(std::vector<std::string>& userSelectedMenuOption);
        void listProducts();
        double minimumBidForProduct(std::string& product, std::string& currentTime);
        double minimumAskForProduct(std::string& product, std::string& currentTime);
        double maximumBidForProduct(std::string& product, std::string& currentTime);
        double maximumAskForProduct(std::string& product, std::string& currentTime);
        void nextStepInTime();
        void processAveragePriceQuery(std::vector<std::string>& userSelectedMenuOption);
        void processPredictPriceQuery(std::vector<std::string>& userSelectedMenuOption);
        void runStatisticsCommand(std::vector<std::string>& userSelectedMenuOption);
        static std::string stringToLowerCase(std::string& userInput);

        ///////////////////////////////////////////

        //these two lines were copied from the provided MerkelMain.h file
        std::string currentTime;                    
        OrderBook orderBook{"history.csv"};
};
