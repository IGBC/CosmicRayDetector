DRAM Based Radiation Detector
=============================
Uses your DRAM to detect cosmic rays causing "bit-flips".

Principle of Operation
----------------------
This tool allocates a large buffer and primes it with a test pattern. It then waits periodically scans the buffer searching for variations from the original data.

See https://en.wikipedia.org/wiki/Soft_error for details on how these errors occur. This program will detect all bit flips inside its buffer array, however these are primarilly caused by cosmic ray fallout, or failing hardware.

System Requirements
-------------------
 - A linux computer you leave running 24/7
 - A tmux shell you can abandon the program in for months on end
 - A large amount of available ram (at least 1GB, recommend 8GB)
 - Patience.
 
Specific RAM requirements:
To actually detect events your ram must be:
 - Uncompressed
 - Active at all times (PC not asleep)
 - Non ECC (sorry no Servers)
 - Not being swapped. (Linux will only swap if you run out of ram)

Other notes: 
This thing is a memory bandwidth hog. It scans the entire memory buffer every 60 seconds. On a R5 1600 with dual channel DDR4 it takes about 20 seconds for the process to cycle the full 8GB buffer. the wait time is exclusive of the processing time, leaving an 80 second cycle. On this system with this config this equates to a load average of about 0.25
 
FAQ
---
 - **Does this actually work?:** I don't know that's why I want lots of people to try it.
 - **How do I build it?:** `gcc main.c -o cosmic_ray_detector -Wall`
 - **What if I don't have 8GB of spare RAM?:** You can configure the amount of RAM used at line 21 of main.c the first number equates to the number of GB used.
 - **What if I don't want to use that much CPU?:** You can lower the scan freqency using the wait time on line 26.
 
Licence (MIT)
-------
Copyright 2018 SEGFAULT

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

