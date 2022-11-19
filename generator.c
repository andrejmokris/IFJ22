/*
 * IFJ 2022 PROJECT
 *
 * File: generator.c
 *
 *
 */
#include "generator.h"

void init_dll(Tinstruction_list *list) {
    list->before_while = NULL;
    list->first = NULL;
    list->last = NULL;
    list->string_end = NULL;
}

void set_before_while_dll(Tinstruction_list *list) {
    list->before_while = list->last;
}

void set_lab_main_dll(Tinstruction_list *list) {
    list->label_main = list->first;
}

void insert_first_dll(Tinstruction_list *list, char *code) {
    list_item new_element = malloc(sizeof(struct item_list));
    if (new_element == NULL) {
        errorExit(INTERNAL_ERROR, "DLL INSERT ERROR\n");
    }

    if (list->first != NULL) {
        new_element->item_prev = NULL;
        new_element->item_next = list->first;
        list->first->item_prev = new_element;
        list->first = new_element;

    } else {
        new_element->item_prev = NULL;
        new_element->item_next = NULL;
        list->first = new_element;
        list->last = new_element;
    }
    new_element->code = code;
}

void insert_last_dll(Tinstruction_list *list, char *code) {
    list_item new_element = malloc(sizeof(struct item_list));
    if (new_element == NULL) {
        errorExit(INTERNAL_ERROR, "DLL INSERT ERROR\n");
    }
    new_element->code = code;
    if (list->last != NULL) {
        new_element->item_prev = list->last;
        new_element->item_next = NULL;
        list->last->item_next = new_element;
        list->last = new_element;
    } else {
        new_element->item_prev = NULL;
        new_element->item_next = NULL;
        list->first = new_element;
        list->last = new_element;
    }
}

// void pre_insert_lab_main(Tinstruction_list *list, bool inwhile);
// void post_insert_before_while(Tinstruction_list *list);
// void post_insert_before_while(Tinstruction_list *list);

// void dipose_dll(Tinstruction_list *list);

// static void print_addr(Tinstruction_list *list);
// static void print_instr(Tinstruction_list *list);

// Tinstr get_instr(ins_t op, Taddr_value addr1, Taddr_value addr2, Taddr_value addr3);

/*void insert_first_dll(Tinstruction_list *list, void *addr1, void *addr2, void *addr3, int data)
{
    list_item new_element = malloc(sizeof(struct item_list));
    if (new_element == NULL)
    {
        errorExit(INTERNAL_ERROR, "DLL INSERT ERROR\n");
    }
    else
    {
        new_element->ins->addr1 = addr1;
        new_element->ins->addr2 = addr2;
        new_element->ins->addr3 = addr3;
        new_element->ins->type = data; // data of instruction
        new_element->item_next = list->first;
        new_element->item_prev = NULL;
        if (list->first != NULL)
        {
            list->first->item_prev = new_element;
        }
        else
        {
            list->first = new_element;
        }
        list->first = new_element;
    }
}

void insert_last_dll(Tinstruction_list *list, void *addr1, void *addr2, void *addr3, int data)
{
    list_item new_element = malloc(sizeof(struct item_list));
    if (new_element == NULL)
    {
        errorExit(INTERNAL_ERROR, "DLL INSERT ERROR\n");
static void print_addr(Tinstruction_list *list);
static void print_instr(Tinstruction_list *list);
        {
            list->first = new_element;
        }
        list->last = new_element;
    }
}

void dipose_dll(Tinstruction_list *list)
{
    while (list->first != NULL)
    {
        list->active = list->first;
        list->first = list->first->item_next;
        free(list->active);
    }
    list->last = NULL;
    list->active = NULL;
}

void delete_first_dll(Tinstruction_list *list)
{
    list_item helper_in_DLList; // pomocna premenna
    if (list->first != NULL)
    {
        helper_in_DLList = list->first;         // zachovanie prveho prvku
        if (list->active == list->first) // kontrola ak je prvy prvok aktivny
        {
            list->active = NULL; // aktivita sa rusi
        }
        if (list->first == list->last) // ak je v zozname len jeden prvok, tak ho zrusime
        {
            list->last = NULL;
            list->first = NULL;
        }
        else
        {
            list->first = list->first->item_next; // nastavenie noveho zaciatku zoznamu
            list->first->item_prev = NULL;
        }
        free(helper_in_DLList); // uvolnenie prvku z pamati
    }
}

void first_dll(Tinstruction_list *list)
{
    list->active = list->first;
}

void last_dll(Tinstruction_list *list)
{
    list->active = list->last;
}

void next_dll(Tinstruction_list *list)
{
    if(list->active != NULL)
    {
        list->active = list->active->item_next;
    }
}

void generate_program_instruction(Tinstruction_list *list, void *addr1, void *addr2, void *addr3, int data)
{
    insert_last_dll(list, addr1, addr2, addr3, data);
}*/