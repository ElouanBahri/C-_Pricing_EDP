#include "Option.h"
#include <cmath>
#include <algorithm>

double Option::price(double initial_stock_price) {
    double dS = S_max / N;  // Stock price step size
    double dt = T / M;      // Time step size

    // Stock price grid and option values
    std::vector<double> S(N + 1), V(N + 1), V_prev(N + 1);

    // Set up stock price grid
    for (int i = 0; i <= N; ++i) {
        S[i] = i * dS;
    }

    // Initialize payoff at maturity
    initializePayoff(V, S);

    // Backward induction in time
    for (int n = M; n > 0; --n) {
        V_prev = V;

        // Update interior points
        for (int i = 1; i < N; ++i) {
            double delta = (V_prev[i + 1] - V_prev[i - 1]) / (2 * dS);
            double gamma = (V_prev[i + 1] - 2 * V_prev[i] + V_prev[i - 1]) / (dS * dS);
            V[i] = V_prev[i] + dt * (0.5 * sigma * sigma * S[i] * S[i] * gamma + r * S[i] * delta - r * V_prev[i]);  //Black-Scholes-Merton PDE
        }

        // Apply boundary conditions
        applyBoundaryConditions(V, n, dt);
    }

    // Return the option price for the given initial stock price
    int index = static_cast<int>(initial_stock_price / dS);
    return V[index];
}