#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

int main() {

    registo* headR = NULL;
    cliente* headC = NULL;
    gestor* headG = NULL;
    meio* headM = NULL;
    
    showMenu(&headC, &headG, &headM, &headR);

    return 0;
}