#ifndef ANALISIS_H
#define ANALISIS_H

#include "Dato.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <map>
#include <cmath>
#include <set>
#include <regex>

using namespace std;

class CSVAnalyzer
{
private:
    vector<Dato> data;
    vector<string> columnNames;
    vector<DataType> columnTypes;
    string filename;
    char delimiter;

public:
    CSVAnalyzer(char delim = ',') : delimiter(delim) {}

    // Method to load CSV file and automatically detect data types
    bool loadCSV(const string &filepath)
    {
        filename = filepath;
        ifstream file(filepath);

        if (!file.is_open())
        {
            std::cerr << "Error: Cannot open file " << filepath << std::endl;
            return false;
        }

        data.clear();
        columnNames.clear();
        columnTypes.clear();

        string line;
        bool isFirstLine = true;
        vector<vector<string>> rawData;

        // Read all data first
        while (std::getline(file, line))
        {
            vector<string> row = parseLine(line);

            if (isFirstLine)
            {
                columnNames = row;
                isFirstLine = false;
            }
            else
            {
                if (row.size() == columnNames.size())
                {
                    rawData.push_back(row);
                }
            }
        }
        file.close();

        if (rawData.empty())
        {
            std::cerr << "Error: No data rows found in CSV" << std::endl;
            return false;
        }

        detectDataTypes(rawData);

        convertRawDataToDato(rawData);

        std::cout << "Successfully loaded " << data.size() << " rows with "
                  << columnNames.size() << " columns from " << filepath << std::endl;

        return true;
    }

    void printBasicStatistics()
    {
        if (data.empty())
        {
            std::cout << "No data loaded." << std::endl;
            return;
        }

        std::cout << "\n===== BASIC STATISTICS =====" << std::endl;
        std::cout << "Dataset: " << filename << std::endl;
        std::cout << "Rows: " << data.size() << std::endl;
        std::cout << "Columns: " << columnNames.size() << std::endl;
        std::cout << std::endl;

        // Print column information
        std::cout << "Column Information:" << std::endl;
        std::cout << std::setw(20) << "Column Name"
                  << std::setw(15) << "Data Type"
                  << std::setw(15) << "Non-null Count" << std::endl;
        std::cout << std::string(50, '-') << std::endl;

        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            int nonNullCount = getNonNullCount(i);
            std::cout << std::setw(20) << columnNames[i]
                      << std::setw(15) << dataTypeToString(columnTypes[i])
                      << std::setw(15) << nonNullCount << std::endl;
        }
        std::cout << std::endl;
    }

    // Method to get detailed statistics for numeric columns
    void printNumericStatistics()
    {
        if (data.empty())
        {
            std::cout << "No data loaded." << std::endl;
            return;
        }

        std::cout << "\n===== NUMERIC STATISTICS =====" << std::endl;

        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            if (columnTypes[i] == DataType::INTEGER || columnTypes[i] == DataType::FLOAT)
            {
                printColumnStatistics(i);
                std::cout << std::endl;
            }
        }
    }

    void printCategoricalStatistics()
    {
        if (data.empty())
        {
            std::cout << "No data loaded." << std::endl;
            return;
        }

        std::cout << "\n===== CATEGORICAL STATISTICS =====" << std::endl;

        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            if (columnTypes[i] == DataType::STRING || columnTypes[i] == DataType::CATEGORY)
            {
                printFrequencyAnalysis(i);
                std::cout << std::endl;
            }
        }
    }

    // Method to print first few rows of data
    void printHead(int numRows = 5)
    {
        if (data.empty())
        {
            std::cout << "No data loaded." << std::endl;
            return;
        }

        std::cout << "\n===== FIRST " << std::min(numRows, (int)data.size()) << " ROWS =====" << std::endl;

        // Print headers
        for (const auto &name : columnNames)
        {
            std::cout << std::setw(15) << name;
        }
        std::cout << std::endl;

        // Print separator
        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            std::cout << std::setw(15) << std::string(14, '-');
        }
        std::cout << std::endl;

        // Print data rows
        for (int i = 0; i < std::min(numRows, (int)data.size()); ++i)
        {
            data[i].display();
        }
    }

    // Method to search for specific values
    std::vector<size_t> searchValue(const string &columnName, const string &value)
    {
        vector<size_t> results;

        auto it = find(columnNames.begin(), columnNames.end(), columnName);
        if (it == columnNames.end())
        {
            std::cout << "Column '" << columnName << "' not found." << std::endl;
            return results;
        }

        size_t columnIndex = std::distance(columnNames.begin(), it);

        for (size_t i = 0; i < data.size(); ++i)
        {
            std::string cellValue = data[i].getValueAsString(columnIndex);
            if (cellValue == value)
            {
                results.push_back(i);
            }
        }

        return results;
    }

    // Method to filter data based on criteria
    vector<Dato> filterData(const string &columnName,
                            const string &operation,
                            const string &value)
    {
        vector<Dato> filtered;

        auto it = find(columnNames.begin(), columnNames.end(), columnName);
        if (it == columnNames.end())
        {
            std::cout << "Column '" << columnName << "' not found." << std::endl;
            return filtered;
        }

        size_t columnIndex = distance(columnNames.begin(), it);

        for (const auto &row : data)
        {
            bool matches = false;

            if (columnTypes[columnIndex] == DataType::INTEGER ||
                columnTypes[columnIndex] == DataType::FLOAT)
            {
                double cellValue = row.getNumericValue(columnIndex);
                double compareValue = stod(value);

                if (operation == "==" || operation == "=")
                {
                    matches = (std::abs(cellValue - compareValue) < 1e-9);
                }
                else if (operation == ">")
                {
                    matches = (cellValue > compareValue);
                }
                else if (operation == "<")
                {
                    matches = (cellValue < compareValue);
                }
                else if (operation == ">=")
                {
                    matches = (cellValue >= compareValue);
                }
                else if (operation == "<=")
                {
                    matches = (cellValue <= compareValue);
                }
                else if (operation == "!=")
                {
                    matches = (std::abs(cellValue - compareValue) >= 1e-9);
                }
            }
            else
            {
                std::string cellValue = row.getValueAsString(columnIndex);

                if (operation == "==" || operation == "=")
                {
                    matches = (cellValue == value);
                }
                else if (operation == "!=")
                {
                    matches = (cellValue != value);
                }
                else if (operation == "contains")
                {
                    matches = (cellValue.find(value) != string::npos);
                }
            }

            if (matches)
            {
                filtered.push_back(row);
            }
        }

        return filtered;
    }

    // Getters
    const vector<Dato> &getData() const { return data; }
    const vector<std::string> &getColumnNames() const { return columnNames; }
    const vector<DataType> &getColumnTypes() const { return columnTypes; }
    size_t getRowCount() const { return data.size(); }
    size_t getColumnCount() const { return columnNames.size(); }

private:
    // Helper method to parse a line based on delimiter
    vector<string> parseLine(const string &line)
    {
        vector<string> result;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, delimiter))
        {
            cell.erase(0, cell.find_first_not_of(" \t\r\n"));
            cell.erase(cell.find_last_not_of(" \t\r\n") + 1);
            result.push_back(cell);
        }
        return result;
    }

    // Helper method to detect data types for each column
    void detectDataTypes(const vector<vector<string>> &rawData)
    {
        columnTypes.resize(columnNames.size());

        for (size_t col = 0; col < columnNames.size(); ++col)
        {
            columnTypes[col] = inferDataType(rawData, col);
        }
    }

    // Helper method to infer data type for a column
    DataType inferDataType(const vector<vector<std::string>> &rawData, size_t columnIndex)
    {
        map<DataType, int> typeScores;

        for (const auto &row : rawData)
        {
            if (columnIndex < row.size() && !row[columnIndex].empty())
            {
                const std::string &value = row[columnIndex];

                if (isInteger(value))
                {
                    typeScores[DataType::INTEGER]++;
                }
                else if (isFloat(value))
                {
                    typeScores[DataType::FLOAT]++;
                }
                else if (isDate(value))
                {
                    typeScores[DataType::DATE]++;
                }
                else
                {
                    typeScores[DataType::STRING]++;
                }
            }
        }

        // Determine the most likely type
        DataType bestType = DataType::STRING;
        int maxScore = 0;

        for (const auto &pair : typeScores)
        {
            if (pair.second > maxScore)
            {
                maxScore = pair.second;
                bestType = pair.first;
            }
        }

        if (bestType == DataType::STRING && mightBeCategory(rawData, columnIndex))
        {
            bestType = DataType::CATEGORY;
        }
        return bestType;
    }

    // Helper methods to check data types
    bool isInteger(const string &str)
    {
        if (str.empty())
            return false;
        size_t start = (str[0] == '-' || str[0] == '+') ? 1 : 0;
        return start < str.length() &&
               std::all_of(str.begin() + start, str.end(), ::isdigit);
    }

    bool isFloat(const std::string &str)
    {
        try
        {
            std::stof(str);
            return str.find('.') != std::string::npos;
        }
        catch (...)
        {
            return false;
        }
    }

    bool isDate(const std::string &str)
    {
        // Simple date pattern check (YYYY-MM-DD)
        return std::regex_match(str, std::regex(R"(\d{4}-\d{2}-\d{2})"));
    }

    bool mightBeCategory(const vector<vector<string>> &rawData, size_t columnIndex)
    {
        set<string> uniqueValues;

        for (const auto &row : rawData)
        {
            if (columnIndex < row.size() && !row[columnIndex].empty())
            {
                uniqueValues.insert(row[columnIndex]);
            }
        }

        // If there are few unique values compared to total rows, it might be categorical
        return uniqueValues.size() <= max(10, (int)(rawData.size() * 0.1));
    }

    void convertRawDataToDato(const vector<vector<string>> &rawData)
    {
        for (const auto &row : rawData)
        {
            Dato dato(columnNames, columnTypes);

            for (size_t i = 0; i < row.size() && i < columnNames.size(); ++i)
            {
                try
                {
                    dato.setValueFromString(i, row[i]);
                }
                catch (const std::exception &e)
                {
                    // If conversion fails, set as string
                    dato.setValue(i, row[i]);
                }
            }

            data.push_back(dato);
        }
    }

    // Helper method to count non-null values in a column
    int getNonNullCount(size_t columnIndex)
    {
        int count = 0;
        for (const auto &row : data)
        {
            std::string value = row.getValueAsString(columnIndex);
            if (!value.empty() && value != "N/A")
            {
                count++;
            }
        }
        return count;
    }

    // Helper method to print statistics for a numeric column
    void printColumnStatistics(size_t columnIndex)
    {
        std::vector<double> values;

        for (const auto &row : data)
        {
            if (row.isNumericColumn(columnIndex))
            {
                double val = row.getNumericValue(columnIndex);
                values.push_back(val);
            }
        }

        if (values.empty())
            return;

        std::sort(values.begin(), values.end());

        double sum = std::accumulate(values.begin(), values.end(), 0.0);
        double mean = sum / values.size();

        double variance = 0.0;
        for (double val : values)
        {
            variance += (val - mean) * (val - mean);
        }
        variance /= values.size();
        double stddev = std::sqrt(variance);

        double median;
        if (values.size() % 2 == 0)
        {
            median = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2.0;
        }
        else
        {
            median = values[values.size() / 2];
        }

        std::cout << "Statistics for '" << columnNames[columnIndex] << "':" << std::endl;
        std::cout << "  Count: " << values.size() << std::endl;
        std::cout << "  Mean: " << std::fixed << std::setprecision(2) << mean << std::endl;
        std::cout << "  Median: " << median << std::endl;
        std::cout << "  Std Dev: " << stddev << std::endl;
        std::cout << "  Min: " << values.front() << std::endl;
        std::cout << "  Max: " << values.back() << std::endl;

        // Quartiles
        if (values.size() >= 4)
        {
            double q1 = values[values.size() / 4];
            double q3 = values[3 * values.size() / 4];
            std::cout << "  Q1: " << q1 << std::endl;
            std::cout << "  Q3: " << q3 << std::endl;
        }
    }

    // Helper method to print frequency analysis for categorical columns
    void printFrequencyAnalysis(size_t columnIndex)
    {
        std::map<std::string, int> frequency;

        for (const auto &row : data)
        {
            std::string value = row.getValueAsString(columnIndex);
            frequency[value]++;
        }

        std::cout << "Frequency analysis for '" << columnNames[columnIndex] << "':" << std::endl;
        std::cout << std::setw(20) << "Value" << std::setw(10) << "Count" << std::setw(10) << "%" << std::endl;
        std::cout << std::string(40, '-') << std::endl;

        // Sort by frequency (descending)
        std::vector<std::pair<std::string, int>> sortedFreq(frequency.begin(), frequency.end());
        std::sort(sortedFreq.begin(), sortedFreq.end(),
                  [](const auto &a, const auto &b)
                  { return a.second > b.second; });

        for (const auto &pair : sortedFreq)
        {
            double percentage = (double)pair.second / data.size() * 100.0;
            std::cout << std::setw(20) << pair.first
                      << std::setw(10) << pair.second
                      << std::setw(9) << std::fixed << std::setprecision(1) << percentage << "%" << std::endl;
        }
    }
};

#endif // ANALISIS_H
