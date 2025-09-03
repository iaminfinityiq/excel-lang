#include "parser/node_types.hpp"
#include "interpolation/interpolation.hpp"

#pragma once

void print_expr(Expression* expr);
void print_stmt(const long long indentation, Statement* stmt);
void debug_instructions(Interpolator* interpolator);
void frontend(std::string src_file, std::string dest_file);