#include "Asset.h"

// Agrega un registro al historial
void Asset::addPriceRecord(year_month_day date, double price) {
    history.push_back({date, price});
}

// Obtiene el último precio registrado
double Asset::getCurrentPrice() const {
    if (history.empty()) return 0.0;
    return history.back().closePrice;
}

// Calcula el promedio histórico
double Asset::getAveragePrice() const {
    if (history.empty()) return 0.0;
    
    double sum = 0.0;
    for (const auto& record : history) {
        sum += record.closePrice;
    }
    return sum / history.size();
}

// Cálculo de la Volatilidad (Desviación Estándar)
// Una volatilidad alta significa que la acción es riesgosa (sube y baja mucho)
double Asset::getVolatility() const {
    if (history.size() < 2) return 0.0; // Necesitamos datos para comparar

    double mean = getAveragePrice();
    double sq_sum = 0.0;

    // Sumatoria de (Precio - Promedio)^2
    for (const auto& record : history) {
        sq_sum += pow(record.closePrice - mean, 2);
    }

    // Varianza = Promedio de las diferencias al cuadrado
    double variance = sq_sum / history.size();

    // Desviación Estándar = Raíz cuadrada de la Varianza
    return sqrt(variance);
}