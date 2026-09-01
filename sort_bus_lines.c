#include "sort_bus_lines.h"
#include <string.h>


//TODO add implementation here
/**
 * Swaps the contents of two BusLine variables.
 */
void swap_bus_lines(BusLine *a, BusLine *b)
{
	BusLine temp = *a;
	*a = *b;
	*b = temp;
}
/**
 * Sorts the bus lines by name using bubble sort.
 */
void bus_bubble_sort(BusLine *start, BusLine *end)
{
	BusLine *i;
	BusLine *j;
	for (i = start; i < end; i++)
	{
		for (j = start; j < end - (i-start); j++)
		{
			if (strcmp(j->name, (j+1)->name) > 0)
			{
				swap_bus_lines(j, j+1);
			}
		}

	}
}

/**
 * Compares two bus lines according to the given sort type.
 * Returns a negative, zero, or positive value according to the comparison.
 */
int compare_by_sort_type(BusLine *a, BusLine *b, SortType sort_type)
	{
		if (sort_type == DISTANCE)
		{
			return a->distance - b->distance;
		}
		if (sort_type == DURATION)
		{
			return a->duration - b->duration;
		}
		return a->frequency - b->frequency;

	}

/**
 * Partitions the array segment around a pivot for quick sort.
 * Returns a pointer to the pivot after partitioning.
 */
BusLine *partition(BusLine *start, BusLine *end, SortType sort_type)
	{
		BusLine *pivot = end;
		BusLine *i = start;
		BusLine *j;
		for (j = start; j < end; j++)
		{
			if (compare_by_sort_type(j, pivot, sort_type) <= 0)
			{
				swap_bus_lines(i, j);
				i++;
			}

		}
		swap_bus_lines(i, end);
		return i;
	}

/**
 * Sorts the bus lines using quick sort according to the given sort type.
 */
void bus_quick_sort(BusLine *start, BusLine *end, SortType sort_type)
{
	BusLine *pivot;
	if (start < end)
		{
			pivot = partition(start, end, sort_type);
			bus_quick_sort(start, pivot-1, sort_type);
			bus_quick_sort(pivot+1, end, sort_type);
		}



}