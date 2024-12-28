#include "Option.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

// Explicit Finite Difference Method
double Option::price(double initial_stock_price) {
    double dS = S_max / N;
    double dt = T / M;

    std::vector<double> S(N + 1), V(N + 1), V_prev(N + 1);

    for (int i = 0; i <= N; ++i) {
        S[i] = i * dS;
    }

    initializePayoff(V, S);

    vector<vector<double> > results(M + 1, vector<double>(N + 1));
    results[M] = V;

    // Backward induction using explicit method
    for (int n = M - 1; n >= 0; --n) {
        V_prev = V;

        for (int i = 1; i < N; ++i) {
            double delta = (V_prev[i + 1] - V_prev[i - 1]) / (2 * dS);
            double gamma = (V_prev[i + 1] - 2 * V_prev[i] + V_prev[i - 1]) / (dS * dS);
            V[i] = V_prev[i] + dt * (0.5 * sigma * sigma * S[i] * S[i] * gamma +
                                     r * S[i] * delta - r * V_prev[i]);
        }

        applyBoundaryConditions(V, n, dt);
        results[n] = V;
    }

    // Save results for plotting
    ofstream outFile("option_values.txt");
    for (int n = 0; n <= M; ++n) {
        for (int i = 0; i <= N; ++i) {
            outFile << n * dt << " " << S[i] << " " << results[n][i] << endl;
        }
        outFile << endl;
    }
    outFile.close();
    cout << "Explicit method results saved to 'option_values.txt'." << endl;

    int index = static_cast<int>(initial_stock_price / dS);
    return V[index];
}

// Crank-Nicolson Method
double Option::priceCrankNicolson(double initial_stock_price) {
    double dS = S_max / N;
    double dt = T / M;

    std::vector<double> S(N + 1), V(N + 1), V_prev(N + 1);
    std::vector<double> a(N + 1), b(N + 1), c(N + 1), d(N + 1);

    for (int i = 0; i <= N; ++i) {
        S[i] = i * dS;
    }

    initializePayoff(V, S);
    vector<vector<double> > results(M + 1, vector<double>(N + 1));
    results[M] = V;

    // Backward induction using Crank-Nicolson method
    for (int n = M - 1; n >= 0; --n) {
        V_prev = V;

        for (int i = 1; i < N; ++i) {
            double alpha = 0.25 * dt * (sigma * sigma * S[i] * S[i] / (dS * dS) - r * S[i] / dS);
            double beta = -0.5 * dt * (sigma * sigma * S[i] * S[i] / (dS * dS) + r);
            double gamma = 0.25 * dt * (sigma * sigma * S[i] * S[i] / (dS * dS) + r * S[i] / dS);

            a[i] = -alpha;
            b[i] = 1 - beta;
            c[i] = -gamma;
            d[i] = (alpha * V_prev[i - 1] + (1 + beta) * V_prev[i] + gamma * V_prev[i + 1]);
        }

        applyBoundaryConditions(V, n, dt);
        thomasAlgorithm(a, b, c, d, V, N);
        results[n] = V;
    }

    // Save results for plotting
    ofstream outFile("option_values_crank_nicolson.txt");
    for (int n = 0; n <= M; ++n) {
        for (int i = 0; i <= N; ++i) {
            outFile << n * dt << " " << S[i] << " " << results[n][i] << endl;
        }
        outFile << endl;
    }
    outFile.close();
    cout << "Crank-Nicolson results saved to 'option_values_crank_nicolson.txt'." << endl;

    int index = static_cast<int>(initial_stock_price / dS);
    return V[index];
}

// Thomas Algorithm for Solving Tridiagonal Matrix
void Option::thomasAlgorithm(vector<double>& a, vector<double>& b, vector<double>& c,
                             vector<double>& d, vector<double>& V, int N) {
    for (int i = 2; i < N; ++i) {
        double w = a[i] / b[i - 1];
        b[i] -= w * c[i - 1];
        d[i] -= w * d[i - 1];
    }

    
    for (int i = N - 1; i >= 1; --i) {
        V[i] = (d[i] - c[i] * V[i + 1]) / b[i];
    }
}