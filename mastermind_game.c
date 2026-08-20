#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CODE_LENGTH 4
#define MAX_ATTEMPTS 10

typedef struct {
    int guess[CODE_LENGTH];
    int bulls;
    int cows;
} Attempt;

typedef struct {
    Attempt items[MAX_ATTEMPTS];
    int top;
} Stack;

void initStack(Stack *s) { s->top = -1; }
int isFull(Stack *s) { return s->top == MAX_ATTEMPTS - 1; }
int isEmpty(Stack *s) { return s->top == -1; }

void push(Stack *s, Attempt att) {
    if (!isFull(s)) {
        s->top++;
        s->items[s->top] = att;
    }
}

void displayHistory(Stack *s) {
    if (isEmpty(s)) {
        printf("\nNo history available yet.\n");
        return;
    }
    printf("\n=== Attempt History (Stack View) ===\n");
    for (int i = 0; i <= s->top; i++) {
        printf("Attempt %d: [ ", i + 1);
        for (int j = 0; j < CODE_LENGTH; j++) {
            printf("%d ", s->items[i].guess[j]);
        }
        printf("] -> Bulls: %d, Cows: %d\n", s->items[i].bulls, s->items[i].cows);
    }
    printf("===================================\n");
}

void generateSecretCode(int secretCode[]) {
    int count = 0;
    while (count < CODE_LENGTH) {
        int num = rand() % 10;
        int exists = 0;
        for (int i = 0; i < count; i++) {
            if (secretCode[i] == num) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            secretCode[count] = num;
            count++;
        }
    }
}

void evaluateGuess(int secretCode[], int guess[], int *bulls, int *cows) {
    *bulls = 0;
    *cows = 0;
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] == secretCode[i]) {
            (*bulls)++;
        } else {
            for (int j = 0; j < CODE_LENGTH; j++) {
                if (i != j && guess[i] == secretCode[j]) {
                    (*cows)++;
                    break;
                }
            }
        }
    }
}

int main() {
    srand(time(0));

    int secretCode[CODE_LENGTH];
    Stack historyStack;
    initStack(&historyStack);
    generateSecretCode(secretCode);

    printf("===========================================\n");
    printf("        WELCOME TO MASTERMIND GAME         \n");
    printf("===========================================\n");
    printf("Rules:\n");
    printf("1. Guess the secret 4-digit unique code.\n");
    printf("2. Digits are between 0 and 9.\n");
    printf("3. Bulls = Correct digit in correct position.\n");
    printf("4. Cows  = Correct digit in wrong position.\n");
    printf("5. Maximum attempts allowed: %d\n", MAX_ATTEMPTS);
    printf("===========================================\n\n");

    int attemptCount = 0;
    int won = 0;

    while (attemptCount < MAX_ATTEMPTS) {
        printf("\nAttempt %d/%d\n", attemptCount + 1, MAX_ATTEMPTS);
        printf("Enter 4 digits space-separated (or enter -1 -1 -1 -1 to view history): ");

        int guess[CODE_LENGTH];
        int inputValid = 1;

        for (int i = 0; i < CODE_LENGTH; i++) {
            if (scanf("%d", &guess[i]) != 1) {
                inputValid = 0;
            }
        }

        if (guess[0] == -1) {
            displayHistory(&historyStack);
            continue;
        }

        if (!inputValid) {
            printf("Invalid input. Please enter numbers only.\n");
            while (getchar() != '\n');
            continue;
        }

        int bulls, cows;
        evaluateGuess(secretCode, guess, &bulls, &cows);

        Attempt currentAttempt;
        for (int i = 0; i < CODE_LENGTH; i++) {
            currentAttempt.guess[i] = guess[i];
        }
        currentAttempt.bulls = bulls;
        currentAttempt.cows = cows;
        push(&historyStack, currentAttempt);

        attemptCount++;

        printf("Result: %d Bulls, %d Cows\n", bulls, cows);

        if (bulls == CODE_LENGTH) {
            won = 1;
            break;
        }
    }

    printf("\n===========================================\n");
    if (won) {
        printf("CONGRATULATIONS! You solved the code in %d attempts.\n", attemptCount);
    } else {
        printf("GAME OVER! You ran out of attempts.\n");
        printf("The secret code was: ");
        for (int i = 0; i < CODE_LENGTH; i++) {
            printf("%d ", secretCode[i]);
        }
        printf("\n");
    }
    printf("===========================================\n");

    return 0;
}
