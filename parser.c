#include "parser.h"

#include "generator.h"
#include "print_inst.h"

String_t code;
Tinstruction_list list;
String_t string;
node_t globalSymTable;
node_t funcTable;
int curToken;
CodeStore_t store;

unsigned long getLabel() {
    static unsigned long labelCounter = 0;
    return labelCounter++;
}

void initParser() {
    StringInit(&string);
    StringInit(&code);
    init_dll(&list);
    store = initCodeStore(25);
    if (store == NULL) {
        endParser(INTERNAL_ERROR);
    }
    PRINT_CODE(write_text, ".IFJcode22");
    PRINT_CODE(write_text, "CALL NULLMAIN42069");
    PRINT_CODE(jump, "ENDENDENDEND");
    PRINT_CODE(write_text, "");
    PRINT_SUBSTRING();
    PRINT_ORD();
    PRINT_STRLEN();
    PRINT_CHR();
    PRINT_FLOATVAL();
    PRINT_INTVAL();
    PRINT_STRVAL();
    PRINT_CODE(label, "NULLMAIN42069");
    PRINT_CODE(tmpF, );
    PRINT_CODE(pushF, );
    PRINT_CODE(new_var, "$optype");
    globalSymTable = NULL;
    funcTable = NULL;
    curToken = 0;
}

int endParser(int errCode) {
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
    }
    return curToken;
}

int getTokenfromStore(String_t *destString) {
    token_t newToken = store->items[store->lastPrinted];
    curToken = newToken->lexID;
    if (destString == NULL) {
        string = newToken->tokenVal;
    } else {
        *destString = newToken->tokenVal;
    }
    store->lastPrinted++;
    return curToken;
}

bool Type() {
    curToken = getTokenfromStore(NULL);
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
    int resDataType;
    int parseExpressionRes;
    node_t newNode = TreeFind(*symTable, string.string);

    int token = getTokenfromStore(NULL);
    if (token != LEX_ASSIGN) {
        store->lastPrinted -= 2;
        if ((parseExpressionRes = parseExpression(LEX_SEMICOL, &resDataType,
                                                  symTable)) != SUCCESS) {
            endParser(parseExpressionRes);
        } else {
            return true;
        }
    }

    store->lastPrinted -= 2;
    getTokenfromStore(NULL);
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
    }

    getTokenfromStore(NULL);

    if ((parseExpressionRes =
             parseExpression(LEX_SEMICOL, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    PRINT_CODE(assign, newNode->NodeID.string);
    PRINT_CODE(clears, );
    newNode->dataType = resDataType;
    return true;
}

bool getSingleParam(node_t funcNode) {
    int newToken = getTokenfromStore(NULL);
    if (newToken == LEX_RPAR) {
        return true;
    }
    if (newToken == LEX_COMMA) {
        if (!Type()) {
            endParser(SYNTAX_ERROR);
        }
        int dataType = curToken;
        curToken = getTokenfromStore(NULL);
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
        int nextToken = getTokenfromStore(NULL);
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

bool functionDeclarationTopG() {
    // get function id
    curToken = getTokenfromStore(NULL);
    if (curToken != LEX_FUNID) {
        endParser(SYNTAX_ERROR);
    }
    if (TreeFind(funcTable, string.string) != NULL || isBuiltIn(&string)) {
        printf("Redefinition of a function\n");
        endParser(SEMANTIC_ERROR);
    }
    node_t funcNode = createFuncNode(LEX_FUNID, string);
    if (TreeInsertNode(&funcTable, funcNode) == NULL) {
        endParser(INTERNAL_ERROR);
    }
    funcNode->function->hasReturn = false;
    funcNode->function->nOfParams = 0;
    // function ID (
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    if (!getParams(funcNode)) {
        return false;
    }
    // function ID (parList) : return type { stat_list
    if (getTokenfromStore(NULL) != LEX_COLON) {
        endParser(SYNTAX_ERROR);
    }
    // return type
    if (!Type()) {
        endParser(SYNTAX_ERROR);
    }
    funcNode->function->returnType = curToken;

    if (getTokenfromStore(NULL) != LEX_LCRB) {
        endParser(SYNTAX_ERROR);
    }
    return true;
}

bool functionDeclaration() {
    // get function id
    curToken = getTokenfromStore(NULL);
    if (curToken != LEX_FUNID) {
        endParser(SYNTAX_ERROR);
    }
    node_t funcNode = TreeFind(funcTable, string.string);

    active_first(&list);
    PRINT_CODE(label, string.string);
    PRINT_CODE(pushF, );
    PRINT_CODE(new_var, "$optype");
    // function ID (
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    while (getTokenfromStore(NULL) != LEX_LCRB)
        ;

    bool res = statementList(true, &(funcNode->function->symTable), funcNode);
    if (funcNode->function->returnType == LEX_VOID) {
        PRINT_CODE(push_null, );
        PRINT_CODE(popF, );
        PRINT_CODE(write_text, "RETURN\n");
    } else {
        if (funcNode->function->hasReturn == false) {
            printf("Missing Return in function \"%s\"\n",
                   funcNode->NodeID.string);
            endParser(RUN_ERROR);
        }
    }
    PRINT_CODE(write_text, "EXIT int@4");
    PRINT_CODE(write_text, "");
    active_last(&list);
    return res;
}

bool readBuiltIn(int *returnType, int type) {
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    if (getTokenfromStore(NULL) != LEX_RPAR) {
        endParser(SYNTAX_ERROR);
    }
    if (getTokenfromStore(NULL) != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = type;
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
    PRINT_CODE(tmpF, );
    PRINT_CODE(new_varTF, "a");
    if (type == LEX_STRING) {
        PRINT_CODE(write_text, "READ TF@a string");
    } else if (type == LEX_INT) {
        PRINT_CODE(write_text, "READ TF@a int");
    } else if (type == LEX_FLOAT) {
        PRINT_CODE(write_text, "READ TF@a float");
    }
    PRINT_CODE(push_operandTF, "a");
    return true;
}

bool writeBuiltInSingleParam(node_t *symTable) {
    int newToken = getTokenfromStore(NULL);
    if (newToken == LEX_RPAR) {
        return true;
    } else if (newToken == LEX_COMMA) {
        newToken = getTokenfromStore(NULL);
        if (newToken == LEX_ID) {
            node_t id_node = TreeFind(*symTable, string.string);
            if (id_node == NULL) {
                printf("Undefined var in Write function\n");
                endParser(UNDEFVAR_ERROR);
            }
            char initialized[9999];
            char typeinstruction[9999];
            unsigned long labelID = getLabel();
            sprintf(initialized, "checkinitializedgood%lu", labelID);
            sprintf(typeinstruction, "TYPE LF@$optype LF@%s", string.string);
            // check initialization of var
            PRINT_CODE(write_text, typeinstruction);
            PRINT_CODE(write_text, "PUSHS LF@$optype");
            PRINT_CODE(push_string, "");
            PRINT_CODE(jumpIfNeqS, initialized);
            PRINT_CODE(write_text, "EXIT int@5");
            PRINT_CODE(label, initialized);
            PRINT_CODE(push_operand, string.string);
            PRINT_CODE(write_var, string.string);
            return writeBuiltInSingleParam(symTable);
        } else if (newToken == LEX_FLOAT) {
            char str[9999];
            char *ptr;
            double ret;
            ret = strtod(string.string, &ptr);
            sprintf(str, "WRITE float@%a", ret);
            PRINT_CODE(write_text, str);
            return writeBuiltInSingleParam(symTable);
        } else if (newToken == LEX_INT || newToken == LEX_STRING) {
            PRINT_CODE(write, string.string);
            return writeBuiltInSingleParam(symTable);
        } else {
            endParser(SYNTAX_ERROR);
        }
    } else {
        endParser(SYNTAX_ERROR);
    }
    return false;
}

bool writeBuiltInParam(node_t *symTable) {
    int newToken = getTokenfromStore(NULL);
    if (newToken == LEX_RPAR) {
        return true;
    } else if (newToken == LEX_ID) {
        node_t id_node = TreeFind(*symTable, string.string);
        if (id_node == NULL) {
            printf("Undefined function in Write function\n");
            endParser(UNDEFVAR_ERROR);
        }
        char initialized[9999];
        char typeinstruction[9999];
        unsigned long labelID = getLabel();
        sprintf(initialized, "checkinitializedgood%lu", labelID);
        sprintf(typeinstruction, "TYPE LF@$optype LF@%s", string.string);
        // check initialization of var
        PRINT_CODE(write_text, typeinstruction);
        PRINT_CODE(write_text, "PUSHS LF@$optype");
        PRINT_CODE(push_string, "");
        PRINT_CODE(jumpIfNeqS, initialized);
        PRINT_CODE(write_text, "EXIT int@5");
        PRINT_CODE(label, initialized);
        PRINT_CODE(push_operand, string.string);
        PRINT_CODE(write_var, string.string);
    } else if (newToken == LEX_FLOAT) {
        char str[9999];
        char *ptr;
        double ret;
        ret = strtod(string.string, &ptr);
        sprintf(str, "WRITE float@%a", ret);
        PRINT_CODE(write_text, str);
    } else if (newToken == LEX_INT || newToken == LEX_STRING) {
        PRINT_CODE(write, string.string);
        return writeBuiltInSingleParam(symTable);
    } else {
        endParser(SYNTAX_ERROR);
    }
    return writeBuiltInSingleParam(symTable);
}

bool writeBuiltIn(int *returnType, node_t *symTable) {
    // printf("WRITE CALL\n");
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = LEX_NULL;
    }
    if (writeBuiltInParam(symTable) != true) {
        endParser(SYNTAX_ERROR);
    }
    if (getTokenfromStore(NULL) != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    return true;
}

bool strLenBuiltIn(int *returnType, node_t *symTable) {
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    // get param
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }

    PRINT_CODE(tmpF, );
    PRINT_CODE(new_varTF, "string");
    PRINT_CODE(assignTF, "string");
    PRINT_CODE(call, "strlen");

    if (getTokenfromStore(NULL) != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = LEX_INT;
    }
    return true;
}

bool ordBuiltIn(int *returnType, node_t *symTable) {
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    // get param
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    PRINT_CODE(tmpF, );
    PRINT_CODE(new_varTF, "string");
    PRINT_CODE(assignTF, "string");
    PRINT_CODE(call, "ord");

    if (getTokenfromStore(NULL) != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = LEX_INT;
    }
    return true;
}

bool floatValBuiltIn(int *returnType, node_t *symTable) {
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    // get param
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    };
    PRINT_CODE(tmpF, );
    PRINT_CODE(new_varTF, "val2convert");
    PRINT_CODE(assignTF, "val2convert");
    PRINT_CODE(call, "floatval");
    if (getTokenfromStore(NULL) != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = LEX_FLOAT;
    }
    return true;
}

bool intValBuiltIn(int *returnType, node_t *symTable) {
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    // get param
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    };
    PRINT_CODE(tmpF, );
    PRINT_CODE(new_varTF, "val2convert");
    PRINT_CODE(assignTF, "val2convert");
    PRINT_CODE(call, "intval");
    if (getTokenfromStore(NULL) != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = LEX_INT;
    }
    return true;
}

bool strValBuiltIn(int *returnType, node_t *symTable) {
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    // get param
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    };
    PRINT_CODE(tmpF, );
    PRINT_CODE(new_varTF, "val2convert");
    PRINT_CODE(assignTF, "val2convert");
    PRINT_CODE(call, "strval");
    if (getTokenfromStore(NULL) != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = LEX_STRING;
    }
    return true;
}

bool chrBuiltIn(int *returnType, node_t *symTable) {
    // chr(int $i)
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    // get param
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    PRINT_CODE(tmpF, );
    PRINT_CODE(new_varTF, "i");
    PRINT_CODE(assignTF, "i");
    PRINT_CODE(call, "chr");

    if (getTokenfromStore(NULL) != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = LEX_INT;
    }
    return true;
}

bool substringBuiltIn(int *returnType, node_t *symTable) {
    // chr(int $i)
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }
    // get param
    int resDataType;
    int parseExpressionRes;
    PRINT_CODE(tmpF, );
    PRINT_CODE(new_varTF, "i");
    PRINT_CODE(new_varTF, "j");
    PRINT_CODE(new_varTF, "s");

    if ((parseExpressionRes =
             parseExpression(LEX_COMMA, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }

    PRINT_CODE(assignTF, "s");

    if ((parseExpressionRes =
             parseExpression(LEX_COMMA, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }

    PRINT_CODE(assignTF, "i");

    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }

    PRINT_CODE(assignTF, "j");

    PRINT_CODE(call, "substring");

    if (getTokenfromStore(NULL) != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    if (returnType != NULL) {
        *returnType = LEX_STRING;
    }
    return true;
}

bool isBuiltIn(String_t *string) {
    if (!strcmp(string->string, "write") || !strcmp(string->string, "reads") ||
        !strcmp(string->string, "readi") || !strcmp(string->string, "readf") ||
        !strcmp(string->string, "floatval") ||
        !strcmp(string->string, "intval") ||
        !strcmp(string->string, "strval") ||
        !strcmp(string->string, "strlen") ||
        !strcmp(string->string, "substring") ||
        !strcmp(string->string, "ord") || !strcmp(string->string, "chr")) {
        return true;
    }
    return false;
}

bool callBuiltIn(int *returnType, String_t *string, node_t *symTable) {
    if (!strcmp(string->string, "readi")) {
        if (readBuiltIn(returnType, LEX_INT)) {
            return true;
        }
    } else if (!strcmp(string->string, "reads")) {
        if (readBuiltIn(returnType, LEX_STRING) == true) {
            return true;
        }
    } else if (!strcmp(string->string, "readf")) {
        if (readBuiltIn(returnType, LEX_FLOAT) == true) {
            return true;
        }
    } else if (!strcmp(string->string, "strlen")) {
        // printf("calling strlen\n");
        if (strLenBuiltIn(returnType, symTable) == true) {
            return true;
        }
    } else if (!strcmp(string->string, "write")) {
        if (writeBuiltIn(returnType, symTable) == true) {
            return true;
        }
    } else if (!strcmp(string->string, "ord")) {
        if (ordBuiltIn(returnType, symTable) == true) {
            return true;
        }
        // bool strLenBuiltIn(int *returnType, node_t *symTable)
    } else if (!strcmp(string->string, "chr")) {
        if (chrBuiltIn(returnType, symTable) == true) {
            return true;
        }
    } else if (!strcmp(string->string, "substring")) {
        if (substringBuiltIn(returnType, symTable) == true) {
            return true;
        }
    } else if (!strcmp(string->string, "floatval")) {
        if (floatValBuiltIn(returnType, symTable) == true) {
            return true;
        }
    } else if (!strcmp(string->string, "intval")) {
        if (intValBuiltIn(returnType, symTable) == true) {
            return true;
        }
    } else if (!strcmp(string->string, "strval")) {
        if (strValBuiltIn(returnType, symTable) == true) {
            return true;
        }
    }
    return false;
}

bool functionCall(String_t *fName, int *returnType, char scope,
                  node_t *symTable) {
    // CREATEFRAME - create a temporary frame
    // prepare params for function call
    // call function
    if (isBuiltIn(fName == NULL ? &string : fName)) {
        if (callBuiltIn(returnType, fName == NULL ? &string : fName,
                        symTable)) {
            return true;
        } else {
            endParser(SYNTAX_ERROR);
        }
    }
    PRINT_CODE(tmpF, );
    PRINT_CODE(new_varTF, "$tmpvar");
    node_t funcNode;
    if (fName == NULL) {
        funcNode = TreeFind(funcTable, string.string);
    } else {
        funcNode = TreeFind(funcTable, fName->string);
    }
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
    }

    if (funcNode == NULL) {
        printf("Undefined function\n");
        endParser(SEMANTIC_ERROR);
    }
    int nOfParams = funcNode->function->nOfParams;
    // loading parameters and comparing data types

    for (int i = 0; i < nOfParams; i++) {
        int endChar = (i == nOfParams - 1) ? LEX_RPAR : LEX_COMMA;

        int nextToken = getTokenfromStore(NULL);

        if (nextToken == LEX_ID) {
            // check for existence of var in the symtable
            node_t checkExistence = TreeFind(*symTable, string.string);
            if (checkExistence == NULL) {
                endParser(UNDEFVAR_ERROR);
            }
            PRINT_CODE(push_operand, string.string);
            char str[9999];
            unsigned long labelID = getLabel();
            int expected_type = funcNode->function->params[i]->dataType;
            if (expected_type == LEX_TYPE_STRING) {
                sprintf(str, "TYPE TF@$tmpvar LF@%s", string.string);
                PRINT_CODE(write_text, str);
                PRINT_CODE(push_operandTF, "$tmpvar");
                PRINT_CODE(push_string, "string");
                sprintf(str, "FAIL%lu", labelID);
                PRINT_CODE(jumpIfEqS, str);
                PRINT_CODE(write_text, "EXIT int@4");
                PRINT_CODE(label, str);
            } else if (expected_type == LEX_TYPE_INT) {
                sprintf(str, "TYPE TF@$tmpvar LF@%s", string.string);
                PRINT_CODE(write_text, str);
                PRINT_CODE(push_operandTF, "$tmpvar");
                PRINT_CODE(push_string, "int");
                sprintf(str, "FAIL%lu", labelID);
                PRINT_CODE(jumpIfEqS, str);
                PRINT_CODE(write_text, "EXIT int@4");
                PRINT_CODE(label, str);
            } else if (expected_type == LEX_TYPE_FLOAT) {
                sprintf(str, "TYPE TF@$tmpvar LF@%s", string.string);
                PRINT_CODE(write_text, str);
                PRINT_CODE(push_operandTF, "$tmpvar");
                PRINT_CODE(push_string, "float");
                sprintf(str, "FAIL%lu", labelID);
                PRINT_CODE(jumpIfEqS, str);
                PRINT_CODE(write_text, "EXIT int@4");
                PRINT_CODE(label, str);
            } else if (expected_type == LEX_TYPE_STRING_OPT) {
                sprintf(str, "TYPE TF@$tmpvar LF@%s", string.string);
                PRINT_CODE(write_text, str);
                PRINT_CODE(push_operandTF, "$tmpvar");
                PRINT_CODE(push_string, "string");
                PRINT_CODE(put_OPERATOR, LEX_EQ);
                PRINT_CODE(push_operandTF, "$tmpvar");
                PRINT_CODE(push_string, "nil");
                PRINT_CODE(put_OPERATOR, LEX_EQ);
                PRINT_CODE(put_OPERATOR, 69);
                PRINT_CODE(push_bool, "true");
                sprintf(str, "FAIL%lu", labelID);
                PRINT_CODE(jumpIfEqS, str);
                PRINT_CODE(write_text, "EXIT int@4");
                PRINT_CODE(label, str);
            } else if (expected_type == LEX_TYPE_INT_OPT) {
                sprintf(str, "TYPE TF@$tmpvar LF@%s", string.string);
                PRINT_CODE(write_text, str);
                PRINT_CODE(push_operandTF, "$tmpvar");
                PRINT_CODE(push_string, "int");
                PRINT_CODE(put_OPERATOR, LEX_EQ);
                PRINT_CODE(push_operandTF, "$tmpvar");
                PRINT_CODE(push_string, "nil");
                PRINT_CODE(put_OPERATOR, LEX_EQ);
                PRINT_CODE(put_OPERATOR, 69);
                PRINT_CODE(push_bool, "true");
                sprintf(str, "FAIL%lu", labelID);
                PRINT_CODE(jumpIfEqS, str);
                PRINT_CODE(write_text, "EXIT int@4");
                PRINT_CODE(label, str);
            } else if (expected_type == LEX_TYPE_FLOAT_OPT) {
                sprintf(str, "TYPE TF@$tmpvar LF@%s", string.string);
                PRINT_CODE(write_text, str);
                PRINT_CODE(push_operandTF, "$tmpvar");
                PRINT_CODE(push_string, "float");
                PRINT_CODE(put_OPERATOR, LEX_EQ);
                PRINT_CODE(push_operandTF, "$tmpvar");
                PRINT_CODE(push_string, "nil");
                PRINT_CODE(put_OPERATOR, LEX_EQ);
                PRINT_CODE(put_OPERATOR, 69);
                PRINT_CODE(push_bool, "true");
                sprintf(str, "FAIL%lu", labelID);
                PRINT_CODE(jumpIfEqS, str);
                PRINT_CODE(write_text, "EXIT int@4");
                PRINT_CODE(label, str);
            }
        } else if (nextToken == LEX_INT) {
            if (funcNode->function->params[i]->dataType == LEX_TYPE_INT ||
                funcNode->function->params[i]->dataType == LEX_TYPE_INT_OPT) {
                PRINT_CODE(push_int, string.string);
            } else {
                endParser(RUN_ERROR);
            }
        } else if (nextToken == LEX_STRING) {
            if (funcNode->function->params[i]->dataType == LEX_TYPE_STRING ||
                funcNode->function->params[i]->dataType ==
                    LEX_TYPE_STRING_OPT) {
                PRINT_CODE(push_string, string.string);
            } else {
                endParser(RUN_ERROR);
            }
        } else if (nextToken == LEX_FLOAT) {
            if (funcNode->function->params[i]->dataType == LEX_TYPE_FLOAT ||
                funcNode->function->params[i]->dataType == LEX_TYPE_FLOAT_OPT) {
                char str[99999];
                char *ptr;
                double ret;
                ret = strtod(string.string, &ptr);
                sprintf(str, "%a", ret);
                PRINT_CODE(push_float, str);  // float val
            } else {
                endParser(RUN_ERROR);
            }
        } else if (nextToken == LEX_NULL) {
            int type = funcNode->function->params[i]->dataType;
            if (type == LEX_TYPE_STRING_OPT || type == LEX_TYPE_FLOAT_OPT ||
                type == LEX_TYPE_INT_OPT) {
                PRINT_CODE(write_text, "PUSHS nil@nil");
            } else {
                endParser(RUN_ERROR);
            }
        }

        PRINT_CODE(new_varTF, funcNode->function->params[i]->ParamID.string);
        // MOVE
        char str[99999];
        sprintf(str, "POPS TF@%s",
                funcNode->function->params[i]->ParamID.string);
        PRINT_CODE(write_text, str);

        if (getTokenfromStore(NULL) != endChar) {
            endParser(RUN_ERROR);
        }
    }
    if (nOfParams == 0) {
        if (getTokenfromStore(NULL) != LEX_RPAR) {
            return false;
        }
    }
    if (returnType != NULL) {
        *returnType = funcNode->function->returnType;
        if (funcNode->function->returnType == LEX_VOID) {
            *returnType = LEX_NULL;
        } else if (funcNode->function->returnType == LEX_TYPE_INT ||
                   funcNode->function->returnType == LEX_TYPE_INT_OPT) {
            *returnType = LEX_INT;
        } else if (funcNode->function->returnType == LEX_TYPE_FLOAT ||
                   funcNode->function->returnType == LEX_TYPE_FLOAT_OPT) {
            *returnType = LEX_FLOAT;
        } else if (funcNode->function->returnType == LEX_TYPE_STRING ||
                   funcNode->function->returnType == LEX_TYPE_STRING_OPT) {
            *returnType = LEX_STRING;
        }
    }
    if (getTokenfromStore(NULL) != LEX_SEMICOL) {
        endParser(SYNTAX_ERROR);
    }
    PRINT_CODE(call, funcNode->NodeID.string);
    return true;
}

bool returnStat(node_t *symTable, node_t functionNode) {
    // push expression
    // return
    // popframe - return val on temp frame
    if (functionNode != NULL) {
        functionNode->function->hasReturn = true;
    }

    if (functionNode != NULL &&
        functionNode->function->returnType == LEX_VOID) {
        if (getTokenfromStore(NULL) != LEX_SEMICOL) {
            // to finish
            endParser(RETURN_ERROR);
        }
        PRINT_CODE(push_null, );
        PRINT_CODE(popF, );
        PRINT_CODE(write_text, "RETURN\n");
        return true;
    }

    if (functionNode != NULL) {
        int retType = functionNode->function->returnType;
        if (retType == LEX_TYPE_FLOAT || retType == LEX_TYPE_INT ||
            retType == LEX_TYPE_STRING) {
            if (getTokenfromStore(NULL) == LEX_SEMICOL) {
                endParser(RETURN_ERROR);
            }
            store->lastPrinted--;
        }
    }
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_SEMICOL, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    if (functionNode == NULL) {
        PRINT_CODE(popF, );
        PRINT_CODE(write_text, "RETURN\n");
        return true;
    }

    // space for return type check
    char str[9999];
    unsigned long labelID = getLabel();
    int expected_type = functionNode->function->returnType;
    // PRINT_CODE(write_text, "PUSHFRAME");
    PRINT_CODE(write_text, "CREATEFRAME");
    PRINT_CODE(write_text, "DEFVAR TF@$expRes");
    PRINT_CODE(write_text, "DEFVAR TF@$tmpvar");
    PRINT_CODE(write_text, "POPS TF@$expRes");
    if (expected_type == LEX_TYPE_STRING) {
        PRINT_CODE(write_text, "TYPE TF@$tmpvar TF@$expRes");
        PRINT_CODE(push_operandTF, "$tmpvar");
        PRINT_CODE(push_string, "string");
        sprintf(str, "FAIL%lu", labelID);
        PRINT_CODE(jumpIfEqS, str);
        PRINT_CODE(write_text, "EXIT int@4");
        PRINT_CODE(label, str);
    } else if (expected_type == LEX_TYPE_INT) {
        PRINT_CODE(write_text, "TYPE TF@$tmpvar TF@$expRes");
        PRINT_CODE(push_operandTF, "$tmpvar");
        PRINT_CODE(push_string, "int");
        sprintf(str, "FAIL%lu", labelID);
        PRINT_CODE(jumpIfEqS, str);
        PRINT_CODE(write_text, "EXIT int@4");
        PRINT_CODE(label, str);
    } else if (expected_type == LEX_TYPE_FLOAT) {
        PRINT_CODE(write_text, "TYPE TF@$tmpvar TF@$expRes");
        PRINT_CODE(push_operandTF, "$tmpvar");
        PRINT_CODE(push_string, "float");
        sprintf(str, "FAIL%lu", labelID);
        PRINT_CODE(jumpIfEqS, str);
        PRINT_CODE(write_text, "EXIT int@4");
        PRINT_CODE(label, str);
    } else if (expected_type == LEX_TYPE_STRING_OPT) {
        PRINT_CODE(write_text, "TYPE TF@$tmpvar TF@$expRes");
        PRINT_CODE(push_operandTF, "$tmpvar");
        PRINT_CODE(push_string, "string");
        PRINT_CODE(put_OPERATOR, LEX_EQ);
        PRINT_CODE(push_operandTF, "$tmpvar");
        PRINT_CODE(push_string, "nil");
        PRINT_CODE(put_OPERATOR, LEX_EQ);
        PRINT_CODE(put_OPERATOR, 69);
        PRINT_CODE(push_bool, "true");
        sprintf(str, "FAIL%lu", labelID);
        PRINT_CODE(jumpIfEqS, str);
        PRINT_CODE(write_text, "EXIT int@4");
        PRINT_CODE(label, str);
    } else if (expected_type == LEX_TYPE_INT_OPT) {
        PRINT_CODE(write_text, "TYPE TF@$tmpvar TF@$expRes");
        PRINT_CODE(push_operandTF, "$tmpvar");
        PRINT_CODE(push_string, "int");
        PRINT_CODE(put_OPERATOR, LEX_EQ);
        PRINT_CODE(push_operandTF, "$tmpvar");
        PRINT_CODE(push_string, "nil");
        PRINT_CODE(put_OPERATOR, LEX_EQ);
        PRINT_CODE(put_OPERATOR, 69);
        PRINT_CODE(push_bool, "true");
        sprintf(str, "FAIL%lu", labelID);
        PRINT_CODE(jumpIfEqS, str);
        PRINT_CODE(write_text, "EXIT int@4");
        PRINT_CODE(label, str);
    } else if (expected_type == LEX_TYPE_FLOAT_OPT) {
        PRINT_CODE(write_text, "TYPE TF@$tmpvar TF@$expRes");
        PRINT_CODE(push_operandTF, "$tmpvar");
        PRINT_CODE(push_string, "float");
        PRINT_CODE(put_OPERATOR, LEX_EQ);
        PRINT_CODE(push_operandTF, "$tmpvar");
        PRINT_CODE(push_string, "nil");
        PRINT_CODE(put_OPERATOR, LEX_EQ);
        PRINT_CODE(put_OPERATOR, 69);
        PRINT_CODE(push_bool, "true");
        sprintf(str, "FAIL%lu", labelID);
        PRINT_CODE(jumpIfEqS, str);
        PRINT_CODE(write_text, "EXIT int@4");
        PRINT_CODE(label, str);
    }
    PRINT_CODE(write_text, "PUSHS TF@$expRes");
    // PRINT_CODE(popF, );
    PRINT_CODE(popF, );
    PRINT_CODE(write_text, "RETURN\n");
    int functionRetType = functionNode->function->returnType;
    if (functionRetType == resDataType) {
        if (functionNode->function->returnType == LEX_VOID) {
            PRINT_CODE(popF, );
            PRINT_CODE(push_null, );
        }
        return true;
    }

    if (functionNode->function->returnType == LEX_VOID) {
        PRINT_CODE(popF, );
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
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
        return FAIL;
    }
    int resDataType;
    int parseExpressionRes;
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    unsigned long labelID = getLabel();
    char strElse[99999];
    sprintf(strElse, "$IfElse%ld", labelID);
    char strEnd[99999];
    sprintf(strEnd, "$IfEnd%ld", labelID);
    CONDITION(labelID);
    PRINT_CODE(push_bool, "true");
    if (list.before_while == NULL && list.before_if == NULL) {
        list.before_if = list.active;
    }
    PRINT_CODE(jumpIfNeqS, strElse);
    if (getTokenfromStore(NULL) != LEX_LCRB) {
        printf("Missing { after IF statement\n");
        endParser(SYNTAX_ERROR);
        return FAIL;
    }
    if (!statementList(true, symTable, functionNode)) {
        return FAIL;
    }
    PRINT_CODE(jump, strEnd);
    PRINT_CODE(label, strElse);
    if (getTokenfromStore(NULL) == LEX_ELSE) {
        if (getTokenfromStore(NULL) != LEX_LCRB) {
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
    if (getTokenfromStore(NULL) != LEX_LPAR) {
        endParser(SYNTAX_ERROR);
        return FAIL;
    }
    int resDataType;
    int parseExpressionRes;
    unsigned long labelID = getLabel();
    char strStart[99999];
    char strEnd[99999];
    sprintf(strStart, "$WhileStart%ld", labelID);
    sprintf(strEnd, "$WhileEnd%ld", labelID);
    PRINT_CODE(label, strStart);
    if (list.before_while == NULL && list.before_if == NULL) {
        list.before_while = list.active;
    }
    if ((parseExpressionRes =
             parseExpression(LEX_RPAR, &resDataType, symTable)) != SUCCESS) {
        endParser(parseExpressionRes);
    }
    CONDITION(labelID);
    PRINT_CODE(push_bool, "true");
    PRINT_CODE(jumpIfNeqS, strEnd);

    if (getTokenfromStore(NULL) != LEX_LCRB) {
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
    if (!wasDefined) {
        list.before_while = NULL;
    }
    return true;
}

// statement list
// similiar to parser loop, but can't have function definition
// is called in if/while or function body
bool statementList(bool getNext, node_t *symTable, node_t functionNode) {
    // find if loading next token is necessary
    // mainly due to absence/presence of else statement
    int resDataType;
    int parseExpressionRes;
    if (getNext) {
        curToken = getTokenfromStore(NULL);
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
            if (!functionCall(NULL, NULL, functionNode == NULL ? 'g' : 'a',
                              symTable)) {
                return SYNTAX_ERROR;
            } else {
                return statementList(true, symTable, functionNode);
            }
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
            if (curToken == LEX_SEMICOL) {
                endParser(SYNTAX_ERROR);
            }
            store->lastPrinted--;
            if ((parseExpressionRes = parseExpression(
                     LEX_SEMICOL, &resDataType, &globalSymTable)) != SUCCESS) {
                endParser(parseExpressionRes);
            }
            return statementList(true, symTable, functionNode);
    }
    return false;
}

// Global program
// can have function definitions f.e.
int ParserLoop(bool getNext) {
    if (getNext) {
        curToken = getTokenfromStore(NULL);
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
            return SUCCESS;
        case LEX_EPILOG:
            if (!checkEpilog()) {
                endParser(SYNTAX_ERROR);
            } else {
                return SUCCESS;
            }
            break;
        default:
            if (curToken == LEX_SEMICOL) {
                endParser(SYNTAX_ERROR);
            }
            store->lastPrinted--;
            if ((parseExpressionRes = parseExpression(
                     LEX_SEMICOL, &resDataType, &globalSymTable)) != SUCCESS) {
                endParser(parseExpressionRes);
            }
            return ParserLoop(true);
    }
    return SUCCESS;
}

bool loadCode() {
    getParsToken();
    while (curToken != LEX_EPILOG && curToken != LEX_EOF) {
        CodeStoreAppend(store, &string, curToken);
        getParsToken();
    }
    CodeStoreAppend(store, &string, curToken);
    for (size_t i = 0; i < store->elementCount; i++) {
        // printf("LEX: %d\t%s\n", store->items[i]->lexID,
        // store->items[i]->tokenVal.string);
        if (store->items[i]->lexID == LEX_FUNKW) {
            store->lastPrinted = i + 1;
            functionDeclarationTopG();
        }
    }
    // stringDeconstruct(&string);
    store->lastPrinted = 0;
    return true;
}

int mainParser() {
    initParser();
    if (!checkProlog(&string)) {
        return endParser(SYNTAX_ERROR);
    }
    loadCode();
    if (ParserLoop(true) == SUCCESS) {
        PRINT_CODE(label, "ENDENDENDEND");
        return endParser(SUCCESS);
    } else {
        return endParser(SYNTAX_ERROR);
    }
    return 0;
}