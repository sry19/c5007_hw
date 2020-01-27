#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "a3.h"
#include <ctype.h>
#include <string.h>

void Big(Name *name) {
    printf("%s ", name -> first);
    if (name -> middle) {
        printf("%s ", name -> middle);
    }
    printf("%s", name -> last);
}

// Print the string <last>, <first>
//  e.g. if first = "Ben", middle = "Ivan",
//  and last = "Bitdiddle, then print:
//  "Bitdiddle, Ben".
//
// name: a pointer to a Name
void Last(Name *name) {
    printf("%s, %s", name -> last, name -> first);
}

// Print the string <first> <last>
//  e.g. if first = "Ben", middle = "Ivan",
//  and last = "Bitdiddle, then print:
//  "Ben Bitdiddle".
//
// name: a pointer to a Name
void Reg(Name *name) {
    printf("%s %s", name -> first, name -> last);
}

// Print the string <first> <middle-initial>. <last>
//  e.g. if first = "Ben", middle = "Ivan",
//  and last = "Bitdiddle, then print:
//  "Ben I. Bitdiddle".
//
// If middle name is NULL, print:
// "Ben Bitdiddle"
//
// name: a pointer to a Name
void Mid(Name *name) {
    printf("%s ", name -> first);
    if (name -> middle) {
      printf("%c. ", (char)*(name -> middle));
    }
    printf("%s", name -> last);
}

// Print the first name only.
//  e.g. if first = "Ben", middle = "Ivan",
//  and last = "Bitdiddle, then print:
//  "Ben".
//
// name: a pointer to a Name
void Small(Name *name) {
    printf("%s", name -> first);
}

// Given a format char, print the given
// name in the format specified by char.
// The mapping is shown below:
// B -> 'big' format
// L -> 'last' format
// R -> 'reg' format
// M -> 'mid' format
// S -> 'small' format
//
// name: a pointer to a Name
// format: a char indicating the format to use (case doesn't matter)
void FormatName(Name *name, char format) {
  if (((char)tolower(format)) == 'b') {
        Big(name);
    } else if (((char)tolower(format)) == 'l') {
        Last(name);
    } else if (((char)tolower(format)) == 'r') {
        Reg(name);
    } else if (((char)tolower(format)) == 'm') {
        Mid(name);
    } else if (((char)tolower(format)) == 's') {
        Small(name);
    }
}

void FillName(Name *name, char format, char *dest) {
    if (((char)tolower(format)) == 'b') {
      snprintf(dest, sizeof(dest), "%s", name->first);
      snprintf(dest, sizeof(dest), "%s", " ");
        if (name -> middle) {
          snprintf(dest, sizeof(dest), "%s", name->middle);
          snprintf(dest, sizeof(dest), "%s", " ");
        }
        snprintf(dest, sizeof(dest), "%s", name->last);
    } else if (((char)tolower(format)) == 'l') {
      snprintf(dest, sizeof(dest), "%s", name->last);
      snprintf(dest, sizeof(dest), "%s", ", ");
      snprintf(dest, sizeof(dest), "%s", name->first);
    } else if (((char)tolower(format)) == 'r') {
      snprintf(dest, sizeof(dest), "%s", name->first);
      snprintf(dest, sizeof(dest), "%s", " ");
      snprintf(dest, sizeof(dest), "%s", name->last);
    } else if (((char)tolower(format)) == 'm') {
      snprintf(dest, sizeof(dest), "%s", name->first);
      snprintf(dest, sizeof(dest), "%s", " ");
        if (name -> middle) {
          strncat(dest, name->middle, 1);
          snprintf(dest, sizeof(dest), "%s", ". ");
        }
        snprintf(dest, sizeof(dest), "%s", name->last);
    } else if (((char)tolower(format)) == 's') {
      snprintf(dest, sizeof(dest), "%s", name->first);
    }
}
