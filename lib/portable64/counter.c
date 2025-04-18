

#include <stdio.h>
#include <stdlib.h>
#include "portable64/counter.h"

/*******************************************************************************
 * Increments the counter.
 *
 * inputs:
 * - counter - The counter to increment.
 *
 * outputs:
 * - none
 ******************************************************************************/
void portable64_counter_increment(uint64_counter_t *counter)
{
    /* If the low part is full
     * 0xFFFFFFFF is equal to 2³² -1
     * Max value unsigned long can hold is 2³² -1
     */
    if (counter->lo == 0xFFFFFFFF)
    {
        /* Reset the low part */
        counter->lo = 0;
        /* Increment the high part */
        counter->hi += 1;

        /* Values under 2³² - 1 should increment normally */
    }
    else
    {
        /* Increment the low part */
        counter->lo += 1;
    }
}

/*******************************************************************************
 * Initializes a 64-bit counter.
 *
 * inputs:
 * - none
 *
 * outputs:
 * - counter - The counter.
 ******************************************************************************/
uint64_counter_t *portable64_counter_init()
{
    uint64_counter_t *counter = (uint64_counter_t *)malloc(
        sizeof(uint64_counter_t));
    counter->lo = 0;
    counter->hi = 0;
    return counter;
}
