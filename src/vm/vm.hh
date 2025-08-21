#include <string>
#include <unordered_map>
#include <utility>
#include <cstdint>

#pragma once

enum class DataType
{
    NUMBER,
    STRING
};

class RuntimeValue
{
public:
    DataType data_type;
    uint64_t start_column;
    uint64_t start_line;
};

class Number : public RuntimeValue
{
public:
    Number(const uint64_t start_column, const uint64_t start_line, const double value);
    double value;
};

class String : public RuntimeValue
{
public:
    String(const uint64_t start_column, const uint64_t start_line, const std::string value);
    std::string value;
};

class Scope
{
public:
    Scope();
    void assign_cell(const std::string column, const uint64_t row, const RuntimeValue value);
    void assign_range(const std::string column1, const uint64_t row1, const std::string column2, const uint64_t row2, const RuntimeValue value);
    RuntimeValue retrieve(const uint64_t start_column, const uint64_t start_row, const std::string column, const uint64_t row) const;

private:
    std::unordered_map<std::string, RuntimeValue> assignments;
};