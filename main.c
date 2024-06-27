#include <stdio.h>
#include <stdlib.h>
#define MAX_PROCESSES 1000
#define PROB_OF_IO_REQ 10
#define PROB_OF_IO_TERM 4

extern int osPRNG(); // Add an external declaration for the osPRNG function

int IOReq() {
    if (osPRNG() % PROB_OF_IO_REQ == 0)
        return 1;
    else
        return 0;
}

int IOTerm() {
    if (osPRNG() % PROB_OF_IO_TERM == 0)
        return 1;
    else
        return 0;
}

/*
	osPRNG() - A pseudo-random library call
*/

#define MAX_LEN 20

/* We will be implimenting a circular linked list
which performs the random function. Start at the
beginning and if you read the end you start back at
the beginning. */

typedef struct linkedList linkedList, *linkedListPtr;
struct linkedList {
	int value;
	linkedListPtr next;
};

/* This function inits the osPRNG library.
It takes the memory address of the "head"
variable. This is actually our current 
variable, since we are implimenting a circular
linked list there is no need for a head. */

int  testChar(char input);
int  getInt(char* buf, FILE* infile);
void init_osPRNG(linkedList** head);


/* Here is our current position in the circular
linked list. We make this global static as we
treat it as the inited variable for this library
function. The library needs to init itself if 
this is NULL. */

static linkedListPtr current = NULL;

int osPRNG(void) {

	int random = -1;

	if( current == NULL )
		init_osPRNG(&current);

	if(current != NULL) {

		random = current->value;
		current = current->next;
	}

	if( random == -1) {
		fprintf(stderr,"There was an error in the osPRNG() function.\n");
		fprintf(stderr,"It's possible you did not put any numbers in the file \"random.txt\".\n");
		exit(1);
	}

return random;

}

void init_osPRNG(linkedList** head) {

	int  err = 0;
	FILE* infile;
	char buf[MAX_LEN];
	linkedListPtr next = NULL, current = NULL;

	infile = fopen("random.txt", "r");

	if( infile == NULL ){
		err = 1;
		fprintf(stderr,"Unable to open the file \"random.txt\".\nPlease maker sure it is in your directory and run again.\n");
	}

	/*Get an int from the file stream while 
	they are there to be read. When the function
	returns a non-zero value then it has reached
	end-of file and whatever is in the buffer is
	invalid.                                    */
	if(err == 0)
		err = getInt(buf, infile);

	if(err == 0) {
		current = malloc(sizeof(linkedList));
		if(current == NULL)
			err = 1;
		else {
			if(*head == NULL)
				*head = current;

			sscanf(buf, "%d", &current->value);
		}		
	}
	
	while( err == 0 && getInt(buf, infile) == 0) {

		current->next = malloc(sizeof(linkedList));
		if(current->next == NULL)
			err = 1;
		else {
			current = current->next;
			sscanf(buf, "%d", &current->value);
		}
	}

	if(err == 0)
		current->next = *head;

}

int testChar(char input) {

int accepted;

	switch(input) {
	
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			accepted = 1;
			break;
		default:
			accepted = 0;
			break;
	}

	return accepted;			

}

int getInt(char* buf, FILE* infile) {

	int count, err = 0;
	char mychar;

	/* Skip all characters until the first number and the
	   continue until the first non-number. */
	mychar = fgetc(infile);

	/* Take out all the chars that are not numerical      */
	while( testChar(mychar) == 0 && err == 0) {
		mychar = fgetc(infile);
		err = feof(infile);
	}

	/* The variable mychar is now the start of our number 
	start placing that in our buffer until there no
	longer is a number to stuff.                       */
	count = 0;
	while( testChar(mychar) && feof(infile) == 0){

		buf[count++] = mychar;
		mychar = fgetc(infile);

	}

//	buf[count] = NULL;
	buf[count] = '\0';

	return err;
}

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
	queue->rear = (queue->rear + 1) % queue->capacity;
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
	queue->front = (queue->front + 1) % queue->capacity;
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

// Comparison function for sorting based on arrival_time
int compareByArrivalTime(const void* a, const void* b) 
{ 
    return ((Process*)a)->arrival_time - ((Process*)b)->arrival_time;
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

    qsort(processes, num_processes, sizeof(Process), compareByArrivalTime); 


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
                    io_request = IOReq();
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