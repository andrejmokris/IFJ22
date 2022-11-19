#ifndef ERROR_MODULE
#define ERROR_MODULE

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define LEX_ERROR 1
#define SYNTAX_ERROR 2
#define SEMANTIC_ERROR 3  // nedefinovana funkcia, pokus o redef funkcie
#define RUN_ERROR 4       // zly pocet parametrov, navratova hodnota
#define UNDEFVAR_ERROR 5  // undefined variable
#define RETURN_ERROR 6
#define TYPECOMP_ERORR 7
#define OTHERSEM_ERROR 8
#define INTERNAL_ERROR 60

void errorExit(int errorCode, const char *fmt, ...);

#endif