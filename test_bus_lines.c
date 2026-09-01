#include "test_bus_lines.h"
#include <string.h>
//TODO add implementation here

/**
 * Checks whether the bus lines are sorted by distance.
 * Returns 1 if sorted, 0 otherwise.
 */
int is_sorted_by_distance (const BusLine *start, const BusLine *end)
{
	const BusLine *current;
	for (current = start; current < end; current++)
	{
	    if (current->distance > (current+1)->distance)
		{
			return 0;
		}
	}
	return 1;
}

/**
 * Checks whether the bus lines are sorted by duration.
 * Returns 1 if sorted, 0 otherwise.
 */
int is_sorted_by_duration (const BusLine *start, const BusLine *end)
{
	const BusLine *current;
	for (current = start; current < end; current++)
	{
		if (current->duration > (current+1)->duration)
		{
			return 0;
		}
	}
	return 1;
}

/**
 * Checks whether the bus lines are sorted by frequency.
 * Returns 1 if sorted, 0 otherwise.
 */
int is_sorted_by_frequency (const BusLine *start, const BusLine *end)
{
	const BusLine *current;
	for (current = start; current < end; current++)
	{
		if (current->frequency > (current+1)->frequency)
		{
			return 0;
		}
	}
	return 1;
}

/**
 * Checks whether the bus lines are sorted by name.
 * Returns 1 if sorted, 0 otherwise.
 */
int is_sorted_by_name (const BusLine *start, const BusLine *end)
{
	const BusLine *current;
	for (current = start; current < end; current++)
	{
		if (strcmp(current->name, (current+1)->name) > 0)
		{
			return 0;
		}
	}
	return 1;
}

/**
 * Checks whether two bus line arrays contain the same line names.
 * Returns 1 if equal, 0 otherwise.
 */
int is_equal(const BusLine *start_sorted,const BusLine *end_sorted,const BusLine *start_original,const BusLine *end_original)
{
	const BusLine *current_sorted;
	const BusLine *current_original;
	int found;
	if (end_sorted - start_sorted != end_original - start_original)
	{
		return 0;
	}
	for (current_sorted = start_sorted; current_sorted <= end_sorted; current_sorted++)
	{
		found = 0;
		for (current_original = start_original; current_original <= end_original; current_original++)
		{
			if(strcmp(current_original->name, current_sorted->name)==0)
			{
				found = 1;
				break;
			}
		}
		if(!found)
		{
			return 0;
		}
	}
	return 1;
}














