#ifndef EUROPEANOPTION_H
#define EUROPEANOPTION_H

#include "Option.h"
#include <algorithm>

class EuropeanOption : public Option {
public:
    // Constructor
    EuropeanOption(double strike, double maturity, double rate, double vol, double max_stock, int price_steps, int time_steps, bool isCall)
        : Option(strike, maturity, rate, vol, max_stock, price_steps, time_steps), isCall(isCall) {}

    // Initialize the payoff at maturity
    void initializePayoff(std::vector<double>& V, const std::vector<double>& S) const override {
        for (size_t i = 0; i < S.size(); ++i) {
            if (isCall) {
                V[i] = std::max(S[i] - K, 0.0);  // Call payoff: max(S - K, 0)
            } else {
                V[i] = std::max(K - S[i], 0.0);  // Put payoff: max(K - S, 0)
            }
        }
    }

    // Apply boundary conditions
    void applyBoundaryConditions(std::vector<double>& V, int n, double dt) const override {
        if (isCall) {
            V[0] = 0.0;  // At S = 0, the call option value is 0
            V[N] = std::max(S_max - K * std::exp(-r * (T - n * dt)), 0.0); // At S = S_max
        } else {
            V[0] = K * std::exp(-r * (T - n * dt)); // At S = 0, the put option value approaches K * e^(-r(T-t))
            V[N] = 0.0;                            // At S = S_max, the put option value is 0
        }
    }

private:
    bool isCall;  // true for call, false for put
};

#endif