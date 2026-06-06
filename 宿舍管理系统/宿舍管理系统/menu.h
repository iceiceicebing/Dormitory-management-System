// menu.h: Declaration of the main menu_select function

#define _CRT_SECURE_NO_WARNINGS 1
#pragma once

#include <stdio.h>
#include <string.h>

// Predefined constants
#define   TRUE        1
#define   QUIT        -2

typedef   int       Status; // Function return type definition

int menu_select();    // Menu selection function
Status password_verify(char password_real[]); // Login password validation function