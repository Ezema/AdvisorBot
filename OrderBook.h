#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

#include <map>

class OrderBook
{
    public:
        /** construct, reading a csv data file */
        OrderBook(std::string filename);
        /** return vector of all know products in the dataset*/
        std::vector<std::string> getKnownProducts();
        /** return vector of Orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                              std::string product, 
                                              std::string timestamp);

        /** returns the earliest time in the orderbook*/
        std::string getEarliestTime();
        /** returns the next time after the 
         * sent time in the orderbook  
         * If there is no next timestamp, wraps around to the start
         * */
        std::string getNextTime(std::string timestamp);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);

        ////////////////////////////////////////////////
        // The code above was provided to the students by the teacher in the assignment
        ////////////////////////////////////////////////

        ////////////////////////////////////////////////
        // The code below was done by the student
        ////////////////////////////////////////////////
        
        //declaration of the functions used for midterm assignment
        double getAveragePriceLastAmountOfDates(OrderBook& orderBook, std::string currentTime, std::string product, OrderBookType orderType, int numberOfTimeStepsDesired);
        double getAveragePriceFromBeginningOfDataset(OrderBook& orderBook, std::string currentTime, std::string product, OrderBookType orderType);
        std::vector<std::vector<std::string>> calculateChangesAtEndOfTimestep(OrderBook& orderBook, std::string product, OrderBookType orderType);
        static double calculateHistoricalAverageGainOrLossUpToSpecifiedDate(std::vector<std::vector<std::string>>& vectorChangeAtEndOfTimestep, std::string specifiedDate);    
        std::vector<double> processAverageBidAskSpread(std::string& product, std::string& timeframe, OrderBook& orderBook, std::string currentTime);
        double getMarketDepth(std::vector<OrderBookEntry>& orders);

    private:
        std::vector<OrderBookEntry> orders;


};
