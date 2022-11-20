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

/*
bool coversion(int from, int to) {
    switch (from) {
        case (LEX_INT):
            switch (to) {
                case (LEX_FLOAT):
                case (LEX_STRING):
            }
        case (LEX_FLOAT):
            switch (to) {
                case (LEX_INT):
                case (LEX_STRING):
            }
        case (LEX_STRING):
            switch (to) {
                case (LEX_FLOAT):
                case (LEX_INT):
            }
    }
    return true;
}
*/
bool write_text(const char *text);

bool jump(const char *label);

bool clears();

bool push_int(const char *value);

bool push_float(const char *value);

bool push_bool(const char *value);

bool push_operand(const char *id);

bool new_varTF(const char *id);

bool assign(const char *id);

bool label(const char *id);

bool tmpF();

bool pushF();

bool popF();

bool new_var(const char *id);

bool enter_function(const char *id);

bool load_param(const char *id, const char *temp_id);

bool jumpIfNeqS(const char *id);

bool push_null();

bool exit_function();

#endif