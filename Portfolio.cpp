#include "Portfolio.h"

// Inicio con 10,000
Portfolio::Portfolio() : cashBalance(10000.00) {}

bool Portfolio::buyAsset(const string& ticker, double currentPrice, int quantity) {
    if (quantity <= 0) return false;

    double totalCost = currentPrice * quantity;

    // Valida que haya dinero suficiente
    if (cashBalance >= totalCost) {
        cashBalance -= totalCost; // Descontar dinero

        // Valida si se tienen acciones de la empresa
        auto it = find_if(holdings.begin(), holdings.end(), 
            [&ticker](const Holding& h) { return h.ticker == ticker; });

        if (it != holdings.end()) {
            // Sí, sumar cantidad
            it->quantity += quantity;
        } else {
            // No, agregar nueva posesión
            holdings.push_back({ticker, quantity});
        }
        
        cout << ">>> Compra exitosa: " << quantity << " de " << ticker << endl;
        return true;
    }
    
    cout << ">>> Error: Fondos insuficientes (" << cashBalance << " vs " << totalCost << ")" << endl;
    return false;
}

bool Portfolio::sellAsset(const string& ticker, double currentPrice, int quantity) {
    if (quantity <= 0) return false;

    // Validar si se tiene la acción
    auto it = find_if(holdings.begin(), holdings.end(), 
        [&ticker](const Holding& h) { return h.ticker == ticker; });

    // 2. Validar si existe y si tengo suficientes
    if (it != holdings.end() && it->quantity >= quantity) {
        double earnings = currentPrice * quantity;
        cashBalance += earnings; // Recibir dinero
        
        it->quantity -= quantity; // Restar acciones

        // Si vendo todas, quitar de la lista
        if (it->quantity == 0) {
            holdings.erase(it);
        }

        cout << ">>> Venta exitosa: " << quantity << " de " << ticker << endl;
        return true;
    }

    cout << ">>> Error: No tienes suficientes acciones de " << ticker << endl;
    return false;
}

double Portfolio::getCashBalance() const {
    return cashBalance;
}

// Suma efectivo + (cantidad_acciones * precio_actual_mercado)
double Portfolio::getTotalValue(const vector<Asset>& marketAssets) const {
    double totalValue = cashBalance;

    for (const auto& holding : holdings) {
        // Buscar el precio actual en el mercado
        auto it = find_if(marketAssets.begin(), marketAssets.end(),
            [&holding](const Asset& a) { return a.ticker == holding.ticker; });

        if (it != marketAssets.end()) {
            totalValue += holding.quantity * it->getCurrentPrice();
        }
    }
    return totalValue;
}

void Portfolio::printSummary() const {
    cout << "\n=== ESTADO DEL PORTAFOLIO ===" << endl;
    cout << "Efectivo: $" << cashBalance << endl;
    cout << "Acciones:" << endl;
    if (holdings.empty()) {
        cout << "  (Ninguna)" << endl;
    } else {
        for (const auto& h : holdings) {
            cout << "  - " << h.ticker << ": " << h.quantity << " unds." << endl;
        }
    }
    cout << "=============================\n" << endl;
}