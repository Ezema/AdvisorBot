#include "advisorBot.h"
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "CSVReader.h"

void advisorBot::init(){
    
    std::string input;
    int menuPrintCount = 0;

    //copied from MerkelMain sample code line 15
    currentTime = orderBook.getEarliestTime();
    
    while(true){
        printMainMenu(menuPrintCount);
        
        //obtain the user input
        std::getline(std::cin, input);
        
        //separate each word that was input by the user and store the result in a vector of strings
        std::vector<std::string> userSelectedMenuOption = CSVReader::tokenise(input, ' ');

        //send the vector of strings created by the user input to be processed by processUserOption()
        processUserOption(userSelectedMenuOption);
    }
        
};

//function in charge of printing the main menu. The argument will be used to control that only on the first run the logo is printed
void advisorBot::printMainMenu(int& menuPrintCount){
    //control if the software is running for the first time. If it is, show the welcome screen
    if(menuPrintCount==0){
        std::cout<< "==========================================================================================";
        
        //The following text was created with the help of the following online tool: https://patorjk.com/software/taag/#p=display&v=0&f=Standard&t=Welcome!%0A
        std::cout<< R"(
                    __        __   _                          _ 
                    \ \      / /__| | ___ ___  _ __ ___   ___| |
                     \ \ /\ / / _ \ |/ __/ _ \| '_ ` _ \ / _ \ |
                      \ V  V /  __/ | (_| (_) | | | | | |  __/_|
                       \_/\_/ \___|_|\___\___/|_| |_| |_|\___(_)
        )";

        //The following text was created with the help of the following online tool: https://patorjk.com/software/taag/#p=display&v=0&f=ANSI%20Shadow&t=AdvisorBot
        std::cout<< R"(
         █████╗ ██████╗ ██╗   ██╗██╗███████╗ ██████╗ ██████╗ ██████╗  ██████╗ ████████╗
        ██╔══██╗██╔══██╗██║   ██║██║██╔════╝██╔═══██╗██╔══██╗██╔══██╗██╔═══██╗╚══██╔══╝
        ███████║██║  ██║██║   ██║██║███████╗██║   ██║██████╔╝██████╔╝██║   ██║   ██║   
        ██╔══██║██║  ██║╚██╗ ██╔╝██║╚════██║██║   ██║██╔══██╗██╔══██╗██║   ██║   ██║   
        ██║  ██║██████╔╝ ╚████╔╝ ██║███████║╚██████╔╝██║  ██║██████╔╝╚██████╔╝   ██║   
        ╚═╝  ╚═╝╚═════╝   ╚═══╝  ╚═╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚═════╝  ╚═════╝    ╚═╝   
        )" << std::endl;
        std::cout<< "=========================================================================================="<<std::endl;
        std::cout<< "University of London student number: REDACTED"<<std::endl;
        std::cout<< "=========================================================================================="<<std::endl;
    }

    //the menuPrintCount variable is used to control the display of the welcome screen
    menuPrintCount = menuPrintCount+1;
    std::cout<<""<<std::endl;
    std::cout<< "======================================="<<std::endl;
    std::cout << "The current time is: " << currentTime << std::endl;
    std::cout<< "--------------------------------------"<<std::endl;
    std::cout << "advisorbot> Please enter a command, or help for a list of commands" << std::endl;
    std::cout << "user> ";
};

//implementation of the processUserOption() method declared on the advisorBot.h file
void advisorBot::processUserOption(std::vector<std::string>& userSelectedMenuOption){
    
    //leave some space for aesthetic purposes
    std::cout << "" << std::endl;
    
    //there is not valid command with more than 3 arguments so an input of more than 4 words is always wrong
    if(userSelectedMenuOption.size()>4){
        std::cout << "advisorbot>Error: entered too many arguments" << std::endl;
        return;
    }
    //validate whether the user entered at least one word. Commands like 'help' or 'time' are one word commands
    if(userSelectedMenuOption.size()<1){
        std::cout << "advisorbot> Error: No arguments" << std::endl;
        return;
    }
    
    //normalize user input into lowecase so both avg and AVG can be used indistinctively by the user
    for (int argument = 0; argument < userSelectedMenuOption.size(); argument++)
    {
        //the method to convert the characters into lower case characters was copied from the following website: 
        //https://thispointer.com/converting-a-string-to-upper-lower-case-in-c-using-stl-boost-library/

        std::for_each(userSelectedMenuOption[argument].begin(), userSelectedMenuOption[argument].end(), [](char & wordCharacter){
            wordCharacter = ::tolower(wordCharacter);
        });
    } 
    
    //check if the first word input is help
    if (userSelectedMenuOption[0].compare("help")==0)
    {   
        //if the command help was introduced with no arguments show the available commands
        if (userSelectedMenuOption.size()==1){            
            std::cout << "advisorbot> The available commands are:"<<std::endl;
            std::cout << "              • help"<<std::endl;
            std::cout << "              • help [command]"<<std::endl;                        
            std::cout << "              • prod"<<std::endl;
            std::cout << "              • min"<<std::endl;
            std::cout << "              • max"<<std::endl;
            std::cout << "              • avg"<<std::endl;
            std::cout << "              • predict"<<std::endl;
            std::cout << "              • time"<<std::endl;
            std::cout << "              • step"<<std::endl;
            std::cout << "              • statistics"<<std::endl;
        }else if (userSelectedMenuOption.size()==2){
            //if the command help was introduced with 1 argument check the argument
            
            //check if the argument for the help command is help (help help)
            if (userSelectedMenuOption[1].compare("help")==0){                
                std::cout << "advisorbot> help usage: help [command]"<<std::endl;
                std::cout << "advisorbot> description: help without arguments lists all the available commands in the advisorbot app. help with one extra argument (e.g: help prod) will show the explanation and usage for the the command that is being passed as a second argument."<<std::endl;                
            }
            //check if the argument for the help command is prod (help prod)
            else if (userSelectedMenuOption[1].compare("prod")==0){
                std::cout << "advisorbot> prod usage: prod"<<std::endl;
                std::cout << "advisorbot> description: prod shows all the available cryptocurrency products"<<std::endl;
            }
            //check if the argument for the help command is min (help min)
            else if (userSelectedMenuOption[1].compare("min")==0){
                std::cout << "advisorbot> min usage: min PRODUCT ORDER_TYPE"<<std::endl;
                std::cout << "advisorbot> min example: min BTC/USDT ask"<<std::endl;
                std::cout << "advisorbot> min example: min ETH/USDT bid"<<std::endl;
                std::cout << "advisorbot> description: min shows the minimum ask or minimum bid of the product specified. To see the available products use the command prod"<<std::endl;
            }
            //check if the argument for the help command is max (help max)
            else if (userSelectedMenuOption[1].compare("max")==0){
                std::cout << "advisorbot> max usage: max PRODUCT ORDER_TYPE"<<std::endl;
                std::cout << "advisorbot> max example: max BTC/USDT ask"<<std::endl;
                std::cout << "advisorbot> max example: max ETH/USDT bid"<<std::endl;
                std::cout << "advisorbot> description: max shows the maximum ask or maximum bid of the product specified. To see the available products use the command prod"<<std::endl;
            }
            //check if the argument for the help command is avg (help avg)
            else if (userSelectedMenuOption[1].compare("avg")==0){
                std::cout << "advisorbot> avg usage: avg PRODUCT ORDER_TYPE TIMESTEPS"<<std::endl;
                std::cout << "advisorbot> avg example: avg BTC/USDT ask 10"<<std::endl;
                std::cout << "advisorbot> avg example: avg BTC/USDT ask -1"<<std::endl;
                std::cout << "advisorbot> avg example: avg ETH/USDT bid 2"<<std::endl;
                std::cout << "advisorbot> description: avg shows the average bid or average ask for the specified product over the specified timesteps. If you want to see the historical average introduce a -1 in the timestep argument field"<<std::endl;
            }
            //check if the argument for the help command is predict (help predict)
            else if (userSelectedMenuOption[1].compare("predict")==0){
                //predict max ETH/BTC bid
                std::cout << "advisorbot> predict usage: predict MAX/MIN PRODUCT ORDER_TYPE"<<std::endl;
                std::cout << "advisorbot> avg example: predict max BTC/USDT ask"<<std::endl;
                std::cout << "advisorbot> avg example: predict min BTC/USDT bid"<<std::endl;
                std::cout << "advisorbot> description: the predict command predicts the minimum or maximum ask or bid for the specified product for the next future timestep."<<std::endl;
            }
            //check if the argument for the help command is time (help time)
            else if (userSelectedMenuOption[1].compare("time")==0){
                std::cout << "advisorbot> time usage: time"<<std::endl;
                std::cout << "advisorbot> description: time shows the current timestep"<<std::endl;
            }
            //check if the argument for the help command is step (help step)
            else if (userSelectedMenuOption[1].compare("step")==0){
                std::cout << "advisorbot> step usage: step"<<std::endl;
                std::cout << "advisorbot> description: step advances to the next time step"<<std::endl;
            }
            //check if the argument for the help command is statistics (help statistics)
            else if (userSelectedMenuOption[1].compare("statistics")==0)
            {                
                std::cout << "advisorbot> statistics usage: statistics"<<std::endl;
                std::cout << "advisorbot> description: statistics allows you to access advanced statistics for the exchange products"<<std::endl;
            }
            //if the argument was not recognized show descriptive error to the user
            else{
                std::cout << "advisorbot> ERROR: unknown argument '"<< userSelectedMenuOption[1]  << "' for help"<< std::endl;
                std::cout << "advisorbot> Use help to see the available commands that can be used as arguments for help"<< std::endl;
            }
        }
        //if the help command has more than 1 argument show an error
        else{
            std::cout << "advisorbot> ERROR: Too many arguments for command help." << std::endl;
            std::cout << "advisorbot> The command help uses zero or one argument. For more information run 'help help'" << std::endl;
        }
    } //check if the first word input is the command prod
    else if (userSelectedMenuOption[0].compare("prod")==0) 
    {   
        listProducts();
    } //check if the first word input is the command min
    else if (userSelectedMenuOption[0].compare("min")==0)
    {        
        //verifying correct number of argument
        if (userSelectedMenuOption.size()==3){
            //get vector of strings that contain the available products in the dataset
            std::vector<std::string> productsAvailable = orderBook.getKnownProducts();

            //compare user input product argument with available products in the dataset
            for (std::string& product : productsAvailable){          
                
                //save "original" uppercase product name to later pass it as an argument
                std::string upperCaseProduct = product;
                
                //convert product names strings to lowercase to be able to compare them
                std::for_each(product.begin(), product.end(), [](char & productCharacter){
                    productCharacter = ::tolower(productCharacter);                                    
                }); 
                //if the user input a valid product, proceed to check whether the following argument "order type" is valid (bid or ask)               
                if(userSelectedMenuOption[1].compare(product)==0){
                    if(userSelectedMenuOption[2].compare("bid")==0){
                        double resultMinimumBidForProduct = minimumBidForProduct(upperCaseProduct,currentTime);
                        std::cout << "advisorbot> "<< product <<" minimum bid: " << resultMinimumBidForProduct << std::endl;
                        std::cout << "advisorbot> Information last updated : "<< currentTime << std::endl;
                    }else if(userSelectedMenuOption[2].compare("ask")==0){
                        double resultMinimumAskForProduct = minimumAskForProduct(upperCaseProduct,currentTime);
                        std::cout << "advisorbot> "<< product <<" minimum ask: " << resultMinimumAskForProduct <<std::endl; 
                        std::cout << "advisorbot> Information last updated : "<< currentTime << std::endl;
                    }//show descriptive error message for the user when the order type was not recognized
                    else{
                        std::cout << "advisorbot> ERROR: Unrecognized argument: '"<< userSelectedMenuOption[2] <<"' "  <<std::endl; 
                        std::cout << "advisorbot> Use help : "<< currentTime << std::endl;
                    }               
                };
            }
        }else{
            std::cout<< "advisorbot> ERROR: inccorect number of arguments for command '"<< userSelectedMenuOption[0] << "'." << std::endl;
            std::cout<< "advisorbot> You introduced "<< userSelectedMenuOption.size() << " arguments and the " << userSelectedMenuOption[0]<< " command must have 2 arguments"<< std::endl;
        }
    } //check if the first word input is the command max
    else if (userSelectedMenuOption[0].compare("max")==0)
    {
        //verifying correct number of argument
        if (userSelectedMenuOption.size()==3){
            //get vector of strings that contain the available products in the dataset
            std::vector<std::string> productsAvailable = orderBook.getKnownProducts();

            //compare user input product argument with available products in the dataset
            for (std::string& product : productsAvailable){          
                //save "original" uppercase product name to later pass it as an argument
                std::string upperCaseProduct = product;
                //convert product names strings to lowercase to be able to compare them
                std::for_each(product.begin(), product.end(), [](char & productCharacter){
                    productCharacter = ::tolower(productCharacter);
                                    
                });
                
                //if the user input a valid product, proceed to check whether the following argument "order type" is valid (bid or ask)               
                if(userSelectedMenuOption[1].compare(product)==0){                    
                    if(userSelectedMenuOption[1].compare(product)==0){
                        if(userSelectedMenuOption[2].compare("bid")==0){
                            double resultMaximumBidForProduct = maximumBidForProduct(upperCaseProduct,currentTime);
                            std::cout << "advisorbot> "<< product <<" maximum bid: " << resultMaximumBidForProduct << std::endl;
                            std::cout << "advisorbot> Information last updated : "<< currentTime << std::endl;
                        }
                        else if(userSelectedMenuOption[2].compare("ask")==0){
                            double resultMaximumAskForProduct = maximumAskForProduct(upperCaseProduct,currentTime);
                            std::cout << "advisorbot> "<< product <<" maximum ask: " << resultMaximumAskForProduct <<std::endl;
                            std::cout << "advisorbot> Information last updated : "<< currentTime << std::endl;
                        }//show descriptive error message for the user when the order type was not recognized
                        else{
                            std::cout << "advisorbot> ERROR: Unrecognized argument: '"<< userSelectedMenuOption[2] <<"' "  <<std::endl; 
                            std::cout << "advisorbot> Use help : "<< currentTime << std::endl;
                        }
                    };
                };
            }
        }else{
            std::cout<< "advisorbot> ERROR: inccorect number of arguments for command '"<< userSelectedMenuOption[0] << "'." << std::endl;
            std::cout<< "advisorbot> You introduced "<< userSelectedMenuOption.size() << " arguments and the " << userSelectedMenuOption[0]<< " command must have 2 arguments"<< std::endl;
        }
    } //check if the first word input is the command avg
    else if (userSelectedMenuOption[0].compare("avg")==0)
    {
        //pass the user input (vector of strings) to the handler processAveragePriceQuery()
        processAveragePriceQuery(userSelectedMenuOption);
    } //check if the first word input is the command predict
    else if (userSelectedMenuOption[0].compare("predict")==0)
    {
        //pass the user input (vector of strings) to the handler processPredictPriceQuery()
       processPredictPriceQuery(userSelectedMenuOption);
    } //check if the first word input is the command time
    else if (userSelectedMenuOption[0].compare("time")==0)
    {
        //show current time
        std::cout << "Current time: " << currentTime << std::endl;
    } //check if the first word input is the command step
    else if (userSelectedMenuOption[0].compare("step")==0)
    {        
        //Call nextStepInTime() (copied from provided sample code) to go to the next time step
        nextStepInTime();
    } //check if the first word input is the command statistics
    else if (userSelectedMenuOption[0].compare("statistics")==0)
    {   
        //pass the user input (vector of strings) to the handler runStatisticsCommand()
        runStatisticsCommand(userSelectedMenuOption);
    }//show descriptive error to the user in case the first input string (that is the command name) is not recognized
    else{
        std::cout << "advisorbot> ERROR: The command you introduced '"<< userSelectedMenuOption[0] << "' is unrecognizable" << std::endl;
        std::cout << "advisorbot> Use the help command to see a list of available commands" << std::endl;
    }
}

//// PROD command handler
////////////////////////////////////
void advisorBot::listProducts(){    
    std::cout << "advisorbot> List of available products: " << std::endl;
    //iterates over the vector of strings returned by orderBook.getKnownProducts()
    for (std::string const& product : orderBook.getKnownProducts())
    {
        std::cout << "                  • " << product << std::endl;
    }
}

//// min and max commands handlers
////////////////////////////////////
// implementation of the function minimumBidForProduct() defined in the advisorBot.h file
double advisorBot::minimumBidForProduct(std::string& product, std::string& currentTime){
    //receives the product desired from the function call argument 
    //forms a vector of OrderBookEntry types for the product and order type specified in the current timestamp
    std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid,product, currentTime);
    // calls getLowPrice() with the vector of entries previously obtained and returns the value    
    return OrderBook::getLowPrice(bidEntries);
}
// implementation of the function minimumAskForProduct() defined in the advisorBot.h file
double advisorBot::minimumAskForProduct(std::string& product, std::string& currentTime){
    //receives the product desired from the function call argument 
    //forms a vector of OrderBookEntry types for the product and order type specified in the current timestamp
    std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask,product, currentTime);
    // calls getLowPrice() with the vector of entries previously obtained and returns the value
    return OrderBook::getLowPrice(askEntries);
}
// implementation of the function maximumBidForProduct() defined in the advisorBot.h file
double advisorBot::maximumBidForProduct(std::string& product, std::string& currentTime){
    //receives the product desired from the function call argument 
    //forms a vector of OrderBookEntry types for the product and order type specified in the current timestamp
    std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid,product, currentTime);
    // calls getHighPrice() with the vector of entries previously obtained and returns the value
    return OrderBook::getHighPrice(bidEntries);    
}
// implementation of the function maximumAskForProduct() defined in the advisorBot.h file
double advisorBot::maximumAskForProduct(std::string& product, std::string& currentTime){
    //receives the product desired from the function call argument 
    //forms a vector of OrderBookEntry types for the product and order type specified in the current timestamp
    std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask,product, currentTime);
    // calls getHighPrice() with the vector of entries previously obtained and returns the value
    return OrderBook::getHighPrice(askEntries);    
}

//// step command handler
////////////////////////////////////
// implementation of the function nextStepInTime() defined in the advisorBot.h file
void advisorBot::nextStepInTime()
{   
    //calls the function getNextTime() that is provided in the sample code
    currentTime = orderBook.getNextTime(currentTime);
    std::cout << "advisorBot> Operation finished " << std::endl;
    std::cout << "advisorBot> New timestep is: " << currentTime << std::endl;
}

//// avg command handler
////////////////////////////////////
// implementation of the function processAveragePriceQuery() defined in the advisorBot.h file
void advisorBot::processAveragePriceQuery(std::vector<std::string>& userSelectedMenuOption){
    //// validate that the length of the user input is 4 words (avg arg1 arg2 arg3)
    if (userSelectedMenuOption.size()==4){
        //get vector of strings that contain the available products in the dataset
        std::vector<std::string> productsAvailable = orderBook.getKnownProducts();
        bool userProductWasFound = false;

        //compare user input product argument with available products in the dataset
        for (std::string& product : productsAvailable){          
            //save "original" uppercase product name to later pass it as an argument
            std::string upperCaseProduct = product;
            //convert product names strings to lowercase to be able to compare them
            std::for_each(product.begin(), product.end(), [](char & productCharacter){
                productCharacter = ::tolower(productCharacter);
                                
            });
            
            //if the user input a valid product, proceed to check whether the following argument "order type" is valid (bid or ask)               
            if(userSelectedMenuOption[1].compare(product)==0){
                userProductWasFound = true;
                
                int numberOfTimeSteps = -1;
                bool numberOfTimeStepsIsDefined = true;
                
                //convert user input string into an integer
                try{                            
                    numberOfTimeSteps = std::stoi(userSelectedMenuOption[3],nullptr,0);                            
                }catch(const std::exception& e){           
                    //if the conversion to int is not successful turn the flag to false
                    numberOfTimeStepsIsDefined = false;
                }                        

                //check if the order type is valid (bid or ask)
                if(userSelectedMenuOption[2].compare("bid")==0){        
                    if(numberOfTimeStepsIsDefined){
                        //if the user input was bid, convert the string into an orderBookType
                        OrderBookType orderType = OrderBookType::bid;
                        //declare flag for later use
                        bool fromBeginningOfDatasetFlag = false;
                        
                        double averagePriceResult;
                        
                        // the user has the option to write -1 to see the historical average
                        if(numberOfTimeSteps==-1){
                            //turn the boolean flag to true
                            fromBeginningOfDatasetFlag = true;
                            //get average price from the beginning of the dataset
                            averagePriceResult = orderBook.getAveragePriceFromBeginningOfDataset(orderBook, currentTime,upperCaseProduct,orderType);
                        }else{
                            //if the numberOfTimeSteps is not -1, it is any other integer
                            //get average price for the desired amount of timesteps
                            averagePriceResult = orderBook.getAveragePriceLastAmountOfDates(orderBook, currentTime,upperCaseProduct,orderType,numberOfTimeSteps);
                        }

                        //different output messages depending if the user input was for the whole dataset (-1) or for a defined amount of timesteps
                        if(averagePriceResult>0 && !fromBeginningOfDatasetFlag){                            
                            std::cout<<"The average "<< upperCaseProduct << " " <<  userSelectedMenuOption[2] << " price for the last "<< numberOfTimeSteps <<" timesteps is: "<< averagePriceResult << std::endl;
                        }
                        if(averagePriceResult>0 && fromBeginningOfDatasetFlag){
                            std::cout<<"The average "<< upperCaseProduct << " " <<  userSelectedMenuOption[2] << " price from the beginning of the series is: "<< averagePriceResult << std::endl;
                        }
                    }else{
                        //timestep string to int conversion unsuccessful
                        std::cout<< "The timestep you specified: '" << userSelectedMenuOption[3]  << "' is invalid" << std::endl;
                        std::cout<< "Timesteps must be numbers"<< std::endl;
                    }
                    
                }else if(userSelectedMenuOption[2].compare("ask")==0){                            
                    if(numberOfTimeStepsIsDefined){
                        //if the user input was ask, convert the string into an orderBookType
                        OrderBookType orderType = OrderBookType::ask;
                        //declare flag for later use
                        bool fromBeginningOfDatasetFlag = false;
                        
                        double averagePriceResult;
                        
                        // the user has the option to write -1 to see the historical average
                        if(numberOfTimeSteps==-1){
                            //turn the boolean flag to true
                            fromBeginningOfDatasetFlag = true;
                            //get average price from the beginning of the dataset
                            averagePriceResult = orderBook.getAveragePriceFromBeginningOfDataset(orderBook, currentTime,upperCaseProduct,orderType);
                        }else{
                            //if the numberOfTimeSteps is not -1, it is any other integer
                            //get average price for the desired amount of timesteps
                            averagePriceResult = orderBook.getAveragePriceLastAmountOfDates(orderBook, currentTime,upperCaseProduct,orderType,numberOfTimeSteps);
                        }
                        //different output messages depending if the user input was for the whole dataset (-1) or for a defined amount of timesteps
                        if(averagePriceResult>0 && !fromBeginningOfDatasetFlag){
                            std::cout<<"The average "<< upperCaseProduct << " " <<  userSelectedMenuOption[2] << " price for the last "<< numberOfTimeSteps <<" timesteps is: "<< averagePriceResult << std::endl;
                        }
                        if(averagePriceResult>0 && fromBeginningOfDatasetFlag){
                            std::cout<<"The average "<< upperCaseProduct << " " <<  userSelectedMenuOption[2] << " price from the beginning of the series is: "<< averagePriceResult << std::endl;
                        }
                    }else{
                        //timestep string to int conversion unsuccessful
                        std::cout<< "The timestep you specified: '" << userSelectedMenuOption[3]  << "' is invalid" << std::endl;
                        std::cout<< "Timesteps must be numbers"<< std::endl;
                    }

                }else{
                    //add message for neither ask nor bid found
                    std::cout<< "The type of order you specified: " << userSelectedMenuOption[2]  << " is invalid" << std::endl;
                }
                //finish loop over available products
                break;                        
            };                    
        }
        if(userProductWasFound==false){
            //add message for product/pair not found
            std::cout<< "The product you specified: " << userSelectedMenuOption[1]  << " was not found" << std::endl;
        }

    }else{
        std::cout << "advisorbot> ERROR: incorrect number of arguments " << std::endl;
        std::cout << "advisorbot> The command avg takes 3 arguments and you entered "<< (userSelectedMenuOption.size()-1) << " arguments." << std::endl;
        std::cout << "advisorbot> Here is an example using the avg command:" << std::endl;
        std::cout << "advisorbot> avg ETH/BTC ask 10" << std::endl;
        std::cout << "advisorbot> For more information on how to use the avg command enter 'help avg'" << std::endl;            
    }
}

////////////////////////////////////

//// predict command handler
////////////////////////////////////
// implementation of the function processPredictPriceQuery() defined in the advisorBot.h file
void advisorBot::processPredictPriceQuery(std::vector<std::string>& userSelectedMenuOption){
    if (userSelectedMenuOption.size()==4){        

            bool userProductWasFound = false;
            
            std::vector<std::vector<std::string>> vectorOfDailyChanges;
            vectorOfDailyChanges.clear();
            
            if(userSelectedMenuOption[1].compare("min")==0){
                //get vector of strings that contain the available products in the dataset
                std::vector<std::string> productsAvailable = orderBook.getKnownProducts();                

                for (std::string& product : productsAvailable){         
                    //save "original" uppercase product name to later pass it as an argument
                    std::string upperCaseProduct = product;
                    
                    //convert product names strings to lowercase to be able to compare them
                    std::for_each(product.begin(), product.end(), [](char & productCharacter){
                        productCharacter = ::tolower(productCharacter);
                                        
                    });
                    //check if the product in the current loop iteration matches the user input product
                    if(userSelectedMenuOption[2].compare(product)==0){
                        //make the user product was found flag true to signal the application that the input was valid
                        userProductWasFound = true;
                        //check if the order type is valid
                        if(userSelectedMenuOption[3].compare("ask")==0){                            
                            //if the user input was ask, convert the string into an orderBookType
                            OrderBookType orderType = OrderBookType::ask;
                            //obtain a 2 dimensional vector that contains each timestamp daily price change 
                            vectorOfDailyChanges = orderBook.calculateChangesAtEndOfTimestep(orderBook,upperCaseProduct,orderType);
                            
                            //pass the previously obtained vector of timestamp price changes to calculate the average gain or loss up to the specified date
                            double historicalAverageGainOrLossResult = OrderBook::calculateHistoricalAverageGainOrLossUpToSpecifiedDate(vectorOfDailyChanges,currentTime);

                            //get the minimum ask in the current time
                            double minimumAsk = minimumAskForProduct(upperCaseProduct, currentTime);                            
                            
                            //the prediction for the next minimum ask price will be given by adding the current minimum ask price to itself multiplied by the historical timestamp price change 
                            double predictionBasedOnHistoricalAverage = minimumAsk + (minimumAsk * historicalAverageGainOrLossResult);
                                                        
                            std::cout << "advisorBot> Based on the historical trend the minimum ask prediction is: "<< predictionBasedOnHistoricalAverage << std::endl;

                        }else if(userSelectedMenuOption[3].compare("bid")==0){
                            //if the user input was bid, convert the string into an orderBookType
                            OrderBookType orderType = OrderBookType::bid;
                            //obtain a 2 dimensional vector that contains each timestamp daily price change 
                            vectorOfDailyChanges = orderBook.calculateChangesAtEndOfTimestep(orderBook,upperCaseProduct,orderType);
                            
                            //pass the previously obtained vector of timestamp price changes to calculate the average gain or loss up to the specified date
                            double historicalAverageGainOrLossResult = OrderBook::calculateHistoricalAverageGainOrLossUpToSpecifiedDate(vectorOfDailyChanges,currentTime);

                            //get the minimum bid in the current time
                            double minimumBid = minimumBidForProduct(upperCaseProduct, currentTime);
                            
                            //the prediction for the next minimum bid price will be given by adding the current minimum bid price to itself multiplied by the historical timestamp price change 
                            double predictionBasedOnHistoricalAverage = minimumBid + (minimumBid * historicalAverageGainOrLossResult);
                            
                            std::cout << "advisorBot> Based on the historical trend the minimum bid prediction is: "<< predictionBasedOnHistoricalAverage << std::endl;
                        }else{
                            //if the order type was not bid or ask show a descriptive error to the user
                            std::cout<< "advisorBot> ERROR: Unknown order type: " << userSelectedMenuOption[3]  << std::endl;
                            std::cout<< "advisorBot> The type of order you specified: " << userSelectedMenuOption[3]  << " is invalid" << std::endl;
                        }
                        
                        //finish loop over available products because it was already found
                        break;                        
                    };                    
                }
                if(!userProductWasFound){
                    //add message for product not found
                    std::cout<< "advisorBot> ERROR: Unknown product: " << userSelectedMenuOption[2]  << std::endl;
                    std::cout<< "The product you specified: " << userSelectedMenuOption[2]  << " was not found" << std::endl;
                }
            }else if(userSelectedMenuOption[1].compare("max")==0){
                //get vector of strings that contain the available products in the dataset
                std::vector<std::string> productsAvailable = orderBook.getKnownProducts();                

                for (std::string& product : productsAvailable){          
                    //save "original" uppercase product name to later pass it as an argument
                    std::string upperCaseProduct = product;
                    //convert product names strings to lowercase to be able to compare them
                    std::for_each(product.begin(), product.end(), [](char & productCharacter){
                        productCharacter = ::tolower(productCharacter);
                                        
                    });
                    //check if the product in the current loop iteration matches the user input product
                    if(userSelectedMenuOption[2].compare(product)==0){
                        //make the user product was found flag true to signal the application that the input was valid
                        userProductWasFound = true;                      
                        //check if the order type is valid  
                        if(userSelectedMenuOption[3].compare("ask")==0){      
                            //if the user input was ask, convert the string into an orderBookType                      
                            OrderBookType orderType = OrderBookType::ask;
                            
                            //obtain a 2 dimensional vector that contains each timestamp daily price change
                            vectorOfDailyChanges = orderBook.calculateChangesAtEndOfTimestep(orderBook,upperCaseProduct,orderType);

                            //pass the previously obtained vector of timestamp price changes to calculate the average gain or loss up to the specified date
                            double historicalAverageGainOrLossResult = OrderBook::calculateHistoricalAverageGainOrLossUpToSpecifiedDate(vectorOfDailyChanges,currentTime);

                            //get the maximum ask in the current time
                            double maximumAsk = maximumAskForProduct(upperCaseProduct, currentTime);                            
                            
                            //the prediction for the next maximum ask price will be given by adding the current maximum ask price to itself multiplied by the historical timestamp price change 
                            double predictionBasedOnHistoricalAverage = maximumAsk + (maximumAsk * historicalAverageGainOrLossResult);
                            
                            std::cout << "advisorBot> Based on the historical trend the maximum ask prediction is: "<< predictionBasedOnHistoricalAverage << std::endl;

                        }else if(userSelectedMenuOption[3].compare("bid")==0){
                            //if the user input was ask, convert the string into an orderBookType                      
                            OrderBookType orderType = OrderBookType::bid;
                            
                            //obtain a 2 dimensional vector that contains each timestamp daily price change
                            vectorOfDailyChanges = orderBook.calculateChangesAtEndOfTimestep(orderBook,upperCaseProduct,orderType);
                            
                            //pass the previously obtained vector of timestamp price changes to calculate the average gain or loss up to the specified date
                            double historicalAverageGainOrLossResult = OrderBook::calculateHistoricalAverageGainOrLossUpToSpecifiedDate(vectorOfDailyChanges,currentTime);
                            
                            //get the maximum bid in the current time
                            double maximumBid = maximumBidForProduct(upperCaseProduct, currentTime);                            
                            
                            //the prediction for the next maximum bid price will be given by adding the current maximum bid price to itself multiplied by the historical timestamp price change 
                            double predictionBasedOnHistoricalAverage = maximumBid + (maximumBid * historicalAverageGainOrLossResult);
                            
                            std::cout << "advisorBot> Based on the historical trend the maximum bid prediction is: "<< predictionBasedOnHistoricalAverage << std::endl;

                        }else{                       
                            //add message for product not found     
                            std::cout<< "advisorBot> ERROR: Unknown order type: " << userSelectedMenuOption[3]  << std::endl;
                            std::cout<< "advisorBot> The type of order you specified: " << userSelectedMenuOption[3]  << " is invalid" << std::endl;
                        }
                        
                        //finish loop over available products because it was already found
                        break;                        
                    };                    
                }
                if(!userProductWasFound){
                    //add message for product not found
                    std::cout<< "advisorBot> ERROR: Unknown product: " << userSelectedMenuOption[2]  << std::endl;
                    std::cout<< "The product you specified: " << userSelectedMenuOption[2]  << " was not found" << std::endl;
                }
            }else{
                //add message for min/max (second argument of predict command) not found
                std::cout<< "advisorBot> ERROR: Unknown argument: " << userSelectedMenuOption[1]  << std::endl;
                std::cout<< "advisorBot> only max or min are allowed arguments for predict" << std::endl;
                std::cout<< "advisorBot> run help predict for more information" << std::endl;
            }            
        }else{
            //add message for excessive number of arguments
            std::cout << "advisorbot> ERROR: incorrect number of arguments " << std::endl;
            std::cout << "advisorbot> The command predict takes 3 arguments and you entered "<< (userSelectedMenuOption.size()-1) << " arguments." << std::endl;
            std::cout << "advisorbot> Here is two examples using the predict command:" << std::endl;
            std::cout << "advisorbot> predict max ETH/BTC ask" << std::endl;
            std::cout << "advisorbot> predict min ETH/BTC bid" << std::endl;
            std::cout << "advisorbot> For more information on how to use the predict command enter 'help predict'" << std::endl;
            
    }
}

//////statistics command handler
////////////////////////////////////
// implementation of the function runStatisticsCommand() defined in the advisorBot.h file
void advisorBot::runStatisticsCommand(std::vector<std::string>& userSelectedMenuOption){
    std::cout << "statistics> Please select one of the following: " << std::endl;    
    std::cout << "              • (1) Average bid-ask spread " << std::endl;
    std::cout << "              • (2) Market depth & sentiment analysis " << std::endl;    
    std::cout << "" << std::endl;    
    std::cout << "statistics> or write exit to go back to the main menu " << std::endl;    
    std::string userInput ="";
    std::cout << "user> ";
    //save user input in userInput variable
    std::getline(std::cin, userInput);
    
    //if the input was a 1 the user wants the "Average bid-ask spread" statistics
    if(userInput.compare("1")==0){
        std::cout << "======   Average bid-ask spread   ======" << std::endl;
        std::cout << "Please select write the product for which you want to see the average bid-ask spread: " << std::endl;    
        
        //get vector of strings that contain the available products in the dataset
        std::vector<std::string> productsAvailable = orderBook.getKnownProducts();
        //show the user the list of available products
        for (std::string const& product : productsAvailable)
        {
            std::cout << "                  • " << product << std::endl;
        }
        
        std::string userInput ="";
        std::cout << "user> ";

        //save user input in userInput variable
        std::getline(std::cin, userInput);
        bool correctInput = false;        
        
        //convert user input into lowercase to be able to compare it
        std::string lowerCaseUserInput = stringToLowerCase(userInput);
        
        //iterate over the vector of avaiable products
        for (std::string& product : productsAvailable){          
            //save "original" uppercase product name to later pass it as an argument
            std::string upperCaseProduct = product;
            //convert product names strings to lowercase to be able to compare them
            std::string lowerCaseProduct = stringToLowerCase(product);
            
            //if the user input coincides with a valid product name
            if(lowerCaseUserInput.compare(lowerCaseProduct)==0){
                correctInput = true;

                //show a sub menu to the user
                std::cout << "======  "<< upperCaseProduct <<" average bid-ask spread   ======" << std::endl;
                std::cout << "Please select the timeframe for which you want to see the average bid-ask spread: " << std::endl;
                std::cout << "                  • (1) Current timestep" << std::endl;
                std::cout << "                  • (2) Historical (from the beginning of the timeseries until the current timestep)" << std::endl;

                std::string userInput ="";
                std::cout << "user> ";

                //save user input in userInput variable
                std::getline(std::cin, userInput);
                bool correctInput = false;

                //if the user selected option 1
                if(userInput.compare("1")==0){
                    // the user wants the statistic for this timestep only
                    std::string timeframeForCalculation = "this-timestep-only";
                    
                    //obtain nominal and percentage average bid ask spread for this timestep
                    std::vector<double> resultProcessAverageBidAskSpread = orderBook.processAverageBidAskSpread(upperCaseProduct, timeframeForCalculation, orderBook, currentTime);

                    std::cout<< "The current spread in nominal terms for "<< upperCaseProduct <<" is: "<< resultProcessAverageBidAskSpread[0] << std::endl;
                    
                    std::cout<< "The current spread in percentage terms for "<< upperCaseProduct <<" is: "<< resultProcessAverageBidAskSpread[1] << "%" << std::endl;

                }
                //if the user selected option 2
                else if(userInput.compare("2")==0){
                    // the user wants the statistic for all of the dataseries
                    std::string timeframeForCalculation = "from-the-beginning-until-current-dataframe";
                    
                    //obtain nominal and percentage average bid ask spread for all of the dataseries
                    std::vector<double> resultProcessAverageBidAskSpread = orderBook.processAverageBidAskSpread(upperCaseProduct, timeframeForCalculation, orderBook, currentTime);

                    std::cout<< "The historical average spread in nominal terms for "<< upperCaseProduct <<" is: "<< resultProcessAverageBidAskSpread[0] << std::endl;
                    
                    std::cout<< "The historical average spread in percentage terms for "<< upperCaseProduct <<" is: "<< resultProcessAverageBidAskSpread[1] << "%" << std::endl;

                }else{
                    //show descriptive error message for when the user doesn't select 1 or 2
                    std::cout<< "ERROR: Unrecognizable timeframe specified: '" << userInput <<"'" << std::endl;                    
                }
                //finish loop over available products because it was already found
                break;
            }
        }
        if(!correctInput){
            //the product introduced by the user was not recognized as valid
            std::cout<< "ERROR: Unrecognizable product specified: '" << userInput <<"'" << std::endl;
        }

    }//if the input was a 2 the user wants the "Market depth & sentiment analysis" statistics
    else if(userInput.compare("2")==0){
        
        //show a sub menu to the user
        std::cout << ""<< std::endl;
        std::cout << "======   Market depth & sentiment analysis   ======" << std::endl;
        std::cout << "Please select write the product for which you want to see the market depth: " << std::endl;    
        
        //convert user input into lowercase to be able to compare it
        std::vector<std::string> productsAvailable = orderBook.getKnownProducts();
        
        //iterate over the vector of avaiable products
        for (std::string const& product : productsAvailable)
        {
            std::cout << "                  • " << product << std::endl;
        }
        
        std::string userInput ="";
        std::cout << "user> ";
        
        std::getline(std::cin, userInput);
        //save user input in userInput variable

        bool correctInput = false;        
        
        //convert product names strings to lowercase to be able to compare them
        std::string lowerCaseUserInput = stringToLowerCase(userInput);
        
        //if the user input coincides with a valid product name
        for (std::string& product : productsAvailable){          
            //save "original" uppercase product name to later pass it as an argument
            std::string upperCaseProduct = product;
            //convert product names strings to lowercase to be able to compare them
            std::string lowerCaseProduct = stringToLowerCase(product);
            
            //if the user input coincides with a valid product name
            if(lowerCaseUserInput.compare(lowerCaseProduct)==0){
                
                correctInput = true;
                //show a sub menu to the user
                std::cout << ""<< std::endl;                
                std::cout << "======  "<< upperCaseProduct <<" market depth & sentiment analysis   ======" << std::endl;
                
                //get vector of bid entries
                std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid,upperCaseProduct, currentTime);
                
                //get vector of ask entries
                std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask,upperCaseProduct, currentTime);

                //sum amount of currency being offered to be bought (bid)
                double bidOpenedOrdersAmount = orderBook.getMarketDepth(bidEntries);
                //sum amount of currency being offered to be sold (ask)
                double askOpenedOrdersAmount = orderBook.getMarketDepth(askEntries);

                std::cout << ""<< std::endl;
                std::cout << "Amount of "<< upperCaseProduct << " to buy in the bid orders: " << bidOpenedOrdersAmount <<std::endl;
                std::cout << "Amount of "<< upperCaseProduct << " for sale in the ask orders: " << askOpenedOrdersAmount <<std::endl;
                std::cout << ""<< std::endl;
                
                //show customized feedback based on the cummulative amount in the ask orders vs the cummulative amount in the bid orders
                if(askOpenedOrdersAmount>bidOpenedOrdersAmount){
                    std::cout << "There open sell orders are " << (askOpenedOrdersAmount/bidOpenedOrdersAmount) <<" times the amount of the open buy orders . The sentiment is bearish ↓ " <<std::endl;
                }else if(bidOpenedOrdersAmount>askOpenedOrdersAmount){
                    std::cout << "There open buy orders are " << (bidOpenedOrdersAmount/askOpenedOrdersAmount) <<" times the amount of the open sell orders . The sentiment is bullish ↑ " <<std::endl;
                }else{
                    std::cout << "The sentiment is neutral" << askOpenedOrdersAmount <<std::endl;
                }

                //finish loop over available products because it was already found
                break;
            }
        }
        if(!correctInput){
            //the product introduced by the user was not recognized as valid
            std::cout<< "ERROR: Unrecognizable product specified: '" << userInput <<"'" << std::endl;
        }
    }//the user has the option to write exit to go back to the main menu
    else if(userInput.compare("exit")==0){
        //do not show welcome screen when going back to main menu
        int forceDisableWelcomeScreenLogo = 1;
        printMainMenu(forceDisableWelcomeScreenLogo);
    }else{
        std::cout << "statistics> ERROR: Unrecognized option selected '" << userInput <<"' " << std::endl;
        std::cout << "statistics> Please enter a number between 1 and 3 or enter 'exit' to go back to the main menu" << std::endl;
    }
}
////////////////////////////////////

////////////////////////////////////
// implementation of the function stringToLowerCase() defined in the advisorBot.h that converts strings to its lowercase form
std::string advisorBot::stringToLowerCase(std::string& userInput){    
    std::for_each(userInput.begin(), userInput.end(), [](char & wordCharacter){
        wordCharacter = ::tolower(wordCharacter);
    });
    return userInput;
}
