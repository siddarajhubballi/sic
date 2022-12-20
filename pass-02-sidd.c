#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 100

int commaflag = 0;
char binary[100] = "";
int val;
char text_record[200];
int text_length = 0;
int empty = 0, emp_con=0;
char starting_text_addr[100] = "";
char starting_address[10] = ""; 
int program_length;
int start, end;
int nextFlag = 0;

// structure to store opcodes and their hex values
typedef struct sym {
    char symbol[MAX_SIZE];
    char addr[MAX_SIZE];
}SYMBOL;

// structure to store opcodes and their hex values
typedef struct op {
    char opcode[MAX_SIZE];
    char opnum[MAX_SIZE];
}OPCODE;

// Array in which all opcodes and their hex values are read from optab
OPCODE opcodes[MAX_SIZE];
int op_count = 0;

SYMBOL symbols[MAX_SIZE];
int sym_count = 0;

typedef struct instruction {
    char addr[20];
    char label[20];
    char opcode[20];
    char operand[20];
}INSTRUCTION;

// Initial function to read all opcodes and their hex values into the opcodes from optab.txt
void readOpcodes() {
    FILE* fptr;
    char line[100];
    char* token;
    char opcode1[20];
    char opnum1[20];

    fptr = fopen("optab.txt", "r+");

    if(fptr == NULL) {
        printf("Unable to open optab.txt\n");
        exit -1;
    }

    while(fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = 0;
        token = strtok(line, " ");
        strcpy(opcodes[op_count].opcode, token);
        while(token != NULL) {
            strcpy(opcodes[op_count].opnum, token);
            token = strtok(NULL, " ");
        }
        // printf("%s %s\n", opcodes[op_count].opcode, opcodes[op_count].opnum);
        op_count++;
    }
    fclose(fptr);
}

void readSymbols() {
    FILE* fptr;
    char line[100];
    char* token;
    char opcode1[20];
    char opnum1[20];

    fptr = fopen("symtab.txt", "r+");

    if(fptr == NULL) {
        printf("Unable to open symtab.txt\n");
        exit -1;
    }

    while(fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = 0;
        token = strtok(line, " ");
        strcpy(symbols[sym_count].addr, token);
        while(token != NULL) {
            strcpy(symbols[sym_count].symbol, token);
            token = strtok(NULL, " ");
        }
        // printf("%s %s\n", symbols[sym_count].symbol, symbols[sym_count].addr);
        sym_count++;
    }
    fclose(fptr);
}

// function used to count number of spaces in an instruction (type of instruction)
int countSpaces(char* s) {
    int i=0,spaces=0;
    while(s[i] != '\0') {
        if(s[i] == ' ') {
            spaces++;
        }
        i++;
    }
    return spaces;
}

INSTRUCTION get_tokens(char* line) {
    int spaces = countSpaces(line);
    INSTRUCTION ins;
    strcpy(ins.addr, " ");
    strcpy(ins.label, " ");
    strcpy(ins.opcode, " ");
    strcpy(ins.operand, " ");

    char* token = strtok(line, " ");
    if(spaces == 5) {                 // for one space use 5
        strcpy(ins.addr, token);
        while(token != NULL) {
            strcpy(ins.opcode, token);
            token = strtok(NULL, " ");
        }
    }
    else if(spaces == 4) {           // for 2 space use 4
        int i = 0;
        char tokens[3][200];
        while(token != NULL) {
            strcpy(tokens[i], token);
            token = strtok(NULL, " ");
            i++;    
        }
        strcpy(ins.addr, tokens[0]);
        strcpy(ins.opcode, tokens[1]);
        strcpy(ins.operand, tokens[2]);
    }
    else if(spaces == 3) {          // for 3 space use 3
        int i = 0;
        char tokens[4][200];
        while(token != NULL) {
            strcpy(tokens[i], token);
            token = strtok(NULL, " ");
            i++;
        }
        strcpy(ins.addr, tokens[0]);
        strcpy(ins.label, tokens[1]);
        strcpy(ins.opcode, tokens[2]);
        strcpy(ins.operand, tokens[3]);
    }
    // printf("%s\n", ins.addr);
    return ins;
}

char* getValueX(char* operand, char* temp) {
    int k,l=0;
    for(k=2;k<strlen(operand)-1;k++) {
        temp[l++] = operand[k];
    }
    temp[l] = '\0';
}

char* getValueC(char* operand, char* temp) {
    int k,l=0,ascii,x;
    char val[2];
    strcpy(temp, "");
    for(k=2;k<strlen(operand)-1;k++) {
        ascii = operand[k];
        sprintf(val, "%X", ascii);
        strcat(temp,val);
        l += 2;
    }
}

void getWORD(char* operand, char* temp) {
    int word = strtol(operand, NULL, 10);
    char temp2[10]="";
    int len,l=0;
    sprintf(temp, "%X", word);
    len = strlen(temp);
    for(int i=6;i>len;i--) {
        temp2[l++] = '0';
    }
    strcat(temp2, temp);
    strcpy(temp, temp2);
}

void checkComma(char* operand) {
    int len = strlen(operand);
    int i,flag = 0;
    for(i=0;i<len;i++) {
        if(operand[i] == ',') {
           flag = 1;
           break;
        }       
    }
    if(flag == 1) {
        commaflag = 1;
    }
    else {
        commaflag = 0;
    }
    // printf("%d\n", commaflag);
}

void decimal_to_binary(char* hexNum, char* binary) {
    int count = 0;
    while(hexNum[count])
	{
		switch(hexNum[count])
		{
			case '0' : strcat(binary, "0000");
				break;
			case '1' : strcat(binary, "0001");
				break;
			case '2' : strcat(binary, "0010");
				break;
			case '3' : strcat(binary, "0011");
				break;
			case '4' : strcat(binary, "0100");
				break;
			case '5' : strcat(binary, "0101");
				break;
			case '6' : strcat(binary, "0110");
				break;
			case '7' : strcat(binary, "0111");
				break;
			case '8' : strcat(binary, "1000");
				break;
			case '9' : strcat(binary, "1001");
				break;
			case 'A' : strcat(binary, "1010");
				break;
            case 'B' : strcat(binary, "1011");
                break;
            case 'C' : strcat(binary, "1100");
                break;
            case 'D' : strcat(binary, "1101");
                break;
            case 'E' : strcat(binary, "1110");
                break;
            case 'F' : strcat(binary, "1111");
                break;
            case 'a' : strcat(binary, "1010");
                break;
			case 'b' : strcat(binary, "1011");
				break;
			case 'c' : strcat(binary, "1100");
				break;
			case 'd' : strcat(binary, "1101");
				break;
			case 'e' : strcat(binary, "1110");
				break;
			case 'f' : strcat(binary, "1111");
				break;
			default : printf("\nInvalid Entry, Please Try Again  %c",hexNum[count]);
		}
		count++;
	}
}

FILE* f4;

int main() {

    readOpcodes();
    readSymbols();

    FILE* f1 = fopen("intermediate.txt", "r+");
    FILE* f2 = fopen("objectprogram.txt", "w+");
    FILE* f3 = fopen("symtab.txt", "r+");
    f4 = fopen("output.txt", "w+"); 
    if(f1 == NULL) {
        printf("Error opening intermediate.txt file\n");
        exit -1;
    }
    if(f2 == NULL) {
        printf("Error opening objectprogram.txt file\n");
        exit -1;
    }
    if(f3 == NULL) {
        printf("Error opening symtab.txt file\n");
        exit -1;
    }

    int k,l,len;
    char line[100], temp[100];
    INSTRUCTION ins;
    char* obj_code;

    fgets(line, sizeof(line), f1);
    ins = get_tokens(line);
    if(strcmp(ins.opcode, "START") == 0) {     
        strcpy(starting_address, ins.addr);        // write header record
        fprintf(f2, "H^%6s^%6s^%6s\n", ins.label, ins.addr, "0000");
    }
    else {
        strcpy(starting_address, "0000"); 
    }
    fprintf(f2, "T^%2s", ins.addr);      //initialize first text record

    while(fgets(line, sizeof(line), f1)) {
        empty = 0;
        line[strcspn(line, "\n")] = 0;
        if(line[0] == '.') {      // checking for comment line
            printf("%s\n", line);
            continue;
        }
        ins = get_tokens(line);
        if(strcmp(ins.opcode, "END") == 0 || strcmp(ins.opcode, "BASE") == 0 ) {
            continue;
        }
        char op_value[10];
        int i,flag=0;
        for(i=0;i<op_count;i++) {
            if(strcmp(ins.opcode, opcodes[i].opcode)==0) {
                flag = 1;
                strcpy(op_value, opcodes[i].opnum);
                break;
            }
        }
        char operand[10], operand_address[10];
        strcpy(operand, ins.operand);

        if(flag == 1) {                      // opcode present in optab
            int flag2 = 0;
            for(i=0;i<sym_count;i++) {
                if(strcmp(operand, symbols[i].symbol) == 0) {
                    flag2 = 1;
                    strcpy(operand_address, symbols[i].addr);
                    break;
                }
            }
            if(flag2 == 0) {                // symbol not present in operand
                strcpy(operand_address, "0000");
            }

            checkComma(ins.operand);          //index addressing : for examples like BUFFER,X
            if(commaflag == 1) {
                operand[strcspn(operand, ",")] = 0;
                for(i=0;i<sym_count;i++) {
                if(strcmp(operand, symbols[i].symbol) == 0) {
                    flag2 = 1;
                    strcpy(operand_address, symbols[i].addr);
                    break;
                }
                
            }
                strcpy(binary, "");
                decimal_to_binary(op_value, binary);
                decimal_to_binary(operand_address, binary);
                binary[8] = '1';
                val = strtol(binary, NULL, 2);
                sprintf(binary, "%X", val);
                strcpy(obj_code, binary);
            }
            else {                         // non-index addressing
                strcpy(obj_code, op_value);
                strcat(obj_code, operand_address);   
            }
            
            
        }
        else if(strcmp(ins.opcode, "BYTE") == 0) {
            strcpy(obj_code, "");
            if(ins.operand[0] == 'X') {
                getValueX(ins.operand, temp);
                strcpy(obj_code, temp);
            }
            else if(ins.operand[0] == 'C') {
                getValueC(ins.operand, temp);
                strcpy(obj_code, temp);
            }
        
        }
        else if(strcmp(ins.opcode, "WORD") == 0) {
            getWORD(ins.operand, temp);
            strcpy(obj_code, temp);
        }
        else {
            strcpy(obj_code, " ");
            empty = 1;
        }
        
        if(empty == 1 && emp_con == 1) {
            nextFlag = 1;
        }
        else if(empty == 1) {
            fprintf(f2, " %s^%2X%s",starting_text_addr, text_length, text_record);
            fprintf(f2, "\nT^ ");
            strcpy(starting_text_addr, ins.addr);
            strcpy(text_record, "");
            text_length = 0;
            emp_con = 1;
            nextFlag = 0;
        }
        else if((text_length+strlen(obj_code)/2) > 30) {
            fprintf(f2, " %s^%2X%s",starting_text_addr, text_length, text_record);
            fprintf(f2, "\nT^ ");
            strcpy(starting_text_addr, ins.addr);
            strcpy(text_record, "");
            text_length = 0;
            emp_con = 0;
            strcat(text_record, "^");
            strcat(text_record, obj_code);
            nextFlag = 0;
        }
        else {
            if(nextFlag == 1) {
                strcpy(starting_text_addr, ins.addr);
            }
            strcat(text_record, "^");
            strcat(text_record, obj_code);
            emp_con = 0;
            nextFlag = 0;
        }
        
        text_length += strlen(obj_code)/2;
        printf("%-10s %-10s %-10s %-10s %s\n",ins.addr, ins.label, ins.opcode, ins.operand, obj_code);
        fprintf(f4, "%-10s %-10s %-10s %-10s %s\n",ins.addr, ins.label, ins.opcode, ins.operand, obj_code);
    }

    // Write the last text record into objectprogram.txt
    fprintf(f2, " %s^%2X%s\n",starting_text_addr, text_length, text_record);  
    // Write the End record into objectprogram.txt
    fprintf(f2, "E^%6s\n",starting_address);

    start = strtol(starting_address, NULL, 16);
    end = strtol(ins.addr, NULL, 16);
    fseek(f2, 0, SEEK_SET);
    fseek(f2, 18, SEEK_CUR);
    fprintf(f2, "%X", end - start);
    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);
    return 0;
}