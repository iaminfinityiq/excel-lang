#include <string>
#include <unordered_map>
#include <utility>
#include "vm.hh"
#include <sstream>
#include <cstdint>

uint64_t column_to_ord(const std::string column)
{
    uint64_t ord = 0;
    for (uint64_t i = 0; i < column.size(); i++)
    {
        ord *= 26;
        ord += std::tolower(column[i]) - 'a';
    }

    return ord;
}

Number::Number(const uint64_t start_column, const uint64_t start_line, const double value)
{
    this->data_type = DataType::NUMBER;
    this->value = value;

    this->start_column = start_column;
    this->start_line = start_line;
}

String::String(const uint64_t start_column, const uint64_t start_line, const std::string value)
{
    this->data_type = DataType::STRING;
    this->value = value;

    this->start_column = start_column;
    this->start_line = start_line;
}

Scope::Scope()
{
    this->assignments.clear();
}

void Scope::assign_cell(const std::string column, const uint64_t row, const RuntimeValue value)
{
    std::stringstream ss;
    ss << "c"; // c stands for cell assignment
    for (uint64_t i = 0; i < column.size(); i++)
    {
        ss << std::tolower(column[i]);
    }

    ss << row;
    this->assignments[ss.str()] = value;
}

void Scope::assign_range(const std::string column1, const uint64_t row1, const std::string column2, const uint64_t row2, const RuntimeValue value)
{
    std::stringstream ss;
    ss << "r"; // r stands for range assignment
    for (uint64_t i = 0; i < column1.size(); i++)
    {
        ss << std::tolower(column1[i]);
    }

    for (uint64_t i = 0; i < column2.size(); i++)
    {
        ss << std::tolower(column2[i]);
    }

    ss << row2;
    this->assignments[ss.str()] = value;
}

RuntimeValue Scope::retrieve(const uint64_t start_column, const uint64_t start_row, const std::string column, const uint64_t row) const
{
    for (auto it = this->assignments.begin(); it != this->assignments.end(); it++)
    {
        if (it->first[0] == 'c')
        {
            // Cell assignment
            std::string column_scope = "";
            uint64_t row_scope = 0;
            for (uint64_t i = 1; i < it->first.size(); i++)
            {
                if ('0' <= it->first[i] && it->first[i] <= '9')
                {
                    row_scope *= 10;
                    row_scope += it->first[i] - '0';
                }
                else
                {
                    column_scope += it->first[i];
                }
            }

            if (column.size() != column_scope.size())
            {
                continue;
            }

            bool flag = true;
            for (uint64_t i = 0; i < column.size(); i++)
            {
                if (std::tolower(column[i]) != column_scope[i])
                {
                    flag = false;
                }
            }

            if (!flag || row != row_scope)
            {
                continue;
            }

            return it->second;
        }

        // Range assignment
        uint64_t column1 = 0;
        uint64_t row1 = 0;

        uint64_t column2 = 0;
        uint64_t row2 = 0;

        for (uint64_t i = 1; i < it->first.size(); i++)
        {
            if ('0' <= it->first[i] && it->first[i] <= '9')
            {
                if (column2 == 0)
                {
                    row1 *= 10;
                    row1 += it->first[i] - '0';
                }
                else
                {
                    row2 *= 10;
                    row2 += it->first[i] - '0';
                }
            }
            else if (row1 == 0)
            {
                column1 *= 26;
                column1 += it->first[i] - 'a';
            }
            else
            {
                column2 *= 26;
                column2 += it->first[i] - 'a';
            }
        }

        uint64_t column_idx = column_to_ord(column);
        uint64_t row_idx = row;

        if (column1 <= column_idx && column_idx <= column2 && row1 <= row_idx && row_idx <= row2)
        {
            return it->second;
        }
    }

    return Number(start_column, start_row, 0);
}