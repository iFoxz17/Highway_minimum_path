#ifndef _STATION_HANDLER_
#define _STATION_HANDLER_

/**
 * @headerfile station_handler.h
 * @brief Interface of stations_handler.c
*/

#include "solver.h"

/**
 * @struct station 
 * @brief Rapresents and stores station informations.
 * 
 * Struct which stores all the elements of a station.
 * 
 * @param distance Distance of a station from the start of the highway.
 * @param cars Fuel of the cars contained in the station.
 * @param capacity Maximum capacity of the dynamic array cars.
 * @param length Actual length of the dynamic array cars.
 * @param car_max_fuel Max fuel among all the cars.
 * 
*/
typedef struct station {
    matrix_size distance;
    matrix_size * cars;
    matrix_size capacity;
    matrix_size length;
    matrix_size car_max_fuel;
} station;

/**
 * @struct highway 
 * @brief Rapresents and stores highway informations.
 * 
 * Struct which stores all the elements of an highway.
 * 
 * @param stations Pointer to the stations contained in the highway.
 * @param capacity Maximum capacity of the dynamic array *stations.
 * @param length Actual length of the dynamic array *stations.
*/
typedef struct highway {
    station ** stations;
    matrix_size capacity;
    matrix_size length;
} highway;

void test_binary_search();
void test_extract_stations();

/**
 * @brief Create an highway struct with an initial capacity.
 * 
 * @param capacity The initial capacity of the highway.
 * 
 * @post highway.capacity = capacity.
 * @post highway.length = 0.
 * @post stations is a pointer to an array of capacity pointers (initializated to NULL).
 * 
 * @return A pointer to the highway allocated on heap.
 * 
 * @note Capacity must be greater than 0 (otherwise no highway is allocated).
*/
highway * create_highway (matrix_size capacity);
/**
 * @brief Delete an highway struct.
 * 
 * @param highway Pointer to the highway to delete.
*/
void delete_highway(highway * highway);

/**
 * @brief Create a new station.
 * 
 * Create a new station with given distance and capacity.
 * 
 * @param distance Distance of the station from the start of the highway.
 * @param capacity Initial capacity oh the station.
 * 
 * @returns A pointer to the stations if it is created successfully, NULL otherwise.
*/
station * create_station(matrix_size distance, matrix_size capacity);
/**
 * @brief Delete a station.
 * 
 * Deallocate the station from the heap.
 * 
 * @param station Pointer to the station to deallocate.
 * 
 * @post The station is deallocated.
*/
void delete_station(station * station);

/**
 * @brief Add a station to an highway.
 * 
 * @param highway The address of a pointer to the highway which will add the station.
 * @param station A pointer to the station to add.
 * 
 * @post Highway takes ownership of station. 
 * 
 * @return 1 if the station is added successfully; 0 otherwise.
 * 
 * @note If the highway is full (highway->length == highway->capacity), his capacity is doubled.
 * @note Stations are stored increasingly ordered.
 * @note There cannot be more than one station at the same distance.
*/
matrix_size add_station(highway ** highway, station * station);
/**
 * @brief Remove a station from an highway.
 * 
 * @param highway Pointer to the highway which will remove the station.
 * @param station Distance of the station to remove.
 * 
 * @post The station is not in the highway. 
 * 
 * @return 1 if the station is removed successfully; 0 otherwise.
*/
matrix_size remove_station(highway * highway, matrix_size distance);
/**
 * @brief Find a station at a given distance in an highway.
 * 
 * @param highway Pointer to the highway which contains the station.
 * @param distance The distance of the station to search.
 * 
 * @return A pointer to station if the station is found successfully; NULL otherwise.
 * 
 * @note Performs binary search to find the station (T(n) = O(log(n))).
*/
station * find_station(const highway * highway, matrix_size distance);

/**
 * @brief Add a car to a station.
 * 
 * @param station A pointer to the station which will add the car.
 * @param fuel The fuel of the car to add. 
 * 
 * @return 1 if the car is added successfully; 0 otherwise.
 * 
 * @note If the station is full (station->length == station->capacity), his capacity is doubled.
*/
matrix_size add_car(station * station, matrix_size fuel);
/**
 * @brief Remove a car from a station.
 * 
 * @param station A pointer to the station which will remove the car.
 * @param fuel The fuel of the car to remove. 
 * 
 * @return 1 if the car is removed successfully; 0 otherwise.
 * 
 * @note If more cars with the same fuel are present, it's removed one of them randomly.
*/
matrix_size remove_car(station * station, matrix_size fuel);
/**
 * @brief Add a car to a station.
 * 
 * @param highway Pointer to the highway which contains the station.
 * @param distance Distance of the station which will add the car.
 * @param fuel The fuel of the car to add. 
 * 
 * @return 1 if the car is added successfully; 0 otherwise.
 * 
 * @note If the station is full (station->length == station->capacity), his capacity is doubled.
*/
matrix_size add_car_by_distance(highway * highway, matrix_size distance, matrix_size fuel);
/**
 * @brief Remove a car from a station.
 * 
 * @param highway Pointer to the highway which contains the station.
 * @param distance Distance of the station which will remove the car.
 * @param fuel The fuel of the car to remove. 
 * 
 * @return 1 if the car is removed successfully; 0 otherwise.
 * 
 * @note If more cars with the same fuel are present, it's removed one of them randomly.
*/
matrix_size remove_car_by_distance(highway * highway, matrix_size distance, matrix_size fuel);

/**
 * @brief Retrieve the optimal path from start to end.
 * 
 * Retrieve the minimum number of stops necessary to reach station at distance end starting from station at distance start.
 * 
 * @param highway Pointer to the highway to use.
 * @param start Distance of the station to use as start.
 * @param end Distance of the station to use as end.
 * @param dir Direction to follow.
 * @param solution Address of the array where the solution will be put.
 * 
 * @returns The minimum number of stops if a solution is avaible; an element of enum result otherwise.
*/
int plan_path(const highway * highway, matrix_size start, matrix_size end, direction dir, matrix_size ** solution);
#endif
