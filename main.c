/**
 * @file main.c
 * @brief Receive and execute commands.
 * 
 * This module receives commands from stdin; it uses module parser to parse and station_handler to execute the command; it reports the output on stdout.   
*/

#include "parser.h"
#include "solver.h"
#include "station_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STD_HIGHWAY_CAPACITY 256
#define STD_STATION_CAPACITY 32

#define BUFFER_CAPACITY 8096

void execute_add_station(highway ** highway, const instruction * instruction, FILE * output) {
    station * station = NULL;
    if(instruction->params[1] > STD_STATION_CAPACITY) {
        station = create_station(instruction->params[0], instruction->params[1]);
    }
    else {
        station = create_station(instruction->params[0], STD_STATION_CAPACITY);
    }
    
    if(station != NULL) {
        uint result = 1, i = 2;
        while(result && i < instruction->params_length) {
            result = result && add_car(station, instruction->params[i++]);
        }

        if(result == 1) {
            result = add_station(highway, station);
            if(result == 1) {
                fprintf(output, "aggiunta\n");
            }
            else {
                delete_station(station);
                fprintf(output, "non aggiunta\n");
            }        
        }
        else {
            delete_station(station);
            fprintf(output, "non aggiunta\n");
        }
    }
    else {
        fprintf(output, "non aggiunta\n");
    }
}

void execute_delete_station(highway * highway, const instruction * instruction, FILE * output) {
    if(remove_station(highway, instruction->params[0])) {
        fprintf(output, "demolita\n");
    }
    else {
        fprintf(output, "non demolita\n");
    }
}

void execute_add_car(highway * highway, const instruction * instruction, FILE * output) {
    if(add_car_by_distance(highway, instruction->params[0], instruction->params[1])) {
        fprintf(output, "aggiunta\n");
    }
    else {
        fprintf(output, "non aggiunta\n");
    }
}

void execute_remove_car(highway * highway, const instruction * instruction, FILE * output) {
    if(remove_car_by_distance(highway, instruction->params[0], instruction->params[1])) {
        fprintf(output, "rottamata\n");
    }
    else {
        fprintf(output, "non rottamata\n");
    };
}

void execute_plan_path(highway * highway, const instruction * instruction, FILE * output) {
    matrix_size * solution = NULL;
    direction dir = forward;
    if(instruction->params[0] > instruction->params[1]) {
        dir = backward;
    }

    int stops = plan_path(highway, instruction->params[0], 
                    instruction->params[1], dir, &solution);

    if(stops >= 0) {
        for(int i = 0; i < stops + 1; ++i) {
            fprintf(output, "%d ", solution[i]);
        }
        fprintf(output, "%d", solution[stops + 1]);
        fprintf(output, "\n");
    }
    else {
        fprintf(output, "nessun percorso\n");
    }

    free(solution);
}

void execute_command(highway ** highway, const instruction * instruction, FILE * output) {
    
    if(validate_instruction(instruction)) {
        switch(instruction->command) {
            
            case add_station_command: execute_add_station(highway, instruction, output);
            break;

            case delete_station_command: execute_delete_station(*highway, instruction, output);
            break;

            case add_car_command: execute_add_car(*highway, instruction, output);
            break;
            
            case remove_car_command: execute_remove_car(*highway, instruction, output);
            break;
            
            case plan_path_command: execute_plan_path(*highway, instruction, output);
            break;

            case no_command:
            break;
        }
    }
    else {
        fprintf(output, "Command syntax error\n");
    }
}

int main() {
    
    FILE * output = stdout;

    if(output == NULL) {
        fprintf(stderr, "Output file not found\n");

        return 1;
    }

    FILE * input = stdin;

    if(input == NULL) {
        fprintf(stderr, "Input file not found\n");
        fclose(output);

        return 1;
    }

    uint line_index = 0, line = 0;     
    char buffer[BUFFER_CAPACITY];

    highway * highway = create_highway(STD_HIGHWAY_CAPACITY);

    char c = '\0';
    int n = 0;

    while(1) {
        n = fscanf(input, "%c", &c);
        if(n == EOF) {
            break;
        }

        if(c == '\n') {
            buffer[line_index] = '\0';

            instruction * instruction = parse_instruction(buffer);
            execute_command(&highway, instruction, output);
            delete_instruction(instruction);

            line_index = 0;
            ++line;
        }
        else {
            buffer[line_index++] = c;
        }    

        if(line_index > BUFFER_CAPACITY) {
            fprintf(stderr, "%d-th command length > buffer capacity = %d\n", line + 1, BUFFER_CAPACITY);

            delete_highway(highway);
            fclose(input);
            fclose(output);

            return 1;
        }
    }    

    delete_highway(highway);
    fclose(input);
    fclose(output);
    
    return 0;
}