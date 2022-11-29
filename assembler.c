/*If there is a lable defenition and after it nothing I chose to print an ERROR*/
/*If there is a lable defenition and after it .entry or .extern I chose to just skip the lable and continue reading the line*/

#include "info.h"
#include "assembler.h"

/*extern variables that we declaired about in info.h*/
int IC;
int DC;
operations_table *op_head;

int main(int argc, char *argv[])
{
	FILE *file;
	char *filename, *filename2;/*variables to store the files names we want to open*/
	/*counter to the loop that goes through all the files that were entered as arguments of the command line*/
	int i = 1;
	/*the head of the symbols table*/
	symbols_table *s_head = NULL;	
	/*the gead of the data table*/
	address_table *data_head = NULL;
	/*the head of the code table*/
	address_table *code_head = NULL;
	/*the head of the externs list*/
	externs *ext_head = NULL;
	/*we send it to first and second read, if the first read set it as YES we dont continue to second read, and if second read set it as YES we dont create the files for printing*/
	boolean error = NO;

	/*asking if there is at least one file name enterd*/
	if(argc > 1)
	{		
		op_info();/*we make the operations info list if there one file at least that was sent to main.*/
		while(i < argc)/*going through the files names*/
		{
			/*setting IC and DC to default each file*/
			IC = IC_DEFAULT;
			DC = DC_DEFAULT;

			/*editing the file name (adding .as)*/
			filename = creat_name(argv[i], ".as", strlen(argv[i]));

			if (!(file = fopen(filename,"r")))
			{
				printf("cannot open file: %s\n", filename);
				exit(0);
			}

			/*first read of the file*/
			first_read(file, &s_head, &data_head, &error, filename);

			/*if there werent any errors found in the first read*/
			if(error == NO)
			{
				/*updating the addresses of the data in the symbole table(if the is_data is YES)and the data table so they come after the code data*/
				update_data_add(s_head, data_head);
				IC = IC_DEFAULT;/*setting IC to its starting value*/
				rewind(file);/*going back to the start of the file*/

				/*second read of the file*/
				second_read(file, &s_head, &code_head, &ext_head, &error, filename);
			}

			fclose(file);/*closing the file*/
			free(filename);/*freeing the file name*/

			/*if there werent any errors found in the second read*/
			if(error == NO)
			{
				/*editing the name of the file(adding .ob)*/
				filename = creat_name(argv[i], ".ob", strlen(argv[i]));
				/*sending the tables to print them to the ob file*/
				print_ob(filename, code_head, data_head);
				/*freeing the file name*/
				free(filename);
	
				/*editing the name of the files(adding .ent and .ext)*/
				filename = creat_name(argv[i], ".ent", strlen(argv[i]));
				filename2 = creat_name(argv[i], ".ext", strlen(argv[i]));
				/*sending the tables to print them to the ent and ext files*/
				print_ent_ext(s_head, ext_head, filename, filename2);
				/*freeing the two file names*/
				free(filename);
				free(filename2);
			}
			/*freeing all the dynamic allocations that we did(exept the operation table)*/
			free_all(s_head, code_head, data_head, ext_head);
			i++;
		}
		free_operations_table();/*freeing the operation table if there is no more arguments left*/
	}
	/*There is no arguments passed.	*/
	else
		printf("0 arguments passed\n");
	 return 0;
}
