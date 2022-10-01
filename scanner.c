/*
* IFJ 2022 PROJECT
*
* File: scanner.c
*
*/
#include <stdio.h>
#include "scanner.h"
#include <ctype.h>

FSM_States state_transition(FSM_States input, char edge_sign)
{
    switch(input)
    {
        case START_STATE:
            if(edge_sign == '$') 
                return VAR0_STATE;
            else if(isalpha(edge_sign) || edge_sign == '_') 
                return ID_FUN_STATE;

            else if(edge_sign == '+') 
                return SUM_STATE;
            else if(edge_sign == '-') 
                return SUB_STATE;
            else if(edge_sign == '*') 
                return MUL_STATE;
            else if(edge_sign == '/') 
                return DIV_STATE;
            else if(edge_sign == '=') 
                return ASSIGN_STATE;
            else if(edge_sign == '!') 
                return NOT_STATE;
            else if(edge_sign == '(') 
                return LPAR_STATE;
            else if(edge_sign == ')') 
                return RPAR_STATE;
            else if(edge_sign == '>') 
                return GREATER_STATE;
            else if(edge_sign == '<') 
                return LESSER_STATE;        
            else if(edge_sign == ',') 
                return COMMMA_STATE;
            else if(edge_sign == '.') 
                return DOT_STATE;
            else if(edge_sign == ';') 
                return SEMICOL_STATE;

            else 
                return ERROR_STATE;
            break;


        case VAR0_STATE:
            if(isalpha(edge_sign) || edge_sign == '_')
                return VAR_STATE;   
            else 
                return ERROR_STATE;
            break;
        case VAR_STATE:
            if(isalpha(edge_sign) || edge_sign == '_')
                return VAR_STATE;   
            else 
                return ERROR_STATE;
            break;

        case SUM_STATE:
        case SUB_STATE:
        case MUL_STATE:
        case DIV_STATE:
        case ASSIGN_STATE:
        case NOT_STATE:
        case LPAR_STATE:
        case RPAR_STATE:
        case GREATER_STATE:
        case LESSER_STATE:
        case COMMMA_STATE:
        case DOT_STATE:
        case SEMICOL_STATE:


         case ID_FUN_STATE:
            if(isalpha(edge_sign) || edge_sign == '_')
                return ID_FUN_STATE;   
            else 
                return ERROR_STATE;
            break;

            case ERROR_STATE:
            fprintf(stderr, "Scenner error.");  
                return ERROR_STATE;
            break;                    
                            
            
            //TO DO Strings, Floats, Comment, ID_Types
            /*else if(edge_sign == '"') */
    }
    return ERROR_STATE;
}

int main() {
    return 0;
}