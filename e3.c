// Universidad del Valle de Guatemala
// Sistemas Operativos
// Lab4 - Ejercicio 3
// Andres Quezada 21085

#include <stdio.h>
#include <stdbool.h>

#define NUM_RESOURCES 3
#define NUM_PROCESSES 2

int available[NUM_RESOURCES];
int maximum[NUM_PROCESSES][NUM_RESOURCES];
int allocation[NUM_PROCESSES][NUM_RESOURCES];
int need[NUM_PROCESSES][NUM_RESOURCES];

void display_state() {
    printf("Available resources: ");
    for (int i = 0; i < NUM_RESOURCES; i++) {
        printf("%d ", available[i]);
    }
    printf("\n");

    printf("Allocation matrix:\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }

    printf("Need matrix:\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
}

void init_resources(int resources[]) {
    printf("Enter the available resources:\n");
    for (int i = 0; i < NUM_RESOURCES; i++) {
        scanf("%d", &resources[i]);
    }
}

void init_max() {
    printf("Enter the maximum resources required for each process:\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            scanf("%d", &maximum[i][j]);
        }
    }
}

void init_allocation() {
    printf("Enter the current allocation of resources for each process:\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            scanf("%d", &allocation[i][j]);
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

bool check_safety(int work[]) {
    bool finish[NUM_PROCESSES] = {false};

    int count = 0;
    while (count < NUM_PROCESSES) {
        bool found = false;
        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (!finish[i]) {
                bool needs_met = true;
                for (int j = 0; j < NUM_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        needs_met = false;
                        break;
                    }
                }
                if (needs_met) {
                    for (int j = 0; j < NUM_RESOURCES; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found) {
            break; // State is unsafe
        }
    }

    if (count == NUM_PROCESSES) {
        return true; // State is safe
    } else {
        return false; // State is unsafe
    }
}

bool request_resources(int process_id, int request[]) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (request[i] > need[process_id][i] || request[i] > available[i]) {
            return false;
        }
    }

    // Check if the system is in a safe state after allocating resources
    int work[NUM_RESOURCES];
    for (int i = 0; i < NUM_RESOURCES; i++) {
        work[i] = available[i]; // Initialize work matrix with available resources
    }

    // Attempt to allocate resources
    for (int i = 0; i < NUM_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[process_id][i] += request[i];
        need[process_id][i] -= request[i];
    }

    // Check if the state is safe after allocation
    if (!check_safety(work)) {
        // If the state is not safe, rollback the allocation
        for (int i = 0; i < NUM_RESOURCES; i++) {
            available[i] += request[i];
            allocation[process_id][i] -= request[i];
            need[process_id][i] += request[i];
        }
        return false;
    }

    // If the state is safe, return true to indicate successful allocation
    return true;
}


void release_resources(int process_id, int release[]) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        available[i] += release[i];
        allocation[process_id][i] -= release[i];
        need[process_id][i] += release[i];
    }
}

int main() {
    init_resources(available);
    init_max();
    init_allocation();

    // Simulate resource requests and releases
    int process_id;
    int request[NUM_RESOURCES];
    int release[NUM_RESOURCES];

    while (true) {
        printf("\nCurrent state:\n");
        display_state();

        printf("\nEnter process ID (0-%d) or -1 to exit: ", NUM_PROCESSES - 1);
        scanf("%d", &process_id);
        if (process_id == -1) {
            break;
        }
        printf("Enter request for process %d: ", process_id);
        for (int i = 0; i < NUM_RESOURCES; i++) {
            scanf("%d", &request[i]);
        }
        if (request_resources(process_id, request)) {
            printf("Request granted!\n");
        } else {
            printf("Request denied!\n");
        }

        printf("Enter release for process %d: ", process_id);
        for (int i = 0; i < NUM_RESOURCES; i++) {
            scanf("%d", &release[i]);
        }
        release_resources(process_id, release);
    }

    return 0;
}
