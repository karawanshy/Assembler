#ifndef INFO2_H
#define INFO2_H

/*1)This function an operation name, its number and how many operands it gets, dynamiclly allocates a node, adds to it all the information it got and attach it to the end of the operation table which is extern*/
void insert_op(char* name, unsigned short int num, operands sum);

/*2)This function gets a string and check if it an operation name while scanning the operation table, if it finds it it return the node, if its not an operation name it returns NULL*/
operations_table* find_op(char* name);

/*3)This function gets a pointer to the head of the symbols table, dynamiclly allocates a new node, puts in it all the information it got as the name if its data and if its extern, the entry field it sets to NO (we change it in the second read if needed with the help of the function add_entry), and attach it to the end of the symbols table*/
void add_to_symbols_table(symbols_table **head, char *token, boolean is_data, boolean is_extern);

/*4)This function gets a pointer to the head of the addreses table, dynamiclly allocates anew node, puts in it the data it got as a parameter, and if the is_data it got is YES it puts DC value in the address field, else it puts IC value, while updating IC or DC, and then attach the new node to the end of the table*/
void add_to_address_table(address_table **head, int data, boolean is_data);

/*5)This function gets a pointer to an empty string and a string(lable), dynamiclly allocates a string which the pointer it got points to it, checks if the lable it got ended with : , if yes it removes : from the end of the lable by copying the lable to the string it allocated without the last char and returns YES, if the lable didnt end with : it copy the lable to the string it allocated as it is and returns NO*/
boolean get_lable(char** lable, char *token);

/*6)This function checks if a word is a legal lable, if its legal it returns YES, if its illegal it prints to stdout why its illegal as an error and returns NO*/
int legal_lable(char* lable, int line, char *filename);

/*7)This function checks if the operand of some operation is a legal lable, if its legal it returns YES, if its illegal it returns NO*/
int is_legal_lable(char* lable);

/*8)This function gets an array of chars, and returns the amount of commas in it*/
int count_commas(char line[]);

/*9)This function gets a string, it check if the string is one of the 4 addressing mode, if it is it returns the mode, else it returns NOT_KNOWN(-1)*/
modes get_addressing_mode(char* mode); 

/*10)This function gets a string and a pointer to the head of the symbols table, it checks if the symbol it got is in the symbols table, if yes but it also extern it prints error with the help of line and filename it got, if its not extern it changes its entry cell to YES and returns YES, if there is not such a symbol it returns NO*/
boolean add_entry(char* symbole, symbols_table **head, int line, char *filename);

/*11)This function gets a pointer to the head of the externs table, a lable(string), and an address, it dynamiclly allocates a new node, add to it all the information it got, and then attach it to the end of the table*/
void add_to_ext_table(externs **head, char *lable, int address);

/*12)This function gets a number(word), and a mode which is the mode of the operand or one of A R E, and it sets 1 in the suitable place according to the mode*/
void set_addressing_mode_and_A_R_E(unsigned short int *word, int mode);

/*13)This function gets a pointer to the head of the symbols table and a symbol(string), checks if the symbol it got is in the symbols table if yes it returns YES, else it returns NO*/
int is_in_symbols_table(symbols_table **head, char *symbol);

/*14)This function gets a pointer to the head of the symbols table, a symbol(string), and a pointer to an intiger named is_extern, checks if the symbol is in the symbols table if it is, it set the is_extern it got to the is_extern field of the node and returns the address of the node, if the symbol doesnt exist it returns NOT_KNOWN(-1)*/
int get_address_from_symbols_table(symbols_table **s_head, char *symbol, boolean *is_extern);

/*15)This function gets a number(one of the operands word) and adds to it the info in the suitable place according to type and is_dest that it got.*/
void set_mode_info(unsigned short int *word, short int info, modes type , boolean is_dest);

/*16)This function gets an operand(string), dynamiclly allocate a string with the size of the operand it got, copy to it all the chars that are digits or - or +, put space (' ') in all the other cells, convert it to intiger with the help of atoi, and returns the number it got*/
short int get_info(char* operand);

#endif
