/**
 * @file parser.c
 * @brief Contains functions used to execute commands parsing.
*/

#include "parser.h"
#include <stdlib.h>
#include <string.h>

#define NDEBUG

#ifndef NDEBUG
#include <stdio.h>
#endif

const uint N_COMMANDS = 5;
const char COMMANDS[][20] = {
    "aggiungi-stazione",
    "demolisci-stazione",
    "aggiungi-auto",
    "rottama-auto",
    "pianifica-percorso"
};
command_type COMMANDS_CODING[] = {
    add_station_command,
    delete_station_command,
    add_car_command,
    remove_car_command,
    plan_path_command
};


instruction * parse_instruction_separator(const char * command, char separator) {
    #ifndef NDEBUG
    printf("Starting parse_instruction\n");
    #endif

    if(command == NULL) {
        #ifndef NDEBUG
        printf("\tNULL command\n");
        #endif

        return NULL;
    }

    uint length = strlen(command);

    instruction * instr = (instruction *) malloc(sizeof(instruction));
    if(instr == NULL) {
        #ifndef NDEBUG
        printf("\tNot enough space to allocate instruction of size %ld\n", sizeof(instruction));
        #endif

        return NULL;
    }

    instr->command = no_command;
    instr->params = NULL;
    instr->params_length = 0;

    uint num_params = 1;
    uint sep_index = 0;

    char tmp[length];

    #ifndef NDEBUG
    printf("\tStarting separator search\n");
    #endif

    for(uint i = 0; i < length; ++i) {
        if(command[i] == separator) {
            if(num_params == 1) {
                
                #ifndef NDEBUG
                printf("\tFirst separator found at index %d\n", i);
                #endif

                sep_index = i;

                #ifndef NDEBUG
                printf("\tCopying command -> ");
                #endif
                for(int j = 0; j < i; ++j) {
                    tmp[j] = command[j];
                    #ifndef NDEBUG
                    printf("%c", command[j]);
                    #endif
                }
                tmp[i] = '\0';
                #ifndef NDEBUG
                printf("\n");
                #endif

                for(uint j = 0; j < N_COMMANDS; ++j) {
                    if(strcmp(tmp, COMMANDS[j]) == 0) {
                    #ifndef NDEBUG
                    printf("\tCommand = %s, command_code = %d\n", tmp, COMMANDS_CODING[j]);
                    #endif

                    instr->command = COMMANDS_CODING[j];
                    }
                }
            }
           
           ++num_params;
        }
    }

    if(instr->command == no_command) {
        #ifndef NDEBUG
        printf("\tCommand not codified\n");
        #endif

        return instr;
    }

    instr->params_length = num_params - 1;

    if(num_params == 1) {
        #ifndef NDEBUG
        printf("\tNo parameters found\n");
        #endif

        return instr;
    }

    uint * params = (uint *) malloc(sizeof(uint) * (num_params - 1));
    if(params == NULL) {
        #ifndef NDEBUG
        printf("\tNot enough space to allocate array of %ld bytes\n", sizeof(uint) * (num_params - 1));
        #endif

        delete_instruction(instr);
        return NULL;
    }

    #ifndef NDEBUG
    printf("\tAllocated array of %ld bytes\n", sizeof(uint) * (num_params - 1));
    printf("\tExtracting %d parameters\n", num_params - 1);
    #endif

    uint j = 0;
    int conv = 0;
    
    for(uint i = sep_index + 1; i < length; ++i) {
        if(command[i] == separator) {
            #ifndef NDEBUG
            printf("\tSpace found at index %d, last_space=%d\n", i, sep_index);
            #endif

            #ifndef NDEBUG
            printf("\tCopying parameter -> ");
            #endif
            for(int k = sep_index + 1; k < i; ++k) {
                tmp[k - sep_index - 1] = command[k];
                #ifndef NDEBUG
                printf("%c", command[k]);
                #endif
            }
            tmp[i - sep_index - 1] = '\0';
            
            #ifndef NDEBUG
            printf("\n\tParameter found: %s\n", tmp);
            #endif

            conv = atoi(tmp);
            if(conv < 0) {
                params[j++] = 0;
            }
            else {
                params[j++] = conv;
            } 

            #ifndef NDEBUG
            printf("\tParameter converted: %d\n", params[j - 1]);
            #endif

            sep_index = i;
        }    
    }

    #ifndef NDEBUG
    printf("\tCopying last parameter -> ");
    #endif
    for(uint k = sep_index + 1; k < length; ++k) {
        tmp[k - sep_index - 1] = command[k];
        #ifndef NDEBUG
        printf("%c", command[k]);
        #endif
    }
    tmp[length - sep_index - 1] = '\0';
    
    #ifndef NDEBUG
    printf("\n\tLast parameter found: %s\n", tmp);
    #endif

    conv = atoi(tmp);
    if(conv < 0) {
        params[j] = 0;
    }
    else {
        params[j] = conv;
    } 

    #ifndef NDEBUG
    printf("\tLast parameter converted: %d\n", params[j - 1]);
    #endif

    instr->params = params;

    #ifndef NDEBUG
    printf("\tList of all parameters found: ");
    for(int i = 0; i < instr->params_length; ++i) {
        printf("%d ", instr->params[i]);
    }
    printf("\n");
    #endif

    #ifndef NDEBUG
    printf("Ending parse_instruction\n");
    #endif

    return instr;
}

instruction * parse_instruction(const char * command) {
    return parse_instruction_separator(command, ' ');
    
}

uint validate_instruction(const instruction * instruction) {

    uint validation = 0;

    switch (instruction->command) {
    case add_station_command: validation = instruction->params_length > 0;
        break;
    
    case delete_station_command: validation = instruction->params_length == 1;
        break;

    case add_car_command: validation = instruction->params_length == 2;
        break;

    case remove_car_command: validation = instruction->params_length == 2;
        break;

    case plan_path_command: validation = instruction->params_length == 2;
        break;

    case no_command: validation = 0;
        break;
    }

    return validation;
}

void delete_instruction(instruction * instruction) {
    #ifndef NDEBUG
    printf("Starting delete_instruction\n");
    #endif

    if(instruction != NULL) {
        #ifndef NDEBUG
        printf("\tDeallocating params\n");
        #endif
        free(instruction->params);

        #ifndef NDEBUG
        printf("\tDeallocating instruction\n");
        #endif
        free(instruction);
    }

    #ifndef NDEBUG
    printf("Ending delete_instruction\n");
    #endif
}