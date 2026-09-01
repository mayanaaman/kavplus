#include <stddef.h>

#include "sort_bus_lines.h"


void sort_by_name_wrapper(BusLine *lines, int length)
{
    if (lines == NULL || length <= 1)
    {
        return;
    }
    bus_bubble_sort(lines, lines + length - 1);
}

void sort_by_distance_wrapper(BusLine *lines, int length)
{
    if (lines == NULL || length <= 1)
    {
        return;
    }
    bus_quick_sort(lines, lines + length - 1, DISTANCE);
}

void sort_by_duration_wrapper(BusLine *lines, int length)
{
    if (lines == NULL || length <= 1)
    {
        return;
    }
    bus_quick_sort(lines, lines + length - 1, DURATION);
}

void sort_by_frequency_wrapper(BusLine *lines, int length)
{
    if (lines == NULL || length <= 1)
    {
        return;
    }
    bus_quick_sort(lines, lines + length - 1, FREQUENCY);
}
