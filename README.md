# AdvisorBot

AdvisorBot is a trading themed CLI project in C++ using a custom-made CSV parser in C. It provides various functionalities related to bid and ask prices for different crypto-currency products over a time-series.

## Prerequisites

Before installing AdvisorBot, make sure you have the following prerequisites:

- C++ compiler (e.g., g++)

## Features

- Get the minimum and maximum bid and ask prices for a product in the current timestep.
- Calculate the average bid and ask prices for any product in the current timestep.
- Calculate the average bid and ask prices for any product from the beginning of the time-series.
- Predict the minimum and maximum bid and ask prices for a product.
- Advance to the next timeframe in the time-series.
- Learn how to use the application with the help command and the prod command, which shows the available cryptocurrencies.
- Get feedback on the current timeframe with the time command, which displays the current time.
- Advance to the following timeframe.

## Installation

To install AdvisorBot, follow these steps:

1. Clone the repository: `git clone https://github.com/ezema/AdvisorBot.git`
2. Compile the program: `g++ *.cpp *.h -Wall`
3. Run the program `./a.out`
