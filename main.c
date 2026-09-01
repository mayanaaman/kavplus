#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INPUT_LENGTH 61
#define TRUE 1
#define FALSE 0
#define DIS_LOWER_LIMIT 0
#define DIS_UPPER_LIMIT 1000
#define DUR_LOWER_LIMIT 10
#define DUR_UPPER_LIMIT 100
#define FRE_LOWER_LIMIT 1
#define FRE_UPPER_LIMIT 50
#define TEST_DISTANCE_SORTED 1
#define TEST_DISTANCE_EQUAL 2
#define TEST_DURATION_SORTED 3
#define TEST_DURATION_EQUAL 4
#define TEST_FREQUENCY_SORTED 5
#define TEST_FREQUENCY_EQUAL 6
#define TEST_NAME_SORTED 7
#define TEST_NAME_EQUAL 8

/**
 * TODO add documentation
 */
/**
 * Checks that the program received exactly one valid command-line argument.
 * Returns EXIT_SUCCESS if the argument is valid, EXIT_FAILURE otherwise.
 */
int check_main_arguments(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: sort_lines <by_duration|by_distance|by_frequency|by_name|test>\n");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "by_duration")!=0 &&
        strcmp(argv[1],"by_distance")!=0 &&
        strcmp(argv[1],"by_frequency")!=0 &&
        strcmp(argv[1],"by_name")!=0 &&
        strcmp(argv[1],"test")!=0){
        printf("Usage: sort_lines <by_duration|by_distance|by_frequency|by_name|test>\n");
        return EXIT_FAILURE;
        }
    return EXIT_SUCCESS;
}
/**
 * Reads the number of bus lines from stdin.
 * Keeps asking until a positive integer is received.
 * Returns EXIT_SUCCESS on success, EXIT_FAILURE on input failure.
 */
int read_lines(int *number_of_lines) {
    char input[MAX_INPUT_LENGTH];
    int temp = 0;
    char end = '\0';
    while (TRUE) {
        printf("Enter number of lines. Then enter\n");
        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
            return EXIT_FAILURE;
        }
        if (sscanf(input, "%d %c", &temp, &end) != 1) {
            printf("Error: number of lines should be a positive integer\n");
            continue;
        }
        if (temp <=0) {
            printf("Error: The number of lines should be a positive integer\n");
            continue;
        }
        *number_of_lines = temp;
        return EXIT_SUCCESS;
    }
}
/**
 * Checks whether a bus line name is valid.
 * A valid name contains only lowercase letters and digits.
 * Returns TRUE if valid, FALSE otherwise.
 */
int is_valid_name(char *name) {
    int i = 0;
    if (strlen(name) >= NAME_LEN || strlen(name) < 1) {
        return FALSE;
    }
    while (name[i] != '\0') {
        if (!((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= '0' && name[i] <= '9'))) {
            return FALSE;
        }
        i++;
    }
    return TRUE;
}
/**
* Checks whether the given distance is in the valid range.
* Returns TRUE if valid, FALSE otherwise.
*/
int is_valid_distance(int distance) {
    if (distance < DIS_LOWER_LIMIT || distance > DIS_UPPER_LIMIT) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Checks whether the given frequency is in the valid range.
 * Returns TRUE if valid, FALSE otherwise.
 */
int is_valid_frequency(int frequency) {
    if (frequency < FRE_LOWER_LIMIT || frequency > FRE_UPPER_LIMIT) {
        return FALSE;
    }
    return TRUE;
}
/**
 * Checks whether the given duration is in the valid range.
 * Returns TRUE if valid, FALSE otherwise.
 */
int is_valid_duration(int duration) {
    if (duration < DUR_LOWER_LIMIT || duration > DUR_UPPER_LIMIT) {
        return FALSE;
    }
    return TRUE;
}

/**
 * Reads one valid bus line from stdin and stores it in the given BusLine.
 * Keeps asking until all fields are valid.
 * Returns EXIT_SUCCESS on success, EXIT_FAILURE on input failure.
 */
int read_line (BusLine *line) {
    char input[MAX_INPUT_LENGTH];
    char name[NAME_LEN];
    int distance = 0;
    int frequency = 0;
    int duration = 0;
    char end_char = '\0';
    while (TRUE) {
        printf("Enter line info. Then enter\n");
        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
            return EXIT_FAILURE;
        }
        if (sscanf(input,"%[^,],%d,%d,%d %c",name,&distance,&duration,&frequency,&end_char) != 4) {
            printf("Error: line should be in the format:<line_name>,<distance>,<duration>,<frequency>\n");
            continue;
        }
        if (!is_valid_name(name)) {
            printf("Error: invalid line name\n");
            continue;
        }
        if (!is_valid_distance(distance)) {
            printf("Error: invalid line distance\n");
            continue;
        }
        if (!is_valid_duration(duration)) {
            printf("Error: invalid line duration\n");
            continue;
        }
        if (!is_valid_frequency(frequency)) {
            printf("Error: invalid line frequency\n");
            continue;
        }
        strcpy(line->name,name);
        line->distance = distance;
        line->frequency = frequency;
        line->duration = duration;
        return EXIT_SUCCESS;
    }

}
/**
 * Reads all bus lines into the given array.
 * Returns EXIT_SUCCESS on success, EXIT_FAILURE on input failure.
 */
int read_all_lines(BusLine *lines, int number_of_lines) {
    for (int i = 0; i < number_of_lines; i++) {
        if (read_line(&lines[i])==EXIT_FAILURE) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
/**
 * Copies all bus lines from the source array to the destination array.
 */
void copy_lines(BusLine *dest, BusLine *src, int number_of_lines) {
    for (int i = 0; i < number_of_lines; i++) {
        dest[i]=src[i];
    }
}

/**
 * Prints all tests results in the required output format.
 */
void print_test_result(int test_number, int passed, char *pass_msg, char *fail_msg) {
    if (passed) {
        printf("TEST %d PASSED: %s\n", test_number, pass_msg);
    }
    else {
        printf("TEST %d FAILED: %s\n", test_number, fail_msg);
    }
}

/**
 * Runs 8 tests on the sorting functions.
 * Tests 1,3,5,7 check sorting correctness,
 * tests 2,4,6,8 check that the array content is unchanged.
 * Prints results in the required format.
 * Returns EXIT_SUCCESS, or EXIT_FAILURE on allocation error.
 */
int run_tests(BusLine *lines, int number_of_lines) {
    BusLine *original = malloc(sizeof(BusLine) * number_of_lines);
    if (original == NULL) {
        return EXIT_FAILURE;
    }
    copy_lines(original,lines,number_of_lines);
    bus_quick_sort(lines, lines+number_of_lines -1, DISTANCE);
    print_test_result(TEST_DISTANCE_SORTED, is_sorted_by_distance(lines, lines+number_of_lines -1),"sorted by distance", "Not sorted by distance");
    print_test_result(TEST_DISTANCE_EQUAL,is_equal(lines, lines+number_of_lines-1, original, original+number_of_lines-1),"same lines after sorting","array changed after sorting");
    copy_lines(lines,original,number_of_lines);
    bus_quick_sort(lines, lines+number_of_lines-1, DURATION);
    print_test_result(TEST_DURATION_SORTED, is_sorted_by_duration(lines, lines+number_of_lines-1),"sorted by duration", "Not sorted by duration" );
    print_test_result(TEST_DURATION_EQUAL,is_equal(lines, lines+number_of_lines-1, original, original+number_of_lines-1),"same lines after sorting","array changed after sorting");
    copy_lines(lines,original,number_of_lines);
    bus_quick_sort(lines, lines+number_of_lines-1, FREQUENCY);
    print_test_result(TEST_FREQUENCY_SORTED,is_sorted_by_frequency(lines, lines+number_of_lines-1),"sorted by frequency","Not sorted by frequency" );
    print_test_result(TEST_FREQUENCY_EQUAL,is_equal(lines, lines+number_of_lines-1, original, original+number_of_lines-1),"same lines after sorting","array changed after sorting");
    copy_lines(lines,original,number_of_lines);
    bus_bubble_sort(lines, lines+number_of_lines-1);
    print_test_result(TEST_NAME_SORTED,is_sorted_by_name(lines, lines+number_of_lines-1),"sorted by name","Not sorted by name");
    print_test_result(TEST_NAME_EQUAL,is_equal(lines, lines+number_of_lines-1, original, original+number_of_lines-1),"same lines after sorting","array changed after sorting");
    free(original);
    return EXIT_SUCCESS;
}

/**
 * Prints all bus lines in the required output format.
 */
void print_lines(BusLine *lines, int number_of_lines) {
    for (int i = 0; i < number_of_lines; i++) {
        printf("%s,%d,%d,%d\n",lines[i].name,lines[i].distance,lines[i].duration,lines[i].frequency);
    }
}

int main (int argc, char *argv[])
{
    int number_of_lines = 0;
    BusLine *lines = NULL;
    if (check_main_arguments(argc, argv)==EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    if (read_lines(&number_of_lines)==EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    lines = malloc(sizeof(BusLine) * number_of_lines);
    if (lines == NULL) {
        return EXIT_FAILURE;
    }
    if (read_all_lines(lines,number_of_lines)==EXIT_FAILURE) {
        free(lines);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1],"test")==0) {
        if (run_tests(lines,number_of_lines)==EXIT_FAILURE) {
            free(lines);
            return EXIT_FAILURE;
        }
    }
    else if (strcmp(argv[1],"by_distance")==0) {
        bus_quick_sort(lines, lines+number_of_lines-1, DISTANCE);
        print_lines(lines, number_of_lines);
    }
    else if (strcmp(argv[1],"by_duration")==0) {
        bus_quick_sort(lines, lines+number_of_lines-1, DURATION);
        print_lines(lines, number_of_lines);
    }
    else if (strcmp(argv[1],"by_frequency")==0) {
        bus_quick_sort(lines, lines+number_of_lines-1, FREQUENCY);
        print_lines(lines, number_of_lines);
    }
    else if (strcmp(argv[1],"by_name")==0) {
        bus_bubble_sort(lines, lines+number_of_lines-1);
        print_lines(lines, number_of_lines);
    }
    free(lines);
    return EXIT_SUCCESS;
}
