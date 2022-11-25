#include "print_inst.h"

extern Tinstruction_list list;
extern String_t code;

bool print_code() {
    list_item ptr = list.first;
    while (ptr != NULL) {
        for (int i = 0; code.string[ptr->string_pos + i] != '\n'; i++) {
            putc(code.string[ptr->string_pos + i], stdout);
        }
        putc('\n', stdout);
        ptr = ptr->item_next;
    }
    return true;
}

bool put_OPERATOR(int type) {
    switch (type) {
        case (LEX_ADD):
            WINSTRUCTION("ADDS");
            break;
        case (LEX_SUB):
            WINSTRUCTION("SUBS");
            break;
        case (LEX_MUL):
            WINSTRUCTION("MULS");
            break;
        case (LEX_DIV):
            WINSTRUCTION("DIVS");
            break;
        case (LEX_EQ):
            WINSTRUCTION("EQS");
            break;
        case (LEX_NEQ):
            WINSTRUCTION("EQS");
            break;
        case (LEX_LE):
            WINSTRUCTION("LTS");
            break;
        case (LEX_LEQ):
            WINSTRUCTION("LTS");
            break;
        case (LEX_GT):
            WINSTRUCTION("GTS");
            break;
        case (0):
            WINSTRUCTION("NOTS");
            break;
        case (69):
            WINSTRUCTION("ORS");
            break;
        case (420):
            WINSTRUCTION("ANDS");
            break;
    }
    return true;
}
bool write_text(const char *text) {
    WTEXT(text);
    WTEXT("\n");
    return true;
}

bool call(const char *id) {
    WTEXT("CALL ");
    WTEXT(id);
    WTEXT("\n");
    return true;
}

bool push_operand(const char *id) {
    WTEXT("PUSHS LF@");
    WTEXT(id);
    WTEXT("\n");
    return true;
}

bool push_operandTF(const char *id) {
    WTEXT("PUSHS TF@");
    WTEXT(id);
    WTEXT("\n");
    return true;
}

bool push_int(const char *value) {
    WTEXT("PUSHS int@");
    WTEXT(value);
    WTEXT("\n");
    return true;
}

bool push_float(const char *value) {
    WTEXT("PUSHS float@");
    WTEXT(value);
    WTEXT("\n");
    return true;
}

bool push_string(const char *value) {
    WTEXT("PUSHS string@");
    WTEXT(value);
    WTEXT("\n");
    return true;
}

bool push_bool(const char *value) {
    WTEXT("PUSHS bool@");
    WTEXT(value);
    WTEXT("\n");
    return true;
}

bool push_null() {
    WINSTRUCTION("PUSHS nil@nil\n");
    return true;
}

bool assign(const char *id) {
    WTEXT("POPS LF@");
    WTEXT(id);
    WTEXT("\n");
    return true;
}

bool assignTF(const char *id) {
    WTEXT("POPS TF@");
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

bool new_varTF(const char *id) {
    WTEXT("DEFVAR TF@");
    WTEXT(id);
    WTEXT("\n");
    return true;
}

bool jumpIfNeqS(const char *label) {
    WTEXT("JUMPIFNEQS ");
    WTEXT(label);
    WTEXT("\n");
    return true;
}

bool jumpIfEqS(const char *label) {
    WTEXT("JUMPIFEQS ");
    WTEXT(label);
    WTEXT("\n");
    return true;
}

bool jump(const char *label) {
    WTEXT("JUMP ");
    WTEXT(label);
    WTEXT("\n");
    return true;
}

bool write(const char *text) {
    WTEXT("WRITE string@");
    WTEXT(text);
    WTEXT("\n");
    return true;
}

bool exit_function() {
    WINSTRUCTION("RETURN");
    return true;
}

bool clears() {
    WINSTRUCTION("CLEARS");
    return true;
}

bool write_var(const char *name) {
    WTEXT("WRITE LF@");
    WTEXT(name);
    WTEXT("\n");
    return true;
}