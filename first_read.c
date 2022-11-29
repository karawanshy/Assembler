#include "info.h"
#include "info2.h"

void first_read(FILE* file, symbols_table **s_head, address_table **data_head, boolean *error_all, char *filename)
{
	char buff[BUFF_SIZE];/*array that holds a line from the file*/
	boolean error;/*shows if there was an error in a line*/
	char *token;/*a pointer to every word(anything) in the line*/
	int line = 0;/*counts the lines in the file, to help printing errors while mentioning the line*/
	int commas;/*we store in it the amount of commas in a line*/
	char *lable;/*stores a lable that was defined at the beginning of the line(legal or illegal)*/
	boolean flag;/*we store in it if the lable that was defined ended with : or not*/
	boolean lable_def;/*a flag to tell us if a legal lable was defined*/
	operations_table* op;/*holds the operation info as a struct node, that we get from function find_op*/
	int i = 0;/*helps counting the chars in the string we get after .string*/
	int add_mode; /*the addressing mode of some operand that we get from the function get_addressing_mode*/
	short int data;/*contains the data, numbers if after .data, each char of the string if after .string*/
	operands count = 0;/*counts how much operands we read after operation*/
	operands reg = 0;/*counts how many registers were after the operation*/
	

	/*going through the lines of the file, while saving each line in an array called buff*/
	while(fgets(buff, BUFF_SIZE, file))
	{
		line++;/*adding to the lines counter*/
		error = NO;/*there is no errors yet*/
		lable_def = NO;/*there is no lable definitions yet*/
		lable = NULL; /*initialising the lable to NULL*/
		flag = NO;/*initialising the flag to NO*/

		commas = count_commas(buff);/*counting the commas in the line with the help of the function count_commas*/
	
		if(strtok(buff, "\n"))/*getting rid of \n in the line*/
		{
			token = strtok(buff, " \t");/*taking the first word in the line*/

			if((token[0] != ';') && strcmp (token, ".entry"))/*we ask if its a documentation line or starts with entry, if no we continue, if yes we skip to the next line*/
			{
				if((token[0] != '.') && !find_op(token))/*we ask if the word doesnt start with . and if its not operation name, if thats true we continue*/
				{
					flag = get_lable(&lable, token);/*putting in the flag if the lable ended with : or not*/
					
					if(legal_lable(lable, line, filename) == YES)/*we ask if its a legal lable*/
					{
						if(flag == NO)
						{
							printf("Error in file: %s, line: %d\nthe lable %s didnt end with :\n", filename, line, token);
							error = YES;
						}
						else if(is_in_symbols_table(s_head, lable) == NO)/*we ask if its not in the symbols table*/
						{
							lable_def = YES;
							token = strtok(NULL, " \t");	
						}
						else{/*its already in the symbols table*/
							printf("Error in file: %s, line: %d\nthe lable %s was enterd before.\n", filename, line, lable);
							error = YES;
							if(lable) free(lable);/*if the lable is not NULL we free it cause we dont need it anymore*/
						}
						if(!token && error == NO)/*if error is no that means its not in the symbols table yet, so we ask if theres another thing after the lable*/
						{
							printf("Error in file: %s, line: %d\nthere is nothing after the lable %s\n", filename, line, lable);
							error = YES;
						}
					}
					else{
						error = YES;/*coming to else it means that the first token in the line is illegal, that means it is an error which we printed to stdout in the function legal_lable, so we put yes in error to show that there was an error in this line, and to help skip the rest of the line*/
						if(lable) free(lable);/*if the lable is not NULL we free it cause we dont need it anymore*/
					}
				}	
				if(!strcmp(token, ".data") && error == NO)/*we ask if the token is .data and if there wasnt any error yet*/
				{
					count = 0;/*initialising the counter*/
					if(lable_def == YES) add_to_symbols_table(s_head, lable, YES, NO);/*if a lable was defined we add it to the symbols table*/
					token = strtok(NULL, " ,\t");
					if(!token)
					{
						printf("Error in file: %s, line: %d\nthere is no operands after .data\n", filename, line);
						error = YES;
					}
					while(token && error == NO)/*if there wasnt any error yet and the token is not NULL, we add the data to the address table*/
					{	
						count++;
						data = atoi(token);
						/*if the data was negative we change the last bit(number 16) to 0*/
						if(data < 0) data = data & ZERO_TO_LAST;
						add_to_address_table(data_head, data, YES);
						token = strtok(NULL, " ,\t");
					}
					if(commas < (count - 1))
					{
						printf("Error in file: %s, line: %d\nthere is less commas than it should be\n", filename, line);
						error = YES;
					}
					else if(commas > (count - 1))
					{
						printf("Error in file %s, line: %d\nthere is more commas than it should be\n", filename, line);
						error = YES;
					}
				}
				else if(!strcmp(token, ".string") && error == NO)/*we ask if the token is .string and if there wasnt any error yet*/
				{
					if(commas > 0)
					{
						printf("Error in file: %s, line: %d\nthere shouldnt be commas in this line\n", filename, line);
						error = YES;
					}
					if(error == NO)
					{
						if(lable_def == YES) add_to_symbols_table(s_head, lable, YES, NO);/*if a lable was defined we add it to the symbols table*/
						token = strtok(NULL, " \t");
						if(!token)
						{
							printf("Error in file: %s, line: %d\nthere is no operands after .string\n", filename, line);
							error = YES;
						}
					}
					if(error == NO && token[0] == '"' && token[strlen(token) - 1] == '"')/*checking if the token is in a legal string format, which means if there is " at the start and at the end of it.*/
					{
						for(i = 1; i < (strlen(token) - 1); i++)
							add_to_address_table(data_head, token[i], YES);
						add_to_address_table(data_head, 0, YES);/*adding 0 to the end of the characters (a word that is 0 in the machine code)*/	
						token = strtok(NULL, " \t");
						if(token) /*asking if there is more information after the string, if yes we get error*/
						{
							printf("Error in file: %s, line: %d\nthere is illegal data after the string\n", filename, line);
							error = YES;
						}
					}
				}
				else if(!strcmp(token, ".extern") && error == NO)/*we ask if the token is .extern and if there wasnt any error yet*/
				{		
					if(commas > 0)/*if there is commas in the line*/
					{
						printf("Error in file: %s, line: %d\nthere shouldnt be commas in this line\n", filename, line);
						error = YES;
					}
					else{
						token = strtok(NULL," \t");
						if(!token)/*if there is no operands after .extern*/
						{
							printf("Error in file: %s, line: %d\nthere is no operands after .extern\n", filename, line);
							error = YES;
						}
					}
					if(error == NO)
					{
						/*checking if the lable was entered before into the symbols table, if NO we add it, if YES we just skip it, as mentioned in the project*/
						if(is_in_symbols_table(s_head, token) == NO)
							add_to_symbols_table(s_head, token, NOT_KNOWN , YES);
						token = strtok(NULL," \t");
						if(token) 
						{
			 				printf("Error in file: %s, line: %d\nthere is more than one operand after .extern\n", filename, line);
							error = YES;
						}
					}
				}
				else if(token && find_op(token) && error == NO)/*if find_op of the token didnt return NULL that means that this is an assembly directives, so we ask if there wasnt any error yet to enter*/
				{
					if(lable_def == YES) add_to_symbols_table(s_head, lable, NO, NO);/*if a lable was defined we add it to the symbols table*/
					op = find_op(token);/*we put in op what operation this is and its information as in the list of operations*/
					token = strtok(NULL, " ,\t");
					if(op->sum < 2 && commas > 0)/*if there is at least one comma*/
					{
						printf("Error in file: %s, line: %d\nthere shouldnt be commas in this line\n", filename, line);
						error = YES;
					}
					else if(op->sum == 2 && commas > 1)/*if there is too much commas*/
					{
						printf("Error in file: %s, line: %d\nthere is more commas than it should be\n", filename, line);
						error = YES;
					}
					else if(op->sum == 2 && commas < 1)/*if there less commas*/
					{
						printf("Error in file: %s, line: %d\nthere is less commas than it should be\n", filename, line);
						error = YES;
					}
			
					count = reg = 0;/*initialising count and reg to zero*/
					while(token)/*going through the operands after the operation*/
					{
						count++;/*counting the number of operands in the line*/
						add_mode = get_addressing_mode(token);/*getting the operand's addressing mode*/
						if(add_mode == INVALID)/*if its none of the four addresssing modes we have we get an error*/
						{
							printf("Error in file: %s, line: %d\nthe operand %s is illegal.\n", filename, line, token);
							error = YES;
						}
						/*if this operation gets two operands and we are in the first one*/
						else if(count == ONE && op->sum == TWO)
						{
							/*checking if the operand can be a source operand of the operation*/
							if(op->source[add_mode] != add_mode)
							{
								printf("Error in file: %s, line: %d\nthe operation %s cant have addressing mode number %d as a source operand\n", filename, line, op->name, add_mode);
								error = YES;
							}
						}
						/*if the operation gets one operand or if we are on the second operand (destination operand in all cases*/
						else if(op->sum == ONE || count == TWO)
						{
							/*checking if the operand can be a destination operand of the operation*/
							if(op->dest[add_mode] != add_mode)
							{
								printf("Error in file: %s, line: %d\nthe operation %s cant have addressing mode number %d as a destination operand\n", filename, line, op->name, add_mode);
								error = YES;
							}
						}
						/*we ask if the operand is register*/
						if(get_addressing_mode(token) == DIRECT_REGISTER || get_addressing_mode(token) == INDIRECT_REGISTER)
							reg++;
						token = strtok(NULL, " ,\t");	
					}	
					/*we ask if the number of operands we counted is the same as what the operation should get*/
					if(count != op->sum) 
					{
						printf("Error in file: %s, line: %d\nthe operation %s should have %d operands not %d\n", filename, line, op->name, op->sum, count);
						error = YES;
					}
					/*if there wasnt any errors till here we update the IC and DC, if there was errors it doesnt really matter because we wont get to the second read where we need IC and DC*/
					if(error == NO)
					{
						if(reg == TWO)
							IC = IC + TWO;
						else					
							IC = IC + count + 1;
					}
  			}		
				else if(error == NO)/*if the token is not a lable, .data, .string, .extern nor operation*/
				{
					printf("Error in file: %s, line: %d\nthe word %s is illegal\n", filename, line, token);
					error = YES;	
				}
			}	
		}	
		if(lable_def == YES) free(lable); /*freeing the lable (in case lable_def is NO we freed it before)*/
		/*if the error_all is NO that means there wasn't any errors yet in the file. so we put the result of the current line, if its YES there is no need to update.*/
		if(*error_all == NO)
			*error_all = error;
	}
}
