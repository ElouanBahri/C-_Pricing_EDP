#include <iostream>
#include "Code/EuropeanCallOption.h"
#include "Code/EuropeanPutOption.h"
#include "Code/EuropeanOption.h"
#include "Code/DigitalOption.h"

#include <iostream>
#include <limits>  // For numeric_limits

double getInput(const std::string& prompt, double default_value) {
    double value;
    std::cout << prompt << " (Default: " << default_value << "): ";
    std::cin >> value;
    if (std::cin.fail()) { // Check if the input is not a number
        std::cin.clear(); // Clear the error flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the invalid input
        return default_value; // Return default value if input is invalid
    }
    return value;
}

int main() {
    // Define default values
    double default_strike = 100.0;
    double default_maturity = 1.0;
    double default_rate = 0.05;
    double default_vol = 0.2;
    double default_max_stock = 200.0;
    int default_price_steps = 10;
    int default_time_steps = 10;
    double default_initial_stock_price = 100.0;

    // Get user input with default values
    double strike = getInput("Enter strike price", default_strike);
    double maturity = getInput("Enter time to maturity (in years)", default_maturity);
    double rate = getInput("Enter risk-free rate", default_rate);
    double vol = getInput("Enter volatility", default_vol);
    double max_stock = getInput("Enter maximum stock price", default_max_stock);
    int price_steps = static_cast<int>(getInput("Enter number of price steps", default_price_steps));
    int time_steps = static_cast<int>(getInput("Enter number of time steps", default_time_steps));
    double initial_stock_price = getInput("Enter initial stock price", default_initial_stock_price);

    // Create a European call option
    EuropeanOption callOption(strike, maturity, rate, vol, max_stock, price_steps, time_steps, true);

    // Create a European put option
    EuropeanOption putOption(strike, maturity, rate, vol, max_stock, price_steps, time_steps, false);

    // Price the call option
    double callPrice = callOption.price(initial_stock_price);
    std::cout << "European Call Option Price 2: " << callPrice << std::endl;

    // Price the put option
    double putPrice2 = putOption.price(initial_stock_price);
    std::cout << "European Put Option Price 2: " << putPrice2 << std::endl;

    return 0;
}