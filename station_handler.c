/**
 * @file station_handler.c
 * @brief Handle stations operations.
 * 
 * This module defines functions to handle all commands types; every function maps to one or more commands.
*/

#include "station_handler.h"
#include <stdlib.h>

#define NDEBUG

#ifndef NDEBUG
#include <stdio.h>
#endif

/**
 * @brief Run binary search to find the position of the element nearest to target.
 * 
 * @param stations Array of pointers to station struct.
 * @param a Lower limit.
 * @param b Upper limit.
 * @param target Target element to find.
 * 
 * @pre stations != NULL
 * @pre a >= 0
 * @pre b <= station.length - 1
 * @pre a <= b
 * 
 * @returns The position of the element nearest to target.
 * 
 * @note T(n) = O(log(n)).
 * @note M(n) = O(1).
 * @note If target is not present, it's returned the index of the nearest element to target.
*/
int bin_search(station** stations, int a, int b, int target) {

  #ifndef NDEBUG
  printf("\tbin_search: a=%d, b=%d, target=%d ", a, b, target);
  #endif

  if(a >= b) {

    if(stations[a] != NULL && stations[a]->distance <= target) {
      return a;
    }

    return b;
  }

  int m = (a + b) / 2;

  #ifndef NDEBUG
  printf("m=%d, station=%d\n", m, stations[m]->distance);
  #endif

  if(stations[m]->distance == target) {
    return m;
  }

  if(target < stations[m]->distance) {
    return bin_search(stations, a, m - 1, target);
  } else {
    return bin_search(stations, m + 1, b, target);
  }
}

void test_binary_search() {
    matrix_size length = 10;
    station * vect[length];
    
    vect[0] = create_station(1, 4);
    vect[1] = create_station(3, 5);
    vect[2] = create_station(6, 12);
    vect[3] = create_station(7, 1);
    vect[4] = create_station(8, 11);
    vect[5] = create_station(10, 8);
    vect[6] = create_station(13, 6);
    vect[7] = create_station(17, 4);
    vect[8] = create_station(18, 3);
    vect[9] = create_station(20, 4);
    
    #ifndef NDEBUG
    for(int i = vect[0]->distance - 1; i < vect[length - 1]->distance + 3; ++i) {
        printf("bin_search(vect, 0, %d, %d) = %d\n", length - 1, i, bin_search(vect, 0, length - 1, i));
    }
    #endif

    for(int i = 0; i < length; ++i) {
      delete_station(vect[i]);
    }
}


highway * create_highway(matrix_size capacity) {

  #ifndef NDEBUG
  printf("Starting highway creation\n");
  #endif

  if(capacity == 0) {
    #ifndef NDEBUG
    printf("\tThe capacity of the highway must be greater than 0\n");
    #endif

    return NULL;
  }

  highway* my_highway = (highway *) malloc(sizeof(highway));
  if(my_highway == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough space to allocate highway of %ld bytes\n", sizeof(highway));
    #endif

    return NULL;
  }

  #ifndef NDEBUG
  printf("\tAllocated highway of %ld bytes\n", sizeof(*my_highway));
  #endif

  my_highway->capacity = capacity;
  my_highway->length = 0;

  #ifndef NDEBUG
  printf("\tSetted highway capacity and length\n");
  #endif

  station** stations = (station**) calloc(capacity, sizeof(station *));
  if(stations == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough memory to allocate stations pointers array of %ld bytes\n", capacity * sizeof(station *));
    #endif

    delete_highway(my_highway);
    return NULL;
  }

  #ifndef NDEBUG
  printf("\tAllocated stations pointers array of %ld bytes\n", sizeof(station *) * capacity);
  #endif

  my_highway->stations = stations;

  #ifndef NDEBUG
  printf("\tSetted highway stations\n");
  printf("Ending highway creation\n");
  #endif

  return my_highway;
}

void delete_highway(highway * my_highway) {
  #ifndef NDEBUG
  printf("Starting highway deletion\n");
  #endif
  
  if(my_highway != NULL) {
    if(my_highway->stations != NULL) {

      for(int i=0; i < my_highway->length; ++i) {
        delete_station(my_highway->stations[i]);
      }

      #ifndef NDEBUG
      printf("\tDeallocated cars and stations\n");
      #endif

      free(my_highway->stations);
      my_highway->stations = NULL;
      #ifndef NDEBUG
      printf("\tDeallocated stations pointer\n");
      #endif
    }

    free(my_highway);
    my_highway = NULL;
    #ifndef NDEBUG
    printf("\tDeallocated highway\n");
    #endif
  }

  #ifndef NDEBUG
  printf("Ending highway deletion\n");
  #endif
}


station * create_station(matrix_size distance, matrix_size capacity) {

    #ifndef NDEBUG
    printf("Starting station creation\n");
    #endif

    if(capacity == 0) {
      ++capacity;
    }

    station * new_station = (station *) malloc(sizeof(station));
    if(new_station == NULL) {
      #ifndef NDEBUG
      printf("\tNot enough memory to allocate a new station of %ld bytes\n", sizeof(station));
      #endif

      return NULL;
    }

    #ifndef NDEBUG
    printf("\tAllocated station of %ld bytes\n", sizeof(*new_station));
    #endif

    new_station->cars = (matrix_size *) malloc(sizeof(matrix_size) * capacity);
    if(new_station->cars == NULL) {
      #ifndef NDEBUG
      printf("\tNot enough memory to allocate cars array of %ld bytes\n", capacity * sizeof(matrix_size));
      #endif

      delete_station(new_station);
      return NULL;
    }

    #ifndef NDEBUG
    printf("\tAllocated cars array of %ld bytes\n", sizeof(matrix_size) * capacity);
    #endif

    new_station->distance = distance;
    new_station->capacity = capacity;
    new_station->length = 0;
    new_station->car_max_fuel = 0;

    #ifndef NDEBUG
    printf("Ending station creation\n");
    #endif

    return new_station;
}

void delete_station(station * station) {
  #ifndef NDEBUG
  printf("Starting station deletion\n");
  #endif
  
  if(station != NULL) {
    free(station->cars);
    station->cars = NULL;
    #ifndef NDEBUG
    printf("\tDeallocated cars\n");
    #endif

    free(station);
    station = NULL;
    #ifndef NDEBUG
    printf("\tDeallocated station\n");
    #endif
  }

  #ifndef NDEBUG
  printf("Ending station deletion\n");
  #endif
}

matrix_size add_station(highway ** highway_pointer, station * new_station) {
  
  #ifndef NDEBUG
  printf("Starting station insertion\n");
  #endif

  #ifndef NDEBUG
    printf("\tHighway length=%d, new_station distance=%d\n", (*highway_pointer)->length, new_station->distance);
    #endif

  if(highway_pointer == NULL || *highway_pointer == NULL || new_station == NULL) {
    #ifndef NDEBUG
    printf("\tNULL pointer\n");
    #endif

    return 0;
  }

  highway * my_highway = *highway_pointer;

  int index = 0;

  if(my_highway->length > 0) {
    #ifndef NDEBUG
    printf("\tHighway length=%d, new_station distance=%d\n", my_highway->length, new_station->distance);
    #endif

    index = bin_search(my_highway->stations, 0, my_highway->length - 1, new_station->distance);
    
    #ifndef NDEBUG
    printf("\tInitial index computed: %d\n", index);
    #endif

    if(index < 0) {
      index = 0;
    }

    if(index < my_highway->length && new_station->distance == my_highway->stations[index]->distance) {
      #ifndef NDEBUG
      printf("\tStation at distance %d already inserted\n", new_station->distance);
      #endif

      return 0;
    }

    if(index < my_highway->length && new_station->distance > my_highway->stations[index]->distance) {
      ++index;
    }
  }

  #ifndef NDEBUG
  printf("\tComputed new station index: %d\n", index);
  #endif
  
  if(my_highway->length == my_highway->capacity) {
    
    #ifndef NDEBUG
    printf("\tArray full (%d/%d), attempting to double capacity\n", my_highway->length, my_highway->capacity);
    #endif

    highway* new_highway = create_highway(my_highway->capacity * 2);
    if(new_highway == NULL) {
      #ifndef NDEBUG
      printf("\tUnable to double capacity\n");
      #endif

      return 0;
    }

    #ifndef NDEBUG
    printf("\tCapacity doubled, inserting new station\n");
    #endif

    for(int i = 0; i < index; ++i) {
      new_highway->stations[i] = my_highway->stations[i];
    }
    new_highway->stations[index] = new_station;
    for(int i = index + 1; i <= my_highway->length; ++i) {
      new_highway->stations[i] = my_highway->stations[i - 1];
    }

    new_highway->length = my_highway->length + 1;
    
    free(my_highway->stations);
    free(my_highway);

    *highway_pointer = new_highway;
    my_highway = *highway_pointer;

    #ifndef NDEBUG
    printf("\tStation inserted in position %d, new length: %d/%d\n", index, (*highway_pointer)->length, (*highway_pointer)->capacity);
    #endif
  } 
  else {
    #ifndef NDEBUG
    printf("\tNo need to double capacity (%d/%d), inserting station\n", my_highway->length, my_highway->capacity);
    #endif
  
    for(int i = my_highway->length; i > index; --i) {
      my_highway->stations[i] = my_highway->stations[i - 1];
    }
  
    my_highway->stations[index] = new_station;
    my_highway->length += 1;

    #ifndef NDEBUG
    printf("\tStation inserted in position %d, new length: %d/%d\n", index, my_highway->length, my_highway->capacity);
    #endif
  }

  #ifndef NDEBUG
  printf("\tStations distances: ");
  for(int i = 0; i < (*highway_pointer)->length; ++i) {
    printf("%d, ", (*highway_pointer)->stations[i]->distance);
  }
  printf("\n");
  #endif

  #ifndef NDEBUG
  printf("Ending station insertion\n");
  #endif

  return 1;
}

matrix_size remove_station(highway * my_highway, matrix_size distance) {

    #ifndef NDEBUG
    printf("Starting station removal\n");
    #endif

    if(my_highway == NULL) {
      #ifndef NDEBUG
      printf("\tNULL pointer\n");
      #endif

      return 0;
    }

    if(my_highway->length == 0) {
      #ifndef NDEBUG
      printf("\tNo stations to remove\n");
      #endif

      return 0;
    }

    matrix_size index = bin_search(my_highway->stations, 0, my_highway->length - 1, distance);
    if(my_highway->stations[index]->distance != distance) {

      #ifndef NDEBUG
      printf("\tStation at distance %d not found\n", distance);
      #endif

      return 0;
    }

    #ifndef NDEBUG
    printf("\tStation at distance %d found in position %d, proceding with removal\n", distance, index);
    #endif

    station * tmp = my_highway->stations[index];

    while(index < my_highway->length - 1) {
        my_highway->stations[index] = my_highway->stations[index + 1];
        ++index;
    }

    --my_highway->length; 

    delete_station(tmp);

    #ifndef NDEBUG
    printf("\tStation removed and deallocated\n");
    #endif

    #ifndef NDEBUG
    printf("Ending station removal\n");
    #endif

    return 1;
}

station * find_station(const highway * highway, matrix_size distance) {

  #ifndef NDEBUG
  printf("Starting station search\n");
  #endif

  if(highway == NULL || highway->stations == NULL) {
    #ifndef NDEBUG
    printf("\tNULL pointer\n");
    #endif

    return 0;
  }

  if(highway->length == 0) {
    #ifndef NDEBUG
    printf("\tNo stations to search\n");
    #endif

    return NULL;
  }

  matrix_size index = bin_search(highway->stations, 0, highway->length - 1, distance);
  if(index < 0 || index >= highway->length || highway->stations[index]->distance != distance) {
    #ifndef NDEBUG
    printf("\tStation at distance %d not found\n", distance);
    #endif

    return NULL;
  }

  #ifndef NDEBUG
  printf("\tStation at distance %d found at index %d\n", distance, index);
  printf("Ending station search\n");
  #endif

  return highway->stations[index];
}


matrix_size add_car(station * station, matrix_size fuel) {
  
  #ifndef NDEBUG
  printf("Starting car insertion\n");
  #endif

  if(station == NULL || station->cars == NULL) {
    #ifndef NDEBUG
    printf("\tNULL pointer\n");
    #endif

    return 0;
  }

  if(station->length == station->capacity) {
    #ifndef NDEBUG
    printf("\tArray full (%d/%d), attempting to double capacity\n", station->length, station->capacity);
    #endif

    matrix_size* new_cars = (matrix_size *) malloc(sizeof(matrix_size) * station->capacity * 2);
    if(new_cars == NULL) {
      #ifndef NDEBUG
      printf("\tUnable to double capacity\n");
      #endif

      return 0;
    }

    #ifndef NDEBUG
    printf("\tCapacity doubled, inserting new car\n");
    #endif

    for(matrix_size i = 0; i < station->length; ++i) {
      new_cars[i] = station->cars[i];
    }
    new_cars[station->length] = fuel;

    station->capacity *= 2;
    ++station->length;
    if(fuel > station->car_max_fuel) {
      station->car_max_fuel = fuel;
    }

    free(station->cars);
    station->cars = new_cars;
    
    #ifndef NDEBUG
    printf("\tCar inserted in position %d, new length: %d/%d\n", station->length - 1, station->length, station->capacity);
    #endif
  }
  else {
    #ifndef NDEBUG
    printf("\tNo need to double capacity (%d/%d), appending new car\n", station->length, station->capacity);
    #endif

    station->cars[station->length++] = fuel;
    if(fuel > station->car_max_fuel) {
      station->car_max_fuel = fuel;

      #ifndef NDEBUG
      printf("\tMax fuel updated at %d\n", station->car_max_fuel);
      #endif
    }
    
    #ifndef NDEBUG
    printf("\tCar inserted in position %d, new length: %d/%d\n", station->length - 1, station->length, station->capacity);
    #endif
  }

  #ifndef NDEBUG
  printf("Ending car insertion\n");
  #endif

  return 1;
}

matrix_size remove_car(station * station, matrix_size fuel) {
  #ifndef NDEBUG
  printf("Starting car removal\n");
  #endif

  if(station == NULL || station->cars == NULL) {
    #ifndef NDEBUG
    printf("\tNULL pointer\n");
    #endif

    return 0;
  }

  if(station->length == 0) {
    #ifndef NDEBUG
    printf("\tNo car to remove\n");
    #endif

    return 0;
  }

  #ifndef NDEBUG
  printf("\tStarting car search\n");
  #endif

  matrix_size i = 0;
  while(i < station->length && station->cars[i] != fuel) {
    ++i;
  }

  if(i == station->length) {
    #ifndef NDEBUG
    printf("\tCar with fuel %d not found\n", fuel);
    #endif

    return 0;
  } 

  #ifndef NDEBUG
  printf("\tCar with fuel %d found at index %d, proceding with removal\n", fuel, i);
  #endif

  station->cars[i] = station->cars[--station->length];

  if(fuel == station->car_max_fuel) {
    #ifndef NDEBUG
    printf("\tUpdating max fuel\n");
    #endif

    station->car_max_fuel = 0;
    
    for(i = 0; i < station->length; ++i) {
      if(station->cars[i] > station->car_max_fuel) {
        station->car_max_fuel = station->cars[i];
      }
    }

    #ifndef NDEBUG
    printf("\tMax fuel updated at %d\n", station->car_max_fuel);
    #endif
  }

  #ifndef NDEBUG
  printf("\tCar removed -> new length: %d/%d\n", station->length, station->capacity);
  printf("Ending car removal\n");
  #endif

  return 1;
}

matrix_size add_car_by_distance(highway * my_highway, matrix_size distance, matrix_size fuel) {
  
  #ifndef NDEBUG
  printf("Starting car insertion by distance\n");
  #endif

  station * station = find_station(my_highway, distance);
  if(station == NULL) {
    return 0;
  }

  matrix_size result = add_car(station, fuel); 

  #ifndef NDEBUG
  printf("Ending car insertion by distance\n");
  #endif

  return result;
}

matrix_size remove_car_by_distance(highway * my_highway, matrix_size distance, matrix_size fuel) {
  #ifndef NDEBUG
  printf("Starting car removal by distance\n");
  #endif

  station * station = find_station(my_highway, distance);
  if(station == NULL) {
    return 0;
  }

  matrix_size result = remove_car(station, fuel); 

  #ifndef NDEBUG
  printf("Ending car removal by distance\n");
  #endif

  return result;
}


int extract_stations(const highway * highway, matrix_size start, matrix_size end, matrix_size ** stations_p, matrix_size ** cars_p) {
  #ifndef NDEBUG
  printf("Starting stations extraction by distance\n");
  #endif

  *stations_p = NULL;
  *cars_p = NULL;

  if(highway == NULL || highway->stations == NULL) {
    #ifndef NDEBUG
    printf("\tNULL pointer\n");
    #endif

    return null_ptr;
  }

  if(start > end) {
    #ifndef NDEBUG
    printf("\tStart station > end station\n");
    #endif

    return no_solution;
  }

  matrix_size i = bin_search(highway->stations, 0, highway->length - 1, start);
  if(highway->stations[i]->distance != start) {
    #ifndef NDEBUG
    printf("\tStart station not found\n");
    #endif

    return no_solution;
  }
  #ifndef NDEBUG
  printf("\tStart station found at index %d\n", i);
  #endif

  matrix_size j = bin_search(highway->stations, 0, highway->length - 1, end);
  if(highway->stations[j]->distance != end) {
    #ifndef NDEBUG
    printf("\tEnd station not found\n");
    #endif

    return no_solution;
  }
  #ifndef NDEBUG
  printf("\tEnd station found at index %d\n", j);
  #endif

  matrix_size * actual_stations = (matrix_size *) malloc(sizeof(matrix_size) * (j - i + 1));
  if(actual_stations == NULL) {
    #ifndef NDEBUG
    printf("\tUnable to allocate array of actual stations of %ld bytes\n", sizeof(matrix_size) * (j - i + 1));
    #endif

    return mem_error;
  }
  #ifndef NDEBUG
  printf("\tAllocated array of actual stations of %ld bytes\n", sizeof(matrix_size) * (j - i + 1));
  #endif

  matrix_size * actual_cars = (matrix_size *) malloc(sizeof(matrix_size) * (j - i + 1));
  if(actual_cars == NULL) {
    #ifndef NDEBUG
    printf("\tUnable to allocate array of actual cars of %ld bytes\n", sizeof(matrix_size) * (j - i + 1));
    #endif

    free(actual_stations);
    return mem_error;
  }
  #ifndef NDEBUG
  printf("\tAllocated array of actual cars of %ld bytes\n", sizeof(matrix_size) * (j - i + 1));
  printf("Proceding with extraction\n");
  #endif
  
  for(int k = i; k <= j; ++k) {
    actual_stations[k - i] = highway->stations[k]->distance; 
    actual_cars[k - i] = highway->stations[k]->car_max_fuel;
  }

  *stations_p = actual_stations;
  *cars_p = actual_cars;

  #ifndef NDEBUG
  printf("\tStation and cars from index %d to index %d extracted\n", i, j);
  printf("Ending stations extraction by distance\n");
  #endif

  return (j - i + 1);
}

void test_extract_stations() {
  #ifndef NDEBUG
  highway * highway_2 = create_highway(3);

  station * s1 = create_station(11, 23);
  add_car(s1, 6);
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
  add_car_by_distance(highway_2, s8->distance, 1);

  matrix_size * station_extracted;
  matrix_size * cars_extracted; 
  
  int result = extract_stations(highway_2, s8->distance, s6->distance, &station_extracted, &cars_extracted);

  if(result == mem_error) {
    printf("Memory error\n");
  }
  else if(result == null_ptr) {
    printf("Null pointer\n");
  }
  else if(result == no_solution) {
    printf("No solution\n");
  }
  else {
    printf("Station:Car extracted:\n");
    for(int i = 0; i < result; ++i) {
      printf("%d:%d ", station_extracted[i], cars_extracted[i]);
    }
    printf("\n");
  }
  free(station_extracted);
  free(cars_extracted);

  result = extract_stations(highway_2, s8->distance, s6->distance, &station_extracted, &cars_extracted);

  if(result == mem_error) {
    printf("Memory error\n");
  }
  else if(result == null_ptr) {
    printf("Null pointer\n");
  }
  else if(result == no_solution) {
    printf("No solution\n");
  }
  else {
    printf("Station:Car extracted:\n");
    for(int i = 0; i < result; ++i) {
      printf("%d:%d ", station_extracted[i], cars_extracted[i]);
    }
    printf("\n");
  }

  free(station_extracted);
  free(cars_extracted);

  printf("%d\n", extract_stations(highway_2, 11, 123, &station_extracted, &cars_extracted));
  printf("%d\n", extract_stations(highway_2, 110, 123, &station_extracted, &cars_extracted));
  printf("%d\n", extract_stations(highway_2, 8, 2, &station_extracted, &cars_extracted));
  printf("%d\n", extract_stations(NULL, 2, 8, &station_extracted, &cars_extracted));
  
  delete_highway(highway_2);
  #endif
}

int plan_path(const highway * highway, matrix_size start, matrix_size end, direction dir, matrix_size ** solution) {

  #ifndef NDEBUG
  printf("Starting plan path\n");
  #endif

  *solution = NULL;

  if(highway == NULL) {
    #ifndef NDEBUG
    printf("\tNULL pointer\n");
    #endif

    return null_ptr;
  }

  matrix_size * stations, * cars;

  int n_stations = 0;
  if(dir == forward) {
    n_stations = extract_stations(highway, start, end, &stations, &cars);
  }
  else {
    n_stations = extract_stations(highway, end, start, &stations, &cars);
  }

  if(n_stations < 0) {
    #ifndef NDEBUG
    printf("\tAborting plan path\n");
    #endif

    return n_stations;
  }

  #ifndef NDEBUG
  printf("\tSTATION:CAR -> ");
  
  for(int i = 0; i < n_stations; ++i) {
    #ifndef NDEBUG
    printf("%d:%d ", stations[i], cars[i]);
    #endif
  }
  printf("\n\tMax fuel: %d\n", max_fuel);
  #endif

  #ifndef NDEBUG
  printf("\tLaunching solve\n");
  #endif
  int min_stops = solve(stations, n_stations, cars, dir, solution);

  #ifndef NDEBUG
  printf("\tFreeing resources\n");
  #endif
  free(stations);
  free(cars);

  #ifndef NDEBUG
  printf("Ending plan path\n");
  #endif

  return min_stops;
}