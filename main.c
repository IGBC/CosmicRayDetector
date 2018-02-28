/* This program is an attempt to detect "bit-flips"
 * in memory, caused by background radiation. The 
 * principle of opperation is to allocate a large 
 * buffer and prime it with a test pattern. Wait
 * for a large period of time, then read the buffer
 * back, searching for variations from the original
 * data 
 *
 * See https://en.wikipedia.org/wiki/Soft_error
 * for details on what it is this insane program
 * is trying to detect 
 * 
 * Build instructions: gcc main.c -o cosmic_ray_detector -Wall
 * 
 * */

/* Memory Priming Pattern */
#define BITPATTERN 0x55 /* Thats 01010101 in ram */

/* Memory Buffer Size */
#define BUFFERSIZE 8LL*1024LL*1024LL*1024LL
/* Thats 8 GiB to you and me */

/* Time to sleep between memory checks in seconds. 
 * Effects the resolution of collected results */
#define WAITTIME 60 /* one minute */

// We would like to thank our sponsors:
#include <stdlib.h> // You thought you had a choice? Cute.
#include <string.h> // Quantum theory now available in C.
#include <unistd.h> // Providing narcolepcy since 1992 
#include <stdio.h>  // The best println on the east cost.
#include <time.h>   // why buy a watch when there's time.h

/* Primes the test array before detection can start */
volatile void* create_buffer(void);

/* Checks the test array for events
 * and repairs damage to the array */
void check_buffer(volatile void* buffer);

int main(void) {
    // init the buffer
    volatile void *b = create_buffer();
    // Loop forever
    while (1) {
        sleep(WAITTIME);
        check_buffer(b);
    }
    return -1;
}

volatile void* create_buffer(void) {
    // Allocate a buffer
    volatile void *buffer = (void*)malloc(BUFFERSIZE);
    // Cycle the buffer and set it's contents
    for (size_t i = 0; i < BUFFERSIZE; i++) {
        // yea we're doing pointer math, 
        // we're bad people
        char *point = (char*)(buffer + i);
        // sue me!

        // set the contents of the 
        // byte to the test pattern
        *point = BITPATTERN;
    }
    return buffer;
}

/* Used for printing the bit mask of a byte of data */
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? 'X' : '-'), \
  (byte & 0x40 ? 'X' : '-'), \
  (byte & 0x20 ? 'X' : '-'), \
  (byte & 0x10 ? 'X' : '-'), \
  (byte & 0x08 ? 'X' : '-'), \
  (byte & 0x04 ? 'X' : '-'), \
  (byte & 0x02 ? 'X' : '-'), \
  (byte & 0x01 ? 'X' : '-') 

void check_buffer(volatile void* buffer) { 
    for (size_t i = 0; i < BUFFERSIZE; i++) {
        // yea we're doing pointer math, 
        // we're bad people
        char *point = (char*)(buffer + i);
        // sue me!

        // test if the cell has changed.
        if (*point != BITPATTERN) {
            // Generate a pattern of the 
            // bits that changed
            char diff = *point ^ BITPATTERN;

            // Get time:
            time_t mytime = time(NULL);
            char *time_str = ctime(&mytime);
            time_str[strlen(time_str)-1] = '\0';

            // Print Event
            // TODO: save to file
            printf("%s - EVENT: %016lx - "BYTE_TO_BINARY_PATTERN"\n", 
                time_str,
                i,
                BYTE_TO_BINARY(diff));

            // restore the cell 
            *point = BITPATTERN;
        }
    }
}