#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 20

int gameover, score;
int x, y, fruitX, fruitY, flag;
int tailX[100], tailY[100];
int nTail;

void setup() {
    gameover = 0;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    srand(time(0));
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
}

void draw() {
    int i, j;
    system("cls");
    for (i = 0; i < WIDTH; i++) printf("#");
    printf("\n");

    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (j == 0 || j == WIDTH - 1) printf("#");
            else if (i == y && j == x) printf("O");
            else if (i == fruitY && j == fruitX) printf("F");
            else {
                int print = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o");
                        print = 1;
                    }
                }
                if (!print) printf(" ");
            }
        }
        printf("\n");
    }

    for (i = 0; i < WIDTH; i++) printf("#");
    printf("\n");
    printf("Score: %d\n", score);
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w': flag = 1; break;
            case 's': flag = 2; break;
            case 'a': flag = 3; break;
            case 'd': flag = 4; break;
            case 'x': gameover = 1; break;
        }
    }
}

void logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (flag) {
        case 1: y--; break;
        case 2: y++; break;
        case 3: x--; break;
        case 4: x++; break;
        default: break;
    }
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) gameover = 1;
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) gameover = 1;
    }
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }
}

int main() {
    setup();
    while (!gameover) {
        draw();
        input();
        logic();
        // Speed of the snake
        for (int i = 0; i < 10000000; i++);
    }
    return 0;
}
suduko:
#include <stdio.h>

#define UNASSIGNED 0
#define N 9

int isSafe(int grid[N][N], int row, int col, int num);

int findUnassignedLocation(int grid[N][N], int *row, int *col) {
    for (*row = 0; *row < N; (*row)++)
        for (*col = 0; *col < N; (*col)++)
            if (grid[*row][*col] == UNASSIGNED)
                return 1;
    return 0;
}

int isUsedInRow(int grid[N][N], int row, int num) {
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return 1;
    return 0;
}

int isUsedInCol(int grid[N][N], int col, int num) {
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return 1;
    return 0;
}

int isUsedInBox(int grid[N][N], int boxStartRow, int boxStartCol, int num) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row + boxStartRow][col + boxStartCol] == num)
                return 1;
    return 0;
}

int isSafe(int grid[N][N], int row, int col, int num) {
    return !isUsedInRow(grid, row, num) &&
           !isUsedInCol(grid, col, num) &&
           !isUsedInBox(grid, row - row % 3, col - col % 3, num) &&
           grid[row][col] == UNASSIGNED;
}

int solveSudoku(int grid[N][N]) {
    int row, col;
    if (!findUnassignedLocation(grid, &row, &col))
        return 1;
    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            grid[row][col] = num;
            if (solveSudoku(grid))
                return 1;
            grid[row][col] = UNASSIGNED;
        }
    }
    return 0;
}

void printGrid(int grid[N][N]) {
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++)
            printf("%2d", grid[row][col]);
        printf("\n");
    }
}

int main() {
    int grid[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    if (solveSudoku(grid))
        printGrid(grid);
    else
        printf("No solution exists");
    return 0;
}
Travel planner:
#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct {
    char name[50];
    char destination[50];
    int days;
    double budget;
} Trip;

void addTrip(Trip trips[], int *count) {
    if (*count >= MAX) {
        printf("Max trips reached.\n");
        return;
    }
    printf("Enter name: ");
    scanf("%s", trips[*count].name);
    printf("Enter destination: ");
    scanf("%s", trips[*count].destination);
    printf("Enter days: ");
    scanf("%d", &trips[*count].days);
    printf("Enter budget: ");
    scanf("%lf", &trips[*count].budget);
    (*count)++;
}

void viewTrips(Trip trips[], int count) {
    for (int i = 0; i < count; i++) {
        printf("Trip %d:\n", i + 1);
        printf("Name: %s\n", trips[i].name);
        printf("Destination: %s\n", trips[i].destination);
        printf("Days: %d\n", trips[i].days);
        printf("Budget: %.2lf\n", trips[i].budget);
    }
}

void searchTrip(Trip trips[], int count) {
    char destination[50];
    printf("Enter destination to search: ");
    scanf("%s", destination);
    for (int i = 0; i < count; i++) {
        if (strcmp(trips[i].destination, destination) == 0) {
            printf("Trip found:\n");
            printf("Name: %s\n", trips[i].name);
            printf("Destination: %s\n", trips[i].destination);
#include <stdio.h>
#include <limits.h>

#define N 3

// Function to get the index of the person who has the maximum credit
int getMax(int arr[]) {
    int maxInd = 0;
    for (int i = 1; i < N; i++)
        if (arr[i] > arr[maxInd])
            maxInd = i;
    return maxInd;
}

// Function to get the index of the person who has the maximum debit
int getMin(int arr[]) {
    int minInd = 0;
    for (int i = 1; i < N; i++)
        if (arr[i] < arr[minInd])
            minInd = i;
    return minInd;
}

// Function to find the minimum of two values
int min(int x, int y) {
    return (x < y) ? x : y;
}

// Function to minimize the cash flow among a group of people
void minCashFlowRec(int amount[]) {
    int maxCredit = getMax(amount);
    int maxDebit = getMin(amount);

    // If both amounts are zero, all debts are settled
    if (amount[maxCredit] == 0 && amount[maxDebit] == 0)
        return;

    // Find the minimum of the two amounts
    int minAmount = min(-amount[maxDebit], amount[maxCredit]);

    // Update the amounts
    amount[maxCredit] -= minAmount;
    amount[maxDebit] += minAmount;

    // Print the transaction
    printf("Person %d pays %d to Person %d\n", maxDebit, minAmount, maxCredit);

    // Recursively settle the amounts
    minCashFlowRec(amount);
}

// Function to calculate the net amount to be paid by each person
void minCashFlow(int graph[N][N]) {
    int amount[N] = {0};

    // Calculate the net amount to be paid by each person
    for (int p = 0; p < N; p++)
        for (int i = 0; i < N; i++)
            amount[p] += (graph[i][p] - graph[p][i]);

    // Minimize the cash flow
    minCashFlowRec(amount);
}

// Main function
int main() {
    int graph[N][N] = { {0, 1000, 2000},
                        {0, 0, 5000},
                        {0, 0, 0}, };

    minCashFlow(graph);
    return 0;
}
text editor:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

char buffer[MAX]; // Main text buffer
char clipboard[MAX]; // Clipboard for cut/copy/paste

void display() {
    printf("Text:\n%s\n", buffer);
}

void insertText() {
    char text[MAX];
    printf("Enter text to insert: ");
    getchar(); // Clear the newline character from input buffer
    fgets(text, MAX, stdin);
    strcat(buffer, text);
}

void cutText() {
    int start, end;
    printf("Enter start and end positions to cut: ");
    scanf("%d %d", &start, &end);
    if (start < 0 || end >= strlen(buffer) || start > end) {
        printf("Invalid positions.\n");
        return;
    }
    strncpy(clipboard, buffer + start, end - start + 1);
    clipboard[end - start + 1] = '\0';
    memmove(buffer + start, buffer + end + 1, strlen(buffer) - end);
}

void copyText() {
    int start, end;
    printf("Enter start and end positions to copy: ");
    scanf("%d %d", &start, &end);
    if (start < 0 || end >= strlen(buffer) || start > end) {
        printf("Invalid positions.\n");
        return;
    }
    strncpy(clipboard, buffer + start, end - start + 1);
    clipboard[end - start + 1] = '\0';
}

void pasteText() {
    int position;
    printf("Enter position to paste: ");
    scanf("%d", &position);
    if (position < 0 || position > strlen(buffer)) {
        printf("Invalid position.\n");
        return;
    }
    char temp[MAX];
    strcpy(temp, buffer + position);
    buffer[position] = '\0';
    strcat(buffer, clipboard);
    strcat(buffer, temp);
}

int main() {
    int choice;
    buffer[0] = '\0';
    clipboard[0] = '\0';

    while (1) {
        printf("Text Editor:\n");
        printf("1. Insert Text\n");
        printf("2. Cut Text\n");
        printf("3. Copy Text\n");
        printf("4. Paste Text\n");
        printf("5. Display Text\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                insertText();
                break;
            case 2:
                cutText();
                break;
            case 3:
                copyText();
                break;
            case 4:
                pasteText();
                break;
            case 5:
                display();
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
