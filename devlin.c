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
 
 This program will simulate how a basic computer works, with pre-allocated
 processes and also how many time (in seconds) each CPU time will last.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Rules
#define MAX_LOOP_BY_PROCESS 50

// IO Hard Drive
static int kIOHardDriveTimeMin = 200;
static int kIOHardDriveTimeMax = 300;

// IO Video Drive
static int kIOVideoDriveTimeMin = 100;
static int kIOVideoDriveTimeMax = 200;

// IO Printer
static int kIOPrinterTimeMin = 500;
static int kIOPrinterTimeMax = 600;


/**
 IO devices
 Hard Drive, Video Drive & Printer.
 */
enum io_devices {
    kIODeviceNone = 0,
    kIODeviceHardDrive = 1,
    kIODeviceVideoDrive = 2,
    kIODevicePrinter = 3
};


/**
 Process states
 
 - Creating: When process is being created.
 - Ready: It's ready to be executed, on queue.
 - Running: Process it's actually running.
 - Blocked: When process is blocked, because of some random reason.
 - Dealloced: Process is already finished and dealloced.
 */
enum states {
    kStateUndefined = 0,
    kStateCreating = 1,
    kStateReady = 2,
    kStateRunning = 3,
    kStateBlocked = 4,
    kStateDealloced = 5
};


/**
 Process struct
 @param pid Process identifier.
 @param total_time Total time that process will take to finish. Contains IO CPU time.
 @param remaining_time Helper that bring to use remaining time to run process.
 @param state Current process state, based on states struct.
 @param io_device Process IO device, based on io_devices struct.
 */
typedef struct {
    int pid;
    int state;
    
    int total_time;
    int remaining_time;
    int running_loop_time;
    int current_status_time;
    int io_time;
} process;


// Timer variables, to know how much time
// the program spent running
clock_t begin, end;

int clear();

process *p_tmp;
process *processes;

int cpu_time_seconds;
int cpu_time_running;
int processes_total;
int p_counter;

/**
  @return Returns if have process to run.
  */
int has_process_to_run();

/**
  @param pid
    PID of the process.

  @param status
    New status of process.
  */
int update_process_status(int pid, int status);

/**
  Update all processes state time
  */
int update_processes_state_time();

/**
  Get the next object in the ready list
  and change status to Running.

  @return True or false if some process changed to running state.
  */
int move_next_ready_process_to_running();

/**
  @return Return currently running process. In case we don't have one,
    it will return NULL.
  */
process *running_process();

/**
  @param process
    Process pointer.

  @param state
    New state of the process.

  @return True or false if process state was changed.
  */
int processes_with_state(process (*buffer), int state);

/**
  @return Return the count of running processes.
  */
int count_running_process();

/**
  @param processes
    Processes list pointer.

  @param state
    State to filter.

  @return Counter of process in the state.
  */
int count_processes_with_state(process *processes, int state);


int main(int argc, char *argv[]) {
    // Start timer
    begin = clock();

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
    cpu_time_seconds = atoi(argv[2]);

    // Create a new array of process, with the
    // total size we will need while application runs
    processes = malloc(processes_total * sizeof(process));

    // OK, let's do the hard work... The first thing
    // we need to do is create process, set PID,
    // random a total time based on the specifications
    // and start with creating state.
    //
    // Then, we loop the application until process state
    // is ready, blocked or dealloced. When it happens,
    // we must move to the next process.
    do {
        clear();

        // Random if should create a new process and if
        // should request I/O in current running process.
        int should_create_process = (rand() % 200) < 20 && p_counter < processes_total;
        int should_request_io = (rand() % 200) == 1;

        process *p_running = running_process();

        process *p_ready = malloc(processes_total * sizeof(p_tmp));
        processes_with_state(p_ready, kStateReady);

        int p_running_count = count_running_process();
        int p_ready_count = count_processes_with_state(processes, kStateReady);
        int p_blocked_count = count_processes_with_state(processes, kStateBlocked);
        int p_dealloced_count = count_processes_with_state(processes, kStateDealloced);

        printf("CPU Time: %d\n", cpu_time_running);
        printf("Running: %d\n", p_running_count);
        printf("Ready: %d\n", p_ready_count);
        printf("Blocked: %d\n", p_blocked_count);
        printf("Dealloced: %d\n", p_dealloced_count);
        
        if (p_running != NULL) {
            p_running->running_loop_time = p_running->running_loop_time + 1;
            p_running->total_time--;

            // Process ended? Let's kill & move to the next.
            if (p_running->total_time <= 0 && p_running->state == kStateRunning) {
                update_process_status(p_running->pid, kStateDealloced);
                move_next_ready_process_to_running();
            }

            // Is there something running? For how long?
            // Validates it and create a new one if needed.
            // If don't, increase loop time of process.
            if (p_running->running_loop_time == MAX_LOOP_BY_PROCESS) {
                update_process_status(p_running->pid, kStateReady);
                move_next_ready_process_to_running();
            }

            printf("Current running PID: %d\n", p_running->pid);
            printf("Current running CPU time: %d\n", p_running->running_loop_time);

        // If there is not process running, let's get the first one
        // and start running it.
        } else {
            printf("There's no process running. Let's get the first one from ReadyList (%d).\n", p_ready_count);
            printf("There's %d processes created.\n", p_counter);
            printf("We still have %d processes to create.\n", processes_total - p_counter);

            if (p_ready_count > 0) {
                update_process_status(p_ready[0].pid, kStateRunning);
            }
        }
        
        // Create a new process, if neeeded
        if (should_create_process) {
            process *new_process = &processes[p_counter];
            p_counter++;

            new_process->pid = p_counter;
            new_process->total_time = (rand() % 200) + 100;
            new_process->remaining_time = new_process->total_time;
            new_process->running_loop_time = 0;
            new_process->current_status_time = 0;
            new_process->io_time = 0;
            new_process->state = kStateReady;
        }
        
        // Update process total time based on
        // IO device that was randomic choiced.
        if (p_running != NULL && should_request_io) {
            int io_time = 0;
            
            switch ((rand() % 3) + 1) {
                case kIODeviceHardDrive:
                    io_time = (rand() % 100) + kIOHardDriveTimeMin;
                    break;
                    
                case kIODeviceVideoDrive:
                    io_time = (rand() % 100) + kIOVideoDriveTimeMin;
                    break;
                    
                case kIODevicePrinter:
                    io_time = (rand() % 100) + kIOPrinterTimeMin;
                    break;
            }
            
            p_running->io_time = io_time;
            update_process_status(p_running->pid, kStateBlocked);
            move_next_ready_process_to_running();

            printf("Process %d was blocked for %d CPU cicles\n", p_running->pid, io_time);
        }

        update_processes_state_time();

        sleep(cpu_time_seconds);
        cpu_time_running++;
    } while (has_process_to_run());

    // End timer
    end = clock();

    // Well done!
    printf("Time running: %.02f seconds.\n", (double)(end - begin) / CLOCKS_PER_SEC);
    printf("Yeah, all process are done.\n");
}


#pragma mark - UI

int clear() {
    const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREE_ANSI, 12);
    return 1;
}


#pragma mark - Verifications

int has_process_to_run() {
    for (int i = 0; i < processes_total; i++) {
        if (processes[i].state != kStateDealloced) {
            return 1;
        }
    }
    
    return sizeof(processes) == 0;
}


#pragma mark - Processes Update operations

int update_process_status(int pid, int status) {
    for (int i = 0; i < p_counter; i++) {
        if (processes[i].pid == pid) {
            processes[i].state = status;
            processes[i].current_status_time = 0;
            processes[i].running_loop_time = 0;
            
            printf("Process %d changed status to %d\n", pid, status);
            
            return 1;
        }
    }
    
    return 0;
}

int update_processes_state_time() {
    for (int i = 0; i < p_counter; i++) {
        processes[i].current_status_time++;
        
        // If process is currenlty blocked by IO
        // let's reduce one cycle from it.
        if (processes[i].io_time > 0) {
            processes[i].io_time--;
            
            if (processes[i].io_time == 0) {
                processes[i].state = kStateReady;
            }
        }
    }
    
    // Sort all values by running time on state.
    for (int j = 1; j < p_counter; j++) {
        for (int k = 0; k < p_counter - j; k++) {
            if (processes[k].current_status_time < processes[k + 1].current_status_time) {
                process temp = processes[k];
                processes[k] = processes[k + 1];
                processes[k + 1] = temp;
            }
        }
    }
    
    return 1;
}

int move_next_ready_process_to_running() {
    for (int i = 0; i < p_counter; i++) {
        processes[i].current_status_time++;
    }
    
    return 1;
}


#pragma mark - Processes

process *running_process() {
    for (int i = 0; i < p_counter; i++) {
        if (processes[i].state == kStateRunning) {
            return &processes[i];
        }
    }
    
    return NULL;
}

int processes_with_state(process (*buffer), int state) {
    int count = 0;
    
    for (int i = 0; i < p_counter; i++) {
        if (processes[i].state == state) {
            buffer[count] = processes[i];
            count++;
        }
    }
    
    return 1;
};


#pragma mark - Counters

int count_running_process() {
    for (int i = 0; i < p_counter; i++) {
        if (processes[i].state == kStateRunning) {
            return 1;
        }
    }
    
    return 0;
}

int count_processes_with_state(process *processes, int state) {
    int count = 0;
    
    for (int i = 0; i < p_counter; i++) {
        if (processes[i].state == state) {
            count++;
        }
    }
    
    return count;
}
