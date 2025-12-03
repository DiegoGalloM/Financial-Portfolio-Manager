#include "Analisis.h"
#include <iostream>
#include <filesystem>
#include <vector>

class InteractiveMenu
{
private:
    CSVAnalyzer analyzer;
    bool dataLoaded = false;
    string currentFile;

public:
    void displayWelcome()
    {
        cout << "\n"
             << endl;
        cout << "|              ADAPTIVE CSV ANALYSIS SYSTEM                   |" << endl;
        cout << "|          Intelligent Data Analysis & Statistics             |" << endl;
        cout << "" << endl;
        cout << "\nThis system automatically adapts to any CSV structure!" << endl;
        cout << "Supports: Integers, Floats, Strings, Dates, and Categories\n"
             << endl;
    }

    

    void loadCSVFile()
    {
        cout << "\n+-------------------------------------+" << endl;
        cout << "|           LOAD CSV FILE             |" << endl;
        cout << "+-------------------------------------+" << endl;

        cout << "\nEnter CSV filename (or full path): ";
        string filename;
        getline(cin, filename);

        // Remove quotes if present
        if (filename.front() == '"' && filename.back() == '"')
        {
            filename = filename.substr(1, filename.length() - 2);
        }

        cout << "\nAttempting to load: " << filename << endl;

        if (analyzer.loadCSV(filename))
        {
            dataLoaded = true;
            currentFile = filename;
            cout << "\n File loaded successfully!" << endl;

            // Show quick preview
            cout << "\n Quick Preview:" << endl;
            analyzer.printBasicStatistics();
        }
        else
        {
            cout << "\n Failed to load file. Please check the filename and try again." << endl;

            // Show available CSV files in current directory
            cout << "\n Available CSV files in current directory:" << endl;
            try
            {
                bool foundCSV = false;
                for (const auto &entry : std::filesystem::directory_iterator("."))
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".csv")
                    {
                        cout << "   • " << entry.path().filename().string() << endl;
                        foundCSV = true;
                    }
                }
                if (!foundCSV)
                {
                    cout << "   No CSV files found in current directory." << endl;
                }
            }
            catch (...)
            {
                cout << "   Could not scan directory." << endl;
            }
        }
        pauseForUser();
    }

    void viewDataInformation()
    {
        if (!checkDataLoaded())
            return;

        cout << "\n+-------------------------------------+" << endl;
        cout << "|         DATA INFORMATION            |" << endl;
        cout << "+-------------------------------------+" << endl;

        cout << "1. Basic Statistics" << endl;
        cout << "2. Column Details" << endl;
        cout << "3. Data Preview (first rows)" << endl;
        cout << "4. Data Preview (custom range)" << endl;
        cout << "0. Back to main menu" << endl;
        cout << "\nSelect option: ";

        int choice = getIntInput();
        cout << endl;

        switch (choice)
        {
        case 1:
            analyzer.printBasicStatistics();
            break;
        case 2:
            showColumnDetails();
            break;
        case 3:
            cout << "How many rows to display? (default 5): ";
            {
                int rows = getIntInput();
                if (rows <= 0)
                    rows = 5;
                analyzer.printHead(rows);
            }
            break;
        case 4:
            analyzer.printHead(5);
            break;
        case 0:
            return;
        default:
            cout << "Invalid option!" << endl;
        }
        pauseForUser();
    }

    void statisticalAnalysis()
    {
        if (!checkDataLoaded())
            return;

        cout << "\n+-------------------------------------+" << endl;
        cout << "|        STATISTICAL ANALYSIS        |" << endl;
        cout << "+-------------------------------------+" << endl;

        cout << "1. Numeric Statistics (all numeric columns)" << endl;
        cout << "2. Categorical Analysis (all categorical columns)" << endl;
        cout << "3. Specific Column Analysis" << endl;
        cout << "4. Complete Statistical Report" << endl;
        cout << "0. Back to main menu" << endl;
        cout << "\nSelect option: ";

        int choice = getIntInput();
        cout << endl;

        switch (choice)
        {
        case 1:
            analyzer.printNumericStatistics();
            break;
        case 2:
            analyzer.printCategoricalStatistics();
            break;
        case 3:
            analyzeSpecificColumn();
            break;
        case 4:
            cout << "COMPLETE STATISTICAL REPORT" << endl;
            cout << string(50, '=') << endl;
            analyzer.printBasicStatistics();
            analyzer.printNumericStatistics();
            analyzer.printCategoricalStatistics();
            break;
        case 0:
            return;
        default:
            cout << "Invalid option!" << endl;
        }
        pauseForUser();
    }

    void dataOperations()
    {
        if (!checkDataLoaded())
            return;

        cout << "\n+-------------------------------------+" << endl;
        cout << "|          DATA OPERATIONS            |" << endl;
        cout << "+-------------------------------------+" << endl;

        cout << "1. Search for specific values" << endl;
        cout << "2. Filter data" << endl;
        cout << "0. Back to main menu" << endl;
        cout << "\nSelect option: ";

        int choice = getIntInput();
        cout << endl;

        switch (choice)
        {
        case 1:
            searchData();
            break;
        case 2:
            filterData();
            break;
        case 0:
            return;
        default:
            cout << "Invalid option!" << endl;
        }
        pauseForUser();
    }

    void exportDisplayOptions()
    {
        if (!checkDataLoaded())
            return;

        cout << "\n+-------------------------------------+" << endl;
        cout << "|      EXPORT & DISPLAY OPTIONS       |" << endl;
        cout << "+-------------------------------------+" << endl;

        cout << "1. Display all data" << endl;
        cout << "2. Display first N rows" << endl;
        cout << "3. Display specific columns" << endl;
        cout << "4. Summary report" << endl;
        cout << "0. Back to main menu" << endl;
        cout << "\nSelect option: ";

        int choice = getIntInput();
        cout << endl;

        switch (choice)
        {
        case 1:
            analyzer.printHead(analyzer.getRowCount());
            break;
        case 2:
        {
            cout << "How many rows to display?: ";
            int rows = getIntInput();
            if (rows > 0)
                analyzer.printHead(rows);
        }
        break;
        case 3:
            displaySpecificColumns();
            break;
        case 4:
            generateSummaryReport();
            break;
        case 0:
            return;
        default:
            cout << "Invalid option!" << endl;
        }
        pauseForUser();
    }

    void run()
    {
        displayWelcome();

        int choice;
        do
        {
            displayMainMenu();
            choice = getIntInput();

            switch (choice)
            {
            case 1:
                loadCSVFile();
                break;
            case 2:
                viewDataInformation();
                break;
            case 3:
                statisticalAnalysis();
                break;
            case 4:
                dataOperations();
                break;
            case 5:
                exportDisplayOptions();
                break;
            case 6:
                handleHistogramOption();
                break;
            case 7:
                handleExportOption();
                break;
            case 0:
                cout << "\n Thank you for using the Adaptive CSV Analysis System!" << endl;
                cout << " Your data insights await your next visit!" << endl;
                break;
            default:
                cout << "\nInvalid option! Please try again." << endl;
                pauseForUser();
            }
        } while (choice != 0);
    }

private:
    bool checkDataLoaded()
    {
        if (!dataLoaded)
        {
            cout << "\n No data loaded! Please load a CSV file first." << endl;
            pauseForUser();
            return false;
        }
        return true;
    }

    int getIntInput()
    {
        string input;
        getline(cin, input);

        try
        {
            return stoi(input);
        }
        catch (...)
        {
            return -1;
        }
    }

    void pauseForUser()
    {
        cout << "\nPress Enter to continue...";
        cin.get();
    }

    void showColumnDetails()
    {
        cout << "COLUMN DETAILS" << endl;
        cout << string(60, '=') << endl;

        auto columnNames = analyzer.getColumnNames();
        auto columnTypes = analyzer.getColumnTypes();

        cout << setw(5) << "No." << setw(20) << "Column Name"
             << setw(15) << "Data Type" << setw(20) << "Description" << endl;
        cout << string(60, '-') << endl;

        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            string description;
            switch (columnTypes[i])
            {
            case DataType::INTEGER:
                description = "Whole numbers";
                break;
            case DataType::FLOAT:
                description = "Decimal numbers";
                break;
            case DataType::STRING:
                description = "Text data";
                break;
            case DataType::DATE:
                description = "Date values";
                break;
            case DataType::CATEGORY:
                description = "Categorical data";
                break;
            }

            cout << setw(5) << (i + 1)
                 << setw(20) << columnNames[i]
                 << setw(15) << dataTypeToString(columnTypes[i])
                 << setw(20) << description << endl;
        }
    }

    void analyzeSpecificColumn()
    {
        // 1. Safety Check
        if (!dataLoaded) {
            cout << "Please load a file first." << endl;
            return;
        }

        cout << "\nAvailable columns:" << endl;
        auto columnNames = analyzer.getColumnNames();
        auto columnTypes = analyzer.getColumnTypes();

        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            cout << (i + 1) << ". " << columnNames[i]
                 << " (" << dataTypeToString(columnTypes[i]) << ")" << endl;
        }

        cout << "\nEnter column name to analyze: ";
        
        // FIX #1: Clean the buffer to prevent skipping input
        string columnName;
        cin >> ws; 
        getline(cin, columnName);

        // Check if column exists
        auto it = find(columnNames.begin(), columnNames.end(), columnName);
        if (it == columnNames.end())
        {
            cout << "Error: Column '" << columnName << "' not found!" << endl;
            return;
        }

        size_t index = distance(columnNames.begin(), it);
        DataType type = columnTypes[index]; // Get type directly

        cout << "\nAnalysis for column: " << columnName << endl;
        cout << string(40, '=') << endl;

        // FIX #2: Removed "row.isNumericColumn" check.
        // We trust 'type' because 'columnTypes' is the source of truth.
        if (type == DataType::INTEGER || type == DataType::FLOAT)
        {
            // --- NUMERIC ANALYSIS ---
            vector<double> values;
            const auto& data = analyzer.getData();

            for (const auto &row : data)
            {
                // Directly get the value. Dato.h handles the variant access.
                values.push_back(row.getNumericValue(index));
            }

            if (!values.empty())
            {
                sort(values.begin(), values.end());
                double sum = accumulate(values.begin(), values.end(), 0.0);
                double mean = sum / values.size();

                cout << " Numeric Analysis:" << endl;
                cout << "   Count: " << values.size() << endl;
                cout << "   Mean: " << fixed << setprecision(2) << mean << endl;
                cout << "   Min: " << values.front() << endl;
                cout << "   Max: " << values.back() << endl;

                if (values.size() > 1)
                {
                    double median;
                    if (values.size() % 2 == 0)
                        median = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2.0;
                    else
                        median = values[values.size() / 2];
                    cout << "   Median: " << median << endl;
                }
            }
        }
        else
        {
            // --- CATEGORICAL / STRING / DATE ANALYSIS ---
            map<string, int> frequency;
            const auto& data = analyzer.getData();

            for (const auto &row : data)
            {
                string value = row.getValueAsString(index);
                if (!value.empty()) {
                    frequency[value]++;
                }
            }

            cout << " Categorical Analysis:" << endl;
            cout << "   Unique values: " << frequency.size() << endl;
            cout << "   Most common values:" << endl;

            vector<pair<string, int>> sortedFreq(frequency.begin(), frequency.end());
            sort(sortedFreq.begin(), sortedFreq.end(),
                 [](const auto &a, const auto &b)
                 { return a.second > b.second; });

            int showCount = min(5, (int)sortedFreq.size());
            for (int i = 0; i < showCount; ++i)
            {
                double percentage = (double)sortedFreq[i].second / analyzer.getRowCount() * 100.0;
                cout << "      " << sortedFreq[i].first << ": "
                     << sortedFreq[i].second << " (" << fixed << setprecision(1)
                     << percentage << "%)" << endl;
            }
        }
        
        cout << "\nPress Enter to continue...";
        cin.get(); 
    }

    void searchData()
    {
        cout << " SEARCH DATA" << endl;
        cout << "Available columns: ";
        auto columnNames = analyzer.getColumnNames();
        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            cout << columnNames[i];
            if (i < columnNames.size() - 1)
                cout << ", ";
        }
        cout << endl;

        cout << "\nEnter column name to search in: ";
        string columnName;
        getline(cin, columnName);

        cout << "Enter value to search for: ";
        string value;
        getline(cin, value);

        auto results = analyzer.searchValue(columnName, value);

        cout << "\nSearch Results:" << endl;
        cout << "Found " << results.size() << " matches for '" << value
             << "' in column '" << columnName << "'" << endl;

        if (results.size() > 0 && results.size() <= 10)
        {
            cout << "\nMatching rows:" << endl;
            for (size_t idx : results)
            {
                cout << "Row " << (idx + 1) << ": ";
                const auto &row = analyzer.getData()[idx];
                for (size_t i = 0; i < columnNames.size(); ++i)
                {
                    cout << columnNames[i] << "=" << row.getValueAsString(i);
                    if (i < columnNames.size() - 1)
                        cout << ", ";
                }
                cout << endl;
            }
        }
        else if (results.size() > 1000)
        {
            cout << "Too many results to display. Showing first 10:" << endl;
            for (int i = 0; i < 10; ++i)
            {
                size_t idx = results[i];
                cout << "Row " << (idx + 1) << ": ";
                const auto &row = analyzer.getData()[idx];
                for (size_t j = 0; j < min((size_t)3, columnNames.size()); ++j)
                {
                    cout << columnNames[j] << "=" << row.getValueAsString(j);
                    if (j < min((size_t)3, columnNames.size()) - 1)
                        cout << ", ";
                }
                cout << "..." << endl;
            }
        }
    }

    void filterData()
    {
        cout << " FILTER DATA" << endl;
        cout << "Available columns: ";
        auto columnNames = analyzer.getColumnNames();
        auto columnTypes = analyzer.getColumnTypes();

        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            cout << columnNames[i] << " (" << dataTypeToString(columnTypes[i]) << ")";
            if (i < columnNames.size() - 1)
                cout << ", ";
        }
        cout << endl;

        cout << "\nEnter column name to filter by: ";
        string columnName;
        getline(cin, columnName);

        cout << "Enter operator (=, !=, >, <, >=, <=, contains): ";
        string operation;
        getline(cin, operation);

        cout << "Enter value to compare: ";
        string value;
        getline(cin, value);

        auto filtered = analyzer.filterData(columnName, operation, value);

        cout << "\n Filter Results:" << endl;
        cout << "Found " << filtered.size() << " rows matching criteria: "
             << columnName << " " << operation << " " << value << endl;

        if (filtered.size() > 0 && filtered.size() <= 10)
        {
            cout << "\nFiltered data:" << endl;
            // Print headers
            for (const auto &name : columnNames)
            {
                cout << setw(15) << name;
            }
            cout << endl;
            cout << string(15 * columnNames.size(), '-') << endl;

            // Print filtered rows
            for (const auto &row : filtered)
            {
                for (size_t i = 0; i < columnNames.size(); ++i)
                {
                    cout << setw(15) << row.getValueAsString(i);
                }
                cout << endl;
            }
        }
        else if (filtered.size() > 10)
        {
            cout << "Too many results to display. Use 'Export & Display Options' to see more." << endl;
        }
    }

    // Función para filtrar datos soportando todos los operadore

    void displaySpecificColumns()
    {
        cout << "SELECT COLUMNS TO DISPLAY" << endl;
        auto columnNames = analyzer.getColumnNames();

        cout << "Available columns:" << endl;
        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            cout << (i + 1) << ". " << columnNames[i] << endl;
        }

        cout << "\nEnter column numbers to display (comma-separated, e.g., 1,3,5): ";
        string input;
        getline(cin, input);

        vector<size_t> selectedColumns;
        stringstream ss(input);
        string token;

        while (getline(ss, token, ','))
        {
            try
            {
                int colNum = stoi(token);
                if (colNum > 0 && colNum <= (int)columnNames.size())
                {
                    selectedColumns.push_back(colNum - 1);
                }
            }
            catch (...)
            {
                // Invalid number, skip
            }
        }

        if (selectedColumns.empty())
        {
            cout << "No valid columns selected!" << endl;
            return;
        }

        cout << "\nSelected Columns Display:" << endl;

        // Print selected headers
        for (size_t col : selectedColumns)
        {
            cout << setw(15) << columnNames[col];
        }
        cout << endl;

        cout << string(15 * selectedColumns.size(), '-') << endl;

        // Print data for selected columns
        const auto &data = analyzer.getData();
        for (const auto &row : data)
        {
            for (size_t col : selectedColumns)
            {
                cout << setw(15) << row.getValueAsString(col);
            }
            cout << endl;
        }
    }

    void generateSummaryReport()
    {
        cout << "\nCOMPREHENSIVE SUMMARY REPORT" << endl;
        cout << string(60, '=') << endl;
        cout << "Generated on: " << __DATE__ << " " << __TIME__ << endl;
        cout << "File: " << currentFile << endl;
        cout << string(60, '=') << endl;

        analyzer.printBasicStatistics();
        analyzer.printNumericStatistics();
        analyzer.printCategoricalStatistics();

        cout << "\n"
             << string(60, '=') << endl;
        cout << "Report completed successfully!" << endl;
    }

    // =========================================================
    // NUEVAS FUNCIONES DEL MENÚ (Parte 2)
    // =========================================================

    // Modifica esta función existente para agregar las nuevas opciones
    void displayMainMenu()
    {
        cout << "\n+-------------------------------------+" << endl;
        cout << "|             MAIN MENU               |" << endl;
        cout << "+-------------------------------------+" << endl;
        cout << "| 1. Load CSV File                    |" << endl;
        cout << "| 2. View Data Information            |" << endl;
        cout << "| 3. Statistical Analysis             |" << endl;
        cout << "| 4. Data Operations                  |" << endl;
        cout << "| 5. Export & Display Options         |" << endl;
        // --- NUEVAS OPCIONES ---
        cout << "| 6. Visualizar Histograma (ASCII)    |" << endl;
        cout << "| 7. Exportar Reporte TXT             |" << endl;
        // -----------------------
        cout << "| 0. Exit                             |" << endl;
        cout << "+-------------------------------------+" << endl;

        if (dataLoaded)
        {
            cout << " Current file: " << currentFile << endl;
        }
        else
        {
            cout << " No file loaded." << endl;
        }
        cout << "\nSelect an option: ";
    }

    // Manejador para la Opción 6
    void handleHistogramOption()
    {
        if (!dataLoaded) {
            cout << "Please load a CSV file first (Option 1)." << endl;
            return;
        }

        cout << "\n--- GENERAR HISTOGRAMA ---" << endl;
        cout << "Columnas disponibles: " << endl;
        
        // Mostrar solo columnas numéricas como sugerencia
        const auto& cols = analyzer.getColumnNames();
        const auto& types = analyzer.getColumnTypes();
        
        for(size_t i=0; i<cols.size(); ++i) {
            if (types[i] == DataType::INTEGER || types[i] == DataType::FLOAT) {
                cout << " - " << cols[i] << endl;
            }
        }

        cout << "\nIngrese el nombre exacto de la columna a graficar: ";
        string colName;
        // Limpieza de buffer por si acaso
        cin >> ws; 
        getline(cin, colName);

        // Llamamos a la función que creamos en Analisis.h
        // Puedes ajustar los 'bins' (barras) aquí, por defecto 10
        analyzer.plotHistogram(colName, 15); 
        
        cout << "\nPresione Enter para continuar...";
        cin.get();
    }

    // Manejador para la Opción 7
    void handleExportOption()
    {
        if (!dataLoaded) {
            cout << "Please load a CSV file first (Option 1)." << endl;
            return;
        }

        cout << "\n--- EXPORTAR REPORTE ---" << endl;
        cout << "Ingrese el nombre del archivo de salida (ej. reporte.txt): ";
        string filename;
        cin >> ws;
        getline(cin, filename);

        // Validación simple de extensión
        if (filename.find(".txt") == string::npos) {
            filename += ".txt";
        }

        analyzer.exportReportTXT(filename);
        
        cout << "\nPresione Enter para continuar...";
        cin.get();
    }
};

/*
int main()
{
    try
    {
        InteractiveMenu menu;
        menu.run();
    }
    catch (const exception &e)
    {
        cerr << "\n Fatal Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
*/
