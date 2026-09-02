#include <stdio.h>
#include <string.h>

#define NUM_LINES 4
#define NAME_LEN 21

typedef struct BusLine
{
    char name[NAME_LEN];
    int distance;
} BusLine;

static const unsigned int FAKE_ADDRESSES[NUM_LINES] = {
    0x1000, 0x1040, 0x1080, 0x10C0
};

static unsigned int fake_address_from_index(int index)
{
    return FAKE_ADDRESSES[index];
}

static void print_array_cells(BusLine arr[])
{
    printf("+--------+----------+-----------+----------+\n");
    printf("| Index  | Address  | Name      | Distance |\n");
    printf("+--------+----------+-----------+----------+\n");
    for (int index = 0; index < NUM_LINES; index++)
    {
        printf("| arr[%d] | 0x%04X   | %-9s | %-8d |\n",
               index,
               fake_address_from_index(index),
               arr[index].name,
               arr[index].distance);
    }
    printf("+--------+----------+-----------+----------+\n");
}

static void print_pointer_row(const char *label, int index, BusLine arr[])
{
    printf("%-8s -> &arr[%d] = 0x%04X (%s, distance %d)\n",
           label,
           index,
           fake_address_from_index(index),
           arr[index].name,
           arr[index].distance);
}

static void print_range(const char *label, int start_index, int end_index,
                        BusLine arr[])
{
    printf("%s [arr[%d] ... arr[%d]]\n", label, start_index, end_index);
    print_pointer_row("start", start_index, arr);
    print_pointer_row("end", end_index, arr);
}

static void swap_bus_lines(BusLine *a, BusLine *b)
{
    BusLine temp = *a;
    *a = *b;
    *b = temp;
}

static void print_iteration_header(int iteration_number)
{
    printf("\n============================================================\n");
    printf("Partition loop iteration %d\n", iteration_number);
    printf("============================================================\n");
}

static int simulate_partition(BusLine arr[], int start_index, int end_index)
{
    int pivot_index = end_index;
    int i_index = start_index;
    int iteration_number = 1;

    printf("Initial quick sort range:\n");
    print_range("Current range:", start_index, end_index, arr);

    printf("\nBecause this implementation does pivot = end:\n");
    print_pointer_row("pivot", pivot_index, arr);

    printf("\nInitial i pointer:\n");
    print_pointer_row("i", i_index, arr);

    printf("\nArray before partition:\n");
    print_array_cells(arr);

    for (int j_index = start_index; j_index < end_index; j_index++)
    {
        int should_swap;

        print_iteration_header(iteration_number);

        printf("Current quick sort range [start ... end]:\n");
        print_range("Range:", start_index, end_index, arr);

        printf("\nCurrent pointers:\n");
        print_pointer_row("pivot", pivot_index, arr);
        print_pointer_row("i", i_index, arr);
        print_pointer_row("j", j_index, arr);

        printf("\nComparison:\n");
        printf("j->distance <= pivot->distance\n");
        printf("%d <= %d\n", arr[j_index].distance, arr[pivot_index].distance);

        should_swap = arr[j_index].distance <= arr[pivot_index].distance;
        printf("Result: %s\n", should_swap ? "true" : "false");

        if (should_swap)
        {
            printf("\nSwap event:\n");
            printf("swap_bus_lines(i, j) swaps arr[%d] and arr[%d]\n",
                   i_index,
                   j_index);
            swap_bus_lines(&arr[i_index], &arr[j_index]);

            printf("\nArray after swap_bus_lines(i, j):\n");
            print_array_cells(arr);

            printf("\ni movement after swap:\n");
            printf("i moves from &arr[%d] = 0x%04X to &arr[%d] = 0x%04X\n",
                   i_index,
                   fake_address_from_index(i_index),
                   i_index + 1,
                   fake_address_from_index(i_index + 1));
            i_index++;
            print_pointer_row("i", i_index, arr);
        }
        else
        {
            printf("\nSwap event:\n");
            printf("swap_bus_lines(i, j) is not called.\n");
            printf("i does not move.\n");

            printf("\nArray after this comparison:\n");
            print_array_cells(arr);
        }

        printf("\nj movement at end of loop iteration:\n");
        if (j_index + 1 < end_index)
        {
            printf("j++ moves from &arr[%d] = 0x%04X to &arr[%d] = 0x%04X\n",
                   j_index,
                   fake_address_from_index(j_index),
                   j_index + 1,
                   fake_address_from_index(j_index + 1));
        }
        else
        {
            printf("j++ moves from &arr[%d] = 0x%04X to end at &arr[%d] = 0x%04X\n",
                   j_index,
                   fake_address_from_index(j_index),
                   end_index,
                   fake_address_from_index(end_index));
            printf("The loop stops because j < end is now false.\n");
        }

        iteration_number++;
    }

    printf("\n============================================================\n");
    printf("Final pivot swap\n");
    printf("============================================================\n");
    printf("After the loop, the implementation calls swap_bus_lines(i, end).\n\n");
    print_pointer_row("i", i_index, arr);
    print_pointer_row("end", end_index, arr);

    printf("\nFinal swap event:\n");
    printf("swap_bus_lines(i, end) swaps arr[%d] and arr[%d]\n",
           i_index,
           end_index);
    swap_bus_lines(&arr[i_index], &arr[end_index]);

    printf("\nArray after partition:\n");
    print_array_cells(arr);

    printf("\nReturned pivot pointer:\n");
    print_pointer_row("return", i_index, arr);

    return i_index;
}

static void print_recursive_ranges(int start_index, int end_index,
                                   int pivot_index, BusLine arr[])
{
    printf("\n============================================================\n");
    printf("Recursive ranges created after partition\n");
    printf("============================================================\n");

    printf("Left recursive call: bus_quick_sort(start, pivot - 1, DISTANCE)\n");
    printf("pivot - 1 moves one whole BusLine left.\n");
    if (start_index <= pivot_index - 1)
    {
        print_range("Left range:", start_index, pivot_index - 1, arr);
    }
    else
    {
        printf("Left range is empty.\n");
    }

    printf("\nRight recursive call: bus_quick_sort(pivot + 1, end, DISTANCE)\n");
    printf("pivot + 1 moves one whole BusLine right.\n");
    if (pivot_index + 1 <= end_index)
    {
        print_range("Right range:", pivot_index + 1, end_index, arr);
    }
    else
    {
        printf("Right range is empty.\n");
    }
}

int main(void)
{
    BusLine arr[NUM_LINES] = {
        {"Dan", 40},
        {"Alice", 10},
        {"Charlie", 30},
        {"Bob", 20}
    };
    int start_index = 0;
    int end_index = 3;
    int returned_pivot_index;

    printf("Quick sort partition pointer visualization\n");
    printf("This standalone program uses fake addresses for learning.\n");
    printf("It simulates the first partition call for DISTANCE sorting.\n\n");

    printf("Fake address rule:\n");
    printf("arr[0] = 0x1000, arr[1] = 0x1040, arr[2] = 0x1080, arr[3] = 0x10C0\n");
    printf("Pointer + 1 moves to the next BusLine cell, not the next byte.\n\n");

    printf("Simulated call:\n");
    printf("bus_quick_sort(&arr[0], &arr[3], DISTANCE)\n\n");

    returned_pivot_index = simulate_partition(arr, start_index, end_index);
    print_recursive_ranges(start_index, end_index, returned_pivot_index, arr);

    return 0;
}