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

/*typedef enum {
    ADD_IN,
    SUB_IN,
    MUL_IN,
    DIV_IN,
    IDIV_IN
} ins_t;*/

typedef enum {
    ADDR_VAR,
    ADDR_VAR_TMP,
    ADDR_STRING,
    ADDR_BOOL,
    ADDR_NIL,
    ADDR_INT,
    ADDR_FLOAT
} Taddr_value;

/*typedef struct {
    Taddr_value type;
    union {
        int i;
        double f;
        char *s;
    } val;
} TAdr;*/

/*typedef struct ins_addr {
    int *addr1;
    int *addr2;
    int *addr3;
    ins_t op;
} Tinstr;*/

// Double linked list of instructions
typedef struct item_list {
    // Tinstr ins;
    int string_pos;
    struct item_list *item_next;
    struct item_list *item_prev;
} * list_item;

typedef struct Tinstruction_list {
    list_item before_while;
    list_item label_main;
    // list_item fun_def_before_while;
    list_item active;
    list_item first;
    list_item last;
    int string_pos;
} Tinstruction_list;

void init_dll(Tinstruction_list *list);
void set_before_while_dll(Tinstruction_list *list);
void set_lab_main_dll(Tinstruction_list *list);

void insert_first_dll(Tinstruction_list *list, int position);
void insert_before_while_dll(Tinstruction_list *list, int position);
void insert_last_dll(Tinstruction_list *list, int position);
void insert_after_active_dll(Tinstruction_list *list, int position);

void activce_first(Tinstruction_list *list);
void activce_last(Tinstruction_list *list);

void dispose_dll(Tinstruction_list *list);

/*void pre_insert_lab_main(Tinstruction_list *list, bool inwhile);
void post_insert_before_while(Tinstruction_list *list);
void post_insert_before_while(Tinstruction_list *list);

void dipose_dll(Tinstruction_list *list);

static void print_addr(Tinstruction_list *list);
static void print_instr(Tinstruction_list *list);*/

// Tinstr get_instr(ins_t op, Taddr_value addr1, Taddr_value addr2, Taddr_value addr3);

/*void insert_first_dll(Tinstruction_list *list, void *, void *, void *, int data);
void insert_last_dll(Tinstruction_list *list, void *, void *, void *, int data);
void dipose_dll(Tinstruction_list *list);
void delete_first_dll(Tinstruction_list *list);
void first_dll(Tinstruction_list *list);
void last_dll(Tinstruction_list *list);
void next_dll(Tinstruction_list *list);
void generate_program_instruction(Tinstruction_list *list, void *, void *, void *, int data);*/

#endif