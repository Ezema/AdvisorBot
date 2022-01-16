#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSVReader.h"
#include <vector>
#include "OrderBookEntry.h"

// as mentioned in the midterm report, the great majority of following getValue function was taken from the following webpage: https://stackoverflow.com/questions/12911299/read-csv-file-in-c
const char* getValue(char* line, int num){
        //declare a string
        const char* tok;

        //iterate over the line saving each string delimited by a comma
        for (tok = strtok(line, ",");
                tok && *tok;
                tok = strtok(NULL, ",\n"))
        {
            if (!--num)
                return tok;
        }
        return NULL;
};

//Implementation of the readCSV function delcared in the CSVReader.h file
std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename){        

    //declare vector that will store the exchange entries
    std::vector<OrderBookEntry> entries;    

    //declare C array of strings
    const char *token[5];
    //convert the argument csvFilename std::string into a C based string
    const char *fileName = csvFilename.c_str();

    //declare file pointer to access the CSV file
    FILE* stream = fopen(fileName, "r");

    //declare the maximum character size of a line in the CSV file
    char line[1024];
    
    //loop over the CSV line 
    while (fgets(line, 1024, stream)){
        
        //for each line in the CSV file, "tokenize" each word delimited by a comma symbol
        for (int i = 1; i < 6; i++)
        {
            //create a new copy of the CSV line
            char* tmp = strdup(line);
            
            //save a individual string delimited by a comma from the CSV line that is being read
            token[i-1] = getValue(tmp, i);            
                        
            //if the loop is in the last iteration i==5            
            if(i==5){            

                char* end;
                //convert C string stored in the array of strings to double
                double price = strtod(token[3], &end);
                //convert C string stored in the array of strings to double
                double amount = strtod(token[4], &end);
                
                //convert C strings stored in the array of strings to C++ std::string
                std::string stringifiedDate(token[0]);
                std::string stringifiedPair(token[1]);
                std::string stringifiedOrderType(token[2]);                

                //Create a new orderbook entry with the above values
                OrderBookEntry obe{price, 
                                amount,
                                stringifiedDate,
                                stringifiedPair, 
                                OrderBookEntry::stringToOrderBookType(stringifiedOrderType)};
                
                //Save orderbookentry created in the vector
                entries.push_back(obe);                
            }
            free(tmp);
        }
    }  
    return entries;
} 

//Implementation of the tokenise function delcared in the CSVReader.h file
std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
   std::vector<std::string> tokens;
   signed int start, end;
   std::string token;
    start = csvLine.find_first_not_of(separator, 0);
    do{
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
    start = end + 1;
    }while(end > 0);

   return tokens; 
}