#ifndef _CODEGEN
#define _CODEGEN

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dynamic_string.h"
#include "generator.h"
#include "scanner.h"

bool print_code();

bool put_OPERATOR(int type);

bool write_text(const char *text);

bool call(const char *id);

bool push_operand(const char *id);

bool push_operandTF(const char *id);

bool push_int(const char *value);

bool push_float(const char *value);

bool push_string(const char *value);

bool push_bool(const char *value);

bool push_null();

bool assign(const char *id);

bool assignTF(const char *id);

bool label(const char *id);

bool tmpF();

bool pushF();

bool popF();

bool new_var(const char *id);

bool new_varTF(const char *id);

bool jumpIfNeqS(const char *label);

bool jump(const char *label);

bool write(const char *text);

bool exit_function();

#endif