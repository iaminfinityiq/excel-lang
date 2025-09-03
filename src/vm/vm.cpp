#include "vm.hpp"
#include <string>
#include <climits>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <stack>

long long column_to_ord(std::string column) {
    long long res = 0;
    for (long long i = 0; i < column.size(); i++) {
        res *= 26;
        res += std::tolower(column[i]) - 'a';
    }

    return res;
}

bool is_positive_integer_check(std::string positive_integer) {
    for (long long i = 0; i < positive_integer.size(); i++) {
        if (positive_integer[i] < '0' || positive_integer[i] > '9') {
            return false;
        }
    }

    return true;
}

bool is_number_check(std::string number) {
    bool dot = false;
    for (long long i = 0; i < number.size(); i++) {
        if (number[i] == '.') {
            if (dot) {
                return false;
            }

            dot = true;
            continue;
        }

        if (number[i] < '0' || number[i] > '9') {
            return false;
        }
    }

    return true;
}

Number::Number(const long long start_column, const long long start_row, const double value) {
    this->data_type = DataType::NUMBER;
    this->value = value;

    this->start_column = start_column;
    this->start_row = start_row;
}

String::String(const long long start_column, const long long start_row, const std::string value) {
    this->data_type = DataType::STRING;
    this->value = value;

    this->start_column = start_column;
    this->start_row = start_row;
}

const RuntimeValue null(const long long start_column, const long long start_row) {
    return String(start_column, start_row, "");
}

Scope::Scope() {
    this->assignments.clear();
    this->order = LLONG_MIN;
}

void Scope::assign_cell(const long long start_column, const long long start_row, const std::string column, const long long row, const RuntimeValue value) {
    long long column_ord = column_to_ord(column);
    this->assignments[{this->order, {{column_ord, row}, {column_ord, row}}}] = value;
    this->order++;
}

void Scope::assign_range(const long long start_column, const long long start_row, const std::string column1, const long long row1, const std::string column2, const long long row2, const RuntimeValue value) {
    long long column_ord1 = column_to_ord(column1);
    long long column_ord2 = column_to_ord(column2);

    long long left_column = std::min(column_ord1, column_ord2);
    long long right_column = std::max(column_ord1, column_ord2);

    long long left_row = std::min(row1, row2);
    long long right_row = std::max(row1, row2);

    this->assignments[{this->order, {{left_column, left_row}, {right_column, right_row}}}] = value;
    this->order++;
}

RuntimeValue Scope::retrieve(const long long start_column, const long long start_row, const std::string column, const long long row) {
    long long column_ord = column_to_ord(column);
    for (auto it = this->assignments.begin(); it != this->assignments.end(); it++) {
        if (it->first.second.first.first <= column_ord && column_ord <= it->first.second.second.first && it->first.second.first.second <= row && row <= it->first.second.second.second) {
            RuntimeValue returned = it->second;
            returned.start_column = start_column;
            returned.start_row = start_row;
        }
    }

    return null(start_column, start_row);
}

VM::VM(const std::string vmcode) {
    std::string instruction = "";
    this->instructions.clear();
    while (!this->stack.empty()) {
        this->stack.pop();
    }

    for (long long i = 0; i < vmcode.size(); i++) {
        if (vmcode[i] == '\n') {
            if (instruction != "") {
                this->instructions.push_back(instruction);
            }

            instruction = "";
        } else {
            instruction += vmcode[i];
        }
    }

    if (instruction != "") {
        this->instructions.push_back(instruction);
    }

    this->idx = -1;
    this->advance();
}

void VM::advance() {
    this->idx++;
    if (0 <= this->idx && this->idx < this->instructions.size()) {
        this->instruction = this->instructions[this->idx];
    } else {
        this->instruction = "\0";
    }
}

void VM::run() {
    while (this->instruction != "\0") {
        this->run_instruction();
        this->advance();
    }
}

void VM::run_instruction() {
    std::vector<std::string> tokens(0);
    std::string current = "";
    bool in_string = false;
    bool backslash = false;

    for (long long i = 0; i < this->instruction.size(); i++) {
        if (in_string) {
            if (backslash) {
                current += this->instruction[i];
                backslash = false;
            } else {
                current += this->instruction[i];
                if (this->instruction[i] == '"') {
                    in_string = false;
                    tokens.push_back(current);
                }
            }
        } else {
            if (this->instruction[i] == '"') {
                in_string = true;
                current += '"';
            } else if (this->instruction[i] == ' ') {
                if (current != "") {
                    tokens.push_back(current);
                }

                current = "";
            } else {
                current += this->instruction[i];
            }
        }
    }

    if (in_string) {
        this->throw_incomplete_string(current);
    } else if (current != "") {
        tokens.push_back(current);
    }

    if (tokens.empty()) {
        return;
    }

    if (tokens[0] == "PUSH") {
        if (tokens.size() != 6) {
            this->throw_not_enough_arguments(6, tokens.size());
        }

        if (!is_positive_integer_check(tokens[1])) {
            this->throw_not_a_positive_integer(tokens[1]);
        }

        if (!is_positive_integer_check(tokens[2])) {
            this->throw_not_a_positive_integer(tokens[2]);
        }

        if (is_number_check(tokens[3])) {
            if (!is_positive_integer_check(tokens[4])) {
                this->throw_not_a_positive_integer(tokens[4]);
            }

            if (!is_positive_integer_check(tokens[5])) {
                this->throw_not_a_positive_integer(tokens[5]);
            }

            this->stack.push(new Number(std::stoll(tokens[4]), std::stoll(tokens[5]), std::stod(tokens[3])));
        } else if (tokens[3][0] == '"') {
            if (!is_positive_integer_check(tokens[4])) {
                this->throw_not_a_positive_integer(tokens[4]);
            }

            if (!is_positive_integer_check(tokens[5])) {
                this->throw_not_a_positive_integer(tokens[5]);
            }

            this->stack.push(new String(std::stoll(tokens[4]), std::stoll(tokens[5]), tokens[3]));
        } else {
            throw std::runtime_error("Expected a runtime value at argument 4");
        }
    }
}

void VM::throw_incomplete_string(const std::string current) {
    std::stringstream ss;
    ss << "Incomplete string: " << current;

    throw std::runtime_error(ss.str());
}

void VM::throw_not_enough_arguments(const long long expected, const long long got) {
    std::stringstream ss;
    ss << "Expected " << expected << " arguments, got " << got << "/" << expected << " arguments";

    throw std::runtime_error(ss.str());
}

void VM::throw_not_a_positive_integer(const std::string positive_integer) {
    std::stringstream ss;
    ss << "'" << positive_integer << "'" << " is not a positive integer!";

    throw std::runtime_error(ss.str());
}

void VM::throw_not_a_number(const std::string number) {
    std::stringstream ss;
    ss << "'" << number << "' is not a valid number!";

    throw std::runtime_error(ss.str());
}