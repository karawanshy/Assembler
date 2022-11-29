#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/*1)This funtion creats a table of the operations info, each node contains the operation name, its number, how many operands it gets, and two arrays one for what addressing modes it can get as source operand, and one for what addressing modes it can get as destination operand, with the help of the function insert_op*/
void op_info();

/*2)This function dynamiclly allocates string with the help of size it got as a parameter, copy to it the first string it got and merge to it the second one, and then returns the new merged string*/
char* creat_name(char* s1, char*s2, int size);

/*3)This function do the first read of the file for us, it gets as parameters: a file which it reads, a pointer to an empty symbols table and a pointer to an empty data table which it fills as it reads from the file, a pointer to boolean, if there was at least one error in the file it stores YES else it stores NO, and a file name which we use when printing errors(to give more informations)*/
void first_read(FILE* file, symbols_table **s_head, address_table **data_head, boolean *error, char *filename);

/*4)This function gets the head of the symbols table and the head of the data table, and updates the address of the, so that the data addresses get after the code address with the help of IC*/
void update_data_add(symbols_table *s_head, address_table *data_head);

/*5)This function do the second read of the file for us, it gets as parameters: a file which it reads, a pointer to the head of the symbols table which help the function to check if there is errors (looking for lables in the table), a pointer to an empty code table which it fills as it reads from the file, a pointer to boolean, if there was at least one error in the file it stores YES else it stores NO, and a file name which we use when printing errors(to give more informations)*/
void second_read(FILE* file, symbols_table **s_head, address_table **code_head, externs **ext_head, boolean *error, char *filename);

/*6)This function gets the head of the symbols table, the head of the externs table, an ent_file_name and an ext_file_name, it scans the symbols table if it finds at least one lable that it entry field is YES it creats an .ent file with the ent_file_name it got and print to it all the lables that they are entry and their addresses, and it scans the externs table, if its not empty it creats an .ext file with the ext_file_name it got and prints to it all the extern lables and their addresses as in the table*/
void print_ent_ext(symbols_table *head, externs *ext_head, char* ent_file_name, char* ext_file_name);

/*7)This function gets each of the heads of the data and the code tables, and a file_name, it checks if the heads it got are not empty, if they arent empty it creates an .ob file wuth the file_name it got, and prints to it the code table and then the data table*/
void print_ob(char *file_name, address_table *code_head, address_table *data_head);

/*8)This fuction gets each of the heads of the symbols, data and code tables, and free them*/
void free_all(symbols_table *s_head, address_table *code_head, address_table *data_head, externs *ext_head);

/*9)This function free the operation table which is declaird as extern*/
void free_operations_table();

#endif
