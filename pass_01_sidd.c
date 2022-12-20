// including required header files
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// maximum size limit
#define MAX_SIZE 100

// structure to store opcodes and their hex values
typedef struct op {
    char opcode[MAX_SIZE];
    char opnum[MAX_SIZE];
}OPCODE;

// structure to store instructions 
typedef struct instruction {
    char label[MAX_SIZE];
    char opcode[MAX_SIZE];
    char operand[MAX_SIZE];
}INSTRUCTION;

// Array in which all opcodes and their hex values are read from optab
OPCODE opcodes[MAX_SIZE];
int op_count = 0;
int locctr = 0;
int startingAddress = 0;

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

// Initial function to read all opcodes and their hex values into the opcodes from optab.txt
void readOpcodes() {
    FILE* fptr;
    char line[100];
    char* token;

    fptr = fopen("optab.txt", "r+");

    if(fptr == NULL) {
        printf("Unable to open optab.txt\n");
        exit -1;
    }

    while(fgets(line, sizeof(line), fptr)) {
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

//reading instruction and getting tokens from it into instruction structure
INSTRUCTION read_tokens(char* line) {
    int spaces = countSpaces(line);
    INSTRUCTION ins;
    strcpy(ins.label, " ");
    strcpy(ins.opcode, " ");
    strcpy(ins.operand, " ");
    
    if(spaces == 0) {
        strcpy(ins.opcode, line);
        return ins;
    }

    char* token = strtok(line, " ");

    if(spaces == 1) {
        strcpy(ins.opcode, token);
        while(token != NULL) {
            strcpy(ins.operand, token);
            token = strtok(NULL, " ");
        }
    }
    else if(spaces == 2) {
        int i = 0;
        char tokens[3][200];
        while(token != NULL) {
            strcpy(tokens[i], token);
            token = strtok(NULL, " ");
            i++;
        }
        strcpy(ins.label, tokens[0]);
        strcpy(ins.opcode, tokens[1]);
        strcpy(ins.operand, tokens[2]);
    }
    else {
        printf("\nInvalid instruction\n");
        exit -1;
    }
    return ins;
}

int main() {

    char line[100];
    char labels[MAX_SIZE][100];
    int label_count = 0;

    readOpcodes();

    FILE* f1 = fopen("input.txt", "r+");
    FILE* f2 = fopen("intermediate-view.txt", "w+");
    FILE* f4 = fopen("intermediate.txt", "w+");
    FILE* f3 = fopen("symtab.txt", "w+");
    
    if(f1 == NULL) {
        printf("Unable to open input.txt file\n");
        exit -1;
    }
    if(f2 == NULL) {
        printf("Unable to open intermediate.txt file\n");
        exit -1;
    }
    if(f3 == NULL) {
        printf("Unable to open symtab.txt file\n");
        exit -1;
    }
    if(f4 == NULL) {
        printf("Unable to open intermediate.txt file\n");
        exit -1;
    }

    INSTRUCTION ins;
    int line_count = 0;
    while(fgets(line, sizeof(line), f1)) {
        line[strcspn(line, "\n")] = 0;
        if(line[0] == '.') {      // checking for comment line
            printf("%s\n", line);
            continue;
        }
        line_count++;
        ins = read_tokens(line);

        if(strcmp(ins.opcode, "START") == 0) {
            locctr = strtol(ins.operand, NULL, 16);
            startingAddress = strtol(ins.operand, NULL, 16);
        }

        int flag = 0;
        if(strcmp(ins.label," ") == 1) {
            for(int i=0;i<label_count;i++) {
                if(strcmp(labels[i], ins.label) == 0) {
                    printf("Duplicate label at line %d\n", line_count);
                    flag = 1;
                    break;
                }
            }
            if(flag == 1) {
                continue;
            }
            if(flag == 0) {
                fprintf(f3,"%-10X %-10s\n",locctr, ins.label);
                strcpy(labels[label_count++], ins.label);
            }
        }
        
        printf("%-10X %-10s %-10s %-10s\n", locctr, ins.label, ins.opcode, ins.operand);
        fprintf(f2,"%-10X %-10s %-10s %-10s\n", locctr, ins.label, ins.opcode, ins.operand);
        fprintf(f4,"%X %s %s %s\n", locctr, ins.label, ins.opcode, ins.operand);

        int flag2 = 0;
        for(int i=0;i<op_count;i++) {
            if(strcmp(ins.opcode, opcodes[i].opcode) == 0) {
                locctr += 3;
                flag2 = 1;
                break;
            }
        }
        if(flag2 == 1) {
            continue;
        }
        if(strcmp(ins.opcode, "WORD") == 0) {
            locctr += 3;
        }
        else if(strcmp(ins.opcode, "RESW") == 0) {
            locctr += (3 * strtol(ins.operand, NULL, 16));
        }
        else if(strcmp(ins.opcode, "RESB") == 0) {
            locctr = locctr + strtol(ins.operand, NULL, 10);
        }
        else if(strcmp(ins.opcode, "BYTE") == 0) {
            if(ins.operand[0] == 'X') {
                locctr = locctr + ((strlen(ins.operand)-3)/2);
            }
            else if(ins.operand[0] == 'C') {
                locctr = locctr + (strlen(ins.operand)-3);
            }
        }
        else if(strcmp(ins.opcode, "END") == 0 || strcmp(ins.opcode, "START") == 0 || strcmp(ins.opcode, "BASE") == 0) {

        }
        else {
            locctr += 3;
        }

    }

    fclose(f1);
    fclose(f2);
    fclose(f3);
    printf("\nLength of the program = %X\n\n", locctr-startingAddress);  
    return 0;
}