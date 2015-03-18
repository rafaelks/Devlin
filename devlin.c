/**

 LICENCE

    The MIT License (MIT)

    Copyright (c) 2015 Rafael Kellermann Streit

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.


 README

    TODO: Comment about what's the application purpose.

 */


#include <stdio.h>

// TODO: Comment about it.
enum states {
    kStateCreating = 1,
    kStateReady = 2,
    kStateRunning = 3,
    kStateBlocked = 4,
    kStateDealloced = 5
};


// TODO: Comment about it.
struct process {
    int pid;
    int total_time;
    int remaining_time;
    int state;
};


int main(int argc, char *argv[]) {
    int processes;
    int cpu_time;

    // Validates if user setted two required parameters:
    // - Number of processes created
    // - CPU time
    if (argc != 2) {
        printf("You must set the number of processes to create and the CPU time simulated.\n");
        printf("Example: ./devlin 150 2\n");
        return 0;
    }

    processes = argv[0];
    cpu_time = argv[1];
}
