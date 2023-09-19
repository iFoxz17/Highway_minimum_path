#include <stdio.h>

#define _OPEN_SYS_ITOA_EXT
#include <stdlib.h>

#include <string.h>

const char test_directory[] = {"test/"};
const unsigned int test_directory_length = 5;

const char test_prefix[] = {"open_"};
const unsigned int test_prefix_length = 5;

const char test_extension[] = {".txt"};
const unsigned int test_extension_length = 4;

const char test_solution_extension[] = {".output.txt"};
const unsigned int test_solution_length = 11;

const unsigned int N_TEST = 111;
const unsigned int TEST_OFFSET = 1;

int main() {

    char command[100];
    unsigned int i = 0, failed = 0, pos = 0;

    while(i < N_TEST && !failed) {
        pos = 0;

        printf("Running test %d --> ", i + TEST_OFFSET);

        strcpy(command, "cat ");
        pos += 4;

        strcpy(command + pos, test_directory);
        pos += test_directory_length;

        strcpy(command + pos, test_prefix);
        pos += test_prefix_length;

        sprintf(command + pos, "%u", i + TEST_OFFSET);
        unsigned int backup = i + TEST_OFFSET;
        while(backup > 0) {
            ++pos;
            backup /= 10;
        }

        strcpy(command + pos, test_extension);
        pos += test_extension_length;

        strcpy(command + pos, " | ./main > tmp\0");
        int return_code = system(command);

        pos = 0;
        strcpy(command, "diff tmp ");
        pos += 9;

        strcpy(command + pos, test_directory);
        pos += test_directory_length;

        strcpy(command + pos, test_prefix);
        pos += test_prefix_length;

        
        sprintf(command + pos, "%u", i + TEST_OFFSET);
        backup = i + TEST_OFFSET;
        while(backup > 0) {
            ++pos;
            backup /= 10;
        }

        strcpy(command + pos, test_solution_extension);
        pos += test_solution_length;

        strcpy(command + pos, " > diff_output\0");

        return_code = system(command);

        FILE * check = fopen("diff_output", "r");
        char c;

        int n = fscanf(check, "%c", &c);
        if(n != EOF) {
            printf("not passed\n");
            failed = 1;
        } 
        else {
            printf("passed\n");
        }

        fclose(check);

        ++i;
    }

    if(failed) {
        printf("Output avaible in file tmp, differences avaibles in file diff_output\n");
    }
    else {
        printf("All tests passed\n");
        system("rm tmp");
        system("rm diff_output");
    }

    return 0;
}