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
#include <stdlib.h>
#include <unistd.h>

// IO Hard Drive
#define kIOHardDriveTimeMin 200
#define kIOHardDriveTimeMax 300

// IO Video Drive
#define kIOVideoDriveTimeMin 100
#define kIOVideoDriveTimeMax 200

// IO Printer
#define kIOPrinterTimeMin 500
#define kIOPrinterTimeMax 600


// TODO: Comment about it.
enum io_devices {
    kIODeviceNone = 0,
    kIODeviceHardDrive = 1,
    kIODeviceVideoDrive = 2,
    kIODevicePrinter = 3
};


// TODO: Comment about it.
enum states {
    kStateUndefined = 0,
    kStateCreating = 1,
    kStateReady = 2,
    kStateRunning = 3,
    kStateBlocked = 4,
    kStateDealloced = 5
};


// TODO: Comment about it.
typedef struct {
    int pid;
    int total_time;
    int remaining_time;
    int state;
    int io_device;
} process;


int main(int argc, char *argv[]) {
    int processes_total;
    int cpu_time;

    // Validates if user setted two required parameters:
    // - Number of processes created
    // - CPU time
    if (argc != 3) {
        printf("You must set the number of processes to create and the CPU time simulated.\n");
        printf("Example: ./devlin 150 2\n");
        return 0;
    }

    // Export arguments
    processes_total = atoi(argv[1]);
    cpu_time = atoi(argv[2]);

    printf("Process is starting with %d processes and %d CPU time.\n", processes_total, cpu_time);

    // Create a new array of process, with the 
    // total size we will need while application runs
    process *processes = malloc(processes_total * (sizeof *processes));

    // OK, let's do the hard work... The first thing
    // we need to do is create process, set PID,
    // random a total time based on the specifications
    // and start with creating state.
    // 
    // Then, we loop the application until process state
    // is ready, blocked or dealloced. When it happens,
    // we must move to the next process.
    for (int i = 0; i < processes_total; i++) {
        int io_time;

        process *p = &processes[i];

        p->pid = i + 1;
        p->total_time = (rand() % 200) + 100;
        p->remaining_time = 100;
        p->state = kStateRunning;
        p->io_device = (rand() % 4);

        // Update process total time based on
        // IO device that was randomic choiced.
        switch (p->io_device) {
            case kIODeviceHardDrive:
                io_time = (rand() % kIOHardDriveTimeMin) + kIOHardDriveTimeMax;
                break;

            case kIODeviceVideoDrive:
                io_time = (rand() % kIOVideoDriveTimeMin) + kIOVideoDriveTimeMax;
                break;

            case kIODevicePrinter:
                io_time = (rand() % kIOPrinterTimeMin) + kIOPrinterTimeMax;
                break;
        }

        p->total_time = p->total_time + io_time;

        printf("Process %d created with total time %d\n", p->pid, p->total_time);
    }
}
