#ifndef INFO_H
#define INFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define IC_DEFAULT 100 /*the starting value of IC*/
#define DC_DEFAULT 0 /*the starting value of DC*/
#define BUFF_SIZE 81 /*max size of a line*/
#define LABLE_SIZE 32 /*max size of a lable*/
#define BIT 4 /*it help us move the 4 bits when putting values in a word*/
#define A 2 
#define R 1
#define E 0
#define ZERO_TO_LAST 32767/*I used unsigned short int to store a word which is 16 bits, but we need just 15, in most cases the last bit would be 0 so it doesnt affect us, but in cases were the last bit is 1 (when we get negative number after .data or as an negative operand with type immediate) , we do word & ZERO_TO_LAST to set bit number 16 to 0. (32767 is 0111111111111111)*/

typedef enum{NOT_KNOWN = -1, YES, NO}boolean;
typedef enum{ZERO, ONE, TWO, THREE}operands;/*operands numbers*/
typedef enum{INVALID = -1, IMMEDIATE, DIRECT, INDIRECT_REGISTER, DIRECT_REGISTER}modes;/*addressing modes*/


/*THE OPERATION TABLE, HOLDS ALL THE OPERATIONS INFO*/
typedef struct op_node{
	char *name;
	unsigned short int num;
	operands sum;
	operands source[4];
	operands dest[4];
	struct op_node* next; 
}operations_table;

/*THE SYMBOLS TABLE, HOLDS ALL THE LEGAL LABLES INFO THAT WAS DEFINED IN THE FILE*/
typedef struct s_node{
	char *name;
	unsigned int address;
	boolean is_data;
	boolean is_extern;
	boolean entry;
	struct s_node *next;
}symbols_table;

/*THE ADDRESS TABLE HOLDS THE DATA AND THE CODE INFO (TWO TABLES)*/
typedef struct ad_node{
	int address;
	short int code;
	struct ad_node *next;
}address_table;

/*THE EXTERNS TABLE HOLDS ALL THE EXTERN LABLES THAT WERE USED AS OPERANDS AND THEIR ADDRESSES AS OPERANDS*/
typedef struct ext_node{
	char *name;
	int address;
	struct ext_node *next;
}externs;

/*EXTERN VARIABLES*/
extern int IC;
extern int DC;
extern operations_table *op_head;

#endif
