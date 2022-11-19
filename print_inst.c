#include "print_inst.h"

extern Tinstruction_list important_ptrs;
extern String_t code;

#define WINSTRUCTION(_name)                                    \
    do {                                                       \
        if (stringConcatenate(&code, (_name "\n")) == false) { \
            endParser(INTERNAL_ERROR);                         \
        }                                                      \
    } while (0)

#define WTEXT(_text)                                      \
    do {                                                  \
        if (stringConcatenate(&code, (_text)) == false) { \
            endParser(INTERNAL_ERROR);                    \
        }                                                 \
    } while (0)

bool print_code() {
    list_item ptr = important_ptrs.first;
    while (ptr != NULL) {
        for (int i = 0; ptr->code[i] != '\n'; i++) {
            putc(ptr->code[i], stdout);
        }
        putc('\n', stdout);
        ptr = ptr->item_next;
    }
}

bool put_OPERATOR(int type) {
    switch (type) {
        case (LEX_ADD):
            WINSTRUCTION("ADDS");
        case (LEX_SUB):
            WINSTRUCTION("SUBS");
        case (LEX_MUL):
            WINSTRUCTION("MULS");
        case (LEX_DIV):
            WINSTRUCTION("DIVS");
        case (LEX_EQ):
            WINSTRUCTION("EQS");
        case (LEX_LE):
            WINSTRUCTION("LTS");
        case (LEX_GT):
            WINSTRUCTION("GTS");
            // NOT AND OR? nenašel jsem
    }
    return true;
}

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

bool push_operand(const char *id) {
    WTEXT("PUSHS LF@");
    WTEXT(id);
    WTEXT("\n");
    return true;
}

bool assign(const char *id) {
    WTEXT("POPS LF@");
    WTEXT(id);
    WTEXT("\n");
    return true;
}

bool label(const char *id) {
    WTEXT("LABEL ");
    WTEXT(id);
    WTEXT("\n");
    return true;
}

bool tmpF() {
    WTEXT("CREATEFRAME\n");
    return true;
}

bool pushF() {
    WTEXT("PUSHFRAME\n");
    return true;
}

bool popF() {
    WTEXT("POPFRAME\n");
    return true;
}

bool new_var(const char *id) {
    WTEXT("DEFVAR LF@");
    WTEXT(id);
    WTEXT("\n");
    return true;
}

bool enter_function(const char *id) {
    WTEXT("CALL ");
    WTEXT(id);
    WTEXT("\n");
    WINSTRUCTION("CREATEFRAME");
    return true;
}

bool load_param(const char *id, const char *temp_id) {
    WTEXT("DEFVAR TF@");
    WTEXT(id);
    WTEXT("\n");
    WTEXT("MOVE TF@id");
    WTEXT(id);
    WTEXT(" LF@id");
    WTEXT(temp_id);
    WTEXT("\n");
}

bool return_value(const char *id) {
    WTEXT("PUSHS LF@");
    if (id != NULL) {
        WTEXT(id);
        WTEXT("\n");
    } else {
        /*switch (func_type) {
            case (LEX_INT):
                WTEXT("int@0");
            case (LEX_FLOAT):
                WTEXT("float@0.0");
            case (LEX_STRING):
                WTEXT("string@");
            case (LEX_BOOL):
                WTEXT("bool@false");
        }*/
    }
    return true;
}

bool exit_function() {
    WINSTRUCTION("POPRFAME");
    WINSTRUCTION("RETURN");
    return true;
}