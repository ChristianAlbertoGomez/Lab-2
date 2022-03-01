#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}

/**
 * Prints all 32 registers in column-format
 */
void print_regs(){
    int col_size = 10;
    for(int i = 0; i < 8; i++){
        printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
        printf(" X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
        printf(" X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
        printf(" X%02i:%.*lld\n", i+24, col_size, (long long int) reg[i+24]);
    }
}

/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
bool interpret(char* instr){
  char delimeters[] = {"ADD,X ADDI"};
  char command[strlength(instr)];

  int i;
  for(i=0;i<strlength(instr);i++){
    command[i] = instr[i];
  }


  char* token = strtok(command,delimeters);
  long int rs1;
  long int rs2;
  char* mem_file = "mem.txt";

  if(findCommand(instr)==1){
     printf("This is ADD instruction: \n");

     token = strtok(NULL,delimeters);
     rs1 = converterAtoi(token);

     token = strtok(NULL,delimeters);
     rs2 = converterAtoi(token);

     long int total = rs1+rs2;
     printf("Total of sum is:%ld \n",total);

   }else if(findCommand(instr)==2){
     printf("This is ADDI instructions \n");

     token = strtok(NULL,delimeters);
     rs1 = converterAtoi(token);

     token = strtok(NULL,delimeters);
     rs2 = converterAtoi(token);

     long int total = rs1+rs2;
     printf("Total of sum is:%ld \n",total);

   }else{
     printf("Nothing \n");
   }
}

int converterAtoi(char* command){

 long int number = atoi(command);

 return number;
}

/*EXTRA METHOD JUST TO CHECK*/
int findCommand(char* command){

     char delimeter = ',';
     char** temp_tokenizer = tokenize(command,delimeter);

     while(*temp_tokenizer){
      if(compareCommand("ADD",*temp_tokenizer)==1){
        return 1;
      }else if(compareCommand("ADDI",*temp_tokenizer)==1){
        return 2;
      }else{
        return 0;
      }
      temp_tokenizer++;
     }
}

int compareCommand(char* command,char* inputUser){
   int i;

   for(i=0;command[i]!='\0'&&inputUser[i]!='\0';i++);
     if(command[i]<inputUser[i]){
       return 0;
     }
     else if(command[i]>inputUser[i]){
       return 0;
     }
     else{
       return 1;
     }
}


/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
	int32_t data_to_write = 0xFFF; // equal to 4095
	int32_t address = 0x98; // equal to 152
	char* mem_file = "mem.txt";

	// Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
	int32_t write = write_address(data_to_write, address, mem_file);
	if(write == (int32_t) NULL)
		printf("ERROR: Unsucessful write to address %0X\n", 0x40);
	int32_t read = read_address(address, mem_file);

	printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}

/**
 * Your code goes in the main
 *
 */
int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE

	// Below is a sample program to a write-read. Overwrite this with your own code.
	write_read_demo();

        print_regs();

        //THIS IS MY PART
        int maxLength = 100;
        char command[maxLength];
        char delimeter;

        printf("Please enter a command: \n");
        fgets(command,maxLength,stdin);

        printf("Please enter your delimeter: \n");
        delimeter = getchar();

        char** temp_tokenize =  tokenize(command,delimeter);
        print_all_tokens(temp_tokenize);

        interpret(command);



	return 0;
}
