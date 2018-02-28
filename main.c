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
 * is trying to detect */

/* Memory Priming Pattern */
#define BITPATTERN 0x55 /* Thats 01010101 in ram */

/* Memory Buffer Size */
#define BUFFERSIZE 1024*1024*1024
/* Thats 1 GiB to you and me */

/* Time to sleep between memory checks in seconds. 
 * Effects the resolution of collected results */
#define WAITTIME 3600 /* one hour */

// We would like to thank our sponsors:
#include <stdio.h> // The best println on the east cost
#include <time.h>  // why buy a watch when there's time.h

/* Primes the test array before detection can start */
void* create_buffer(void);

/* Checks the test array for events
 * and repairs damage to the array */
void check_buffer(void* buffer);

int main(int argc, char* argv) {

}

void* create_buffer(void) {
    // Allocate a buffer
    void *buffer = (void*)malloc(BUFFERSIZE);
    // Cycle the buffer and set it's contents
    for (int i = 0; i < BUFFERSIZE; i++) {
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

void check_buffer(void* buffer) { 
    // Get the time of this check:
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime (&rawtime);

    for (int i = 0; i < BUFFERSIZE; i++) {
        // yea we're doing pointer math, 
        // we're bad people
        char *point = (char*)(buffer + i);
        // sue me!

        // test if the cell has changed.
        if (*point != BITPATTERN) {
            // Generate a pattern of the 
            // bits that changed
            char diff = *point ^ BITPATTERN;

            // Print Event
            // TODO: save to file
            printf("%s - EVENT: %08x - "BYTE_TO_BINARY_PATTERN"\n", 
                asctime(timeinfo),
                i,
                BYTE_TO_BINARY(diff));

            // restore the cell 
            *point = BITPATTERN;
        }
    }
}