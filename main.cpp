#include <iostream>
#include "../include/InteractiveMenu.h"
#include "../include/Asset.h" // VS Code lo buscará en la carpeta 'include' gracias al comando que te daré

using namespace std;
using namespace std::chrono;

int main()
{
    // 1. Crear un Activo de prueba
    Asset apple("AAPL", Sector::Tecnologia);

    // 2. Agregarle precios históricos (Fechas C++20)
    apple.addPriceRecord(year(2023) / 1 / 1, 150.00);
    apple.addPriceRecord(year(2023) / 1 / 2, 155.00);
    apple.addPriceRecord(year(2023) / 1 / 3, 152.50);

    // 3. Imprimir usando tu sobrecarga de operador <<
    cout << "--- Prueba de Asset ---" << endl;
    cout << apple << endl;

    // 4. Probar cálculos
    cout << "Precio Promedio: $" << apple.getAveragePrice() << endl;
    cout << "Volatilidad: " << apple.getVolatility() << endl;

    return 0;
}