#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define ROWS 10
#define COLS 10

typedef struct {
    char name[MAX_NAME_LENGTH];
    char time[20];
    int seats[ROWS][COLS];
    float price;
    int hallNumber;
} Movie;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int age;
    char phone[11];
    int movieIndex;
    int row[10];
    int col[10];
    int ticketCount;
    char bookingDateTime[30];
} User;

void displayMenu();
void viewBookedTickets(Movie movies[], User *users, int userCount, int movieCount);
void showMovies(Movie movies[], int movieCount);
void chooseSeat(Movie movies[], User *user);
void purchaseTicket(Movie movies[], User **users, int *userCount, int *userCapacity, int movieCount);
void cancelSeat(Movie movies[], User *users, int *userCount);
void exitSystem();
void userDetails(User *user);
void adminSection(Movie movies[], int movieCount);
bool authenticateAdmin();
void saveBookingsToFile(User *users, int userCount);
void loadBookingsFromFile(User **users, int *userCount, int *userCapacity);
char getRowLetter(int row);
int getRowIndex(char letter);
void printSeats(Movie movies[], int movieIndex);
void maskPassword(char *password);
void generateReport(User *users, int userCount, int daysAgo);

Movie movies[] = {
    {"Bahubali 3", "9:00 AM", {{0}}, 200, 1},
    {"KGF Chapter 3", "12:00 PM", {{0}}, 150, 1},
    {"RRR 2", "3:00 PM", {{0}}, 180, 1},
    {"Spiderman", "9:00 AM", {{0}}, 250, 2},
    {"Batman", "12:00 PM", {{0}}, 250, 2},
    {"Moonknight", "3:00 PM", {{0}}, 250, 2}
};
int movieCount = sizeof(movies) / sizeof(movies[0]);

int main() {
    int choice;
    int userCapacity = 10;
    int userCount = 0;
    User *users = (User *)malloc(userCapacity * sizeof(User));

    loadBookingsFromFile(&users, &userCount, &userCapacity);

    do {
        displayMenu();
        printf("Enter your choice (1-8): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (authenticateAdmin()) {
                    adminSection(movies, movieCount);
                } else {
                    printf("Invalid credentials.\n");
                }
                break;
            case 2:
                viewBookedTickets(movies, users, userCount, movieCount);
                break;
            case 3:
                showMovies(movies, movieCount);
                break;
            case 4:
                printf("You must purchase a ticket first.\n");
                break;
            case 5:
                purchaseTicket(movies, &users, &userCount, &userCapacity, movieCount);
                break;
            case 6:
                cancelSeat(movies, users, &userCount);
                break;
            case 7:
                saveBookingsToFile(users, userCount);
                exitSystem();
                break;
            case 8:
                printf("Generate report for how many days ago? ");
                int daysAgo;
                scanf("%d", &daysAgo);
                generateReport(users, userCount, daysAgo);
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 8.\n");
        }
    } while (choice != 7);

    free(users);
    return 0;
}

void displayMenu() {
    printf("\n              Simple Movie Booking System\n");
    printf(" ==================================================================\n");
    printf("||             Menu:                                              ||\n");
    printf("||             1- Admin Section (admin)                           ||\n");
    printf("||             2- View booked tickets (admin)                     ||\n");
    printf("||             3- Show available movies                           ||\n");
    printf("||             4- Choose seat                                     ||\n");
    printf("||             5- Purchase ticket                                 ||\n");
    printf("||             6- Cancel the seat                                 ||\n");
    printf("||             7- Exit system                                     ||\n");
    printf("||             8- Generate report                                 ||\n");
    printf("||================================================================||\n");
    printf("||                    Wel-come                                    ||\n");
}

bool authenticateAdmin() {
    char username[20], password[20];
    printf("Enter admin username: ");
    scanf("%s", username);

    printf("Enter admin password: ");
    maskPassword(password);
    printf("\n");

    return strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0;
}

void maskPassword(char *password) {
    int i = 0;
    while (1) {
        password[i] = _getch();
        if (password[i] == '\r') {
            password[i] = '\0';
            break;
        } else if (password[i] == 8 && i > 0) {
            printf("\b \b");
            i--;
        } else {
            printf("*");
            i++;
        }
    }
}

void adminSection(Movie movies[], int movieCount) {
    int choice, movieIndex;
    char newName[MAX_NAME_LENGTH], newTime[20];
    float newPrice;
    bool logout = false;

    while (!logout) {
        printf(" ==================================================================\n");
        printf("||                     Admin Section                              ||\n");
        printf("|| 1. Change movie name                                           ||\n");
        printf("|| 2. Change movie time                                           ||\n");
        printf("|| 3. Change movie price                                          ||\n");
        printf("|| 4. Log out                                                     ||\n");
        printf("||================================================================||\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);

        if (choice == 4) {
            logout = true;
            continue;
        }

        showMovies(movies, movieCount);

        printf("Select Movie (1-%d): ", movieCount);
        scanf("%d", &movieIndex);
        movieIndex--;

        if (movieIndex < 0 || movieIndex >= movieCount) {
            printf("Invalid movie choice.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter new movie name: ");
                scanf(" %[^\n]s", newName);
                strcpy(movies[movieIndex].name, newName);
                printf("Movie name updated successfully.\n");
                break;
            case 2:
                printf("Enter new movie time: ");
                scanf("%s", newTime);
                strcpy(movies[movieIndex].time, newTime);
                printf("Movie time updated successfully.\n");
                break;
            case 3:
                printf("Enter new movie price: ");
                scanf("%f", &newPrice);
                movies[movieIndex].price = newPrice;
                printf("Movie price updated successfully.\n");
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
}

void viewBookedTickets(Movie movies[], User *users, int userCount, int movieCount) {
    int i, r, c, u, t, booked;
    for (i = 0; i < movieCount; i++) {
        printf("\nMovie: %s at %s (Hall %d)\n", movies[i].name, movies[i].time, movies[i].hallNumber);
        printf("Booked seats:\n");
        booked = 0;
        for (r = 0; r < ROWS; r++) {
            for (c = 0; c < COLS; c++) {
                if (movies[i].seats[r][c] == 1) {
                    for (u = 0; u < userCount; u++) {
                        for (t = 0; t < users[u].ticketCount; t++) {
                            if (users[u].movieIndex == i && users[u].row[t] == r && users[u].col[t] == c) {
                                printf("%c%d - %s (Age: %d, Phone: %s)\n", getRowLetter(r), c + 1, users[u].name, users[u].age, users[u].phone);
                                booked++;
                            }
                        }
                    }
                }
            }
        }
        if (booked == 0) {
            printf("None");
        }
        printf("\n");
    }
}

void showMovies(Movie movies[], int movieCount) {
    int i;
    printf("Available Movies:\n");
    for (i = 0; i < movieCount; i++) {
        printf("+---------------------------------------------+\n");
        printf("| %d. %-41s |\n", i + 1, movies[i].name);
        printf("| Time: %-36s |\n", movies[i].time);
        printf("| Hall: %-2d                               Price: Rs %.2f |\n", movies[i].hallNumber, movies[i].price);
        printf("+---------------------------------------------+\n");
    }
}

void chooseSeat(Movie movies[], User *user) {
    int row, col;
    printSeats(movies, user->movieIndex);
    for (int i = 0; i < user->ticketCount; i++) {
        printf("Enter row letter and column number for ticket %d: ", i + 1);
        char rowLetter;
        scanf(" %c%d", &rowLetter, &col);
        row = getRowIndex(rowLetter);
        col--;

        if (row < 0 || row >= ROWS || col < 0 || col >= COLS || movies[user->movieIndex].seats[row][col] == 1) {
            printf("Invalid or already booked seat. Please choose a different seat.\n");
            i--; // Retry this ticket
        } else {
            movies[user->movieIndex].seats[row][col] = 1;
            user->row[i] = row;
            user->col[i] = col;
        }
    }
}

void purchaseTicket(Movie movies[], User **users, int *userCount, int *userCapacity, int movieCount) {
    if (*userCount >= *userCapacity) {
        *userCapacity *= 2;
        *users = realloc(*users, *userCapacity * sizeof(User));
    }

    User *newUser = &(*users)[*userCount];
    userDetails(newUser);

    printf("Available movies:\n");
    showMovies(movies, movieCount);

    printf("Select a movie (1-%d): ", movieCount);
    int movieIndex;
    scanf("%d", &movieIndex);
    movieIndex--;

    if (movieIndex < 0 || movieIndex >= movieCount) {
        printf("Invalid movie choice.\n");
        return;
    }

    newUser->movieIndex = movieIndex;

    printf("Enter the number of tickets: ");
    scanf("%d", &newUser->ticketCount);

    if (newUser->ticketCount <= 0) {
        printf("Invalid number of tickets.\n");
        return;
    }

    chooseSeat(movies, newUser);

    // Save booking date and time
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    strftime(newUser->bookingDateTime, sizeof(newUser->bookingDateTime), "%Y-%m-%d %H:%M:%S", local);

    (*userCount)++;
    printf("Tickets purchased successfully.\n");
}

void cancelSeat(Movie movies[], User *users, int *userCount) {
    char phone[11];
    printf("Enter your phone number: ");
    scanf("%s", phone);

    for (int i = 0; i < *userCount; i++) {
        if (strcmp(users[i].phone, phone) == 0) {
            for (int t = 0; t < users[i].ticketCount; t++) {
                int row = users[i].row[t];
                int col = users[i].col[t];
                movies[users[i].movieIndex].seats[row][col] = 0;
            }

            printf("Booking for %s canceled successfully.\n", users[i].name);

            // Shift remaining users
            for (int j = i; j < *userCount - 1; j++) {
                users[j] = users[j + 1];
            }

            (*userCount)--;
            return;
        }
    }

    printf("Booking not found.\n");
}

void exitSystem() {
    printf("Thank you for using the Simple Movie Booking System. Goodbye!\n");
}

void userDetails(User *user) {
    printf("Enter your name: ");
    scanf(" %[^\n]s", user->name);
    printf("Enter your age: ");
    scanf("%d", &user->age);
    printf("Enter your phone number: ");
    scanf("%s", user->phone);
}

void saveBookingsToFile(User *users, int userCount) {
    FILE *file = fopen("bookings.txt", "w");

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "Name: %s, Age: %d, Phone: %s, Movie Index: %d, Tickets: %d, Date: %s\n",
                users[i].name, users[i].age, users[i].phone, users[i].movieIndex, users[i].ticketCount, users[i].bookingDateTime);
        for (int t = 0; t < users[i].ticketCount; t++) {
            fprintf(file, "%c%d ", getRowLetter(users[i].row[t]), users[i].col[t] + 1);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void loadBookingsFromFile(User **users, int *userCount, int *userCapacity) {
    FILE *file = fopen("bookings.txt", "r");

    if (file == NULL) {
        printf("No previous bookings found.\n");
        return;
    }

    while (!feof(file)) {
        if (*userCount >= *userCapacity) {
            *userCapacity *= 2;
            *users = realloc(*users, *userCapacity * sizeof(User));
        }

        User *user = &(*users)[*userCount];
        fscanf(file, "Name: %[^\n], Age: %d, Phone: %s, Movie Index: %d, Tickets: %d, Date: %[^\n]\n",
               user->name, &user->age, user->phone, &user->movieIndex, &user->ticketCount, user->bookingDateTime);

        for (int t = 0; t < user->ticketCount; t++) {
            char rowLetter;
            fscanf(file, "%c%d", &rowLetter, &user->col[t]);
            user->row[t] = getRowIndex(rowLetter);
            user->col[t]--;
            (*users)[*userCount].row[t] = getRowIndex(rowLetter);
        }
        fscanf(file, "\n");

        (*userCount)++;
    }

    fclose(file);
}

char getRowLetter(int row) {
    return 'A' + row;
}

int getRowIndex(char letter) {
    return letter - 'A';
}

void printSeats(Movie movies[], int movieIndex) {
    printf("Seats:\n");
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (movies[movieIndex].seats[r][c] == 1) {
                printf("[X] ");
            } else {
                printf("[%c%d] ", getRowLetter(r), c + 1);
            }
        }
        printf("\n");
    }
}

void generateReport(User *users, int userCount, int daysAgo) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    local->tm_mday -= daysAgo;
    mktime(local);

    char compareDate[30];
    strftime(compareDate, sizeof(compareDate), "%Y-%m-%d", local);

    printf("Report for bookings from %d days ago (%s):\n", daysAgo, compareDate);

    for (int i = 0; i < userCount; i++) {
        if (strncmp(users[i].bookingDateTime, compareDate, 10) == 0) {
            printf("Name: %s, Age: %d, Phone: %s, Movie Index: %d, Tickets: %d, Date: %s\n",
                   users[i].name, users[i].age, users[i].phone, users[i].movieIndex, users[i].ticketCount, users[i].bookingDateTime);
            for (int t = 0; t < users[i].ticketCount; t++) {
                printf("%c%d ", getRowLetter(users[i].row[t]), users[i].col[t] + 1);
            }
            printf("\n");
        }
    }
}
