#ifndef BARRIEROPTION_H
#define BARRIEROPTION_H

#include "Option.h"
#include <algorithm>

class BarrierOption : public Option {
public:
    // Constructor
    BarrierOption(double strike, double maturity, double rate, double vol, double max_stock, int price_steps, int time_steps,
                  bool isCall, bool isKnockIn, double barrier)
        : Option(strike, maturity, rate, vol, max_stock, price_steps, time_steps), isCall(isCall), isKnockIn(isKnockIn), barrier(barrier) {}

    // Initialize the payoff at maturity
    void initializePayoff(std::vector<double>& V, const std::vector<double>& S) const override {
        for (size_t i = 0; i < S.size(); ++i) {
            if (isKnockIn) {
                // Knock-In: Payoff applies only if the barrier was hit
                V[i] = (barrierHit(S[i])) ? calculatePayoff(S[i]) : 0.0;
            } else {
                // Knock-Out: Payoff is zero if the barrier was hit
                V[i] = (barrierHit(S[i])) ? 0.0 : calculatePayoff(S[i]);
            }
        }
    }

    // Apply boundary conditions
    void applyBoundaryConditions(std::vector<double>& V, int n, double dt) const override {
        if (isCall) {
            V[0] = 0.0;  // At S = 0, the value of a call is 0
            V[N] = std::max(S_max - K * std::exp(-r * (T - n * dt)), 0.0); // At S_max
        } else {
            V[0] = K * std::exp(-r * (T - n * dt)); // At S = 0, the value of a put approaches K * e^(-r(T-t))
            V[N] = 0.0;                            // At S_max, the value of a put is 0
        }
    }

private:
    bool isCall;     // true for call, false for put
    bool isKnockIn;  // true for knock-in, false for knock-out
    double barrier;  // Barrier level

    // Check if the barrier condition is met
    bool barrierHit(double spot) const {
        if (isKnockIn) {
            return (isCall) ? (spot >= barrier) : (spot <= barrier);  // Up-and-In or Down-and-In
        } else {
            return (isCall) ? (spot >= barrier) : (spot <= barrier);  // Up-and-Out or Down-and-Out
        }
    }

    // Calculate the intrinsic payoff
    double calculatePayoff(double spot) const {
        if (isCall) {
            return std::max(spot - K, 0.0);  // Call payoff: max(S - K, 0)
        } else {
            return std::max(K - spot, 0.0);  // Put payoff: max(K - S, 0)
        }
    }
};

#endif