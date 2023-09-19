#ifndef _SOLVER_
#define _SOLVER_

/**
 * @headerfile solver.h
 * @brief Interface of solver.c
*/

/**
 * Value to rapresent infinity.
*/
#define INF 10e8

/**
 * if defined, is searched the solution nearest to the start of the highway.
*/
#define MINIMIZE_DISTANCE

/**
 * @enum direction
 * Codifies the direction of travel of the highway.
*/
typedef enum {
    forward = 1,
    backward = -1

} direction;

/**
 * @enum result
 * Codifies possible errors code.
*/
typedef enum {
    no_solution = -1,
    mem_error = -2,
    null_ptr = -3
} result;

typedef unsigned int matrix_size;

/**
 *  @brief Compute the optimal solution for the problem of finding the minimum number of stops.
 *  
 *  @param stations Distances of the stations from start.
 *  @param n_stations Number of stations.
 *  @param cars Maximum fuel of the cars at stations.
 *  @param dir Direction to follow (forward or backward).
 *  @param solution Address of the pointer which will reference the solution (composed by the distances of the station from start).
 * 
 *  @post Solution contains the optimal solution (if exists, otherwise NULL).
 * 
 *  @returns The minimum number of stops necessary (excludind starting and ending stations); if no solution is possible, is returned no_solution;
 *           if the execution generates memory errors, is returned mem_error.
 * 
 *  @note If more solutions are avaible, is choosen the solution which minimizes the distance from the actual start 
 *  (if dir=forward from the beginning, if dir=backward from the end).
 *  @note if MINIMIZE_DISTANCE in defined, is always choosen the solution which minimizes the distance from the start (stations[0]).
*/
int solve(matrix_size * stations, matrix_size n_stations, matrix_size * cars, 
        direction dir, matrix_size ** solution);
                    
#endif
