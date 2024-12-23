#ifndef EUROPEANCALLOPTION_H
#define EUROPEANCALLOPTION_H

#include "Option.h"
#include <algorithm>

class EuropeanCallOption : public Option {
public:
    // Constructor
    EuropeanCallOption(double strike, double maturity, double rate, double vol, double max_stock, int price_steps, int time_steps)
        : Option(strike, maturity, rate, vol, max_stock, price_steps, time_steps) {}

    // Initialize the payoff at maturity
    void initializePayoff(std::vector<double>& V, const std::vector<double>& S) const override {
        for (size_t i = 0; i < S.size(); ++i) {
            V[i] = std::max(S[i] - K, 0.0);  // Call payoff: max(S - K, 0)
        }
    }

    // Apply boundary conditions
    void applyBoundaryConditions(std::vector<double>& V, int n, double dt) const override {
        V[0] = 0.0;                              // At S = 0, the option value is 0
        V[N] = S_max - K * std::exp(-r * (T - n * dt)); // At S = S_max
    }
};

#endif