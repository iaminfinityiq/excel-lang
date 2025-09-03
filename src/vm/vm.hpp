#include <string>
#include <map>
#include <utility>
#include <vector>
#include <stack>

#pragma once

enum class DataType {
    NUMBER,
    STRING
};

class RuntimeValue {
public:
    DataType data_type;
    long long start_column;
    long long start_row;
};

class Number : public RuntimeValue {
public:
    double value;

    Number(const long long start_column, const long long start_row, const double value);
};

class String : public RuntimeValue {
public:
    std::string value;

    String(const long long start_column, const long long start_row, const std::string value);
};

class Scope {
public:
    Scope();
    void assign_cell(const long long start_column, const long long start_row, const std::string column, const long long row, const RuntimeValue value);
    void assign_range(const long long start_column, const long long start_row, const std::string column1, const long long row1, const std::string column2, const long long row2, const RuntimeValue value);
    RuntimeValue retrieve(const long long start_column, const long long start_row, const std::string column, const long long row);
private:
    std::map<std::pair<long long, std::pair<std::pair<long long, long long>, std::pair<long long, long long>>>, RuntimeValue> assignments;
    long long order;
};

class VM {
public:
    VM(const std::string vmcode);
    void run();
private:
    std::vector<std::string> instructions;
    std::stack<RuntimeValue*> stack;

    // Running through instructions
    long long idx;
    std::string instruction;
    void advance();

    // Running the instruction
    void run_instruction();

    // Errors
    void throw_incomplete_string(const std::string current);
    void throw_not_enough_arguments(const long long expected, const long long got);
    void throw_not_a_positive_integer(const std::string positive_integer);
    void throw_not_a_number(const std::string number);
};