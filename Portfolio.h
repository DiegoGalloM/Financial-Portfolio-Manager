#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm> // Para buscar en vectores
#include "Asset.h"   

using namespace std;

// Struct para saber qué acciones se tienen
struct Holding {
    string ticker;
    int quantity;
};

class Portfolio {
private:
    double cashBalance;       // Dinero efectivo
    vector<Holding> holdings; // Acciones compradas

public:
    // Constructor: Inicia con $10,000 USD por defecto
    Portfolio(); 

    // Métodos de Negocio
    bool buyAsset(const string& ticker, double currentPrice, int quantity);
    bool sellAsset(const string& ticker, double currentPrice, int quantity);
    
    double getCashBalance() const;
    
    // Calcula cuánto vale todo lo que tienes sumado
    double getTotalValue(const vector<Asset>& marketAssets) const;

    void printSummary() const;
};