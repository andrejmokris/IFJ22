#include "parser.h"

String_t string;
node_t symTable;
int curToken;

void initParser() {
    StringInit(&string);
    symTable = NULL;
    curToken = 0;
}

// ID = <expr>;
bool VarAssign() {
    node_t newNode = TreeInsert(&symTable, 0, string);
    if(get_Token(&string) != LEX_ASSIGN) {
        return false;
    }
    int resDataType = 0;
    if(parseExpression(LEX_SEMICOL, &resDataType) != SUCCESS) {
        return false;
    }
    newNode->dataType = resDataType;
    return true;
}

int mainParser() {
    initParser();
    if (!checkProlog(&string)) {
        stringDeconstruct(&string);
        TreeDeconstruct(symTable);
        errorExit(SYNTAX_ERROR, "Wrong prolog\n");
    }
    curToken = get_Token(&string);
    switch (curToken) {
        case LEX_ID:
            if(!VarAssign()) {
                return SYNTAX_ERROR;
            }
            break;
        default:
            break;
    }
    printf("ROOT: %s | TYPE: %d\n", symTable->NodeID.string, symTable->dataType);
    return SUCCESS;
}
