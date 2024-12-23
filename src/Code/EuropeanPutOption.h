#ifndef EUROPEANPUTOPTION_H
#define EUROPEANPUTOPTION_H

#include "Option.h"
#include <algorithm>

class EuropeanPutOption : public Option {
public:
    // Constructor
    EuropeanPutOption(double strike, double maturity, double rate, double vol, double max_stock, int price_steps, int time_steps)
        : Option(strike, maturity, rate, vol, max_stock, price_steps, time_steps) {}

    // Initialize the payoff at maturity
    void initializePayoff(std::vector<double>& V, const std::vector<double>& S) const override {
        for (size_t i = 0; i < S.size(); ++i) {
            V[i] = std::max(K - S[i], 0.0);  // Put payoff: max(K - S, 0)
        }
    }

    // Apply boundary conditions
    void applyBoundaryConditions(std::vector<double>& V, int n, double dt) const override {
        V[0] = K * std::exp(-r * (T - n * dt)); // At S = 0, the option value approaches K * e^(-r(T-t))
        V[N] = 0.0;                             // At S = S_max, the option value is 0
    }
};

#endif