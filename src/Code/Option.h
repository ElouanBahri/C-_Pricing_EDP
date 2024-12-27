#ifndef OPTION_H
#define OPTION_H

#include <vector>

class Option {
protected:
    double K;       // Strike price
    double T;       // Time to maturity
    double r;       // Risk-free rate
    double sigma;   // Volatility
    double S_max;   // Maximum stock price
    int N;          // Number of stock price steps
    int M;          // Number of time steps

public:
    // Constructor
    Option(double strike, double maturity, double rate, double vol, double max_stock, int price_steps, int time_steps)
        : K(strike), T(maturity), r(rate), sigma(vol), S_max(max_stock), N(price_steps), M(time_steps) {}

    virtual ~Option() {}

    // Pure virtual functions for option-specific behavior
    virtual void initializePayoff(std::vector<double>& V, const std::vector<double>& S) const = 0;
    virtual void applyBoundaryConditions(std::vector<double>& V, int n, double dt) const = 0;

    // Pricing methods
    double price(double initial_stock_price);            // Explicit finite difference method
    double priceCrankNicolson(double initial_stock_price); // Crank-Nicolson method

private:
    // Helper function for solving tridiagonal matrix (used in Crank-Nicolson)
    void thomasAlgorithm(std::vector<double>& a, std::vector<double>& b, std::vector<double>& c,
                         std::vector<double>& d, std::vector<double>& V, int N);
};

#endif