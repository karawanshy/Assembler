#include "info.h"
#include "info2.h"

void op_info()
{
	int num;
	operations_table *curr;
	op_head = NULL;
	/*creating nodes and inserting the name, number, and the number of operand a function gets with the help of the function insert_op*/
	insert_op("mov", 0, TWO);
	insert_op("cmp", 1, TWO);
	insert_op("add", 2, TWO);
	insert_op("sub", 3, TWO);
	insert_op("lea", 4, TWO);
	insert_op("clr", 5, ONE);
	insert_op("not", 6, ONE);
	insert_op("inc", 7, ONE);
	insert_op("dec", 8, ONE);
	insert_op("jmp", 9, ONE);
	insert_op("bne", 10, ONE);
	insert_op("red", 11, ONE);
	insert_op("prn", 12, ONE);
	insert_op("jsr", 13, ONE);
	insert_op("rts", 14, ZERO);
	insert_op("stop", 15, ZERO);
	
	curr = op_head;
	while(curr)/*going through all the operations*/
	{
		/*initialising all the cells in the source and destination arrays to -1*/
		curr->source[0] = curr->source[1] = curr->source[2] = curr->source[3] = curr->dest[0] = curr->dest[1] = curr->dest[2] =curr->dest[3] = -1;

		num = curr->num;/*putting in num the number of the operation*/
	
		/*filling the source and destinations array as written in the maman*/
		if(num == 0 || num == 1 || num == 2 || num == 3)	
		{
			curr->source[0] = ZERO;
			curr->source[1] = ONE;
			curr->source[2] = TWO;
			curr->source[3] = THREE;
		}
		else if(num == 4)
			curr->source[1] = ONE;
		if(num == 0 || num == 2 || num == 3 || num == 4 || num == 5 || num == 6 || num == 7 || num == 8 || num == 11)
		{
			curr->dest[1] = ONE;
			curr->dest[2] = TWO;
			curr->dest[3] = THREE;
		}
		else if (num == 9 || num == 10 || num == 13)
		{
			curr->dest[1] = ONE;
			curr->dest[2] = TWO;
		}
		else if(num == 1 || num == 12)
		{
			curr->dest[0] = ZERO;
			curr->dest[1] = ONE;
			curr->dest[2] = TWO;
			curr->dest[3] = THREE;
		}
		curr = curr->next;
	}
}

void insert_op(char* name, unsigned short int num, operands sum)
{
	operations_table* curr = op_head;
	operations_table* new_node = (operations_table*)malloc(sizeof(operations_table));
	new_node->name = name;/*name of operation*/
	new_node->num = num;/*number of operation*/
	new_node->sum = sum;/*how many operand the operation gets*/
	if(op_head == NULL)
		op_head = new_node;
	else{ 
		while(curr->next)
			curr = curr->next;
		curr->next = new_node;
	}
}

operations_table* find_op(char* name)
{
	operations_table *curr = op_head;
	while(curr)
	{
		if(!strcmp(curr->name, name))
			return curr;
		curr = curr->next;
	}
	return NULL;
}

boolean get_lable(char **lable, char *token)
{
	int size = strlen(token);
	
	*lable = (char*)malloc(size*sizeof(char));
	if (!*lable)
	{
		printf("Memory allocation failed");
		exit(0);
	}		

	if(token[size - 1] == ':')
	{
		strncpy(*lable, token, size - 1);
		return YES;
	}
	else{
		*lable = strcpy(*lable, token);
		return NO;
	}
}

int legal_lable(char *lable, int line, char *filename)
{
	int i = 0;
	int size = strlen(lable);

	/*if the lable is a saved word*/
	if(find_op(lable) || !strcmp(lable,"r0") || !strcmp(lable,"r1") || !strcmp(lable,"r2") || !strcmp(lable,"r3") || !strcmp(lable,"r4") || !strcmp(lable,"r5") || !strcmp(lable,"r6") || !strcmp(lable,"r7") || !strcmp(lable, "entry") || !strcmp(lable, "extern") || !strcmp(lable, "data") || !strcmp(lable, "string"))
	{
		printf("Error in file: %s, line: %d\nthe lable %s is a saved word\n", filename, line, lable);
		return NO;
	}

	/*if the lable is too long*/
	if(size > LABLE_SIZE)
	{
		printf("Error in file: %s, line: %d\nthe lable %s is longer than the limited size %d\n", filename, line, lable, LABLE_SIZE + 1);
		return NO;
	} 
	else if(isalpha(lable[i]))/*if the first letter is alphabet*/
	{
		i++;		
		while(i < size - 1)
		{
			if(isalpha(lable[i]) || isdigit(lable[i]))/*if the letter are digits or alphabets*/
				i++;
			else{
				printf("Error in file: %s, line: %d\nthe lable %s contains illegal chars\n", filename, line, lable);
				return NO;
			}
		}
	}
	else{ 
		printf("Error in file: %s, line: %d\nthe lable %s didnt start with alphabet letter\n", filename, line, lable);
		return NO;
	}
	return YES;
}

int is_legal_lable(char *lable)
{
	int i = 0;
	int size = strlen(lable);
	if(size <= LABLE_SIZE && isalpha(lable[i]))
	{
		i++;		
		while(i < size)
		{
			if(isalpha(lable[i]) || isdigit(lable[i]))
				i++;
			else
				return NO;
		}
	}
	else return NO;
	return YES;
}

int count_commas(char line[])
{
	int i, count;
	i = count = 0;
	while(line[i] != '\0')
	{
		if(line[i] == ',')
		{
			count++;			
		}
		i++;
	}
	return count;
}

void add_to_symbols_table(symbols_table **head, char *lable, boolean is_data, boolean is_extern)
{
	symbols_table *curr = *head;	
	symbols_table *new_node = (symbols_table*)malloc(sizeof(symbols_table));	
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
	if(is_extern == 0)
		new_node->address = 0;
	else if(is_data == 0)
		new_node->address = DC;
	else
		new_node->address = IC;
	new_node->is_data = is_data;
	new_node->is_extern = is_extern;	
	new_node->entry = NO;
	new_node->next = NULL;
	if(*head == NULL)	
		*head = new_node;
	else{
		while(curr->next)
			curr = curr->next;
		curr->next = new_node;
	}
}

void add_to_address_table(address_table **head, int data, boolean is_data)
{
	address_table *curr = *head;
 	address_table *new_node = (address_table*)malloc(sizeof(address_table)); 
  if (!new_node)
	{
		printf("Memory allocation failed");
		exit(0);
	}	 
	if(is_data == YES)
	{
		new_node->address = DC;
		DC++;
	}
	else {
		new_node->address = IC;
		IC++;
	}
	new_node->code = data;
	new_node->next = NULL;
	
	if(*head == NULL)	
		*head = new_node;
	else {
		while(curr->next)
			curr = curr->next;
		curr->next = new_node;
	}
}

modes get_addressing_mode(char* mode)
{
	int i;
	boolean flag = YES;/*flag for checking if the addressing mode is immediate*/
	if(mode[0] == '#' && (mode[1] == '-' || mode[1] == '+' || isdigit(mode[1])))
	{
		for(i = 2; i < strlen(mode); i++)
		{
			if(!isdigit(mode[i])) flag = NO;/*if its not a digit*/
		}
		if(flag == YES) return IMMEDIATE;
	}
	if((strlen(mode) == 3) && (mode[0] == '*') && (mode[1] == 'r') && (mode[2] >= '0') && (mode[2] <= '7'))
		return INDIRECT_REGISTER;
	if((strlen(mode) == 2) && (mode[0] == 'r') && (mode[1] >= '0') && (mode[1] <= '7'))
		return DIRECT_REGISTER;
	if(is_legal_lable(mode) == YES)
		return DIRECT;
	
	return INVALID;
}

boolean add_entry(char* symbole, symbols_table **head, int line, char *filename)
{
	symbols_table *curr = *head;
	while(curr)
	{
		if(!strcmp(curr->name, symbole) && curr->is_extern == YES)
		{
			printf("Error in file: %s, line: %d\nthe lable %s after .entry wasnt defined in the file\n", filename, line, symbole);
			return NO;	
		}
		if(!strcmp(curr->name, symbole) && curr->is_extern == NO)
		{
			curr->entry = YES;
			return YES;
		}
		curr = curr->next;
	}
	return NO;
}

short int get_info(char* operand)
{
	int i = 0;
	int num;
	int size = strlen(operand);
	char *s = (char*)malloc(size*sizeof(char));
	while(operand[i] != '\0')/*copying to the string just the digits and - or +*/
	{
		if(isdigit(operand[i]) || operand[i] == '-' || operand[i] == '+')
			s[i] = operand[i];
		else s[i] = ' ';
		i++;
	}
	num = atoi(s);
	free(s);
	return num;
}
