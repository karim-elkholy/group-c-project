

#ifndef PORTABLE64_COUNTER_H
#define PORTABLE64_COUNTER_H

/* 
 * Implementation of a 64-bit counter.
 * 
 * This struct satisfies 2 purposes:
 * 1. In ANSI C, unsigned long long is not supported.
 * 2. In 32-bit systems, allows numbers larger than 2³² - 1 to be stored easily.
 * 
 * Each increment adds to 'lo'. 
 * Once 'lo' reaches 2³² - 1, it is reset to 0 and 'hi' is incremented.
 * 
 * The max value is 2³² - 1 since counting starts at 0.
 */
typedef struct {

    /* Low 32 bits 
    * Re
    */
    unsigned long lo; 

    /* High 32 bits 
     */
    unsigned long hi; 

} uint64_counter_t;

uint64_counter_t *portable64_counter_init();
void portable64_counter_increment(uint64_counter_t *counter);

#endif /* PORTABLE64_COUNTER_H */
