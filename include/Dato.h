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

enum class Category
{
    UNKNOWN,
    GRADE_A,
    GRADE_B,
    GRADE_C,
    GRADE_D,
    GRADE_F,
    CUSTOM
};

using DataValue = variant<int, float, string, chrono::year_month_day, Category>;

class Dato
{
private:
    vector<string> columnNames;
    vector<DataType> columnTypes;
    vector<DataValue> values;
    map<string, std::vector<string>> categoryMappings; // For custom categories

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
          values(other.values),
          categoryMappings(other.categoryMappings) {}

    Dato &operator=(const Dato &other)
    {
        if (this != &other)
        {
            columnNames = other.columnNames;
            columnTypes = other.columnTypes;
            values = other.values;
            categoryMappings = other.categoryMappings;
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
    std::string getValueAsString(size_t columnIndex) const
    {
        if (columnIndex >= values.size())
        {
            return "N/A";
        }

        return visit([this](const auto &value) -> string
                     {
            using T = decay_t<decltype(value)>;
            if constexpr (is_same_v<T, int>) {
                return to_string(value);
            } else if constexpr (is_same_v<T, float>) {
                ostringstream oss;
                oss << fixed << setprecision(2) << value;
                return oss.str();
            } else if constexpr (is_same_v<T, std::string>) {
                return value;
            } else if constexpr (is_same_v<T, std::chrono::year_month_day>) {
                std::ostringstream oss;
                oss << value.year() << "-" << std::setfill('0') << std::setw(2) 
                    << static_cast<unsigned>(value.month()) << "-" 
                    << std::setfill('0') << std::setw(2) << static_cast<unsigned>(value.day());
                return oss.str();
            } else if constexpr (is_same_v<T, Category>) {
                return categoryToString(value);
            }
            return "Unknown"; }, values[columnIndex]);
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

    // Helper method to parse category from string
    Category parseCategory(const std::string &catStr)
    {
        std::string upper = catStr;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

        if (upper == "A")
            return Category::GRADE_A;
        if (upper == "B")
            return Category::GRADE_B;
        if (upper == "C")
            return Category::GRADE_C;
        if (upper == "D")
            return Category::GRADE_D;
        if (upper == "F")
            return Category::GRADE_F;

        return Category::CUSTOM;
    }

    // Helper method to convert category to string
    std::string categoryToString(Category cat) const
    {
        switch (cat)
        {
        case Category::GRADE_A:
            return "A";
        case Category::GRADE_B:
            return "B";
        case Category::GRADE_C:
            return "C";
        case Category::GRADE_D:
            return "D";
        case Category::GRADE_F:
            return "F";
        case Category::CUSTOM:
            return "Custom";
        default:
            return "Unknown";
        }
    }
};

// Helper function to convert string to DataType
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

// Helper function to convert DataType to string
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
