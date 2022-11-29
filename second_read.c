#include "info.h"
#include "info2.h"

void second_read(FILE* file, symbols_table **s_head, address_table **code_head, externs **ext_head, boolean *error_all, char *filename)
{
	char buff[BUFF_SIZE];/*array that holds a line from the file*/
	boolean error;/*shows if there was an error in a line*/
	char *token;/*a pointer to every word(anything) in the line*/
	int line = 0;/*counts the lines in the file, to help printing errors while mentioning the line*/
	operations_table* op;/*holds the operation info as a struct node, that we get from function find_op*/
	unsigned short int result = 0;/*stores the first word of an instruction(as machine code)*/
	unsigned short int operand1 = 0;/*stores the second word(if there is) of an instruction(as machine code)*/
	unsigned short int operand2 = 0;/*stores the third wordd(if there is) of an instruction(as machine code)*/
	int add_mode; /*the addressing mode of the operand that we get from the function get_addressing_mode*/
	short int info;/*contains the info we need to add to the code table according to the operand addressing mode*/
	operands reg = 0; /*counts registers in each instruction*/
	operands sum = 0;/*counts operands in each instruction*/
	boolean is_extern = NO;/*flag to know if an operand with addressing mode number 1 is extern or not according to the symbols table, and it help us decide whether to put R or E in ARE, and whether to add to the extern table or not*/
	
	/*going through the lines of the file, while saving each line in an array caled buff*/
	while(fgets(buff, BUFF_SIZE, file))
	{
		line++;/*adding to the lines counter*/
		error = NO;/*there is no errors yet*/
		result = operand1 = operand2 = 0;	/*initialising again every line*/
		reg = sum = 0;/*initialising again every line*/
		if(strtok(buff, "\n"))/*getting rid of \n from the line*/
		{
			token = strtok(buff, " ,:\t");/*taking the first word in the line*/
			if(token[0] != ';' && token)/*we ask if its a documentation line or the token is NULL*/
			{
				if((token[0] != '.') && !find_op(token))/*that means its a legal lable(because if its not we wouldnt have entered to seconed read)*/
				{
						token = strtok(NULL, " ,:\t");
				} 
				if(strcmp(token, ".extern") && strcmp(token, ".data") && strcmp(token, ".string"))/*asking if its .data or .string or .string, if yes we skip because we took care of this in first read*/
				{
					if(!strcmp(token, ".entry"))/*if the token is .entry*/
					{
						token = strtok(NULL, " ,:\t");/*we take what after it*/
						if(!token) /*if there is nothing then it an error*/
						{
							printf("Error in file: %s, line: %d\nthere is no operands after .entry\n", filename, line);
							error = YES;
						}
						/*checking if the token is a lable that was defined in the file, if no we get an error, if yes we update the entry feild of it in the symbols table to YES with the help of the function add_entry*/
						else {
							if(add_entry(token, s_head, line, filename) == NO)	
							{
								printf("Error in file: %s, line: %d\nthe lable %s after .entry wasnt defined in the file\n", filename, line, token);
								error = YES;
							}
							token = strtok(NULL," ,\t");
							if(token)/*asking if there is more than one operand after .entry*/
							{
								printf("Error in file: %s, line: %d\nthere is more than one operand after .entry\n", filename, line);
								error = YES;
							}
						}
					}
					/*assembly directives*/
					else{
						op = (find_op(token));/*we set op to be the operation(token) info*/
						result = result | op->num;/*we put the operation number into the word*/
						result = result << BIT;/*moving it to the left 4 times, to get place for the first operand addressing mode*/
						sum = op->sum;/*we put in sum how many operands this operation gets*/
						token = strtok(NULL, " ,:\t");/*we take the first operand*/
						
						
						if(sum == ZERO)/*if the operation doesnt get any operands*/
						{
							result = result << (BIT + BIT - 1);/*then we move it to the left 7 times, to put the ARE, cause there is no operands*/
							set_addressing_mode_and_A_R_E(&result, A);/*putting A cause its the first word of the instruction*/
							add_to_address_table(code_head, result, NO);/*adding it to the code table*/
						}
						else if(sum == ONE)/*if the operation gets one operand*/
						{
							is_extern = NO;/*initialising extern to NO in case the operand was a lable*/
							result = result << BIT;/*moving it to the left 4 more times cause there is no source operand*/
							add_mode = get_addressing_mode(token);/*getting the addressing mode of the operand*/
							set_addressing_mode_and_A_R_E(&result, add_mode);/*putting the addresing mode in the word*/
							result = result << (BIT - 1);/*moving it 3 times, to get place for the ARE*/
							set_addressing_mode_and_A_R_E(&result, A);/*putting A cause its the first word of the instruction*/
							if(add_mode == DIRECT)/*if the operand is lable*/
							{
								info = get_address_from_symbols_table(s_head, token, &is_extern);/*getting the address of the lable from the symbols table, if its not found we get NOT_KNOWN(-1)*/
								if(info == NOT_KNOWN)/*if the lable wasnt defined in the file*/
								{
									printf("Error in file: %s, line: %d\nthe lable %s that was entered as an operand wasnt defined in the file\n", filename, line, token);
									error = YES;
								}
								else{
									/*putting the address of the lable into the second word of the instruction(machine code)*/
									set_mode_info(&operand1, info, add_mode, YES);
									/*if the lable is not extern we put R into the word*/
									if(is_extern == NO) set_addressing_mode_and_A_R_E(&operand1, R);
									else 
									{
										/*if the lable is extern we put E into the word*/
										set_addressing_mode_and_A_R_E(&operand1, E);
										/*and we add it to the extern table, with the address(IC) of the second word*/
										add_to_ext_table(ext_head, token, IC + 1);
									}
								}
							}
							else{
								/*getting the numbers we need from the operand according to the addresing modes*/
								info = get_info(token);
								
								set_mode_info(&operand1, info, add_mode, YES);
								/*setting the ARE to A*/
								set_addressing_mode_and_A_R_E(&operand1, A);
								/*if the info was negative we change the last bit(number 16) to 0*/
								if(info < 0) operand1 = operand1 & ZERO_TO_LAST;
							}
							/*adding the two words to the code table*/
							add_to_address_table(code_head, result, NO);
							add_to_address_table(code_head, operand1, NO);
						}
						else if(sum == TWO)/*if the operation gets two operands*/
						{
							sum = ZERO;/*to count how many times we enter the loop*/
							while(token && error == NO)/*going through the two operands*/
							{
								sum++;/*counting an operand*/
								is_extern = NO;/*initialising is_extern to no*/
								add_mode = get_addressing_mode(token);/*getting the addressing mode of the operand*/
								set_addressing_mode_and_A_R_E(&result, add_mode);/*putting the addresing mode into the word*/
								/*if this is the source operand we move 4 bits to make place for the second operand*/
								if(sum == ONE) result = result << BIT;
								else{/*else this is the destination operand so we move 3 bits to make place for ARE*/
									result = result << (BIT - 1);
									set_addressing_mode_and_A_R_E(&result, A);
								}
								if(add_mode == DIRECT_REGISTER || add_mode == INDIRECT_REGISTER)/*counting id its register*/
									reg++;
								if(add_mode == DIRECT)/*if the addressing mode was number 1*/
								{
									/*getting the address of the lable from the symbols table, if its not found we get NOT_KNOWN(-1)*/
									info = get_address_from_symbols_table(s_head, token, &is_extern);
									if(info == NOT_KNOWN)/*if the lable wasnt defined in the file*/
									{
										printf("Error in file: %s, line: %d\nthe lable %s that was entered as an operand wasnt defined in the file\n", filename, line, token);
										error = YES;
									}
									else/*the lable was defined in the file*/
									{
										if(sum == ONE)/*if this the source operand*/
										{
											/*putting the address of the lable into the second word of the instruction(machine code)*/
											set_mode_info(&operand1, info, add_mode, NO);
											/*if the lable is not extern we put R into the word*/
											if(is_extern == NO) 
												set_addressing_mode_and_A_R_E(&operand1, R);
											/*if the lable is extern we put E into the word*/
											else set_addressing_mode_and_A_R_E(&operand1, E);
										}
										else{
											/*putting the address of the lable into the third word of the instruction(machine code)*/
											set_mode_info(&operand2, info, add_mode, YES);
											/*if the lable is not extern we put R into the word*/
											if(is_extern == NO) 
												set_addressing_mode_and_A_R_E(&operand2, R);
											/*if the lable is extern we put E into the word*/
											else set_addressing_mode_and_A_R_E(&operand2, E);
										}
										/*if the lable is extern, we add it to the extern table with the suitable address(according if its the first or the second operand) */
										if(is_extern == YES) 
											add_to_ext_table(ext_head, token, IC + sum);
									}	
								}	
								else{	/*the addressing mode is 0, 2 or 3*/
									/*getting the numbers we need from the operand according to the addressing mode*/
									info = get_info(token);
									if(sum == ONE) /*if this is the source operand*/
									{
										/*putting the the info we got into the second word of the instruction(machine code)*/
										set_mode_info(&operand1, info, add_mode, NO);
										/*setting the ARE to A*/
										set_addressing_mode_and_A_R_E(&operand1, A);
										/*if the info was negative we change the last bit(number 16) to 0*/
										if(info < 0) operand1 = operand1 & ZERO_TO_LAST;
									}
									else if(reg == TWO)/*if the two operands are registers*/
									{
										/*putting the the second operand info into the second word of the instruction(machine code)*/
										set_mode_info(&operand1, info, add_mode, YES);
										/*setting the ARE to A*/
										set_addressing_mode_and_A_R_E(&operand1, A);
										/*if the info was negative we change the last bit(number 16) to 0*/
										if(info < 0) operand1 = operand1 & ZERO_TO_LAST;
									}
									else{/*this is the destination operand*/
										/*putting the the second operand info into the third word of the instruction(machine code)*/
										set_mode_info(&operand2, info, add_mode, YES);
										/*setting the ARE to A*/
										set_addressing_mode_and_A_R_E(&operand2, A);
										/*if the info was negative we change the last bit(number 16) to 0*/
										if(info < 0) operand2 = operand2 & ZERO_TO_LAST;
									}
								}
								token = strtok(NULL, " ,:\t");
							}		
							/*adding the first two words to the code table*/
							add_to_address_table(code_head, result, NO);
							add_to_address_table(code_head, operand1, NO);	
							/*if there was third word we add it (if the two operands were registers we wont have a third word)*/
							if(operand2)
								add_to_address_table(code_head, operand2, NO);
						}
					}
				}	
			}
		}
		/*if the error_all is NO that means there wasn't any errors yet in the file. so we put the result of the current line, if its YES there is no need to update.*/
		if(*error_all == NO)
			*error_all = error;
	}
}
