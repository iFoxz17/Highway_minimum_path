/**
 * @file test.c
 * @brief Contains unity tests for all the commands. 
*/

#include "parser.h"
#include "solver.h"
#include "station_handler.h"

#include <stdio.h>
#include <stdlib.h>

void print_vec(matrix_size * vect, matrix_size length) {
  for(int i = 0; i < length; ++i) {
    printf("%d ", vect[i]);
  }

  printf("\n");
}

//-------------------------------------------------------------------------------------

void test_solve(matrix_size * stations, matrix_size n_stations, matrix_size * cars, direction dir) {
  matrix_size * solution;

  printf("Starting solve\n");
  int result = solve(stations, n_stations, cars, dir, &solution);
  
  printf("Result -> ");
  if(result == mem_error) {
    printf("memory error during computation");
  }
  else if(result == no_solution) {
    printf("no solution avaible");
  }
  else {
    printf("solution found:\n\tNumber of stops = %d\n\tStations: ", result);
    for(int i = 0; i < result + 2; ++i) {
      printf("%d ", solution[i]);
    }
  }

  printf("\n");
  free(solution);
}

void test_dynamic_programming_example() {
    
    printf("STARTING TEST EXAMPLE\n");

    matrix_size stations[] = {1, 2, 3, 4, 5, 6};
    matrix_size cars[] =     {2, 3, 1, 2, 1, 0};

    printf("Stations: ");
    print_vec(stations, sizeof(stations) / sizeof(matrix_size));

    printf("Cars: ");
    print_vec(cars, sizeof(cars) / sizeof(matrix_size));

    printf("Forward:\n");
    test_solve(stations, sizeof(cars) / sizeof(matrix_size), cars, forward);

    matrix_size back_cars[] = {0, 1, 2, 1, 3, 2};

    printf("Backward:\n");
    test_solve(stations, sizeof(cars) / sizeof(matrix_size), back_cars, backward);
}

void test_dynamic_programming_small() {

    printf("STARTING TEST SMALL\n");

    matrix_size stations[] = {1, 2, 4, 5, 7, 13, 15, 20, 21, 25, 26, 27};
    matrix_size cars[] =     {5, 1, 1, 4, 8,  8,  6,  8,  1,  6,  1,  1};

    printf("Stations: ");
    print_vec(stations, sizeof(stations) / sizeof(matrix_size));

    printf("Cars: ");
    print_vec(cars, sizeof(cars) / sizeof(matrix_size));

    printf("Forward:\n");
    test_solve(stations, sizeof(cars) / sizeof(matrix_size), cars, forward);

    printf("Backward:\n");
    test_solve(stations, sizeof(cars) / sizeof(matrix_size), cars, backward);
}

void test_dynamic_programming_huge() {
    printf("STARTING TEST HUGE\n");

    matrix_size n_stations = 10000;
    matrix_size max_fuel = 1200;

    matrix_size stations[n_stations];
    matrix_size cars[n_stations];

    for(int i=0; i < n_stations; ++i) {
      stations[i] = i * 2 + 1;
      cars[i] = (i % max_fuel) + 2;
    }

    printf("Stations:\n");
    //print_vec(stations, n_stations);

    printf("Cars:\n");
    //print_vec(cars, n_stations);

    printf("Forward:\n");
    test_solve(stations, n_stations, cars, forward);

    printf("Backward:\n");
    test_solve(stations, n_stations, cars, backward);
}

//-------------------------------------------------------------------------------------

void test_highway() {
    highway * highway_1 = create_highway((matrix_size) 10000);
    highway * highway_2 = create_highway((matrix_size) 438744);
    highway * highway_3 = create_highway((matrix_size) 12);

    //highway* highway_4 = create_highway((matrix_size) 3998744761);        //Not enough memory
    //delete_highway(highway_4);

    delete_highway(highway_1);
    delete_highway(highway_2);
    delete_highway(highway_3);

    delete_highway(NULL);

    highway* highway_4 = create_highway((matrix_size) 0);
    delete_highway(highway_4);
    
    highway * only_highway = (highway *) malloc(sizeof(highway));
    only_highway->stations = NULL;
    delete_highway(only_highway);
}

void test_station_creation() {
    station * s1 = create_station((matrix_size) 123, (matrix_size) 23);
    station * s2 = create_station((matrix_size) 121931, (matrix_size) 2385);
    station * s3 = create_station((matrix_size) 456, (matrix_size) 737823);

    station * s0 = create_station((matrix_size) 12197183781, (matrix_size) 1278173138);        //Not enough memory
    delete_station(s0);

    delete_station(s1);
    delete_station(s2);
    delete_station(s3);

    station * s4 = create_station((matrix_size) 0, (matrix_size) 0);
    delete_station(s4);
}

void test_station_insertion() {
    highway * highway_1 = create_highway(12);

    station * s1 = create_station(11, 23);
    station * s2 = create_station(2, 11);
    station * s3 = create_station(8, 25);
    station * s4 = create_station(7, 34);
    station * s5 = create_station(10, 4);

    printf("Insertion 1: %d\n", add_station(&highway_1, s1));
    printf("Insertion 2: %d\n", add_station(&highway_1, s2));
    printf("Insertion 3: %d\n", add_station(&highway_1, s3));
    printf("Insertion 4: %d\n", add_station(&highway_1, s4));
    printf("Insertion 5: %d\n", add_station(&highway_1, s5));

    delete_highway(highway_1);

    highway * highway_2 = create_highway(3);

    s1 = create_station(11, 23);
    s2 = create_station(2, 11);
    s3 = create_station(8, 25);
    s4 = create_station(7, 34);
    s5 = create_station(10, 2);
    station * s6 = create_station(24, 1);
    station * s7 = create_station(19, 42);
    station * s8 = create_station(1, 34);

    printf("Insertion 1: %d\n", add_station(&highway_2, s1));
    printf("Insertion 2: %d\n", add_station(&highway_2, s2));
    printf("Insertion 3: %d\n", add_station(&highway_2, s3));
    printf("Insertion 4: %d\n", add_station(&highway_2, s4));
    printf("Insertion 5: %d\n", add_station(&highway_2, s5));
    printf("Insertion 6: %d\n", add_station(&highway_2, s6));
    printf("Insertion 7: %d\n", add_station(&highway_2, s7));
    printf("Insertion 8: %d\n", add_station(&highway_2, s8));

    delete_highway(highway_2);

    highway * highway_3 = create_highway(2);

    for(matrix_size i = 1; i <= 1026; ++i) {
        s1 = create_station(i * 2 - 1, i % 14);
        add_station(&highway_3, s1);
    }

    delete_highway(highway_3);

    highway * highway = create_highway(0);

    station * s = create_station(87, 0);
    add_station(&highway, s);
    delete_station(s);

    delete_highway(highway);
}

void test_station_removal() {
    highway * highway_1 = create_highway(12);

    station * s1 = create_station(11, 23);
    station * s2 = create_station(2, 11);
    station * s3 = create_station(8, 25);
    station * s4 = create_station(7, 34);
    station * s5 = create_station(10, 4);

    printf("Insertion 1: %d\n", add_station(&highway_1, s1));
    printf("Insertion 2: %d\n", add_station(&highway_1, s2));
    printf("Insertion 3: %d\n", add_station(&highway_1, s3));
    printf("Insertion 4: %d\n", add_station(&highway_1, s4));
    printf("Insertion 5: %d\n", add_station(&highway_1, s5));

    printf("Removal 1: %d\n", remove_station(highway_1, s1->distance));
    printf("Removal 2: %d\n", remove_station(highway_1, s2->distance));
    printf("Removal error: %d\n", remove_station(highway_1, 66));
    printf("Removal 3: %d\n", remove_station(highway_1, s3->distance));
    printf("Removal 4: %d\n", remove_station(highway_1, s4->distance));
    printf("Removal 5: %d\n", remove_station(highway_1, s5->distance));

    printf("Removal empty: %d\n", remove_station(highway_1, s5->distance));

    delete_highway(highway_1);

    highway * highway_3 = create_highway(2);

    for(matrix_size i = 1; i <= 567; ++i) {
        s1 = create_station(i * 2 - 1, i % 14);
        add_station(&highway_3, s1);
    }

    for(matrix_size i = 1; i <= 200; ++i) {
        remove_station(highway_3, i * 3 + 1);
    }

    delete_highway(highway_3);
}

void test_station_search() {
    highway * highway_2 = create_highway(3);

    station * s1 = create_station(11, 23);
    station * s2 = create_station(2, 11);
    station * s3 = create_station(8, 25);
    station * s4 = create_station(7, 34);
    station * s5 = create_station(10, 2);
    station * s6 = create_station(24, 1);
    station * s7 = create_station(19, 42);
    station * s8 = create_station(1, 34);

    add_station(&highway_2, s1);
    add_station(&highway_2, s2);
    add_station(&highway_2, s3);
    add_station(&highway_2, s4);
    add_station(&highway_2, s5);
    add_station(&highway_2, s6);
    add_station(&highway_2, s7);
    add_station(&highway_2, s8);

    s1 = find_station(highway_2, 24);
    if(s1 == NULL) {
        printf("Search 1: NULL\n");
    }
    else {
        printf("Search 1: found\n");
    }
    find_station(highway_2, 39);
    printf("Search 3: %d\n", find_station(highway_2, 10)->distance);
    find_station(highway_2, 13);
    find_station(highway_2, 0);

    delete_highway(highway_2);
}

void test_car_insertion() {
    highway * highway_1 = create_highway(2);

    station * s1 = create_station(11, 2);

    printf("Car insertion 1: %d\n", add_car(s1, 23));
    printf("Car insertion 2: %d\n", add_car(s1, 11));
    printf("Car insertion 3: %d\n", add_car(s1, 25));
    printf("Car insertion 4: %d\n", add_car(s1, 2));
    printf("Car insertion 5: %d\n", add_car(s1, 23));
    printf("Car insertion 6: %d\n", add_car(s1, 28));
    printf("Car insertion 7: %d\n", add_car(s1, 16));

    printf("Cars: ");
    for(int i = 0; i < s1->length; ++i) {
        printf("%d ", s1->cars[i]);
    }
    printf("\n");

    delete_station(s1);

    s1 = create_station(11, 3);
    add_station(&highway_1, s1);

    for(matrix_size i = 1; i <= 6781; ++i) {
        add_car(s1, i % 134);
    }

    delete_highway(highway_1);

    highway_1 = create_highway(1);
    printf("Car insertion capacity=0: %d\n", add_car(create_station(89, 0), 12));
    delete_highway(highway_1);
}

void test_car_removal() {
  station * s1 = create_station(16, 1);

  printf("Car insertion 1: %d\n", add_car(s1, 23));
  printf("Car insertion 2: %d\n", add_car(s1, 11));
  printf("Car insertion 3: %d\n", add_car(s1, 25));
  printf("Car insertion 4: %d\n", add_car(s1, 2));
  printf("Car insertion 5: %d\n", add_car(s1, 23));
  printf("Car insertion 6: %d\n", add_car(s1, 28));
  printf("Car insertion 7: %d\n", add_car(s1, 16));

  printf("Cars before removal: ");
  for(int i = 0; i < s1->length; ++i) {
      printf("%d ", s1->cars[i]);
  }
  printf("\n");
  printf("Max fuel: %d\n", s1->car_max_fuel);

  printf("Car removal 1: %d\n", remove_car(s1, 28));
  printf("Car removal 2: %d\n", remove_car(s1, 23));
  printf("Car removal 3: %d\n", remove_car(s1, 29));
  printf("Car removal 4: %d\n", remove_car(s1, 2));
  printf("Car removal 5: %d\n", remove_car(s1, 11));

  printf("Cars after removal: ");
  for(int i = 0; i < s1->length; ++i) {
      printf("%d ", s1->cars[i]);
  }
  printf("\n");
  printf("Max fuel: %d\n", s1->car_max_fuel);

  delete_station(s1);

  s1 = create_station(16, 3);

  printf("Car removal empty : %d\n", remove_car(s1, 23));
  printf("Car removal NULL: %d\n", remove_car(NULL, 23));

  delete_station(s1);
}

void test_car_insertion_by_distance() {
    highway * highway_1 = create_highway(2);

    station * s1 = create_station(11, 2);
    add_station(&highway_1, s1);

    station * s2 = create_station(11, 5);
    if(add_station(&highway_1, s2) == 0) {
      delete_station(s2);
    }

    printf("Car insertion 1: %d\n", add_car_by_distance(highway_1, 11, 23));
    printf("Car insertion 2: %d\n", add_car_by_distance(highway_1, 11, 11));
    printf("Car insertion 3: %d\n", add_car_by_distance(highway_1, 11, 25));
    printf("Car insertion 4: %d\n", add_car_by_distance(highway_1, 11, 23));
    printf("Car insertion 5: %d\n", add_car_by_distance(highway_1, 13, 23));
    printf("Car insertion 6: %d\n", add_car_by_distance(highway_1, 11, 28));
    printf("Car insertion 7: %d\n", add_car_by_distance(highway_1, 11, 16));

    printf("Cars: ");
    for(int i = 0; i < s1->length; ++i) {
        printf("%d ", s1->cars[i]);
    }
    printf("\n");

    remove_station(highway_1, 11);

    s1 = create_station(11, 3);
    add_station(&highway_1, s1);

    
    for(matrix_size i = 1; i <= 6781; ++i) {
        add_car_by_distance(highway_1, 11, i % 134);
    }
    
    delete_highway(highway_1);
}

void test_car_removal_by_distance() {

  matrix_size distance = 16;

  highway * my_highway = create_highway(34);
  station * s1 = create_station(distance, 1);

  add_station(&my_highway, s1);

  printf("Car insertion 1: %d\n", add_car_by_distance(my_highway, distance, 23));
  printf("Car insertion 2: %d\n", add_car_by_distance(my_highway, distance, 24));
  printf("Car insertion 3: %d\n", add_car_by_distance(my_highway, distance, 55));
  printf("Car insertion 4: %d\n", add_car_by_distance(my_highway, distance, 1));
  printf("Car insertion 5: %d\n", add_car_by_distance(my_highway, distance, 23));
  printf("Car insertion 6: %d\n", add_car_by_distance(my_highway, 9, 23));
  printf("Car insertion 7: %d\n", add_car_by_distance(my_highway, distance, 23));

  printf("Cars before removal: ");
  for(int i = 0; i < s1->length; ++i) {
      printf("%d ", s1->cars[i]);
  }
  printf("\n");
  printf("Max fuel: %d\n", s1->car_max_fuel);

  printf("Car removal 1: %d\n", remove_car_by_distance(my_highway, distance, 23));
  printf("Car removal 2: %d\n", remove_car_by_distance(my_highway, distance, 234));
  printf("Car removal 3: %d\n", remove_car_by_distance(my_highway, distance, 1));
  printf("Car removal 4: %d\n", remove_car_by_distance(my_highway, distance, 55));
  printf("Car removal 5: %d\n", remove_car_by_distance(my_highway, distance + 1, 23));

  printf("Cars after removal: ");
  for(int i = 0; i < s1->length; ++i) {
      printf("%d ", s1->cars[i]);
  }
  printf("\n");
  printf("Max fuel: %d\n", s1->car_max_fuel);

  remove_station(my_highway, distance);

  s1 = create_station(distance, 3);

  add_station(&my_highway, s1);

  printf("Car removal empty : %d\n", remove_car_by_distance(my_highway, 23, 10));
  printf("Car removal NULL: %d\n", remove_car_by_distance(NULL, distance, 10));

  delete_highway(my_highway);
}

void test_plan_path() {
  highway * highway_2 = create_highway(3);

  station * s1 = create_station(11, 23);
  add_car(s1, 10);
  add_car(s1, 5);
  station * s2 = create_station(2, 11);
  station * s3 = create_station(8, 25);
  station * s4 = create_station(7, 34);
  add_car(s4, 3);
  add_car(s4, 9);
  station * s5 = create_station(10, 2);
  station * s6 = create_station(24, 1);
  add_car(s6, 7);
  add_car(s6, 7);
  station * s7 = create_station(19, 42);
  add_car(s7, 3);
  add_car(s7, 10);
  station * s8 = create_station(1, 34);

  printf("Insertion 1: %d\n", add_station(&highway_2, s1));
  printf("Insertion 2: %d\n", add_station(&highway_2, s2));
  add_car_by_distance(highway_2, s2->distance, 9);
  printf("Insertion 3: %d\n", add_station(&highway_2, s3));
  add_car_by_distance(highway_2, s3->distance, 4);
  add_car_by_distance(highway_2, s3->distance, 2);
  printf("Insertion 4: %d\n", add_station(&highway_2, s4));
  printf("Insertion 5: %d\n", add_station(&highway_2, s5));
  add_car_by_distance(highway_2, s5->distance, 4);
  add_car_by_distance(highway_2, s5->distance, 5);
  add_car_by_distance(highway_2, s5->distance, 6);
  printf("Insertion 6: %d\n", add_station(&highway_2, s6));
  printf("Insertion 7: %d\n", add_station(&highway_2, s7));
  printf("Insertion 8: %d\n", add_station(&highway_2, s8));
  add_car_by_distance(highway_2, s8->distance, 8);

  matrix_size * solution;
  int stops = plan_path(highway_2, 2, 19, forward, &solution);

  if(stops >= 0) {
    printf("Solution: ");
    print_vec(solution, stops + 2);
  }
  else {
    printf("Error %d\n", stops);
  }

  free(solution);
  delete_highway(highway_2);
}

//-------------------------------------------------------------------------------------

void print_instruction(const instruction * instruction) {
  if(instruction == NULL) {
    printf("NULL instruction\n");

    return;
  }

  printf("Command: ");
  switch(instruction->command) {
    case add_station_command: printf("aggiungi-stazione\n");
    break;
    case delete_station_command: printf("rimuovi-stazione\n");
    break;
    case add_car_command: printf("aggiungi-macchina\n");
    break;
    case remove_car_command: printf("demolisci-macchina\n");
    break;
    case plan_path_command: printf("pianifica-percorso\n");
    break;
    case no_command: printf("command not codified\n");
    break;
  }

  printf("Params: ");
  print_vec(instruction->params, instruction->params_length);
  printf("Params number: %d\n", instruction->params_length);
}

void test_parse_instruction() {

  instruction * instruction;

  char command_1[] = "aggiungi-stazione 15 673 1 0 1232 12 34 54 66 7 12 34";
  instruction = parse_instruction(command_1);
  print_instruction(instruction);
  delete_instruction(instruction);

  char command_2[] = "demolisci-stazione 15";
  instruction = parse_instruction(command_2);
  print_instruction(instruction);
  delete_instruction(instruction);

  char command_3[] = "aggiungi-auto 159384 673";
  instruction = parse_instruction(command_3);
  print_instruction(instruction);
  delete_instruction(instruction);

  char command_4[] = "rottama-auto 7 8673";
  instruction = parse_instruction(command_4);
  print_instruction(instruction);
  delete_instruction(instruction);

  char command_5[] = "pianifica-percorso 4 6732345";
  instruction = parse_instruction(command_5);
  print_instruction(instruction);
  delete_instruction(instruction);

  char command_6[] = "random-command 4 6732345 7 3 4 5";
  instruction = parse_instruction(command_6);
  print_instruction(instruction);
  delete_instruction(instruction);

  char command_initial_space[] = " pianifica-percorso 4 6732345";
  instruction = parse_instruction(command_initial_space);
  print_instruction(instruction);
  delete_instruction(instruction);

  char command_double_space[] = "pianifica-percorso  4 6732345";
  instruction = parse_instruction(command_double_space);
  print_instruction(instruction);
  delete_instruction(instruction);

  char command_last_space[] = "pianifica-percorso 4 6732345 ";
  instruction = parse_instruction(command_last_space);
  print_instruction(instruction);
  delete_instruction(instruction);

  char command_parameter_wrong[] = "pianifica-percorso 4 654g t543 thfg";
  instruction = parse_instruction(command_parameter_wrong);
  print_instruction(instruction);
  delete_instruction(instruction);

  instruction = parse_instruction("");
  print_instruction(instruction);
  delete_instruction(instruction);

  instruction = parse_instruction(" ");
  print_instruction(instruction);
  delete_instruction(instruction);

  instruction = parse_instruction("    ");
  print_instruction(instruction);
  delete_instruction(instruction);
}

//-------------------------------------------------------------------------------------

void test_solver() {

  test_dynamic_programming_example();

  //test_dynamic_programming_small();
  
  //test_dynamic_programming_huge();
}

void test_station_handler() {

    test_binary_search();

    test_highway();
    
    test_station_creation();

    test_station_insertion();

    test_station_removal();

    test_station_search();

    test_car_insertion();

    test_car_removal();

    test_car_insertion_by_distance();

    test_car_removal_by_distance();

    test_extract_stations();

    test_plan_path();
}

void test_parser() {
  test_parse_instruction();
}

void test_example() {

  matrix_size cars_capacity = 2;

  highway * highway = create_highway(1);

  matrix_size result = 1;
  result = result && add_station(&highway, create_station(20, cars_capacity));
  result = result && add_car_by_distance(highway, 20, 3);
  result = result && add_car_by_distance(highway, 20, 5);
  result = result && add_car_by_distance(highway, 20, 10);
  result = result && add_car_by_distance(highway, 20, 15);
  if(result == 1) {
    printf("aggiunta\n");
  }
  else {
    printf("non aggiunta\n");
  }

  result = 1;
  result = result && add_station(&highway, create_station(4, cars_capacity));
  result = result && add_car_by_distance(highway, 4, 3);
  result = result && add_car_by_distance(highway, 4, 1);
  result = result && add_car_by_distance(highway, 4, 2);
  result = result && add_car_by_distance(highway, 4, 3);
  if(result == 1) {
    printf("aggiunta\n");
  }
  else {
    printf("non aggiunta\n");
  }

  result = 1;
  result = result && add_station(&highway, create_station(30, cars_capacity));
  result = result && add_car_by_distance(highway, 30, 0);
  if(result == 1) {
    printf("aggiunta\n");
  }
  else {
    printf("non aggiunta\n");
  }

  if(remove_station(highway, 3) == 1) {
    printf("demolita\n");
  }
  else {
    printf("non demolita\n");
  }

  if(remove_station(highway, 4) == 1) {
    printf("demolita\n");
  }
  else {
    printf("non demolita\n");
  }

  if(add_car_by_distance(highway, 30, 40) == 1) {
    printf("aggiunta\n");
  }
  else {
    printf("non aggiunta\n");
  }

  result = 1;
  result = result && add_station(&highway, create_station(50, cars_capacity));
  result = result && add_car_by_distance(highway, 50, 3);
  result = result && add_car_by_distance(highway, 50, 20);
  result = result && add_car_by_distance(highway, 50, 25);
  result = result && add_car_by_distance(highway, 50, 7);
  if(result == 1) {
    printf("aggiunta\n");
  }
  else {
    printf("non aggiunta\n");
  }

  if(remove_car_by_distance(highway, 20, 8) == 1) {
    printf("rottamata\n");
  }
  else {
    printf("non rottamata\n");
  }

  if(remove_car_by_distance(highway, 9999, 5) == 1) {
    printf("rottamata\n");
  }
  else {
    printf("non rottamata\n");
  }

  if(remove_car_by_distance(highway, 50, 7) == 1) {
    printf("rottamata\n");
  }
  else {
    printf("non rottamata\n");
  }

  matrix_size * solution = NULL;
  int stops = plan_path(highway, 20, 30, forward, &solution);
  if(stops >= 0) {
    print_vec(solution, stops + 2);
  }
  else {
    printf("Error %d\n", stops);
  }
  free(solution);

  solution = NULL;
  stops = plan_path(highway, 20, 50, forward, &solution);
  if(stops >= 0) {
    print_vec(solution, stops + 2);
  }
  else {
    printf("Error %d\n", stops);
  }
  free(solution);

  solution = NULL;
  stops = plan_path(highway, 50, 30, backward, &solution);
  if(stops >= 0) {
    print_vec(solution, stops + 2);
  }
  else {
    printf("Error %d\n", stops);
  }
  free(solution);

  solution = NULL;
  stops = plan_path(highway, 50, 20, backward, &solution);
  if(stops >= 0) {
    print_vec(solution, stops + 2);
  }
  else {
    printf("Error %d\n", stops);
  }
  free(solution);

  if(add_car_by_distance(highway, 50, 30) == 1) {
    printf("aggiunta\n");
  }
  else {
    printf("non aggiunta\n");
  }

  solution = NULL;
  stops = plan_path(highway, 50, 20, backward, &solution);
  if(stops >= 0) {
    print_vec(solution, stops + 2);
  }
  else {
    printf("Error %d\n", stops);
  }
  free(solution);

  delete_highway(highway);
}
