#ifndef _PARSER_
#define _PARSER_

/**
 * @headerfile parser.h
 * @brief Interface of parser.c.
*/

typedef unsigned int uint;

/**
 * @enum command_type
 * @brief Codifies commands with unsigned integers.
*/
typedef enum c_type{
    add_station_command = 0,
    delete_station_command = 1,
    add_car_command = 2,
    remove_car_command = 3,
    plan_path_command = 4,
    no_command = 5,
} command_type;

/**
 * @struct instruction
 * @brief Codifies and store all the informations of a command.
 * 
 * @param command Codification of the command to be executed.
 * @param params Pointer to an array of the parameters of the command
 * @param params_length Length of the params array.
*/
typedef struct instruction {
    command_type command;
    uint * params;
    uint params_length;
} instruction;

/**
 * @brief Parse a command.
 * 
 *  Parse a command from the string rapresentation to an instruction struct.
 * 
 * @param command String rapresentation of the command to parse.
 * 
 * @pre command != NULL
 * 
 * @returns Pointer to the instruction if the command respects the syntax, NULL otherwise.
 *
 * @note The separator used can be changed at compile time.
*/
instruction * parse_instruction(const char * command);

/**
 * @brief Validate semantically an instruction.
 * 
 * @param instruction Pointer to the instruction to validate.
 * 
 * @pre instruction != NULL
 * 
 * @returns 1 if the instruction is semantically valid, 0 otherwise.
*/
uint validate_instruction(const instruction * instruction);

/**
 * @brief Delete an instruction allocated on heap.
 * 
 * @param instruction Pointer to the instruction to delete.
 * 
 * @post The instruction is deallocated. 
*/
void delete_instruction(instruction * instruction);

#endif
