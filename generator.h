/*
 * IFJ 2022 PROJECT
 *
 * File: generator.h
 *
 * Generator library
 */
#ifndef _GENERATOR
#define _GENERATOR

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_string.h"
#include "error.h"
#include "parser.h"

typedef enum {
    ADDR_VAR,
    ADDR_VAR_TMP,
    ADDR_STRING,
    ADDR_BOOL,
    ADDR_NIL,
    ADDR_INT,
    ADDR_FLOAT
} Taddr_value;

// Double linked list of instructions
typedef struct item_list {
    // Tinstr ins;
    int string_pos;
    struct item_list *item_next;
    struct item_list *item_prev;
} * list_item;

typedef struct Tinstruction_list {
    list_item before_if;
    list_item before_while;
    list_item label_main;
    list_item active;
    list_item first;
    list_item last;
    int string_pos;
} Tinstruction_list;

void init_dll(Tinstruction_list *list);
void set_before_while_dll(Tinstruction_list *list);
void set_lab_main_dll(Tinstruction_list *list);

void insert_first_dll(Tinstruction_list *list, int position);
void insert_before_active_dll(Tinstruction_list *list, int position);
void insert_last_dll(Tinstruction_list *list, int position);
void insert_after_active_dll(Tinstruction_list *list, int position);

void active_first(Tinstruction_list *list);
void active_last(Tinstruction_list *list);

void dispose_dll(Tinstruction_list *list);
#endif