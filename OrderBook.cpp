#include "OrderBook.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>

/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{   
    orders = CSVReader::readCSV(filename);    
}

/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string,bool> prodMap;

    for (OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }
    
    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}
/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, 
                                        std::string product, 
                                        std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry& e : orders)
    {
        if (e.orderType == type && 
            e.product == product && 
            e.timestamp == timestamp )
            {
                orders_sub.push_back(e);
            }
    }
    return orders_sub;
}

//returns the highest price found in the vector of OrderBookEntries passed in the argument
double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max)max = e.price;
    }
    return max;
}

//returns the lowest price found in the vector of OrderBookEntries passed in the argument
double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{    
    double min = orders[0].price;    
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min)min = e.price;
    }    
    return min;
}

//returns the first timestamp
std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

//returns the next timestamp. Used by advisorBot to advance to the next timestamp
std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp) 
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

//this function takes 2 arguments: a) a vector that contains the daily nominal change (min ask at end of day - min ask at the beginning of the day) for a given timestamp and b) the current timestamp where the user is. It will proceed to calculate the historical average nominal change until the current timestamp
double OrderBook::calculateHistoricalAverageGainOrLossUpToSpecifiedDate(std::vector<std::vector<std::string>>& vectorChangeAtEndOfTimestep, std::string specifiedDate){
    
    double sumOfDailyChange = 0;
    double entriesCounted = 0;
    bool stopCountingAfterThisTimestep = false;
    bool newTimestepInForLoop = false;
    std::string currentTimeStepInForLoop = vectorChangeAtEndOfTimestep[0][0];
    std::string lastSeenTimestep = vectorChangeAtEndOfTimestep[0][0];

    for (int i = 0; i < vectorChangeAtEndOfTimestep.size(); i++){
        
        currentTimeStepInForLoop = vectorChangeAtEndOfTimestep[i][0];

        if(currentTimeStepInForLoop.compare(lastSeenTimestep)!=0){
            newTimestepInForLoop = true;
        }else{
            newTimestepInForLoop = false;
        }

        if(newTimestepInForLoop && stopCountingAfterThisTimestep){
            break;
        }

        if(vectorChangeAtEndOfTimestep[i][0].compare(specifiedDate)==0){
            stopCountingAfterThisTimestep = true;
        }

        double parsedDoubleValue = std::stod(vectorChangeAtEndOfTimestep[i][1]);
        sumOfDailyChange = sumOfDailyChange + parsedDoubleValue;
        entriesCounted =  entriesCounted+1;
        
        lastSeenTimestep = vectorChangeAtEndOfTimestep[i][0];
    }

    return (sumOfDailyChange/entriesCounted);
}

//This function calculates the difference between the minimum ask/bid at the last entry of a given timestamp and the minimum ask/bid at the first entry of that same timestampt to get to know the nominal price change in the given "day" (timestamp). Note how it receives as argument the product and the orderbook 
std::vector<std::vector<std::string>> OrderBook::calculateChangesAtEndOfTimestep(OrderBook& orderBook, std::string product, OrderBookType orderType)
{
    //the orderBook is always sorted so orderBook.orders[0].timestamp is the first entry in chronological order 
    std::string currentTimeStepInForLoop = orderBook.orders[0].timestamp;
    std::string lastSeenTimestep = orderBook.orders[0].timestamp;

    double priceInTheFirstAskOfTimestep = 0;
    double priceInTheLastAskOfTimestep = 0;

    //this vector will hold one "innerVector" for each timestamp in the orderBook
    std::vector<std::vector<std::string>> vectorChangeAtEndOfTimestep;
    //this inner vector will hold the timestamp (string) and the nominal change (double)
    std::vector<std::string> innerVector;

    int vectorIndex = 0;

    //this for loop will filter by the product and ordertype passed as parameters and then for each timestamp will loop in the orderbook to save the price in the first entry of that timestamp and the last price for that timestamp. When the timestamp changes, the process will be repeated
    for (int i = 0; i < orderBook.orders.size(); i++){    

        innerVector.clear();            
        
        //filter by product, only consider ask for this purpose

        if( priceInTheFirstAskOfTimestep == 0 &&
            orderBook.orders[i].orderType == orderType &&
            orderBook.orders[i].product.compare(product) == 0)
        {            
            priceInTheFirstAskOfTimestep = orderBook.orders[i].price;            
        }
        if( orderBook.orders[i].orderType == orderType &&
            orderBook.orders[i].product.compare(product) == 0)
        {            
            priceInTheLastAskOfTimestep = orderBook.orders[i].price;            
        }        

        innerVector.push_back(orderBook.orders[i].timestamp);        
        
        std::string timestepChangeResultStringified = std::to_string((priceInTheLastAskOfTimestep - priceInTheFirstAskOfTimestep)/priceInTheFirstAskOfTimestep);        
        
        innerVector.push_back(timestepChangeResultStringified);

        currentTimeStepInForLoop = orderBook.orders[i].timestamp;

        if(currentTimeStepInForLoop.compare(orderBook.orders[0].timestamp) == 0){
            vectorChangeAtEndOfTimestep.push_back(innerVector);            
        }
        if(currentTimeStepInForLoop.compare(lastSeenTimestep) != 0){
            vectorIndex =  vectorIndex +1;
            vectorChangeAtEndOfTimestep.push_back(innerVector);            
        }
        vectorChangeAtEndOfTimestep.at(vectorIndex) = innerVector;

        lastSeenTimestep = orderBook.orders[i].timestamp;        
    }    
    return vectorChangeAtEndOfTimestep;
};

//This function name describes the task performed: getting the average price for a given product from the beginning (the first entry of the orderBook) 
double OrderBook::getAveragePriceFromBeginningOfDataset(OrderBook& orderBook, std::string currentTime, std::string product, OrderBookType orderType){
    double averagePrice = 0;
    int entriesCounted = 0;
    double sumOfEntriesCounted = 0;
    std::cout<<"Calculating average price..." <<std::endl;
    
    //the orderBook is always sorted so orderBook.orders[0].timestamp is the first entry in chronological order 
    std::string currentTimeStepInForLoop = orderBook.orders[0].timestamp;
    std::string lastSeenTimestep = orderBook.orders[0].timestamp;
    int saveLastCurrentTimeIndex = 0;
    int saveLastEntryForCurrentTimeIndex = 0;
    int numberOfTimeStepsIterated =0;
    
    
    //find index of last entry of today
    for (int i = 0; i < orderBook.orders.size(); i++){
        currentTimeStepInForLoop = orderBook.orders[i].timestamp;
        if(currentTimeStepInForLoop <= currentTime){
            if(currentTimeStepInForLoop != lastSeenTimestep){
                lastSeenTimestep = currentTimeStepInForLoop;
                numberOfTimeStepsIterated = numberOfTimeStepsIterated+1;
            }
            saveLastEntryForCurrentTimeIndex = i;
        }else{
            //if the date in the dataset is bigger than the current time finish the loop iteration
            break;
        }
    }

    //reset counter for this loop
    numberOfTimeStepsIterated = 0;
    currentTimeStepInForLoop = orderBook.orders[saveLastEntryForCurrentTimeIndex].timestamp;
    //iterate number of days previous to this timestep        
    
    for (int i = saveLastEntryForCurrentTimeIndex; i > 0; i--){
                
        if( orderBook.orders[i].orderType == orderType &&
            orderBook.orders[i].product.compare(product) == 0)
        {
            entriesCounted = entriesCounted +1;
            sumOfEntriesCounted = sumOfEntriesCounted + orderBook.orders[i].price;
        }    
    }
    averagePrice = sumOfEntriesCounted/entriesCounted;    
    
    return averagePrice;
}

//This function name describes the task performed: getting the average price for the number of timestamps defined by the user. As an example when the user inputs 'avg BTC/USDT ask 2' this function will calculate the average price for BTC/USDT for during the last 2 timestamps
double OrderBook::getAveragePriceLastAmountOfDates(OrderBook& orderBook, std::string currentTime, std::string product, OrderBookType orderType, int numberOfTimeStepsDesired){

    double averagePrice = 0;
    int entriesCounted = 0;
    double sumOfEntriesCounted = 0;
    std::cout<<"Calculating average price..." <<std::endl;
    
    //the orderBook is always sorted so orderBook.orders[0].timestamp is the first entry in chronological order 
    std::string currentTimeStepInForLoop = orderBook.orders[0].timestamp;
    std::string lastSeenTimestep = orderBook.orders[0].timestamp;
    int saveLastCurrentTimeIndex = 0;
    int saveLastEntryForCurrentTimeIndex = 0;
    int numberOfTimeStepsIterated =0;

    //find index of last entry of today
    for (int i = 0; i < orderBook.orders.size(); i++){
        currentTimeStepInForLoop = orderBook.orders[i].timestamp;
        if(currentTimeStepInForLoop <= currentTime){
            if(currentTimeStepInForLoop != lastSeenTimestep){
                lastSeenTimestep = currentTimeStepInForLoop;
                numberOfTimeStepsIterated = numberOfTimeStepsIterated+1;
            }
            saveLastEntryForCurrentTimeIndex = i;
        }else{
            //if the date in the dataset is bigger than the current time finish the loop iteration
            break;
        }
    }
    
    std::string stringifyOrderType ="";

    if(numberOfTimeStepsIterated<numberOfTimeStepsDesired){
        
        std::cout<<"ERROR: you want to know the average price for the last " << numberOfTimeStepsDesired <<" timesteps but only "<< numberOfTimeStepsIterated << " timesteps have passed from the beginning "<<std::endl;
        std::cout << "advisorBot suggestion: Use the command step to advance to the next timestep" << std::endl;
        
        
        if(orderType == OrderBookType::ask){
            stringifyOrderType = "ask"; 
        }
        if(orderType == OrderBookType::bid){
            stringifyOrderType = "bid"; 
        }
        if(numberOfTimeStepsIterated>0){
            std::cout << "advisorBot suggestion: run avg " << product << " " << stringifyOrderType << " " << numberOfTimeStepsIterated << std::endl;
        }
        

    }else{        
        //reset counter for this loop
        numberOfTimeStepsIterated = 0;
        currentTimeStepInForLoop = orderBook.orders[saveLastEntryForCurrentTimeIndex].timestamp;
        //iterate number of days previous to this timestep        
        
        for (int i = saveLastEntryForCurrentTimeIndex; i > 0; i--){
            
            //std::cout<<"inside for loop:"<<std::endl;            
            

            if(orderBook.orders[i].timestamp != currentTimeStepInForLoop){                
                
                numberOfTimeStepsIterated = numberOfTimeStepsIterated +1;
                currentTimeStepInForLoop = orderBook.orders[i].timestamp;
                                
            }
            if( numberOfTimeStepsIterated <= numberOfTimeStepsDesired &&
                orderBook.orders[i].orderType == orderType &&
                orderBook.orders[i].product.compare(product) == 0)
            {
                entriesCounted = entriesCounted +1;
                sumOfEntriesCounted = sumOfEntriesCounted + orderBook.orders[i].price;
            }
            
            
        }
        averagePrice = sumOfEntriesCounted/entriesCounted;    
    }    
    
    return averagePrice;
}

//This function calculates the average difference between the lowest ask price and the highest bid price for a given product. Two timeframe options can be specified: calculate it from the beginning of the dataframe (first entry of the orderBook) or calculate it for the timestamp where the user in the current moment.
std::vector<double> OrderBook::processAverageBidAskSpread(std::string& product, std::string& timeframe, OrderBook& orderBook, std::string currentTime){    
    
    std::vector<double> resultBidAskSpread;

    if(timeframe.compare("from-the-beginning-until-current-dataframe")==0){
        
        std::cout<< "Processing your request..." <<std::endl;
                
        //the orderBook is always sorted so orderBook.orders[0].timestamp is the first entry in chronological order 
        std::string currentTimeStepInForLoop = orderBook.orders[0].timestamp;
        std::string lastSeenTimestep = orderBook.orders[0].timestamp;
        int saveLastCurrentTimeIndex = 0;
        int saveLastEntryForCurrentTimeIndex = 0;
        int numberOfTimeStepsIterated =0;
        int sumEntries =0;

        double sumOfNominalSpreads=0;
        double sumOfPercentageSpreads=0;

        //find index of last entry of today
        for (int i = 0; i < orderBook.orders.size(); i++){
            currentTimeStepInForLoop = orderBook.orders[i].timestamp;
            if(currentTimeStepInForLoop <= currentTime){
                if(currentTimeStepInForLoop != lastSeenTimestep){
                    lastSeenTimestep = currentTimeStepInForLoop;
                    numberOfTimeStepsIterated = numberOfTimeStepsIterated+1;
                }
                saveLastEntryForCurrentTimeIndex = i;
            }else{
                //if the date in the dataset is bigger than the current time finish the loop iteration
                break;
            }
        }
        

        if(numberOfTimeStepsIterated==0){
            std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid,product, currentTime);    
            double maxBid = getHighPrice(bidEntries);
            std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask,product, currentTime);    
            double minAsk = getHighPrice(askEntries);
            
            
            double nominalSpread = (minAsk-maxBid);
            double percentageSpread = (minAsk-maxBid)/minAsk*100;
            resultBidAskSpread.push_back(nominalSpread);
            resultBidAskSpread.push_back(percentageSpread);
        
        }else{
            
            currentTimeStepInForLoop = orderBook.orders[saveLastEntryForCurrentTimeIndex].timestamp;
            lastSeenTimestep = orderBook.orders[saveLastEntryForCurrentTimeIndex].timestamp;
            bool runOnceControlFlag = true;

            for (int i = saveLastEntryForCurrentTimeIndex; i >= 0; i--){
                
                currentTimeStepInForLoop = orderBook.orders[i].timestamp;
                
                if( runOnceControlFlag &&
                    (currentTimeStepInForLoop.compare(orderBook.orders[saveLastEntryForCurrentTimeIndex].timestamp)==0)
                )
                {
                    runOnceControlFlag = false;
                    
                    std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid,product, currentTimeStepInForLoop);
                    
                    double maxBid = getHighPrice(bidEntries);
                    
                    std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask,product, currentTimeStepInForLoop);    
                    
                    double minAsk = getHighPrice(askEntries);

                    double nominalSpread = (minAsk-maxBid);
                    double percentageSpread = (minAsk-maxBid)/minAsk*100;

                    sumOfNominalSpreads = sumOfNominalSpreads + nominalSpread;
                    sumOfPercentageSpreads = sumOfPercentageSpreads + percentageSpread;
                    sumEntries = sumEntries + 1;                            

                }
                if(currentTimeStepInForLoop.compare(lastSeenTimestep)!=0){
                                        
                    std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid,product, currentTimeStepInForLoop);
                    
                    double maxBid = getHighPrice(bidEntries);
                    
                    std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask,product, currentTimeStepInForLoop);    
                    
                    double minAsk = getHighPrice(askEntries);

                    double nominalSpread = (minAsk-maxBid);
                    double percentageSpread = (minAsk-maxBid)/minAsk*100;

                    sumOfNominalSpreads = sumOfNominalSpreads + nominalSpread;
                    sumOfPercentageSpreads = sumOfPercentageSpreads + percentageSpread;
                    sumEntries = sumEntries + 1;

                }
                lastSeenTimestep = currentTimeStepInForLoop;
                
            }
        }

        resultBidAskSpread.push_back(sumOfNominalSpreads/sumEntries);
        resultBidAskSpread.push_back(sumOfPercentageSpreads/sumEntries);

    }
    if(timeframe.compare("this-timestep-only")==0){
        std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid,product, currentTime);    
        double maxBid = getHighPrice(bidEntries);
        std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask,product, currentTime);    
        double minAsk = getHighPrice(askEntries);
        
        
        double nominalSpread = (minAsk-maxBid);
        double percentageSpread = (minAsk-maxBid)/minAsk*100;
        resultBidAskSpread.push_back(nominalSpread);
        resultBidAskSpread.push_back(percentageSpread);
        
    }
    return resultBidAskSpread;
};

//This function adds the amount of currency in the orderBook passed. The product and ordertype are previously filtered in the advisortBot.cpp file before calling this function.
double OrderBook::getMarketDepth(std::vector<OrderBookEntry>& orders)
{    
    double amount = 0;    
    int entriesCounted = 0;    
    
    for (int i = 0; i < orders.size(); i++)
    {
        amount = amount + orders[i].amount;        

    }    
    return amount;
}