/*
 * IFJ 2022 PROJECT
 *
 * File: main.c
 *
 * Andrej Mokris: xmokri01
 * Veronika Simkova: xsimko14
 * Leos Nevoral: xnevor03
 * Adam Pap: xpapad11
 *
 */
#include "main.h"

int main() {
    int res = mainParser();
    if(res == SUCCESS) {
        return 0;
    } else {
        errorExit(res, "Error Occured\n");
    }
}
