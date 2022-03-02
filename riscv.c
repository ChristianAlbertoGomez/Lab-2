#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file

int32_t* reg; // Array of 32 32-bit registers

char* file_name = "mem.txt";

void init_regs();
bool interpret(char* instr);
void write_read_demo();
int converterAtoi(char* command);
int findCommand(char* command);
int compareCommand(char* command,char* inputUser);
int offsetValue(char* string);
int baseValue(char* string);
char* get_baseToken(char* instr);

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
  char delimetersTwo[] = {"SWORD,LW () X"};
  char baseDelim[] = {"SWORD,LW"};

  char command[strlength(instr)];

  int i;
  for(i=0;i<strlength(instr);i++){
    command[i] = instr[i];
  }


  char* token = strtok(command,delimeters);

  long int rs1;
  long int rs2;
  long int rd;
  long int base;
  long int address;

  if(findCommand(instr)==1){
     printf("This is ADD instruction: \n");

     rd = converterAtoi(token);

     token = strtok(NULL,delimeters);
     rs1 = converterAtoi(token);

     token = strtok(NULL,delimeters);
     rs2 = converterAtoi(token);

     long int total = rs1+rs2;

      reg[rd] = total;


     printf("Total of sum is:%ld \n",total);
     print_regs();

     return true;

   }else if(findCommand(instr)==2){
     printf("This is ADDI instructions \n");

     rd = converterAtoi(token);

     token = strtok(NULL,delimeters);
     rs1 = converterAtoi(token);

     token = strtok(NULL,delimeters);
     rs2 = converterAtoi(token);

     long int total = rs1+rs2;
     reg[rd] =  total;

     printf("Total of sum is:%ld \n",total);
     print_regs();
     return true;

   }else if(findCommand(instr)==3){
     printf("This is LW instructions:  \n");

     char* lwToken = strtok(instr,delimetersTwo);

     rd = converterAtoi(lwToken);
     printf("%d \n",rd);

     lwToken = strtok(NULL,delimetersTwo);
     long int offset = atoi(lwToken)*4;
     printf("%s:%ld \n",lwToken,offset);

     lwToken = strtok(NULL,delimetersTwo);
     base = atoi(lwToken);
     printf("%s:%ld \n",lwToken,base);

     address = base+offset;
     reg[rd] = read_address(address,file_name);

     print_regs();
     return true;

   }else if(findCommand(instr)==4){
     printf("This is SW instructions \n");
     char* swToken = strtok(instr,delimetersTwo);

     rd = converterAtoi(swToken);
     printf("%d \n",rd);

     swToken = strtok(NULL,delimetersTwo);
     long int offset = atoi(swToken)*4;
     printf("%s:%ld \n",swToken,offset);

     swToken = strtok(NULL,delimetersTwo);
     base = atoi(swToken);
     printf("%s:%ld \n",swToken,base);

     address = base+offset;
     write_address(rd,address,file_name);

     print_regs();
     return true;

   }else{
     printf("Nothing \n");
     return false;
   }
}

//FIX THIS METHOD
char* get_baseToken(char* instr){
  char baseDelim[] = {"SW,LW"};
  char command[strlength(instr)];

  int i;
  for(i=0;i<strlength(instr);i++){
    command[i] = instr[i];
  }

  char* baseToken = strtok(command,baseDelim);

  baseToken = strtok(NULL,baseDelim);

  return baseToken;
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
       }//1
      if(compareCommand("ADDI",*temp_tokenizer)==1){
          return 2;
       }//2
      if(compareCommand("LW",*temp_tokenizer)==1){
          return 3;
       }//3
      if(compareCommand("SWORD",*temp_tokenizer)==1){
          return 4;
       }//4
      else{
          return 0;
       }//else
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

int offsetValue(char* string){
   int length = strlength(string);
   int i=0;
   int j;
   int immediateValue;

   while(string[i] != '('){
     i++;
   }

   char immediate[length-i];

   for(j=0;j<(length-i);j++){
     immediate[j] = string[j];
   }
    immediateValue = atoi(immediate);
    return immediateValue;
}
int baseValue(char* string){
  int length = strlength(string);
  int i=0;
  int j;
  int immediateValue;

   while(string[i]!='X'){
     i++;
   }
     i++;

   char* immediate = (char*)malloc(length-1);

   for(j=0;j<(length-i);j++){
       immediate[j] = string[j-i];
    }
    immediateValue = atoi(immediate);
    return immediateValue;
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
