#ifndef DIGITALOPTION_H
#define DIGITALOPTION_H

#include "Option.h"
#include <algorithm>



//A digital option is a type of financial derivative that pays a fixed amount (called the payout) 
//if a certain condition is met at expiration. 
//Otherwise, it pays nothing. It’s often called a binary option because the payoff is “all or nothing.”


class DigitalOption : public Option {
public:
    // Constructor
    DigitalOption(double strike, double maturity, double rate, double vol, double max_stock, int price_steps, int time_steps, bool isCall, double payout = 1.0)
        : Option(strike, maturity, rate, vol, max_stock, price_steps, time_steps), isCall(isCall), payout(payout) {}

    // Initialize the payoff at maturity
    void initializePayoff(std::vector<double>& V, const std::vector<double>& S) const override {
        for (size_t i = 0; i < S.size(); ++i) {
            if (isCall) {
                V[i] = (S[i] > K) ? payout : 0.0;  // Digital call: fixed payout if S > K
            } else {
                V[i] = (S[i] < K) ? payout : 0.0;  // Digital put: fixed payout if S < K
            }
        }
    }

    // Apply boundary conditions
    void applyBoundaryConditions(std::vector<double>& V, int n, double dt) const override {
        if (isCall) {
            V[0] = 0.0;  // At S = 0, the option value is 0 for a digital call
            V[N] = payout * std::exp(-r * (T - n * dt));  // At S = S_max, discounted payout
        } else {
            V[0] = payout * std::exp(-r * (T - n * dt));  // At S = 0, discounted payout for digital put
            V[N] = 0.0;  // At S = S_max, the option value is 0
        }
    }

private:
    bool isCall;    // true for call, false for put
    double payout;  // Fixed payout of the digital option
};

#endif