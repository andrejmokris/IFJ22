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
    TreeDeconstruct(funcTable);
    if (errCode != SUCCESS) {
        errorExit(errCode, "Chyba\n");
    }
    return errCode;
}

int getParsToken() {
    curToken = get_Token(&string);
    if (curToken == LEX_ERR) {
        endParser(LEX_ERROR);
        errorExit(LEX_ERROR, "Lexikalna chyba\n");
    }
    return curToken;
}

bool Type() {
    curToken = getParsToken();
    if (curToken == LEX_TYPE_STRING || curToken == LEX_TYPE_STRING_OPT) {
        return true;
    } else if (curToken == LEX_TYPE_INT || curToken == LEX_TYPE_INT_OPT) {
        return true;
    } else if (curToken == LEX_TYPE_FLOAT || curToken == LEX_TYPE_FLOAT_OPT) {
        return true;
    } else if (curToken == LEX_VOID) {
        return true;
    } else {
        return false;
    }
}

// ID = <expr>;
bool VarAssign() {
    //printf("VarAssign\n");
    node_t newNode = TreeFind(symTable, string.string);
    if (newNode == NULL) {
        if((newNode = TreeInsert(&symTable, 0, string)) == NULL) {
            endParser(INTERNAL_ERROR);
        }
    }
    //printf("NEW NODE: %s\n", newNode->NodeID.string);
    if (getParsToken() != LEX_ASSIGN) {
        return false;
    }
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_SEMICOL, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    newNode->dataType = resDataType;
    return true;
}

bool getSingleParam(node_t funcNode) {
    bool lastComma = (curToken == LEX_COMMA);
    int dataType;
    if (!Type()) {
        if (curToken == LEX_RPAR && !lastComma) {
            return true;
        }
        return false;
    }
    dataType = curToken;
    curToken = getParsToken();
    if (curToken != LEX_ID) {
        return false;
    }
    if (!addParam(funcNode, dataType, string)) {
        return false;
    }
    curToken = getParsToken();
    if (curToken == LEX_COMMA) {
        return getSingleParam(funcNode);
    } else if (curToken == LEX_RPAR) {
        return true;
    }
    return false;
}

bool getFuncParams(node_t funcNode) {
    if (getParsToken() != LEX_LPAR) {
        return false;
    }
    return getSingleParam(funcNode);
}

bool functionDeclaration() {
    // get function id
    curToken = getParsToken();
    if (curToken != LEX_FUNID) {
        return false;
    }
    if (TreeFind(funcTable, string.string) != NULL) {
        printf("Redefinition of a function\n");
        endParser(SEMANTIC_ERROR);
    }
    node_t funcNode = createFuncNode(LEX_FUNID, string);
    if (TreeInsertNode(&funcTable, funcNode) == NULL) {
        return false;
    }
    if (!getFuncParams(funcNode)) {
        return false;
    }
    if (getParsToken() != LEX_COLON) {
        return false;
    }
    if (!Type()) {
        return false;
    }
    funcNode->function->returnType = curToken;
    return true;
}

// check if functin call matches parameter data types
/* op1 - ocakavany dt   op2 - prichadzajuci dt */
bool parameterDataTypeVerify(int op1, int op2) {
    if (op2 == LEX_ID) {
        op2 = TreeFind(symTable, string.string)->dataType;
    }
    if (op1 == LEX_TYPE_STRING_OPT) {
        if (op2 == LEX_STRING || op2 == LEX_NULL) {
            return true;
        } else {
            return false;
        }
    } else if (op1 == LEX_TYPE_STRING && op2 == LEX_STRING) {
        return true;
    }

    if (op1 == LEX_TYPE_FLOAT_OPT) {
        if (op2 == LEX_FLOAT || op2 == LEX_NULL) {
            return true;
        } else {
            return false;
        }
    } else if (op1 == LEX_TYPE_FLOAT && op2 == LEX_FLOAT) {
        return true;
    }

    if (op1 == LEX_TYPE_INT_OPT) {
        if (op2 == LEX_INT || op2 == LEX_NULL) {
            return true;
        } else {
            return false;
        }
    } else if (op1 == LEX_TYPE_INT && op2 == LEX_INT) {
        return true;
    }

    return false;
}

bool functionCall() {
    node_t funcNode = TreeFind(funcTable, string.string);
    if (funcNode == NULL) {
        printf("Undefined function\n");
        endParser(SEMANTIC_ERROR);
    }
    int nOfParams = funcNode->function->nOfParams;
    printf("N of params: %d\n", nOfParams);
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    // loading parameters and comparing data types
    for (int i = 0; i < nOfParams; i++) {
        getParsToken();
        // verify if expected datatype of parameter arrives
        if (!parameterDataTypeVerify(funcNode->function->params[i]->dataType,
                                     curToken)) {
            endParser(RUN_ERROR);
        } else {
            // PARAM HAS GOOD DATA TYPE, it can be pushed to stack or idk
            printf("Good param\n");
        }
        if (i < nOfParams - 1 && getParsToken() != LEX_COMMA) {
            endParser(SYNTAX_ERROR);
            return false;
        } else if (i == nOfParams - 1 && getParsToken() == LEX_RPAR) {
            break;
        }
    }
    if (nOfParams == 0) {
        if (getParsToken() != LEX_RPAR) {
            return false;
        }
    }
    return getParsToken() == LEX_SEMICOL;
}

bool ifRule() {
    printf("IF RULE\n");
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
        return false;
    }
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    // datatype of IF condition has to be BOOL
    if (resDataType != LEX_BOOL) {
        printf("Invalid expression in IF statement\n");
        endParser(TYPECOMP_ERORR);
        return false;
    }
    if (getParsToken() != LEX_LCRB) {
        printf("Missing { after IF statement\n");
        endParser(SYNTAX_ERROR);
        return false;
    }
    return statementList();
}

bool statementList() {
    curToken = getParsToken();
    switch (curToken) {
        case LEX_IF:
            if(ifRule()) {
                return statementList();
            } else {
                endParser(SYNTAX_ERROR);
            }
            break;
        case LEX_WHILE:
        case LEX_FUNID:
            return functionCall();
        case LEX_ID:
            if(VarAssign()) {
                return statementList();
            } else {
                endParser(SYNTAX_ERROR);
            }
            break;
        case LEX_ERR:
            return false;
        case LEX_RCRB:
            return true;
        default:
            return false;
    }
    return false;
}

// Global
int ParserLoop() {
    curToken = getParsToken();
    switch (curToken) {
        case LEX_ID:
            if (VarAssign() == false) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop();
            }
            break;
        case LEX_EPILOG:
            return SUCCESS;
        case LEX_FUNKW:
            if (functionDeclaration() == false) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop();
            }
        case LEX_FUNID:
            printf("function call\n");
            if (!functionCall()) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop();
            }
        case LEX_IF:
            if (!ifRule()) {
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
    }
}
