/*
 * IFJ 2022 PROJECT
 *
 * generator.c
 *
 * Adam Pap: xpapad11
 * 
 * Double linked list library used from printing out
 * the generated code in the right order
 * 
 */

#include "generator.h"

void init_dll(Tinstruction_list *list) {
    list->before_while = NULL;
    list->active = NULL;
    list->first = NULL;
    list->last = NULL;
    list->string_pos = 0;
}

void active_first(Tinstruction_list *list) {
    list->active = list->first->item_next->item_next->item_next;
}

void active_last(Tinstruction_list *list) {
    list->active = list->last;
}

void set_before_while_dll(Tinstruction_list *list) {
    list->before_while = list->last->item_prev;
}

void set_lab_main_dll(Tinstruction_list *list) {
    list->label_main = list->first;
}

void insert_before_active_dll(Tinstruction_list *list, int position) {
    list_item new_element = malloc(sizeof(struct item_list));
    if (new_element == NULL) {
        errorExit(INTERNAL_ERROR, "DLL INSERT ERROR\n");
    }
    new_element->string_pos = position;
    if (list->active != NULL) {
        new_element->item_prev = list->active->item_prev;
        new_element->item_next = list->active;
        list->active->item_prev->item_next = new_element;
        list->active->item_prev = new_element;
    } else {
        free(new_element);
        insert_last_dll(list, position);
    }
}

void insert_after_active_dll(Tinstruction_list *list, int position) {
    list_item new_element = malloc(sizeof(struct item_list));
    if (new_element == NULL) {
        errorExit(INTERNAL_ERROR, "DLL INSERT ERROR\n");
    }
    new_element->string_pos = position;
    if (list->active != NULL) {
        new_element->item_prev = list->active;
        new_element->item_next = list->active->item_next;
        list->active->item_next = new_element;
        if (list->last == list->active) {
            list->last = new_element;
        }
        list->active = new_element;
    } else {
        free(new_element);
        insert_last_dll(list, position);
    }
}

void insert_last_dll(Tinstruction_list *list, int position) {
    list_item new_element = malloc(sizeof(struct item_list));
    if (new_element == NULL) {
        errorExit(INTERNAL_ERROR, "DLL INSERT ERROR\n");
    }
    new_element->string_pos = position;
    if (list->last != NULL) {
        new_element->item_prev = list->last;
        new_element->item_next = NULL;
        list->last->item_next = new_element;
        list->last = new_element;
        list->active = new_element;
    } else {
        new_element->item_prev = NULL;
        new_element->item_next = NULL;
        list->first = new_element;
        list->last = new_element;
        list->active = new_element;
    }
}

void dispose_dll(Tinstruction_list *list) {
    while (list->first != NULL) {
        list_item tmp = list->first->item_next;
        free((list->first));
        list->first = tmp;
    }

    list->last = NULL;
    list->active = NULL;
}