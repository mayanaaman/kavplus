#include <stdio.h>
#include <string.h>

#define NUM_LINES 3
#define NAME_LEN 21

typedef struct BusLine
{
    char name[NAME_LEN];
} BusLine;

static const unsigned int FAKE_ADDRESSES[NUM_LINES] = {0x1000, 0x1040, 0x1080};

static unsigned int fake_address_from_index(int index)
{
    return FAKE_ADDRESSES[index];
}

static void print_array_cells(BusLine arr[])
{
    printf("+--------+----------+-----------+\n");
    printf("| Index  | Address  | Name      |\n");
    printf("+--------+----------+-----------+\n");
    for (int index = 0; index < NUM_LINES; index++)
    {
        printf("| arr[%d] | 0x%04X   | %-9s |\n",
               index,
               fake_address_from_index(index),
               arr[index].name);
    }
    printf("+--------+----------+-----------+\n");
}

static void print_pointer_row(const char *label, int index)
{
    printf("%-7s -> &arr[%d] = 0x%04X\n",
           label,
           index,
           fake_address_from_index(index));
}

static void swap_bus_lines(BusLine *a, BusLine *b)
{
    BusLine temp = *a;
    *a = *b;
    *b = temp;
}

static void print_step_header(int pass_number, int comparison_number)
{
    printf("\n============================================================\n");
    printf("Pass %d, comparison %d\n", pass_number, comparison_number);
    printf("============================================================\n");
}

static void print_step(BusLine arr[], int i_index, int j_index, int pass_number,
                       int comparison_number)
{
    int next_index = j_index + 1;
    int comparison = strcmp(arr[j_index].name, arr[next_index].name);
    int should_swap = comparison > 0;

    print_step_header(pass_number, comparison_number);

    printf("Array cells before comparison:\n");
    print_array_cells(arr);

    printf("\nFixed range pointers:\n");
    print_pointer_row("start", 0);
    print_pointer_row("end", 2);

    printf("\nLoop pointers for this comparison:\n");
    print_pointer_row("i", i_index);
    print_pointer_row("j", j_index);
    print_pointer_row("j + 1", next_index);

    printf("\nComparison:\n");
    printf("strcmp(j->name, (j + 1)->name) compares \"%s\" and \"%s\"\n",
           arr[j_index].name,
           arr[next_index].name);
    printf("strcmp result > 0? %s\n", should_swap ? "yes" : "no");

    if (should_swap)
    {
        printf("\nSwap event:\n");
        printf("swap_bus_lines(j, j + 1) swaps arr[%d] and arr[%d]\n",
               j_index,
               next_index);
        swap_bus_lines(&arr[j_index], &arr[next_index]);
    }
    else
    {
        printf("\nSwap event:\n");
        printf("No swap is called.\n");
    }

    printf("\nArray state after this comparison:\n");
    print_array_cells(arr);
}

int main(void)
{
    BusLine arr[NUM_LINES] = {{"Dan"}, {"Alice"}, {"Charlie"}};
    int pass_number = 1;
    int comparison_number = 1;

    printf("Bubble sort pointer memory visualization\n");
    printf("This standalone program uses fake addresses for learning.\n\n");
    printf("Initial memory layout:\n");
    print_array_cells(arr);

    printf("\nAssumptions:\n");
    print_pointer_row("start", 0);
    print_pointer_row("end", 2);
    printf("Each fake address is 0x40 bytes after the previous one.\n");
    printf("So for a BusLine * pointer, j + 1 moves to the next BusLine cell.\n");

    for (int i_index = 0; i_index < NUM_LINES - 1; i_index++)
    {
        int last_j_index = (NUM_LINES - 2) - i_index;

        for (int j_index = 0; j_index <= last_j_index; j_index++)
        {
            print_step(arr, i_index, j_index, pass_number, comparison_number);
            comparison_number++;
        }

        pass_number++;
    }

    printf("\n============================================================\n");
    printf("Final sorted array\n");
    printf("============================================================\n");
    print_array_cells(arr);

    return 0;
}