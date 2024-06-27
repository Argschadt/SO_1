#include <stdio.h>
#include <stdlib.h>
#define MAX_PROCESSES 1000

// A structure to represent a queue
struct Queue {
	int front, rear, size;
	unsigned capacity;
	int* array;
};

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
	struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
	queue->capacity = capacity;
	queue->front = queue->size = 0;

	// This is important, see the enqueue
	queue->rear = capacity - 1;
	queue->array = (int*)malloc(queue->capacity * sizeof(int));
	return queue;
}

// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue)
{
	return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
	return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
	if (isFull(queue))
		return;
	queue->rear = (queue->rear + 1)
				% queue->capacity;
	queue->array[queue->rear] = item;
	queue->size = queue->size + 1;
	//printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue)
{
	if (isEmpty(queue))
		return -2147483648;
	int item = queue->array[queue->front];
	queue->front = (queue->front + 1)
				% queue->capacity;
	queue->size = queue->size - 1;
	return item;
}

// Function to get front of queue
int front(struct Queue* queue)
{
	if (isEmpty(queue))
		return -2147483648;
	return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue* queue)
{
	if (isEmpty(queue))
		return -2147483648;
	return queue->array[queue->rear];
}

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int state; // 0: Ready, 1: Running, 2: Wait, 3: Swapped Out
} Process;

// Comparison function for sorting based on age 
int compareByPriority(const void* a, const void* b) 
{ 
    if (((Process*)a)->priority - ((Process*)b)->priority) {
        return ((Process*)a)->priority - ((Process*)b)->priority;
    } else {
        return ((Process*)a)->arrival_time - ((Process*)b)->arrival_time;
    }
    //return ((Process*)a)->priority - ((Process*)b)->priority; 
}

int main(int argc, char **argv) {
    Process processes[MAX_PROCESSES];

    // Initialize clock
    int clock = 0;
    
    int n = 0;
    while(scanf("%d:%d:%d:%d", &processes[n].pid, &processes[n].arrival_time, &processes[n].burst_time, &processes[n].priority) != EOF) {
        //printf("Process %d: %d %d %d \n", processes[n].pid, processes[n].arrival_time, processes[n].burst_time, processes[n].priority);
        processes[n].remaining_time = processes[n].burst_time;
        processes[n].state = 0; // Set all processes to Ready state initially
        n++;
    }
    clock++;

    int num_processes = n;

    qsort(processes, num_processes, sizeof(Process), compareByPriority); 


    for (int i = 0; i < num_processes; i++) {
        //printf("Process %d: %d %d %d \n", processes[i].pid, processes[i].arrival_time, processes[i].burst_time, processes[i].priority);
    }

    // Create 8 queues based on priority
    struct Queue* queues[7];
    for (int i = 0; i < 8; i++) {
        queues[i] = createQueue(MAX_PROCESSES);
    }

    // Put processes in the corresponding queue based on priority
    for (int i = num_processes-1; i >= 0; i--) {
        enqueue(queues[processes[i].priority], i);
    }

    // Print queues size
    /* for (int i = 0; i < 8; i++) {
        printf("Queue %d size: %d\n", i, queues[i]->size);
    } */

    // Main loop of the Supervisor
    while (1) {
        // Check if there are any processes in the system
        int has_processes = 0;
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].state != 3) {
                has_processes = 1;
                break;
            }
        }

        if (!has_processes) {
            return 0;
        }

        // Add new processes to the Ready state
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time == clock) {
                processes[i].state = 0; // Set process to Ready state
            }
        }

        // Choose the process to execute
        Process *current_process = NULL;
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].state == 1) {
                current_process = &processes[i];
                break;
            }
        }

        // If no process is currently running, select the next process to run
        if (current_process == NULL) {
            for (int i = 0; i < num_processes; i++) {
                if (processes[i].state == 0) {
                    current_process = &processes[i];
                    current_process->state = 1; // Set process to Running state
                    break;
                }
            }
        }

        // Process the current process
        if (current_process != NULL) {
            // Check if the current process has completed its burst
            if (current_process->remaining_time == 1) {
                current_process->state = 3; // Set process to Swapped Out state
            } else {
                // Check if the current process requests I/O
                int io_request = 0;
                if (current_process->remaining_time > 1) {
                    //io_request = IOReq();
                    io_request = 0;
                }

                if (io_request) {
                    current_process->state = 3; // Set process to Swapped Out state
                } else {
                    current_process->state = 0; // Set process to Ready state
                }

                current_process->remaining_time--;
            }
        }

        // Update statistics

        clock++;
    }

    // Print statistics

    return 0;
}