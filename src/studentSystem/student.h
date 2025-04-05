#ifndef __STUDENT__H
#define __STUDENT__H
#include <stdint.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define NAME_SIZE 20
typedef struct{
    char name[NAME_SIZE];
    int8_t gendar;
    int8_t age;
}student; 
int myscanf_int(int *p, int size);
int myscanf_char(char str[], int size);
int student_write(FILE **fp);
int student_read(FILE **fp);
#endif