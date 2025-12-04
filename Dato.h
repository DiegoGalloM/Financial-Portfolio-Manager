#ifndef DATO_H
#define DATO_H

#include <string>
#include <variant>
#include <vector>
#include <map>
#include <chrono>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

enum class DataType
{
    INTEGER,
    FLOAT,
    STRING,
    DATE,
    CATEGORY
};

struct NamedCategory
{
    string value;

    // Overload operators so this struct works inside variants and comparisons
    bool operator==(const NamedCategory &other) const { return value == other.value; }
    bool operator<(const NamedCategory &other) const { return value < other.value; }
};

using DataValue = variant<int, float, string, chrono::year_month_day, NamedCategory>;

class Dato
{
private:
    vector<string> columnNames;
    vector<DataType> columnTypes;
    vector<DataValue> values;

public:
    Dato() = default;

    Dato(const vector<string> &names, const vector<DataType> &types)
        : columnNames(names), columnTypes(types)
    {
        values.resize(names.size());
    }

    Dato(const Dato &other)
        : columnNames(other.columnNames),
          columnTypes(other.columnTypes),
          values(other.values) {}

    void setValue(size_t index, const string &val)
    {
        if (index >= values.size())
            return;

        try
        {
            switch (columnTypes[index])
            {
            case DataType::INTEGER:
                values[index] = stoi(val);
                break;
            case DataType::FLOAT:
                values[index] = stof(val);
                break;
            case DataType::STRING:
                values[index] = val;
                break;
            case DataType::DATE:
                values[index] = parseDate(val);
                break;
            case DataType::CATEGORY:
                values[index] = parseCategory(val);
                break;
            }
        }
        catch (...)
        {
            // Handle conversion errors (defaults)
            if (columnTypes[index] == DataType::INTEGER)
                values[index] = 0;
            else if (columnTypes[index] == DataType::FLOAT)
                values[index] = 0.0f;
            else if (columnTypes[index] == DataType::STRING)
                values[index] = "";
            else if (columnTypes[index] == DataType::CATEGORY)
                values[index] = NamedCategory{"Unknown"};
        }
    }

    Dato &operator=(const Dato &other)
    {
        if (this != &other)
        {
            columnNames = other.columnNames;
            columnTypes = other.columnTypes;
            values = other.values;
        }
        return *this;
    }

    // Method to set the structure (columns and types)
    void setStructure(const vector<string> &names, const vector<DataType> &types)
    {
        if (names.size() != types.size())
        {
            throw std::invalid_argument("Column names and types must have the same size");
        }
        columnNames = names;
        columnTypes = types;
        values.resize(names.size());
    }

    void addColumn(const string &name, DataType type)
    {
        columnNames.push_back(name);
        columnTypes.push_back(type);
        values.emplace_back();
    }

    void setValue(size_t columnIndex, const DataValue &value)
    {
        if (columnIndex < 0 || columnIndex >= values.size())
        {
            throw std::out_of_range("Column index out of range");
        }
        values[columnIndex] = value;
    }

    void setValue(const string &columnName, const DataValue &value)
    {
        auto it = find(columnNames.begin(), columnNames.end(), columnName);
        if (it == columnNames.end())
        {
            throw std::invalid_argument("Column name not found: " + columnName);
        }
        size_t index = distance(columnNames.begin(), it);
        setValue(index, value);
    }

    DataValue getValue(size_t columnIndex) const
    {
        if (columnIndex >= values.size())
        {
            throw std::out_of_range("Column index out of range");
        }
        return values[columnIndex];
    }

    DataValue getValue(const std::string &columnName) const
    {
        auto it = find(columnNames.begin(), columnNames.end(), columnName);
        if (it == columnNames.end())
        {
            throw std::invalid_argument("Column name not found: " + columnName);
        }
        size_t index = distance(columnNames.begin(), it);
        return getValue(index);
    }

    // Method to get value as string for display
    string getValueAsString(size_t index) const
    {
        if (index >= values.size())
            return "";

        return visit([this, index](auto &&arg) -> string
                     {
            using T = decay_t<decltype(arg)>;
            if constexpr (is_same_v<T, int>)
                return to_string(arg);
            else if constexpr (is_same_v<T, float>) {
                stringstream ss;
                ss << fixed << setprecision(2) << arg;
                return ss.str();
            }
            else if constexpr (is_same_v<T, string>)
                return arg;
            else if constexpr (is_same_v<T, chrono::year_month_day>) {
                return format("{:%Y-%m-%d}", arg);
            }
            else if constexpr (is_same_v<T, NamedCategory>) {
                // <--- CHANGED: Simply return the stored string value
                return arg.value; 
            }
            return ""; }, values[index]);
    }

    void setValueFromString(size_t columnIndex, const string &strValue)
    {
        if (columnIndex >= columnTypes.size())
        {
            throw std::out_of_range("Column index out of range");
        }

        switch (columnTypes[columnIndex])
        {
        case DataType::INTEGER:
            setValue(columnIndex, std::stoi(strValue));
            break;
        case DataType::FLOAT:
            setValue(columnIndex, std::stof(strValue));
            break;
        case DataType::STRING:
            setValue(columnIndex, strValue);
            break;
        case DataType::DATE:
            setValue(columnIndex, parseDate(strValue));
            break;
        case DataType::CATEGORY:
            setValue(columnIndex, parseCategory(strValue));
            break;
        }
    }

    void setValueFromString(const string &columnName, const std::string &strValue)
    {
        auto it = find(columnNames.begin(), columnNames.end(), columnName);
        if (it == columnNames.end())
        {
            throw std::invalid_argument("Column name not found: " + columnName);
        }
        size_t index = distance(columnNames.begin(), it);
        setValueFromString(index, strValue);
    }

    // The Getters
    const vector<std::string> &getColumnNames() const { return columnNames; }
    const vector<DataType> &getColumnTypes() const { return columnTypes; }
    size_t getColumnCount() const { return columnNames.size(); }

    DataType getColumnType(const string &columnName) const
    {
        auto it = std::find(columnNames.begin(), columnNames.end(), columnName);
        if (it == columnNames.end())
        {
            throw std::invalid_argument("Column name not found: " + columnName);
        }
        size_t index = distance(columnNames.begin(), it);
        return columnTypes[index];
    }

    void display() const
    {
        // Print column headers
        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            std::cout << std::setw(15) << columnNames[i];
        }
        std::cout << std::endl;

        // Print separator
        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            std::cout << std::setw(15) << "---------------";
        }
        std::cout << std::endl;

        // Print values
        for (size_t i = 0; i < columnNames.size(); ++i)
        {
            std::cout << std::setw(15) << getValueAsString(i);
        }
        std::cout << std::endl;
    }

    bool isNumericColumn(size_t columnIndex) const
    {
        if (columnIndex >= columnTypes.size())
            return false;
        return columnTypes[columnIndex] == DataType::INTEGER ||
               columnTypes[columnIndex] == DataType::FLOAT;
    }

    bool isNumericColumn(const string &columnName) const
    {
        auto it = std::find(columnNames.begin(), columnNames.end(), columnName);
        if (it == columnNames.end())
            return false;
        size_t index = distance(columnNames.begin(), it);
        return isNumericColumn(index);
    }

    double getNumericValue(size_t columnIndex) const
    {
        if (!isNumericColumn(columnIndex))
            return 0.0;

        return visit([](const auto &value) -> double
                     {
            using T = decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, int>) {
                return static_cast<double>(value);
            } else if constexpr (std::is_same_v<T, float>) {
                return static_cast<double>(value);
            }
            return 0.0; }, values[columnIndex]);
    }

private:
    std::chrono::year_month_day parseDate(const std::string &dateStr)
    {
        // Simple date parsing - assumes YYYY-MM-DD format
        std::istringstream ss(dateStr);
        std::string item;
        std::vector<int> parts;

        while (std::getline(ss, item, '-'))
        {
            parts.push_back(std::stoi(item));
        }

        if (parts.size() == 3)
        {
            return std::chrono::year_month_day(
                std::chrono::year(parts[0]),
                std::chrono::month(parts[1]),
                std::chrono::day(parts[2]));
        }

        // Default date if parsing fails
        return std::chrono::year_month_day(std::chrono::year(1900), std::chrono::month(1), std::chrono::day(1));
    }

    NamedCategory parseCategory(const string &val)
    {
        // You can still add validation logic here if you want to restrict categories,
        // but for "Custom" names, we just accept the value.
        return NamedCategory{val};
    }

    // Helper method to convert category to string
};

inline DataType stringToDataType(const std::string &typeStr)
{
    std::string lower = typeStr;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower == "int" || lower == "integer")
        return DataType::INTEGER;
    if (lower == "float" || lower == "double")
        return DataType::FLOAT;
    if (lower == "string" || lower == "text")
        return DataType::STRING;
    if (lower == "date" || lower == "datetime")
        return DataType::DATE;
    if (lower == "category" || lower == "enum")
        return DataType::CATEGORY;

    return DataType::STRING; // Default to string
}

inline std::string dataTypeToString(DataType type)
{
    switch (type)
    {
    case DataType::INTEGER:
        return "Integer";
    case DataType::FLOAT:
        return "Float";
    case DataType::STRING:
        return "String";
    case DataType::DATE:
        return "Date";
    case DataType::CATEGORY:
        return "Category";
    default:
        return "Unknown";
    }
}

#endif // DATO_H
