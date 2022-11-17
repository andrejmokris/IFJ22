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
    // printf("VarAssign\n");
    node_t newNode = TreeFind(symTable, string.string);
    if (newNode == NULL) {
        if ((newNode = TreeInsert(&symTable, 0, string)) == NULL) {
            endParser(INTERNAL_ERROR);
        }
    }
    // printf("NEW NODE: %s\n", newNode->NodeID.string);
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
    int newToken = getParsToken();
    if (newToken == LEX_RPAR) {
        return true;
    }
    if (newToken == LEX_COMMA) {
        if(!Type()) {
            endParser(SYNTAX_ERROR);
        }
        int dataType = curToken;
        curToken = getParsToken();
        if(curToken != LEX_ID) {
            endParser(SYNTAX_ERROR);
        }
        int addRes;
        if((addRes = addParam(funcNode, dataType, string)) != SUCCESS) {
            endParser(addRes);
        }
        return getSingleParam(funcNode);
    } else {
        endParser(SYNTAX_ERROR);
    }
    return false;
}

bool getParams(node_t funcNode) {
    if (Type()) {
        int dataType = curToken;
        int nextToken = getParsToken();
        if (nextToken != LEX_ID) {
            endParser(SYNTAX_ERROR);
        } else {
            int addRes;
            if ((addRes = addParam(funcNode, dataType, string)) != SUCCESS) {
                endParser(addRes);
            }
            return getSingleParam(funcNode);
        }
    } else {
        if (curToken != LEX_RPAR) {
            endParser(SYNTAX_ERROR);
        }
        return true;
    }
    return false;
}

bool functionDeclaration() {
    // get function id
    curToken = getParsToken();
    if (curToken != LEX_FUNID) {
        endParser(SYNTAX_ERROR);
    }
    if (TreeFind(funcTable, string.string) != NULL) {
        printf("Redefinition of a function\n");
        endParser(SEMANTIC_ERROR);
    }
    node_t funcNode = createFuncNode(LEX_FUNID, string);
    if (TreeInsertNode(&funcTable, funcNode) == NULL) {
        endParser(INTERNAL_ERROR);
    }
    funcNode->function->nOfParams = 0;
    // function ID (
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    if (!getParams(funcNode)) {
        return false;
    }
    // function ID (parList) : return type { stat_list
    if (getParsToken() != LEX_COLON) {
        endParser(SYNTAX_ERROR);
    }
    // return type
    if (!Type()) {
        endParser(SYNTAX_ERROR);
    }
    funcNode->function->returnType = curToken;

    if (getParsToken() != LEX_LCRB) {
        endParser(SYNTAX_ERROR);
    }
    return statementList(true);
}

// check if function call matches parameter data types
/* op1 - ocakavany dt   op2 - prichadzajuci dt */
bool parameterDataTypeVerify(int op1, int op2) {
    if (op2 == LEX_ID) {
        node_t findVar = TreeFind(symTable, string.string);
        if(findVar == NULL) {
            endParser(UNDEFVAR_ERROR);
        }
        op2 = findVar->dataType;
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
    // case for built-in function
    // TODO: add typechecking and codegen for built-in functions
    if (!strcmp(string.string, "write") || !strcmp(string.string, "reads") ||
        !strcmp(string.string, "readi") || !strcmp(string.string, "readf")) {
        printf("built in function\n");
        return true;
    }
    node_t funcNode = TreeFind(funcTable, string.string);
    
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    
    if (funcNode == NULL) {
        printf("Undefined function\n");
        endParser(SEMANTIC_ERROR);
    }
    
    int nOfParams = funcNode->function->nOfParams;
    printf("N of params: %d\n", nOfParams);
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

int ifRule() {
    printf("IF RULE\n");
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
        return FAIL;
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
        return FAIL;
    }
    if (getParsToken() != LEX_LCRB) {
        printf("Missing { after IF statement\n");
        endParser(SYNTAX_ERROR);
        return FAIL;
    }

    if (!statementList(true)) {
        return FAIL;
    }

    if (getParsToken() == LEX_ELSE) {
        printf("HAVING ELSE\n");
        if (getParsToken() != LEX_LCRB) {
            printf("Missing { after ELSE statement\n");
            endParser(SYNTAX_ERROR);
            return FAIL;
        }
        if (statementList(true)) {
            return SUCCESS_ELSE;
        }
    } else {
        return SUCCESS_NOELSE;
    }
    return FAIL;
}

bool whileRule() {
    printf("WHILE RULE\n");
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
        return FAIL;
    }
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    // datatype of IF condition has to be BOOL
    if (resDataType != LEX_BOOL) {
        printf("Invalid expression in WHILE statement\n");
        endParser(TYPECOMP_ERORR);
        return FAIL;
    }
    if (getParsToken() != LEX_LCRB) {
        printf("Missing { after WHILE statement\n");
        endParser(SYNTAX_ERROR);
        return FAIL;
    }

    return statementList(true);
}

// statement list
// similiar to parser loop, but can't have function definition
// is called in if_statement or function body
bool statementList(bool getNext) {
    // find if loading next token is necessary
    // mainly due to absence/presence of else statement
    if (getNext) {
        curToken = getParsToken();
    }
    int resIF;
    switch (curToken) {
        case LEX_IF:
            resIF = ifRule();
            if (resIF == FAIL) {
                return SYNTAX_ERROR;
            } else if (resIF == SUCCESS_ELSE) {
                return statementList(true);
            } else if (resIF == SUCCESS_NOELSE){
                return statementList(false);
            }
            break;
        case LEX_WHILE:
            if (!whileRule()) {
                return SYNTAX_ERROR;
            } else {
                return statementList(true);
            }
        case LEX_FUNID:
            return functionCall();
        case LEX_ID:
            if (VarAssign()) {
                return statementList(true);
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

// Global program
// can have function definitions f.e.
int ParserLoop(bool getNext) {
    if (getNext) {
        curToken = getParsToken();
    }
    int resIF;
    switch (curToken) {
        case LEX_ID:
            if (VarAssign() == false) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop(true);
            }
            break;
        case LEX_FUNKW:
            if (functionDeclaration() == false) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop(true);
            }
        case LEX_FUNID:
            if (!functionCall()) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop(true);
            }
        case LEX_IF:
            // cases for different outcomes of if - wheather if had or had not
            // else
            resIF = ifRule();
            if (resIF == FAIL) {
                return SYNTAX_ERROR;
            } else if (resIF == SUCCESS_ELSE) {
                return ParserLoop(true);
            } else {
                return ParserLoop(false);
            }
        case LEX_WHILE:
            if (!whileRule()) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop(true);
            }
        case LEX_ERR:
            return SYNTAX_ERROR;
        case LEX_EOF:
        case LEX_EPILOG:
            return SUCCESS;
        default:
            return SYNTAX_ERROR;
    }
    return SUCCESS;
}

int mainParser() {
    initParser();
    if (!checkProlog(&string)) {
        return endParser(SYNTAX_ERROR);
    }
    if (ParserLoop(true) == SUCCESS) {
        return endParser(SUCCESS);
    } else {
        return endParser(SYNTAX_ERROR);
    }
}