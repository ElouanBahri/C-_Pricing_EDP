#include <iostream>
#include "Code/EuropeanCallOption.h"

int main() {
    // Define parameters
    double strike = 100.0;      // Strike price
    double maturity = 1.0;      // Time to maturity (1 year)
    double rate = 0.05;         // Risk-free rate
    double vol = 0.2;           // Volatility
    double max_stock = 200.0;   // Maximum stock price
    int price_steps = 100;      // Stock price steps
    int time_steps = 100;       // Time steps
    double initial_stock_price = 100.0; // Initial stock price

    // Create a European call option
    EuropeanCallOption callOption(strike, maturity, rate, vol, max_stock, price_steps, time_steps);

    // Price the option
    double callPrice = callOption.price(initial_stock_price);
    std::cout << "European Call Option Price: " << callPrice << std::endl;

    return 0;
}