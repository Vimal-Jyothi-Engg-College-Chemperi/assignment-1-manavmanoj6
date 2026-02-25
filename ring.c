#include <stdio.h>

#define MAX 100

int main() {
    int n;
    int id[MAX];
    int active[MAX];
    int participant[MAX] = {0};

    int initiator;
    int current, next;
    int msg;
    int coordinator;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter process IDs:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &id[i]);
        active[i] = 1;   // all processes active initially
    }

    printf("Enter index of initiator (0 to %d): ", n-1);
    scanf("%d", &initiator);

    if (!active[initiator]) {
        printf("Initiator is not active.\n");
        return 0;
    }

    printf("\n===== ELECTION STARTED BY PROCESS %d =====\n\n", id[initiator]);

    participant[initiator] = 1;
    msg = id[initiator];
    current = initiator;

    // -------------------------
    // Election Phase
    // -------------------------
    while (1) {

        next = (current + 1) % n;

        if (!active[next]) {
            current = next;
            continue;
        }

        printf("Process %d -> Process %d : ELECTION(%d)\n",
               id[current], id[next], msg);

        // Move message to next
        current = next;

        // If process receives its own ID → coordinator
        if (msg == id[current]) {
            coordinator = id[current];
            printf("\nProcess %d receives its own ID back.\n", coordinator);
            printf("***** PROCESS %d BECOMES COORDINATOR *****\n\n", coordinator);
            break;
        }

        // If received ID greater → forward unchanged
        if (msg > id[current]) {
            participant[current] = 1;
            // msg unchanged
        }
        // If received ID smaller and not participant → replace
        else if (msg < id[current] && participant[current] == 0) {
            msg = id[current];
            participant[current] = 1;
        }
        // If already participant and smaller → discard (just forward current msg)
    }

    // -------------------------
    // Coordinator Announcement
    // -------------------------
    printf("===== COORDINATOR ANNOUNCEMENT =====\n\n");

    msg = coordinator;
    int start = current;

    while (1) {
        next = (current + 1) % n;

        if (!active[next]) {
            current = next;
            continue;
        }

        printf("Process %d -> Process %d : ELECTED(%d)\n",
               id[current], id[next], msg);

        current = next;

        if (current == start)
            break;
    }

    printf("\n===== ELECTION COMPLETED =====\n");

    return 0;
}