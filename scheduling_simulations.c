// RoundRobbin.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_BURST 10
#define PRIORITY_LEVELS 5
#define MAX_ARRIVAL 8
#define MAX_PROCESSES 10
#define MAX_TIME 200
#define preemptive 1
#define nonpreemptive 0
/* Define a PCB (Process Control Block) */
struct process {
	int pid;
	int burst;
	int priority;
	int arrival;
};
typedef struct process Process;

/* Process initialization */
Process process_init(int, int, int, int);
void sort_readyQ(Process[], int);

/* Scheduling Algorithm Prototypes */
void fcfs(int[], Process[], int);
void sjf(int[], int p, Process[], int);
void hpf(int[], int p, Process[], int);
void rr(int[], Process[], int);

/* Display Gantt Chart of processes */
void gannt_chart(int[]);

int main() {
	int i;
	int totalburst = 0;
	int pid;
	int burst;
	int priority;
	int arrival;
	int pGannt[MAX_TIME];
	int n; // number of processes
		   /* Define the ready queue */
	Process readyQ[MAX_PROCESSES]; //Array is used to implement the ready queue

								   /* Initialize a ready queue */
	srand(time(0));
	n = (rand() % MAX_PROCESSES) + 1;
	pid = 1;
	while (pid <= n) {
		burst = (rand() % MAX_BURST) + 1;
		priority = (rand() % PRIORITY_LEVELS);
		arrival = (rand() % MAX_ARRIVAL);
		readyQ[pid - 1] = process_init(pid, burst, priority, arrival);
		totalburst += readyQ[pid - 1].burst;
		pid += 1;
	}
	sort_readyQ(readyQ, n); //sort by arrival time
							/* Display Processes */
	printf("Processes in the Ready Queue: \n");
	for (i = 0; i<n; i++) {
		printf("P[%d]: arrival time = %d, burst time = %d, priority = %d\n", readyQ[i].pid, readyQ[i].arrival, readyQ[i].burst, readyQ[i].priority);
	}
	printf("-----------------------------------------------------------\n");
	/*Start scheduling algorithms */
	printf("\nFirst Come First Served Scheduling Gannt Chart:\n");
	fcfs(pGannt, readyQ, n);
	gannt_chart(pGannt);
	printf("\nNon-Preemptive Shortest Job First Scheduling Gannt Chart:\n");
	sjf(pGannt, nonpreemptive, readyQ, n);
	gannt_chart(pGannt);
	printf("\nPreemptive Shortest Job First Scheduling Gannt Chart:\n");
	sjf(pGannt, preemptive, readyQ, n);
	gannt_chart(pGannt);
	printf("\nNon-Preemptive Priority Scheduling Gannt Chart:\n");
	hpf(pGannt, nonpreemptive, readyQ, n);
	gannt_chart(pGannt);
	printf("\nPreemptive Priority Scheduling Gannt Chart:\n");
	hpf(pGannt, preemptive, readyQ, n);
	gannt_chart(pGannt);
	printf("\nRound Robin Scheduling Gannt Chart:\n");
	rr(pGannt, readyQ, n);
	gannt_chart(pGannt);

	return 0;
}

Process process_init(int pid, int burst, int priority, int arrival) {

	Process proc;
	proc.pid = pid;
	proc.burst = burst;
	proc.priority = priority;
	proc.arrival = arrival;
	return proc;
}

void sort_readyQ(Process readyQ[], int n) {
	Process tempP;
	int i, j;
	for (i = 0; i<n - 1; i++) {
		for (j = i + 1; j<n; j++) {
			if (readyQ[i].arrival > readyQ[j].arrival) {
				tempP = readyQ[i];
				readyQ[i] = readyQ[j];
				readyQ[j] = tempP;
			}
		}
	}
}

/* Display Gannt chart */
void gannt_chart(int pGannt[]) {
	int i = 0;
	int j;
	int f = 0;
	int c = 0;
	int pNow;
	printf("-------------------------------------------------------------\n");
	if (pGannt[0] != -1) { pNow = pGannt[0]; }
	i = 0;
	while (pGannt[i] != -1) {
		if (pGannt[i] == pNow) {
			f = 0;
			c++;
			i++;
		}
		else {
			f = 1;
		}
		if (f == 1) {
			printf("|");
			for (j = 1; j <= ((2 * c - 2) / 2); j++) {
				printf(" ");
			}
			printf("P%d", pNow);
			for (j = 1; j <= ((2 * c - 2) / 2); j++) {
				printf(" ");
			}
			c = 0;
			pNow = pGannt[i];
		}
	}
	printf("|");
	for (j = 1; j <= ((2 * c - 2) / 2); j++) {
		printf(" ");
	}
	printf("P%d", pNow);
	for (j = 1; j <= ((2 * c - 2) / 2); j++) {
		printf(" ");
	}
	printf("|\n");
	printf("-------------------------------------------------------------\n");
}
/* Scheduling Algorithm Implementations */
void fcfs(int pGannt[], Process readyQ[], int n) {
	Process p[MAX_PROCESSES];
	int i;
	int j;
	int t;
	// clear time units (they will be loaded with pid of running processes)
	for (i = 0; i<MAX_TIME; i++) {
		pGannt[i] = -1;
	}
	//Copy ready queue in a temp queue
	for (i = 0; i<n; i++) {
		p[i] = readyQ[i];
	}
	i = 0;
	t = 0;
	while (i<n) {
		if (t<p[i].arrival) {
			pGannt[t] = 0;
			t++;
		}
		else {
			for (j = 0; j<p[i].burst; j++) {
				pGannt[t + j] = p[i].pid;
			}
			t = t + j;
			i += 1;
		}
	}
}

void sjf(int pGannt[], int preemption, Process readyQ[], int n) {
	Process p[MAX_PROCESSES];
	int i, j, t,fTime=0;
	// clear time units (they will be loaded with pid of running processes)
	for (i = 0; i<MAX_TIME; i++) {
		pGannt[i] = -1;
	}
	//Copy ready queue in a temp queue
	for (i = 0; i<n; i++) {
		p[i] = readyQ[i];
	}
	if (preemption == nonpreemptive) {
		//your code here		
		i = 0;
		t = 0;
		Process tp;
		for (j = 0; j<p[i].burst; j++) {
			pGannt[t + j] = p[i].pid;
		}

		while (i < n - 1)
		{
			if (t<p[i].arrival) {
				pGannt[t] = 0;
				t++;
			}
			else
			{
				for (int x = 1; x < n; x++) {
					if (p[x].arrival <= t) {
						for (int k = x + 1; k < n; k++) {
							if (p[k].burst < p[x].burst) {
								tp = p[x];
								p[x] = p[k];
								p[k] = tp;
							}
						}
					}
				}

				for (j = 0; j<p[i + 1].burst; j++) {
					pGannt[t + j] = p[i].pid;
				}
				t = t + j;
				i += 1;
			}
		}
		for (j = 0; j<p[n - 1].burst; j++) {
			pGannt[t + j] = p[n - 1].pid;
		}
	}
	if (preemption == preemptive) {
		//your code here
		Process p2[MAX_PROCESSES];	//new queue (p2) to sort processes by burst time
		Process pmp[MAX_PROCESSES];
		int a = 0, n2 = 0, t = 0, i = 0, j;

		for (; t<MAX_TIME && a != n; t++) {

			for (j = i; j<n && p[j].arrival <= t; j++) {
				pmp[n2++] = p[j];
				i++;
			}

			//sort
			Process temp;
			int x, k;
			for (x = 0; x<n2 - 1; x++) {
				for (k = x + 1; k<n2; k++) {
					if (pmp[x].burst > pmp[k].burst) {
						temp = pmp[x];
						pmp[x] = pmp[k];
						pmp[k] = temp;
					}
				}
			}


			if (a<n2) {
				pGannt[t] = pmp[a].pid;
				pmp[a].burst--;
			}
			else {
				pGannt[t] = 0;
			}

			if (pmp[a].burst == 0) {
				a++;
			}


		}
	}
}

void hpf(int pGannt[], int preemption, Process readyQ[], int n) {
	Process p[MAX_PROCESSES];
	int i, j;
	// clear time units (they will be loaded with pid of running processes)
	for (i = 0; i<MAX_TIME; i++) {
		pGannt[i] = -1;
	}
	//Copy ready queue in a temp queue
	for (i = 0; i<n; i++) {
		p[i] = readyQ[i];
	}
	if (preemption == nonpreemptive) {
		//your code here
		i = 0;
		int t = 0;
		Process tp;
		for (j = 0; j<p[i].priority; j++) {
			pGannt[t + j] = p[i].pid;
		}

		while (i < n - 1)
		{
			if (t<p[i].arrival) {
				pGannt[t] = 0;
				t++;
			}
			else
			{
				for (int x = 1; x < n; x++) {
					if (p[x].arrival <= t) {
						for (int k = x + 1; k < n; k++) {
							if (p[k].priority < p[x].priority) {
								tp = p[x];
								p[x] = p[k];
								p[k] = tp;
							}
						}
					}
				}

				for (j = 0; j<p[i + 1].priority; j++) {
					pGannt[t + j] = p[i].pid;
				}
				t = t + j;
				i += 1;
			}
		}
		for (j = 0; j<p[n - 1].priority; j++) {
			pGannt[t + j] = p[n - 1].pid;
		}
	}
	if (preemption == preemptive) {
		//your code here
		Process pmp[MAX_PROCESSES];
		int a = 0, n2 = 0, t = 0, i = 0, j;

		for (; t<MAX_TIME && a != n; t++) {

			for (j = i; j<n && p[j].arrival <= t; j++) {
				pmp[n2++] = p[j];
				i++;
			}

			//sort
			Process temp;
			int x, k;
			for (x = 0; x<n2 - 1; x++) {
				for (k = x + 1; k<n2; k++) {
					if (pmp[x].priority > pmp[k].priority) {
						temp = pmp[x];
						pmp[x] = pmp[k];
						pmp[k] = temp;
					}
				}
			}


			if (a<n2) {
				pGannt[t] = pmp[a].pid;
				pmp[a].burst--;
			}
			else {
				pGannt[t] = 0;
			}

			if (pmp[a].burst == 0) {
				a++;
			}


		}
	}
}


void rr(int pGannt[], Process readyQ[], int n) {
	Process p[MAX_PROCESSES];
	int i, q = 2, t = 0, fTime = 0, x = n;
	Process temp;
	// clear time units (they will be loaded with pid of running processes)
	for (i = 0; i<MAX_TIME; i++) {
		pGannt[i] = -1;
	}
	//Copy ready queue in a temp queue
	for (i = 0; i<n; i++) {
		p[i] = readyQ[i];
		fTime += p[i].burst;
	}
	// your code here
	while (t < fTime)
	{
		if (t<p[0].arrival) {
			pGannt[t] = 0;
			t++;
		}
		else {

			if (p[0].burst == 0) {
				x--;
				for (int j = 0; j < x; j++) {
					p[j] = p[j + 1];
				}
			}

			else if (p[0].burst >= q)
			{
				for (int k = 0; k < p[0].burst; k++) {
					pGannt[t + k] = p[0].pid;
				}
				t += q;
				int ind;
				for (ind = 1; ind < x; ind++)
				{
					if (t < p[ind].arrival)
					{
						break;
					}
				}
				temp = p[0];
				for (int f = 0; f < ind; f++)
				{
					p[f] = p[f + 1];
				}
				p[ind] = temp;
			}

			else if (p[0].burst == 1)
			{
				pGannt[t] = p[0].pid;
				t ++;
				int ind;
				for (ind = 1; ind < x; ind++)
				{
					if (t < p[ind].arrival)
					{
						break;
					}
				}
				temp = p[0];
				for (int f = 0; f < ind; f++)
				{
					p[f] = p[f + 1];
				}
				p[ind] = temp;
			}
		}
	}
}


/*
if (p[k].burst >= q)
{
for (int l = 0; l < q; l++) {
pGannt[t + l] = p[k].pid;
}
t += q;
p[k].burst -= q;
temp = p[k];
for (int j = 0; j < n - 1; j++)
{
p[j] = p[j + 1];
}
p[n - 1] = temp;
break;
}
*/

