#include "info.h"

void add_to_ext_table(externs **head, char *lable, int address)
{
	externs *curr = *head;	
	externs *new_node = (externs*)malloc(sizeof(externs));	
	if (!new_node)
	{
		printf("Memory allocation failed");
		exit(0);
	}	
	new_node->name = (char*)malloc(strlen(lable)*sizeof(char));
	if (!new_node->name)
	{
		printf("Memory allocation failed");
		exit(0);
	}	
	strcpy(new_node->name, lable);
	new_node->address = address;
	new_node->next = NULL;
	if(*head == NULL)	
		*head = new_node;
	else{
		while(curr->next)
			curr = curr->next;
		curr->next = new_node;
	}
}

int is_in_symbols_table(symbols_table **head, char *symbol)
{
	symbols_table *curr = *head;
	
	while(curr)
	{
		if(!strcmp(curr->name, symbol))
			return YES;
		curr = curr->next;
	}
	
	return NO;
}

int get_address_from_symbols_table(symbols_table **head, char *symbol, boolean *is_extern)
{
	symbols_table *curr = *head;
	while(curr)
	{
		if(!strcmp(curr->name, symbol))/*if we found the lable we return the address*/
		{	
			*is_extern = curr->is_extern;
			return curr->address;
		}
		curr = curr->next;
	}
	return NOT_KNOWN;/*if the lable wasnt found we return NOT_KNOWN(-1)*/
}

void set_mode_info(unsigned short int *word, short int info, modes type, boolean is_dest)
{
	if(type == INDIRECT_REGISTER || type == DIRECT_REGISTER)/*if the operand is register*/
	{
		if(is_dest == NO)/*if its destination operand*/
		{
			*word = *word | info;/*we add the info*/
			*word = *word << (BIT - 1);/*and we move 3 bits to the left to make place for ARE*/
		}
		else if(*word != ZERO)/*if the word is not empty it means there i the source operand info in it*/
		{
			*word = *word >> (BIT - 1);/*so we move 3 bits to the right to make place for the destinamtion operand info*/
			*word = *word | info;/*we add the destination operand info*/
		}
		else *word = *word | info;/*if its register but a source operand we just add the info to it*/
	}
	else *word = *word | info;/*if its not register we just add the info to the word*/
	*word = *word << (BIT - 1);	/*we move 3 bits to the left to make place for ARE*/
}

void set_addressing_mode_and_A_R_E(unsigned short int *word, int mode)
{
		unsigned short int mask = 1;
		mask = mask << mode;/*moving to the left according to the mode or ARE*/
		*word = *word | mask;/*adding to the word*/
}

void update_data_add(symbols_table *s_head, address_table *data_head)
{
	symbols_table *s_curr = s_head;
	address_table *data_curr = data_head;
	while(s_curr)
	{
		if(s_curr->is_data == YES)
			s_curr->address = s_curr->address + IC;/*adding IC(starting value) to the addresses*/
		s_curr = s_curr->next;
	}
	while(data_curr)
	{
		data_curr->address = data_curr->address + IC;/*adding IC(starting value) to the addresses*/
		data_curr = data_curr->next;
	}
}

char* creat_name(char* s1, char* s2, int size)/*name somthing else*/
{
	char *filename = (char*)malloc(size*sizeof(char));
	strcpy(filename, s1);/*copying the name*/
	strcat(filename, s2);/*adding to its end s2*/
	return filename;
}

void print_ob(char* file_name, address_table *code_head, address_table *data_head)
{
	address_table *curr;
	FILE *file;
	if(code_head || data_head)/*if at least one of the table is not empty*/
	{
		if (!(file = fopen(file_name,"w")))/*openning the file*/
		{
			printf("cannot open file: %s\n", file_name);
			exit(0);
		}
		fprintf(file, "   %d %d\n", IC - IC_DEFAULT, DC);/*printing the IC and DC*/
		curr = code_head;
		while(curr)/*going through the code table*/
		{
			fprintf(file, "%04d  %05o\n", curr->address, curr->code);
			curr = curr->next;
		}
		curr = data_head;
		while(curr)/*going through the data table*/
		{
			fprintf(file, "%04d  %05o\n", curr->address, curr->code);
			curr = curr->next;
		}
		fclose(file);/*closing the file*/
	}
}

void print_ent_ext(symbols_table *head, externs *ext_head, char* ent_file_name, char* ext_file_name)
{
	FILE *ent_file, *ext_file;
	symbols_table *curr = head;
	externs *ext_curr = ext_head;
	boolean ent, ext;/*flags to help us know if there is info inside the tables or nor, so we can know if to creat the files or no*/
	ent = ext = NO;
	
	while(curr)/*going through the symbols table*/
	{
		if(curr->entry == YES)
		{
			if(ent == NO)/*if its the first*/
			{
				if (!(ent_file = fopen(ent_file_name,"w")))
				{
					printf("cannot open file: %s\n", ent_file_name);
					exit(0);
				}
				ent = YES;
			}
			fprintf(ent_file, "%-3s  %04d\n", curr->name, curr->address);
		}	
		curr = curr->next;
	}
	while(ext_curr)/*going through the externs table*/
	{
		if(ext == NO)/*if its the first*/
		{
			if (!(ext_file = fopen(ext_file_name,"w")))
			{
				printf("cannot open file: %s\n", ext_file_name);
				exit(0);
			}
			ext = YES;
		}
			fprintf(ext_file, "%-3s  %04d\n", ext_curr->name, ext_curr->address);
			ext_curr = ext_curr->next;
	}
	/*if we created the files we close them*/
	if(ent == YES) fclose(ent_file);
	if(ext == YES) fclose(ext_file);
}


void free_all(symbols_table *s_head, address_table *code_head, address_table *data_head, externs *ext_head)
{
	symbols_table *curr;
	address_table *ad_curr;
	externs *ext_curr = ext_head;
	
	while(s_head)
	{
		curr = s_head;
		s_head = s_head->next;
		free(curr->name);
		free(curr);
	}
	while(code_head)
	{
		ad_curr = code_head;
		code_head = code_head->next;
		free(ad_curr);
	}
	while(data_head)
	{
		ad_curr = data_head;
		data_head = data_head->next;
		free(ad_curr);
	}	
	while(ext_head)
	{
		ext_curr = ext_head;
		ext_head = ext_head->next;
		free(ext_curr->name);
		free(ext_curr);
	}
}

void free_operations_table()
{
	operations_table *op_curr = op_head;
	while(op_head)
	{
		op_curr = op_head;
		op_head = op_head->next;
		free(op_curr);
	}	
}
