#include "parser.h"

String_t string;
node_t symTable;
node_t funcTable;
int curToken;

void initParser() {
    StringInit(&string);
    symTable = NULL;
    funcTable = NULL;
    curToken = 0;
}

int endParser(int errCode) {
    stringDeconstruct(&string);
    TreeDeconstruct(symTable);
    return errCode;
}

bool Type() {
    curToken = get_Token(&string);
    if (curToken == LEX_TYPE_STRING || curToken == LEX_TYPE_STRING_OPT) {
        return true;
    } else if (curToken == LEX_TYPE_INT || curToken == LEX_TYPE_INT_OPT) {
        return true;
    } else if (curToken == LEX_TYPE_FLOAT || curToken == LEX_TYPE_FLOAT_OPT) {
        return true;
    } else {
        return false;
    }
}

// ID = <expr>;
bool VarAssign() {
    printf("CALL ASSIGN\n");
    node_t newNode = TreeFind(symTable, string.string);
    if (newNode == NULL) {
        newNode = TreeInsert(&symTable, 0, string);
    }
    if (get_Token(&string) != LEX_ASSIGN) {
        return false;
    }
    int resDataType;
    if (parseExpression(LEX_SEMICOL, &resDataType, symTable) != SUCCESS) {
        return false;
    }
    newNode->dataType = resDataType;
    return true;
}

bool getSingleParam(node_t funcNode) {
    bool lastComma = (curToken == LEX_COMMA);
    int dataType;
    if (!Type()) {
        if(curToken == LEX_RPAR && !lastComma) {
            return true;
        }
        return false;
    }
    dataType = curToken;
    curToken = get_Token(&string);
    if (curToken != LEX_ID) {
        return false;
    }
    addParam(funcNode, dataType, string);
    curToken = get_Token(&string);
    if (curToken == LEX_COMMA) {
        return getSingleParam(funcNode);
    } else if(curToken == LEX_RPAR) {
        return true;
    }
    return false;
}

bool getFuncParams(node_t funcNode) {
    if (get_Token(&string) != LEX_LPAR) {
        return false;
    }
    return getSingleParam(funcNode);
}

bool functionDeclaration() {
    // get function id
    curToken = get_Token(&string);
    if (curToken != LEX_FUNID) {
        return false;
    }
    if(TreeFind(funcTable, string.string) != NULL) {
        printf("redefinition of function\n");
        return false;
    }
    node_t funcNode = createFuncNode(LEX_FUNID, string);
    if(TreeInsertNode(&funcTable, funcNode) == NULL) {
        return false;
    }
    if (!getFuncParams(funcNode)) {
        return false;
    }
    if (get_Token(&string) != LEX_COLON) {
        return false;
    }
    if (!Type()) {
        return false;
    }
    funcNode->function->returnType = curToken;
    printf("FUNC NAME: %s\n", funcNode->NodeID.string);
    for(int i = 0; i < funcNode->function->nOfParams; i++) {
        printf("NAME: %s | TYPE: %d\n", funcNode->function->params[i]->ParamID.string, funcNode->function->params[i]->dataType);
    }
    printf("\n");
    return true;
}

int ParserLoop() {
    curToken = get_Token(&string);
    switch (curToken) {
        case LEX_ID:
            if (VarAssign() == false) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop();
            }
            break;
        case LEX_EPILOG:
            printf("EPILOG\n");
            return SUCCESS;
        case LEX_FUNKW:
            if (functionDeclaration() == false) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop();
            }
        case LEX_ERR:
            return SYNTAX_ERROR;
        default:
            return SUCCESS;
    }
    return SUCCESS;
}

int mainParser() {
    initParser();
    if (!checkProlog(&string)) {
        return endParser(SYNTAX_ERROR);
    }
    if (ParserLoop() == SUCCESS) {
        return endParser(SUCCESS);
    } else {
        return endParser(SYNTAX_ERROR);
        ;
    }
}
