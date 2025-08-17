#include <string>
#include <unordered_map>
#include <utility>

#pragma once

enum class DataType {
    NUMBER,
    STRING
};

class RuntimeValue {
public:
    DataType data_type;
    long long start_column;
    long long start_line;
};

class Number : public RuntimeValue {
public:
    Number(const long long start_column, const long long start_line, const double value);
    double value;
};

class String : public RuntimeValue {
public:
    String(const long long start_column, const long long start_line, const std::string value);
    std::string value;
};

class Scope {
public:
    Scope();
    void assign_cell(const std::string column, const long long row, const RuntimeValue value);
    void assign_range(const std::string column1, const long long row1, const std::string column2, const long long row2, const RuntimeValue value);
    RuntimeValue retrieve(const long long start_column, const long long start_row, const std::string column, const long long row) const;
private:
    std::unordered_map<std::string, RuntimeValue> assignments;
};