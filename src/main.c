#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPU1_RAM 512
#define CPU2_RAM 1536

// Define a struct to hold process information
typedef struct {
  int process_number;
  int arrival_time;
  int priority;
  int burst_time;
  int ram_required;
  int cpu_percentage;
} Process;

int num_processes = 0;
int num_processes0 = 0;
int num_processes1 = 0;
int num_processes2 = 0;
int num_processes3 = 0;
int num_fcfs = 0;
int num_sjf = 0;
int num_rr8 = 0;
int num_rr16 = 0;

Process processes[400];
Process *processes0[100];
Process *processes1[100];
Process *processes2[100];
Process *processes3[100];
Process *fcfsResult[100];
Process *sjfResult[100];
Process *rr8Result[100];
Process *rr16Result[100];

char *input_filename;
char *output_filename;

// Function prototypes
void read_processes();
void clear_output_file();
void process_fcfs(int *ram_source, int *cpu_source);
void process_sjf(int *ram_source, int *cpu_source);
void process_rr(int *ram_source, int *cpu_source, int q_time);
void output_to_file();
void print_queues();
void sort_according_to_arrival_time(Process *array[], int size);

int main(int argc, char *argv[]) {
    // Check arguments
  if (argc != 3) {
    printf("Usage: %s <input file> <output file>\n", argv[0]);
    return 1;
  }

  input_filename = argv[1];
  output_filename = argv[2];

  // Read process information from file
  read_processes();

  // empty the output file
  clear_output_file();

  // define the resources
  int cpu1_ram = CPU1_RAM;
  int cpu2_ram = CPU2_RAM;
  int cpu1 = 100;
  int cpu2 = 100;

  // Process the processes
  process_fcfs(&cpu1_ram, &cpu1);
  process_sjf(&cpu2_ram, &cpu2);
  process_rr(&cpu2_ram, &cpu2, 8);
  process_rr(&cpu2_ram, &cpu2, 16);

  // Print queues to the screen
  print_queues();

  return 0;
}

// Function to read process information from file
void read_processes() {
  FILE *file = fopen(input_filename, "r");
  if (file == NULL) {
    printf("File could not be opened: %s\n", input_filename);
    exit(1);
  }

  while (fscanf(file, "P%d,%d,%d,%d,%d,%d\n",
                &processes[num_processes].process_number,
                &processes[num_processes].arrival_time,
                &processes[num_processes].priority,
                &processes[num_processes].burst_time,
                &processes[num_processes].ram_required,
                &processes[num_processes].cpu_percentage) == 6) {
    if (processes[num_processes].priority == 0) {
      processes0[num_processes0] = &processes[num_processes];
      num_processes0++;
    } else if (processes[num_processes].priority == 1) {
      processes1[num_processes1] = &processes[num_processes];
      num_processes1++;
    } else if (processes[num_processes].priority == 2) {
      processes2[num_processes2] = &processes[num_processes];
      num_processes2++;
    } else if (processes[num_processes].priority == 3) {
      processes3[num_processes3] = &processes[num_processes];
      num_processes3++;
    }

    num_processes++;
                }

  fclose(file);
}

void clear_output_file() {
  FILE *file = fopen(output_filename, "w");
  fclose(file);
}

// Function to assign processes to CPU-1
void process_fcfs(int *ram_source, int *cpu_source) {
  // Sort the temporary processes array by arrival time
  sort_according_to_arrival_time(processes0, num_processes0);

  // Create a copy of the processes array to avoid modifying the original data
  num_fcfs = num_processes0;
  memcpy(fcfsResult, processes0, sizeof(Process *) * num_fcfs);

  // Simulate FCFS scheduling
  FILE *file = fopen(output_filename, "a");
  if (file == NULL) {
    printf("Output file could not be opened: %s\n", output_filename);
    exit(1);
  }
  for (int i = 0; i < num_fcfs; i++) {
    fprintf(file, "\n");
    Process p = *fcfsResult[i];
    fprintf(file, "Process P%d is queued to be assigned to CPU-1\n",
            p.process_number);
    fprintf(file, "Process P%d is assigned to CPU-1\n", p.process_number);
    fprintf(file, "Process P%d is completed and terminated\n",
            p.process_number);
  }
  fclose(file);
}

void process_sjf(int *ram_source, int *cpu_source) {
  // sort acordding to arrival time
  sort_according_to_arrival_time(processes1, num_processes1);

  // Create a copy of the processes array to avoid modifying the original data
  num_sjf = num_processes1;
  memcpy(sjfResult, processes1, sizeof(Process *) * num_sjf);

  // Implement SJF scheduling
  int current_time = 0;
  int completed = 0;

  FILE *file = fopen(output_filename, "a");
  if (file == NULL) {
    printf("Output file could not be opened: %s\n", output_filename);
    exit(1);
  }
  while (completed < num_sjf) {
    // Find the process with the shortest burst time among arrived processes
    int min_burst_index = -1;
    int min_burst_time = INT_MAX;

    for (int i = 0; i < num_sjf; i++) {
      if (sjfResult[i]->arrival_time <= current_time &&
          sjfResult[i]->burst_time < min_burst_time &&
          !(sjfResult[i]->burst_time == 0)) {
        min_burst_index = i;
        min_burst_time = sjfResult[i]->burst_time;
        fprintf(file, "\n");
        Process p = *sjfResult[i];
        fprintf(file,
                "Process P%d is placed in the sjfResult queue to be assigned "
                "to CPU-2.\n",
                p.process_number);
      }
    }

    // If no process is available, move to the next time unit
    if (min_burst_index == -1) {
      current_time++;
      continue;
    }

    // Process the process with the shortest burst time
    current_time += sjfResult[min_burst_index]->burst_time;
    sjfResult[min_burst_index]->burst_time = 0; // Mark as completed
    completed++;

    // You can add code here to track completion time, waiting time, etc. for
    // SJF analysis
    Process p = *sjfResult[min_burst_index];
    fprintf(file, "Process P%d is assigned to CPU-2.\n", p.process_number);
    fprintf(file, "The operation of process P%d is completed and terminated.\n",
            p.process_number);
  }
  fprintf(file, "\n");
  fclose(file);
}
