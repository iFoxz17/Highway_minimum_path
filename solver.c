/**
 * @file solver.c
 * @brief Contains functions to find the minimum number of stops.
*/

#include "solver.h"
#include <stdlib.h>

#define NDEBUG

#ifndef NDEBUG
#include <stdio.h>
#endif

inline matrix_size max(int x, int y);
matrix_size max(int x, int y) {
  if(x >= y) {
    return x;
  }

  return y;
}

inline matrix_size min(matrix_size x, matrix_size y);
matrix_size min(matrix_size x, matrix_size y) {
  if(x <= y) {
    return x;
  }

  return y;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Dynamic programming approach
//-----------------------------------------------------------------------------------------------------------------------------------------
matrix_size * retrieve_traceback(const matrix_size * stations, matrix_size n_stations, matrix_size ** matrix, 
                                  matrix_size ** traceback, matrix_size ** last_station, matrix_size stops, matrix_size fuel_index) {
  #ifndef NDEBUG
  printf("Starting traceback computation\n");
  #endif

  matrix_size * solution = (matrix_size *) malloc((stops + 2) * sizeof(matrix_size));
  if(solution == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough space to allocate solution array of %ld bytes\n", sizeof(matrix_size) * (stops + 2));
    #endif

    return NULL;
  }
  
  #ifndef NDEBUG
  printf("\tAllocated traceback array of %ld bytes\n", sizeof(matrix_size) * (stops + 2));
  printf("Starting traceback reconstruction\n");
  #endif
  
  int pos = 0;
  int s = n_stations - 1;
    
  solution[(stops + 2) - ++pos] = stations[s];
  #ifndef NDEBUG
  printf("\tAdded station=%d at distance=%d to solution\n", s, stations[s]);
  #endif
  
  while(s > 0) {

    #ifndef NDEBUG
    printf("\tStation=%d, distance=%d, pos=%d, fuel_index=%d, stops=%d\n", s, stations[s], pos, fuel_index, matrix[s][fuel_index]);
    #endif  

    if(matrix[s - 1][traceback[s][fuel_index]] < matrix[s][fuel_index]) {
      #ifndef NDEBUG
      printf("\tAdded station=%d at distance=%d to solution\n", s, stations[s]);
      #endif

      solution[(stops + 2) - ++pos] = stations[s];
    }

    fuel_index = traceback[s][fuel_index];
    --s;
  }
  
  solution[(stops + 2) - ++pos] = stations[0];
  #ifndef NDEBUG
  printf("\tAdded station=%d at distance=%d to solution\n", s, stations[s]);
  #endif

  #ifndef NDEBUG
  printf("Traceback reconstruction completed\n");
  #endif

  return solution;
}

void free_memory(matrix_size ** matrix, matrix_size ** traceback_fuel, matrix_size ** traceback_station, 
                                  matrix_size * fuel, matrix_size * offset, matrix_size rows) {
  #ifndef NDEBUG
  printf("Starting memory deallocation\n");
  printf("\tDeallocating dynamic programming matrix\n");
  #endif
  
  if(matrix != NULL) {
    for(matrix_size i = 0; i < rows; ++i) {
      free(matrix[i]);
      matrix[i] = NULL;
    }

    free(matrix);
    matrix = NULL;
  }

  #ifndef NDEBUG
  printf("\tDynamic programming matrix deallocated\n");
  printf("\tDeallocating traceback_fuel matrix\n");
  #endif

  if(traceback_fuel != NULL) {
    for(matrix_size i = 0; i < rows; ++i) {
      free(traceback_fuel[i]);
      traceback_fuel[i] = NULL;
    }

    free(traceback_fuel);
    traceback_fuel = NULL;
  }

  #ifndef NDEBUG
  printf("\tTraceback_fuel matrix deallocated\n");
  printf("\tDeallocating traceback_station matrix\n");
  #endif

  if(traceback_station != NULL) {
    for(matrix_size i = 0; i < rows; ++i) {
      free(traceback_station[i]);
      traceback_station[i] = NULL;
    }

    free(traceback_station);
    traceback_station = NULL;
  }

  #ifndef NDEBUG
  printf("\tTraceback_station matrix deallocated\n");
  printf("\tDeallocating fuel array\n");
  #endif

  free(fuel);

  #ifndef NDEBUG
  printf("\tTraceback_station matrix deallocated\n");
  printf("\tDeallocating offset array\n");
  #endif

  free(offset);

  #ifndef NDEBUG
  printf("\tOffset array deallocated\n");
  printf("Ending memory deallocation\n");
  #endif
}

void allocate_dynamic_matrixes(matrix_size ** matrix, matrix_size ** traceback_fuel, matrix_size ** traceback_station, matrix_size size) {
  
  *matrix = (matrix_size *) calloc(size, sizeof(matrix_size));
  if(*matrix == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough space to allocate dynamic matrix row of %ld bytes\n", 
          sizeof(matrix_size *) * size);
    #endif

    return;
  }

  #ifndef NDEBUG
  printf("\tAllocated dynamic programming matrix row of %ld bytes\n",
    sizeof(matrix_size *) * size);
  #endif

  *traceback_fuel = (matrix_size *) calloc(size, sizeof(matrix_size));
  if(*traceback_fuel == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough space to allocate traceback_fuel matrix row of %ld bytes\n", 
          sizeof(matrix_size *) * size);
    #endif

    return;
  }

  #ifndef NDEBUG
  printf("\tAllocated traceback_fuel row of %ld bytes\n",
    sizeof(matrix_size *) * size);
  #endif

  *traceback_station = (matrix_size *) calloc(size, sizeof(matrix_size));
  if(*traceback_station == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough space to allocate traceback_station matrix row of %ld bytes\n", 
          sizeof(matrix_size *) * size);
    #endif
  }
  #ifndef NDEBUG
  printf("\tAllocated traceback_station matrix row of %ld bytes\n", 
    sizeof(matrix_size *) * size);
  #endif
}

/**
 * @brief Compute the optimal solution through dynamic programming, computing every way of arriving at station[i] from station[i - 1]; save only the
 * fuels which actully allow to arrive at station[i] (skip all infinite of the recurrence equation).
 * 
 * @note If dir = forward, the solution that minimizes the distance from start; otherwise (dir = backward), is computed the solution which minimizes
 * the distance to the end.
 * @note Time complexity is T(n) = O(n * f), where f is the mean number of way you can arrive at a station.  
 * @note Space complexity is M(n) = O(n * f), where f is the mean number of way you can arrive at a station. 
*/
int min_stops_dynamic(const matrix_size * stations, matrix_size n_stations, const matrix_size * cars, matrix_size ** solution, matrix_size max_fuel, direction dir) {

  matrix_size ** matrix = NULL;
  matrix_size ** traceback_fuel = NULL;
  matrix_size ** traceback_station = NULL;

  matrix_size * fuel = NULL, * offset = NULL;

  #ifndef NDEBUG
  printf("Starting memory allocation\n");
  #endif

  matrix = (matrix_size **) calloc(n_stations, sizeof(matrix_size *));
  if(matrix == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough space to allocate dynamic matrix rows of %ld bytes\n", sizeof(matrix_size *) * n_stations);
    #endif

    free_memory(matrix, traceback_fuel, traceback_station, fuel, offset, n_stations);
    return mem_error;
  }

  #ifndef NDEBUG
  printf("\tAllocated dynamic matrix rows of %ld bytes\n", sizeof(matrix_size *) * n_stations);
  #endif

  traceback_fuel = (matrix_size **) calloc(n_stations, sizeof(matrix_size *));
  if(traceback_fuel == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough space to allocate traceback_fuel matrix rows of %ld bytes\n", 
          sizeof(matrix_size *) * n_stations);
    #endif

    free_memory(matrix, traceback_fuel, traceback_station, fuel, offset, n_stations);
    return mem_error;
  }

  #ifndef NDEBUG
  printf("\tAllocated traceback_fuel matrix rows of %ld bytes\n", sizeof(matrix_size *) * n_stations);
  #endif

  traceback_station = (matrix_size **) calloc(n_stations, sizeof(matrix_size *));
  if(traceback_station == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough space to allocate traceback_station matrix rows of %ld bytes\n", 
          sizeof(matrix_size *) * n_stations);
    #endif

    free_memory(matrix, traceback_fuel, traceback_station, fuel, offset, n_stations);
    return mem_error;
  }

  #ifndef NDEBUG
  printf("\tAllocated traceback_station matrix rows of %ld bytes\n", sizeof(matrix_size *) * n_stations);
  #endif

  fuel = (matrix_size *) malloc(sizeof(matrix_size) * min(n_stations, max_fuel + 1));
  if(fuel == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough space to allocate fuel array rows of %ld bytes\n", 
          sizeof(matrix_size ) * max(n_stations, max_fuel + 1));
    #endif

    free_memory(matrix, traceback_fuel, traceback_station, fuel, offset, n_stations);
    return mem_error;
  }

  #ifndef NDEBUG
  printf("\tAllocated fuel array of %ld bytes\n", sizeof(matrix_size) * max(n_stations, max_fuel + 1));
  #endif

  offset = (matrix_size *) malloc(sizeof(matrix_size) * min(n_stations, max_fuel + 1));
  if(offset == NULL) {
    #ifndef NDEBUG
    printf("\tNot enough space to allocate offset array rows of %ld bytes\n", 
          sizeof(matrix_size ) * max(n_stations, max_fuel + 1));
    #endif

    free_memory(matrix, traceback_fuel, traceback_station, fuel, offset, n_stations);
    return mem_error;
  }

  #ifndef NDEBUG
  printf("\tAllocated offset array of %ld bytes\n", sizeof(matrix_size) * max(n_stations, max_fuel + 1));
  #endif

  matrix_size index_length = 1, new_index_length = 0, offset_index = 0;
  int reload_index = -1;
  matrix_size min_stops = INF, last_station = INF, min_fuel = INF;

  allocate_dynamic_matrixes(&matrix[0], &traceback_fuel[0], &traceback_station[0], index_length);
  #ifndef NDEBUG
  printf("Ending memory allocation, starting base case initialization\n");
  #endif

  fuel[0] = cars[0];
  offset[0] = 0;
  matrix[0][0] = 0;
  traceback_station[0][0] = 0;
  traceback_fuel[0][0] = 0;

  #ifndef NDEBUG
  printf("Base case initializated, starting dynamic programming matrix computation\n\t");
  for(matrix_size f = 0; f < index_length; ++f) {
    printf("%d->%d:%d:%d  ", fuel[f], matrix[0][f], traceback_fuel[0][f], traceback_station[0][f]);
  }
  printf("\n");
  #endif

  for(matrix_size s = 0; s < n_stations - 1; ++s) {
    
    #ifndef NDEBUG
    printf("Starting iteration for s=%d, distance=%d, cars=%d, next_s=%d\n", s, stations[s], cars[s], stations[s + 1]);
    #endif

    for(matrix_size i = 0; i < index_length; ++i) {
      if(fuel[i] >= stations[s + 1] - stations[s]) {
        if(fuel[i] + stations[s] - stations[s + 1] == cars[s + 1]) {
          reload_index = new_index_length;
        }

        fuel[new_index_length++] = fuel[i] + stations[s] - stations[s + 1];
        offset[offset_index + 1] = offset[offset_index];
        ++offset_index;
      }
      else {
        ++offset[offset_index];
      }
    }

    if(reload_index >= 0) {
      #ifndef NDEBUG
      printf("\tAllocating dynamic programming matrixes for row %d and size %d\n", s + 1, new_index_length);
      #endif

      allocate_dynamic_matrixes(&matrix[s + 1], &traceback_fuel[s + 1], &traceback_station[s + 1], new_index_length);
    }
    else {
      #ifndef NDEBUG
      printf("\tAllocating dynamic programming matrixes for row %d and size %d\n", s + 1, new_index_length + 1);
      #endif

      allocate_dynamic_matrixes(&matrix[s + 1], &traceback_fuel[s + 1], &traceback_station[s + 1], new_index_length + 1);
    }

    if(traceback_station == NULL) {
      free_memory(matrix, traceback_fuel, traceback_station, fuel, offset, s);
      return mem_error;
    }

    min_stops = INF; 
    if(dir == forward) {
      last_station = INF;
    } 
    else {
      last_station = 0;
    }
    min_fuel = INF;

    for(matrix_size i = 0; i < new_index_length; ++i) {
      
      matrix[s + 1][i] = matrix[s][i + offset[i]];
      traceback_station[s + 1][i] = traceback_station[s][i + offset[i]];
      traceback_fuel[s + 1][i] = i + offset[i];

      if(matrix[s + 1][i] < min_stops) {
        min_stops = matrix[s  + 1][i];
        last_station = traceback_station[s + 1][i];
        min_fuel = traceback_fuel[s + 1][i];
      }
      else if(matrix[s + 1][i] == min_stops && dir == forward && traceback_station[s + 1][i] < last_station) {
        min_stops = matrix[s  + 1][i];
        last_station = traceback_station[s + 1][i];
        min_fuel = traceback_fuel[s + 1][i];  

        #ifndef NDEBUG
        printf("New best found: stops=%d, last_station=%d, last_fuel=%d\n", min_stops, stations[last_station], min_fuel);
        #endif
      }
      else if(matrix[s + 1][i] == min_stops && dir == backward && traceback_station[s + 1][i] > last_station) {
        min_stops = matrix[s  + 1][i];
        last_station = traceback_station[s + 1][i];
        min_fuel = traceback_fuel[s + 1][i];

        #ifndef NDEBUG
        printf("New best found: stops=%d, last_station=%d, last_fuel=%d\n", min_stops, stations[last_station], min_fuel);
        #endif
      }

      #ifndef NDEBUG
      printf("Index:%d --> %d->%d:%d:%d=%d\n", i,fuel[i], matrix[s + 1][i], traceback_fuel[s + 1][i], 
      traceback_station[s + 1][i], stations[traceback_station[s + 1][i]]);
      #endif
    }

    if(reload_index < 0) {
      matrix[s + 1][new_index_length] = min_stops + 1;
      traceback_station[s + 1][new_index_length] = s + 1;
      traceback_fuel[s + 1][new_index_length] = min_fuel;

      fuel[new_index_length] = cars[s + 1];

      if(last_station == INF)
        last_station = 0;
      #ifndef NDEBUG
      printf("Station reload not computed\n\t%d->%d:%d:%d, last_station=%d\n", fuel[new_index_length], matrix[s + 1][new_index_length], 
      traceback_fuel[s + 1][new_index_length], traceback_station[s + 1][new_index_length], stations[last_station]);
      #endif
      if(last_station == 0)
        last_station = INF;

      ++new_index_length;
    }
    else {
      if(dir == forward && min_stops + 1 < matrix[s + 1][reload_index]) {
        matrix[s + 1][reload_index] = min_stops + 1;
        traceback_station[s + 1][reload_index] = s + 1;
        traceback_fuel[s + 1][reload_index] = min_fuel;
      }
      else if(dir == backward && min_stops + 1 <= matrix[s + 1][reload_index]) {
        matrix[s + 1][reload_index] = min_stops + 1;
        traceback_station[s + 1][reload_index] = s + 1;
        traceback_fuel[s + 1][reload_index] = min_fuel;
      }

      #ifndef NDEBUG
      printf("Station reload upgraded\n\t%d->%d:%d:%d\n", fuel[reload_index], matrix[s + 1][reload_index], 
      traceback_fuel[s + 1][reload_index], traceback_station[s + 1][reload_index]);
      #endif
    }
    
    index_length = new_index_length;
    new_index_length = 0;
    reload_index = -1;
    offset[0] = 0;
    offset_index = 0;
  }

  #ifndef NDEBUG
  printf("Dynamic programming matrix computed\n");
  #endif

  min_stops = INF;
  min_fuel = INF;
  
  if(dir == forward) {
    last_station = INF;
    for(matrix_size i = 0; i < index_length; ++i) {
      if(matrix[n_stations - 1][i] < min_stops || (matrix[n_stations - 1][i] == min_stops && traceback_station[n_stations - 1][i] < last_station)) {
          min_stops = matrix[n_stations - 1][i];
          min_fuel = i;
          last_station = traceback_station[n_stations - 1][i];
      }
    }
  }
  else {
    last_station = 0;
    for(matrix_size i = 0; i < index_length; ++i) {
      if(matrix[n_stations - 1][i] < min_stops || (matrix[n_stations - 1][i] == min_stops && traceback_station[n_stations - 1][i] > last_station)) {
          min_stops = matrix[n_stations - 1][i];
          min_fuel = i;
          last_station = traceback_station[n_stations - 1][i];
      }
    }
  }

  if(min_stops < INF) {
    *solution = retrieve_traceback(stations, n_stations, matrix, traceback_fuel, traceback_station, min_stops, min_fuel);
    if(*solution == NULL) {
      min_stops = mem_error;
    }
  }
  else {
    *solution = NULL;
    min_stops = no_solution;
  }

  #ifndef NDEBUG
  printf("Traceback completed\n");
  #endif

  free_memory(matrix, traceback_fuel, traceback_station, fuel, offset, n_stations);
  return min_stops;
}
//-----------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------
//Simplest approach
//-----------------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief Compute the optimal solution starting from the end and finding the furthest station (e.g. the nearest to the start) which allows to reach
 * the last station; repeat until you arrive at the start. 
 * 
 * @note Is always found the solution that minimizes the distance from the first station (stations[0]).
 * @note Time complexity is T(n) = O(n^2) in the worst case, but generally looks linear (T(n) = O(n)). 
 * @note Space complexity is M(n) = O(n), reducible to O(s), where s is the length of the solution.
*/
int min_stops(const matrix_size * stations, matrix_size n_stations, const matrix_size * cars, matrix_size ** solution) {
  matrix_size stops, further_station_index, no_solution_found = 0;

  int end_index, start_index;

  end_index = n_stations - 1;
  start_index = 0;
  matrix_size * tmp_solution = (matrix_size *) malloc(sizeof(matrix_size) * n_stations);

  tmp_solution[0] = end_index;
  stops = 0;

  while(!no_solution_found && end_index > start_index) {

    further_station_index = end_index;

    for(int i = end_index - 1; i >= start_index; --i) {

      if(cars[i] >= stations[end_index] - stations[i]) {
        further_station_index = i;
      }
    }
    if(further_station_index < end_index) {
      if(further_station_index > start_index) {
        tmp_solution[stops + 1] = further_station_index;
        ++stops;
      }
      end_index = further_station_index;
    }
    else {
      no_solution_found = 1;
    }
  } 

  int return_value = no_solution;

  if(!no_solution_found) {
    tmp_solution[stops + 1] = start_index;
    (*solution) = (matrix_size *) malloc(sizeof(matrix_size) * (stops + 2));
    
    for(matrix_size i = 0; i < stops + 2; ++i) {
      (*solution)[i] = stations[tmp_solution[stops - i + 1]];
    }

    return_value = stops;
  }

  free(tmp_solution);
  return return_value;
}
//-----------------------------------------------------------------------------------------------------------------------------------------


void explain_solution(const matrix_size * stations, matrix_size n_stations, const matrix_size * cars, matrix_size * solution, matrix_size stops, direction dir) {
  for(matrix_size i = 0; i < stops + 1; ++i) {
    for(matrix_size j = 0; j < n_stations; ++j) {
      if(stations[j] == solution[i]) {
        if(dir == forward) {
          #ifndef NDEBUG
          printf("%d --> %d using %d/%d unit of fuel (%f)\n", solution[i], solution[i + 1], 
          solution[i + 1] - solution[i], cars[j], (double)(solution[i + 1] - solution[i]) / cars[j]);
          #endif
        }
        else {
          #ifndef NDEBUG
          printf("%d --> %d using %d/%d unit of fuel (%f)\n", solution[i], solution[i + 1], 
          solution[i] - solution[i + 1], cars[j], (double)(solution[i] - solution[i + 1]) / cars[j]);
          #endif
        }
        break;
      }
    }
  }
}

int solve(matrix_size * stations, matrix_size n_stations, matrix_size * cars, direction dir, matrix_size ** solution) {
    
    #ifndef NDEBUG
    printf("Starting solve\n");
    #endif

    int stops = 0;
    *solution = NULL;

    if(stations == NULL || cars == NULL) {
      #ifndef NDEBUG
      printf("\tNULL pointer\n");
      #endif

      return null_ptr;
    }

    if(dir == forward) {
        stops = min_stops(stations, n_stations, cars, solution);
    }
    else {
      
      matrix_size last_station = stations[n_stations - 1], tmp = 0;

      for(matrix_size i = 0; i < n_stations / 2; ++i) {
        tmp = last_station - stations[i];
        stations[i] = last_station - stations[n_stations - i - 1];
        stations[n_stations - i - 1] = tmp;

        tmp = cars[i];
        cars[i] = cars[n_stations - i - 1];
        cars[n_stations - i - 1] = tmp;
      }
      if(n_stations % 2 == 1) {
        stations[n_stations / 2] = last_station - stations[n_stations / 2];
      }

      #ifdef MINIMIZE_DISTANCE
      matrix_size max_fuel = 0;
      for(matrix_size i = 0; i < n_stations; ++i) {
        if(cars[i] > max_fuel) {
          max_fuel = cars[i];
        }
      }

      stops = min_stops_dynamic(stations, n_stations, cars, solution, max_fuel, backward);
      #else
      stops = min_stops(stations, n_stations, cars, solution);
      #endif

      if(stops >= 0){
        for(int i = 0; i < stops + 2; ++i){
          (*solution)[i] = last_station - (*solution)[i];
        }
      }

      for(matrix_size i = 0; i < n_stations / 2; ++i) {
        tmp = last_station - stations[i];
        stations[i] = last_station - stations[n_stations - i - 1];
        stations[n_stations - i - 1] = tmp;

        tmp = cars[i];
        cars[i] = cars[n_stations - i - 1];
        cars[n_stations - i - 1] = tmp;
      }
      if(n_stations % 2 == 1) {
        stations[n_stations / 2] = last_station - stations[n_stations / 2];
      }
    }

    #ifndef NDEBUG
    if(stops >= 0) {
      printf("Solution explanation:\n");
      explain_solution(stations, n_stations, cars, *solution, stops, dir);
    }
    #endif
    
    return stops;
}