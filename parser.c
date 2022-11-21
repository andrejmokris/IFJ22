#include "parser.h"

#include "generator.h"
#include "print_inst.h"

String_t code;
Tinstruction_list list;
String_t string;
node_t globalSymTable;
node_t funcTable;
int curToken;
unsigned long labelCounter = 0;

unsigned long getLabel() { return labelCounter++; }

void initParser() {
    StringInit(&string);
    StringInit(&code);
    init_dll(&list);
    PRINT_CODE(write_text, ".IFJcode22");
    PRINT_CODE(write_text, "CALL NULLMAIN42069");
    PRINT_CODE(jump, "ENDENDENDEND");
    PRINT_CODE(label, "NULLMAIN42069");
    PRINT_CODE(tmpF, );
    PRINT_CODE(pushF, );
    globalSymTable = NULL;
    funcTable = NULL;
    curToken = 0;
}

int endParser(int errCode) {
    stringDeconstruct(&string);
    print_code();
    stringDeconstruct(&code);
    TreeDeconstruct(globalSymTable);
    TreeDeconstruct(funcTable);
    dispose_dll(&list);

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
bool VarAssign(node_t *symTable) {
    // printf("VarAssign\n");
    node_t newNode = TreeFind(*symTable, string.string);
    if (newNode == NULL) {
        if ((newNode = TreeInsert(symTable, 0, string)) == NULL) {
            endParser(INTERNAL_ERROR);
        }
        list_item stash;
        stash = list.active;
        if (list.before_while != NULL) {
            list.active = list.before_while;
            insert_before_active_dll(&list, list.string_pos);
            list.active = stash;
        } else if (list.before_if != NULL) {
            list.active = list.before_if;
            insert_before_active_dll(&list, list.string_pos);
            list.active = stash;
        } else {
            insert_after_active_dll(&list, list.string_pos);
        }
        new_var(string.string);
        list.string_pos = code.length;
        // TU VYTVARAME NOVU VARIABLE
        // AK SME VO WHILE, MUSIME TO VYTIAHNUT VONKU
        // MUSIME ZAISTIT DOBRY FRAME
        /*
        struct: param: topmost while (pointer)
        .IFJ22code22

        LABE
        MAIN:
            GLOBAL
            .
            .
            .
            .
        */
    }
    // printf("NEW NODE: %s\n", newNode->NodeID.string);
    if (getParsToken() != LEX_ASSIGN) {
        return false;
    }
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes = parseExpression(LEX_SEMICOL, &resDataType,
                                              *symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }

    PRINT_CODE(assign, newNode->NodeID.string);
    PRINT_CODE(clears, );
    // printf("POPS GF@%s\n", newNode->NodeID.string);
    // printf("CLEARS\n");
    newNode->dataType = resDataType;
    return true;
}

bool getSingleParam(node_t funcNode) {
    int newToken = getParsToken();
    if (newToken == LEX_RPAR) {
        return true;
    }
    if (newToken == LEX_COMMA) {
        if (!Type()) {
            endParser(SYNTAX_ERROR);
        }
        int dataType = curToken;
        curToken = getParsToken();
        if (curToken != LEX_ID) {
            endParser(SYNTAX_ERROR);
        }
        int addRes;
        if ((addRes = addParam(funcNode, dataType, string)) != SUCCESS) {
            endParser(addRes);
        }
        node_t newParam;
        if ((newParam = TreeInsert(&(funcNode->function->symTable), dataType,
                                   string)) == NULL) {
            endParser(INTERNAL_ERROR);
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
            // VYTVORENY PRVY PARAMETER
            int addRes;
            if ((addRes = addParam(funcNode, dataType, string)) != SUCCESS) {
                endParser(addRes);
            }
            node_t newParam;
            if ((newParam = TreeInsert(&(funcNode->function->symTable),
                                       dataType, string)) == NULL) {
                endParser(INTERNAL_ERROR);
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
    if (TreeFind(funcTable, string.string) != NULL ||
        isBuiltIn(NULL, &string)) {
        printf("Redefinition of a function\n");
        endParser(SEMANTIC_ERROR);
    }
    node_t funcNode = createFuncNode(LEX_FUNID, string);
    if (TreeInsertNode(&funcTable, funcNode) == NULL) {
        endParser(INTERNAL_ERROR);
    }
    active_first(&list);
    PRINT_CODE(label, string.string);
    PRINT_CODE(pushF, );
    // LABEL string.string
    // PUSHFRAME
    // LF
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
    // POPFRAME
    // priradenie vysledku
    // MOV LF@var TF@returnvar
    // ak to robis cez stack, tak na konci CLEA
    bool res = statementList(true, &(funcNode->function->symTable), funcNode);
    if (funcNode->function->returnType == LEX_VOID) {
        PRINT_CODE(push_null, );
        PRINT_CODE(write_text, "RETURN\n");
    }
    active_last(&list);
    return res;
}

// check if function call matches parameter data types
/* op1 - ocakavany dt   op2 - prichadzajuci dt */
bool parameterDataTypeVerify(int op1, int op2, node_t *symTable) {
    if (op2 == LEX_ID) {
        node_t findVar = TreeFind(symTable, string.string);
        if (findVar == NULL) {
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

bool readiBuiltIn(int *returnType) {
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    if (getParsToken() != LEX_RPAR) {
        endParser(SYNTAX_ERROR);
    }
    if (getParsToken() != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = LEX_INT;
    }

    list_item stash;
    stash = list.active;
    if (list.before_while != NULL) {
        list.active = list.before_while;
        insert_before_active_dll(&list, list.string_pos);
        list.active = stash;
    } else if (list.before_if != NULL) {
        list.active = list.before_if;
        insert_before_active_dll(&list, list.string_pos);
        list.active = stash;
    } else {
        insert_after_active_dll(&list, list.string_pos);
    }
    char str[9999];
    unsigned long labelN = getLabel();
    sprintf(str, "TMP%lu", labelN);
    new_var(str);
    list.string_pos = code.length;

    sprintf(str, "READ LF@TMP%lu int", labelN);
    PRINT_CODE(write_text, str);
    sprintf(str, "TMP%lu", labelN);
    PRINT_CODE(push_operand, str);
    return true;
}

bool writeBuiltIn(int *returnType) {
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = LEX_NULL;
    }
    // write($a, "fas", )
    int newToken;
    while ((newToken = getParsToken()) != LEX_RPAR) {
        // int newToken = getParsToken();
        if (newToken == LEX_STRING) {
            PRINT_CODE(write, string.string);
            // string je na string.string
        } else if (newToken == LEX_ID) {
            PRINT_CODE(write_var, string.string);
        }
        if (getParsToken() == LEX_RPAR) {
            break;
        }
    }
    if (getParsToken() != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    return true;
}

bool isBuiltIn(int *returnType, String_t *string) {
    if (!strcmp(string->string, "readi")) {
        // printf("tu som\n");
        if (readiBuiltIn(returnType)) {
            return true;
        }
    } else if (!strcmp(string->string, "write")) {
        // printf("tu som\n");
        if (writeBuiltIn(returnType)) {
            return true;
        }
    }
    if (!strcmp(string->string, "write") || !strcmp(string->string, "reads") ||
        !strcmp(string->string, "readi") || !strcmp(string->string, "readf") ||
        !strcmp(string->string, "floatval") ||
        !strcmp(string->string, "intval") ||
        !strcmp(string->string, "strval") ||
        !strcmp(string->string, "strlen") ||
        !strcmp(string->string, "substring") ||
        !strcmp(string->string, "ord") || !strcmp(string->string, "chr")) {
        while (curToken != LEX_SEMICOL) {
            getParsToken(string);
        }
        return true;
    }
    return false;
}

bool functionCall(String_t *fName, int *returnType, char scope, node_t *symTable) {
    // case for built-in function
    // TODO: add typechecking and codegen for built-in functions

    // CREATEFRAME - vytvori sa tempframe
    // priprava parametrov
    // call function
    if (isBuiltIn(returnType, fName == NULL ? &string : fName)) {
        return true;
    }
    PRINT_CODE(tmpF, );
    node_t funcNode;
    if (fName == NULL) {
        funcNode = TreeFind(funcTable, string.string);
    } else {
        funcNode = TreeFind(funcTable, fName->string);
    }
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }

    if (funcNode == NULL) {
        printf("Undefined function\n");
        endParser(SEMANTIC_ERROR);
    }
    int nOfParams = funcNode->function->nOfParams;
    // loading parameters and comparing data types
    for (int i = 0; i < nOfParams; i++) {
        getParsToken();
        // verify if expected datatype of parameter arrives
        if (!parameterDataTypeVerify(funcNode->function->params[i]->dataType, curToken, symTable)) {
            endParser(RUN_ERROR);
        } else {
            PRINT_CODE(new_varTF,
                       funcNode->function->params[i]->ParamID.string);
            // MOVE
            char str[99999];
            if (curToken == LEX_ID) {
                sprintf(str, "MOVE TF@%s LF@%s",
                        funcNode->function->params[i]->ParamID.string,
                        string.string);
                PRINT_CODE(write_text, str);
            } else if (curToken == LEX_INT) {
                sprintf(str, "MOVE TF@%s int@%s",
                        funcNode->function->params[i]->ParamID.string,
                        string.string);
                PRINT_CODE(write_text, str);
            }
            // PARAM HAS GOOD DATA TYPE, it can be pushed to stack or idk
            // printf("Good param\n");
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
    if (returnType != NULL) {
        *returnType = funcNode->function->returnType;
        if (funcNode->function->returnType == LEX_VOID) {
            *returnType = LEX_NULL;
        } else if (funcNode->function->returnType == LEX_TYPE_INT) {
            *returnType = LEX_INT;
        } else if (funcNode->function->returnType == LEX_TYPE_FLOAT) {
            *returnType = LEX_FLOAT;
        } else if (funcNode->function->returnType == LEX_TYPE_STRING) {
            *returnType = LEX_STRING;
        }
    }
    if (getParsToken() != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    PRINT_CODE(call, funcNode->NodeID.string);
    return true;
}

bool returnStat(node_t *symTable, node_t functionNode) {
    // push expression
    // return
    // popframe - mas ju na temp.frame
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes = parseExpression(LEX_SEMICOL, &resDataType,
                                              *symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    if (functionNode == NULL) {
        PRINT_CODE(popF, );
        PRINT_CODE(write_text, "RETURN\n");
        return true;
    }
    if (functionNode->function->returnType == LEX_VOID) {
        PRINT_CODE(push_null, );
    }
    PRINT_CODE(popF, );
    PRINT_CODE(write_text, "RETURN\n");
    int functionRetType = functionNode->function->returnType;
    if (functionRetType == resDataType) {
        if (functionNode->function->returnType == LEX_VOID) {
            PRINT_CODE(push_null, );
        }
        return true;
    }
    if (!parameterDataTypeVerify(functionRetType, resDataType, symTable)) {
        endParser(RETURN_ERROR);
    }
    if (functionNode->function->returnType == LEX_VOID) {
        PRINT_CODE(push_null, );
    }
    return true;
}

int ifRule(node_t *symTable, node_t functionNode) {
    bool wasDefined;
    if (list.before_if != NULL) {
        wasDefined = true;
    } else {
        wasDefined = false;
    }
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
        return FAIL;
    }
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, *symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    // datatype of IF condition has to be BOOL
    /*
    if (resDataType != LEX_BOOL) {
        printf("Invalid expression in IF statement\n");
        endParser(TYPECOMP_ERORR);
        return FAIL;
    }
    */
    unsigned long labelID = getLabel();
    char strElse[99999];
    sprintf(strElse, "IfElse%ld", labelID);
    char strEnd[99999];
    sprintf(strEnd, "IfEnd%ld", labelID);
    PRINT_CODE(push_bool, "true");
    if (list.before_if == NULL) {
        list.before_if = list.active;
    }
    PRINT_CODE(jumpIfNeqS, strElse);
    if (getParsToken() != LEX_LCRB) {
        printf("Missing { after IF statement\n");
        endParser(SYNTAX_ERROR);
        return FAIL;
    }
    if (!statementList(true, symTable, functionNode)) {
        return FAIL;
    }
    PRINT_CODE(jump, strEnd);
    PRINT_CODE(label, strElse);
    if (getParsToken() == LEX_ELSE) {
        // printf("WRITE string@inELSEStatement\n");
        if (getParsToken() != LEX_LCRB) {
            printf("Missing { after ELSE statement\n");
            endParser(SYNTAX_ERROR);
            return FAIL;
        }
        if (statementList(true, symTable, functionNode)) {
            PRINT_CODE(label, strEnd);
            if (!wasDefined) {
                list.before_if = NULL;
            }
            return SUCCESS_ELSE;
        }
    } else {
        PRINT_CODE(label, strEnd);
        if (!wasDefined) {
            list.before_if = NULL;
        }
        return SUCCESS_NOELSE;
    }
    return FAIL;
}

bool whileRule(node_t *symTable, node_t functionNode) {
    bool wasDefined;
    if (list.before_while != NULL) {
        wasDefined = true;
    } else {
        wasDefined = false;
    }
    if (getParsToken() != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
        return FAIL;
    }
    int resDataType;
    int parseExpressionRes;
    unsigned long labelID = getLabel();
    char strStart[99999];
    char strEnd[99999];
    sprintf(strStart, "WhileStart%ld", labelID);
    sprintf(strEnd, "WhileEnd%ld", labelID);
    PRINT_CODE(label, strStart);
    if (list.before_while == NULL) {
        list.before_while = list.active;
    }
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, *symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    PRINT_CODE(push_bool, "true");
    PRINT_CODE(jumpIfNeqS, strEnd);
    // PUSHS bool@true
    /*
    datatype of IF condition has to be BOOL
    if (resDataType != LEX_BOOL) {
        printf("Invalid expression in WHILE statement\n");
        endParser(TYPECOMP_ERORR);
        return FAIL;
    }
    */
    if (getParsToken() != LEX_LCRB) {
        printf("Missing { after WHILE statement\n");
        endParser(SYNTAX_ERROR);
        return FAIL;
    }

    bool res = statementList(true, symTable, functionNode);
    if (res != true) {
        endParser(SYNTAX_ERROR);
    }
    PRINT_CODE(jump, strStart);
    PRINT_CODE(label, strEnd);
    // create label whileStartLabelID
    // list.before_while = NULL;
    if (!wasDefined) {
        list.before_while = NULL;
    }
    return true;
    // GENERATE INSTRUCTION JUMP BACK ON WHILE START
}

// statement list
// similiar to parser loop, but can't have function definition
// is called in if_statement or function body
bool statementList(bool getNext, node_t *symTable, node_t functionNode) {
    // find if loading next token is necessary
    // mainly due to absence/presence of else statement
    if (getNext) {
        curToken = getParsToken();
    }
    int resIF;
    switch (curToken) {
        case LEX_IF:
            resIF = ifRule(symTable, functionNode);
            if (resIF == FAIL) {
                return SYNTAX_ERROR;
            } else if (resIF == SUCCESS_ELSE) {
                return statementList(true, symTable, functionNode);
            } else if (resIF == SUCCESS_NOELSE) {
                return statementList(false, symTable, functionNode);
            }
            break;
        case LEX_WHILE:
            if (!whileRule(symTable, functionNode)) {
                return SYNTAX_ERROR;
            } else {
                return statementList(true, symTable, functionNode);
            }
            break;
        case LEX_FUNID:
            return functionCall(NULL, NULL, functionNode == NULL ? 'g' : 'a', symTable);
        case LEX_ID:
            if (VarAssign(symTable)) {
                return statementList(true, symTable, functionNode);
            } else {
                endParser(SYNTAX_ERROR);
            }
            break;
        case LEX_RETURN:
            if (returnStat(symTable, functionNode)) {
                return statementList(true, symTable, functionNode);
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
    // help variable to determine, if we need to load next token after if
    // statement list
    int resIF;
    int resDataType;
    int parseExpressionRes;
    switch (curToken) {
        case LEX_ID:
            if (VarAssign(&globalSymTable) == false) {
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
            if (!functionCall(NULL, NULL, 'g', &globalSymTable)) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop(true);
            }
        case LEX_IF:
            // cases for different outcomes of if - wheather if had or had not
            // else
            resIF = ifRule(&globalSymTable, NULL);
            if (resIF == FAIL) {
                return SYNTAX_ERROR;
            } else if (resIF == SUCCESS_ELSE) {
                return ParserLoop(true);
            } else {
                return ParserLoop(false);
            }
        case LEX_WHILE:
            if (!whileRule(&globalSymTable, NULL)) {
                return SYNTAX_ERROR;
            } else {
                return ParserLoop(true);
            }
        case LEX_RETURN:
            if (returnStat(&globalSymTable, NULL)) {
                return ParserLoop(true);
            } else {
                endParser(SYNTAX_ERROR);
            }
            break;
        case LEX_ERR:
            return SYNTAX_ERROR;
        case LEX_EOF:
        case LEX_EPILOG:
            return SUCCESS;
        default:
            if ((parseExpressionRes = parseExpression(
                     LEX_SEMICOL, &resDataType, globalSymTable)) != SUCCESS) {
                endParser(parseExpressionRes);
            }
            if (parseExpressionRes == SUCCESS) {
                return ParserLoop(true);
            } else {
                return SYNTAX_ERROR;
            }
    }
    return SUCCESS;
}

int mainParser() {
    initParser();
    if (!checkProlog(&string)) {
        return endParser(SYNTAX_ERROR);
    }
    // PRINT HEADER FOR GENERATED FILE
    // printf(".IFJcode22\n");
    if (ParserLoop(true) == SUCCESS) {
        PRINT_CODE(label, "ENDENDENDEND");
        return endParser(SUCCESS);
    } else {
        return endParser(SYNTAX_ERROR);
    }
}