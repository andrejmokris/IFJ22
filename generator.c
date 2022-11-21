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
    list->active = NULL;
    list->first = NULL;
    list->last = NULL;
    list->string_pos = 0;
}

void active_first(Tinstruction_list *list) {
    list->active = list->first->item_next;
}

void active_last(Tinstruction_list *list) {
    list->active = list->last;
}

void set_before_while_dll(Tinstruction_list *list) {
    list->before_while = list->last;
}

void set_lab_main_dll(Tinstruction_list *list) {
    list->label_main = list->first;
}

void insert_first_dll(Tinstruction_list *list, int position) {
    list_item new_element = malloc(sizeof(struct item_list));
    if (new_element == NULL) {
        errorExit(INTERNAL_ERROR, "DLL INSERT ERROR\n");
    }
    new_element->string_pos = position;
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
}

void insert_before_while_dll(Tinstruction_list *list, int position) {
    list_item new_element = malloc(sizeof(struct item_list));
    if (new_element == NULL) {
        errorExit(INTERNAL_ERROR, "DLL INSERT ERROR\n");
    }
    new_element->string_pos = position;
    if (list->before_while != NULL) {
        new_element->item_prev = list->before_while->item_prev;
        new_element->item_next = list->before_while;
        list->before_while->item_prev->item_next = new_element;
        list->before_while->item_prev = new_element;
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
        list->active = new_element;

        if (list->last == list->active) {
            list->last = new_element;
        }
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
    } else {
        new_element->item_prev = NULL;
        new_element->item_next = NULL;
        list->first = new_element;
        list->last = new_element;
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
