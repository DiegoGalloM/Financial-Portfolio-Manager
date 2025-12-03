#pragma once

// LIBRERIAS ESENCIALES
#include <iostream>
#include <string>
#include <vector>
#include <chrono>     // Para fechas C++20
#include <cmath>      // Para sqrt y pow 
#include <numeric>    // Para accumulate 
#include <utility>    // Para std::move

using namespace std;
using namespace std::chrono;

// Enum del sector de bolsa
enum class Sector {
    Tecnologia,
    Salud,
    Automotriz,
    Consumo,
    Energia,
    Desconocido
};

// Conversiones inline (para no crear un cpp extra solo para esto)
inline string sectorToString(const Sector& sec) {
    switch(sec) {
        case Sector::Tecnologia: return "Tecnologia";
        case Sector::Salud: return "Salud";
        case Sector::Automotriz: return "Automotriz";
        case Sector::Consumo: return "Consumo";
        case Sector::Energia: return "Energia";
        default: return "Desconocido";
    }
}

inline Sector stringToSector(const string& str) {
    if (str == "Tecnologia") return Sector::Tecnologia;
    if (str == "Salud") return Sector::Salud;
    if (str == "Automotriz") return Sector::Automotriz;
    if (str == "Consumo") return Sector::Consumo;
    if (str == "Energia") return Sector::Energia;
    return Sector::Desconocido;
}

//  Struct de los precios, con fecha y valor
struct PriceRecord {
    year_month_day date;
    double closePrice;
};

//  Clase de los Assets
class Asset {
public:
    string ticker;
    Sector sector;
    vector<PriceRecord> history;

    // Constructores
    Asset() = default;
    Asset(string t, Sector s) : ticker(std::move(t)), sector(s) {}

    // Métodos (Se implementan en el cpp)
    void addPriceRecord(year_month_day date, double price);
    double getCurrentPrice() const;
    double getAveragePrice() const;
    
    // Cálculo de Riesgo (Desviación Estándar)
    double getVolatility() const; 

    // Sobrecarga <<
    friend ostream& operator<<(ostream& os, const Asset& asset) {
        os << "[" << asset.ticker << "] " << sectorToString(asset.sector);
        os << " | Datos: " << asset.history.size();
        if (!asset.history.empty()) {
            os << " | Precio Actual: $" << asset.history.back().closePrice;
        }
        return os;
    }
};