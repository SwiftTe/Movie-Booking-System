#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <time.h>

#define MAX_MOVIES 6
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
void viewBookedTickets(Movie movies[], User users[], int userCount, int movieCount);
void showMovies(Movie movies[], int movieCount);
void chooseSeat(Movie movies[], User *user);
void purchaseTicket(Movie movies[], User users[], int *userCount, int movieCount);
void cancelSeat(Movie movies[], User users[], int *userCount);
void exitSystem();
void userDetails(User *user);
void adminSection(Movie movies[], int movieCount);
bool authenticateAdmin();
void saveBookingsToFile(User users[], int userCount);
void loadBookingsFromFile(User users[], int *userCount);
char getRowLetter(int row);
int getRowIndex(char letter);
void printSeats(Movie movies[], int movieIndex);
void maskPassword(char *password);

Movie movies[MAX_MOVIES] = {
    {"Bahubali 3", "9:00 AM", {{0}}, 200, 1},
    {"KGF Chapter 3", "12:00 PM", {{0}}, 150, 1},
    {"RRR 2", "3:00 PM", {{0}}, 180, 1},
    {"Spiderman", "9:00 AM", {{0}}, 250, 2},
    {"Batman", "12:00 PM", {{0}}, 250, 2},
    {"Moonknight", "3:00 PM", {{0}}, 250, 2}
};

int main() {
    int choice;
    User users[100];
    int userCount = 0;

    loadBookingsFromFile(users, &userCount);

    do {
        displayMenu();
        printf("Enter your choice (1-7): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (authenticateAdmin()) {
                    adminSection(movies, MAX_MOVIES);
                } else {
                    printf("Invalid credentials.\n");
                }
                break;
            case 2:
                viewBookedTickets(movies, users, userCount, MAX_MOVIES);
                break;
            case 3:
                showMovies(movies, MAX_MOVIES);
                break;
            case 4:
                printf("You must purchase a ticket first.\n");
                break;
            case 5:
                purchaseTicket(movies, users, &userCount, MAX_MOVIES);
                break;
            case 6:
                cancelSeat(movies, users, &userCount);
                break;
            case 7:
                saveBookingsToFile(users, userCount);
                exitSystem();
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 7.\n");
        }
    } while (choice != 7);

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
    printf("||================================================================||\n");
}

bool authenticateAdmin() {
    char username[20], password[20];
    printf("Enter admin username: ");
    scanf("%s", username);

    printf("Enter admin password: ");
    maskPassword(password);
    printf("\n");

    if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
        return true;
    } else {
        return false;
    }
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

void viewBookedTickets(Movie movies[], User users[], int userCount, int movieCount) {
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
        printf("| Hall: %-2d                               Price: $%.2f |\n", movies[i].hallNumber, movies[i].price);
        printf("+---------------------------------------------+\n");
    }
}

void chooseSeat(Movie movies[], User *user) {
    int movieIndex = user->movieIndex;
    int r, c, i;
    char rowLetter;
    int rowIndex;

    printSeats(movies, movieIndex);

    printf("Enter the number of tickets: ");
    scanf("%d", &user->ticketCount);

    for (i = 0; i < user->ticketCount; i++) {
        printf("Enter seat %d (row letter and column number): ", i + 1);
        scanf(" %c%d", &rowLetter, &c);
        rowIndex = getRowIndex(rowLetter);
        if (rowIndex == -1 || c < 1 || c > COLS || movies[movieIndex].seats[rowIndex][c - 1] == 1) {
            printf("Invalid seat or already booked. Please try again.\n");
            i--;
        } else {
            user->row[i] = rowIndex;
            user->col[i] = c - 1;
            movies[movieIndex].seats[rowIndex][c - 1] = 1;
        }
    }
}

void purchaseTicket(Movie movies[], User users[], int *userCount, int movieCount) {
    int movieIndex, i;
    User user;
    char confirmation;

    showMovies(movies, movieCount);

    printf("Select a movie (1-%d): ", movieCount);
    scanf("%d", &movieIndex);
    movieIndex--;

    if (movieIndex < 0 || movieIndex >= movieCount) {
        printf("Invalid movie selection.\n");
        return;
    }

    userDetails(&user);

    user.movieIndex = movieIndex;

    chooseSeat(movies, &user);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(user.bookingDateTime, sizeof(user.bookingDateTime), "%02d-%02d-%04d %02d:%02d:%02d", 
             tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    printf("\nPurchase Summary:\n");
    printf("Name: %s\n", user.name);
    printf("Age: %d\n", user.age);
    printf("Phone: %s\n", user.phone);
    printf("Movie: %s\n", movies[movieIndex].name);
    printf("Time: %s\n", movies[movieIndex].time);
    printf("Hall: %d\n", movies[movieIndex].hallNumber);
    printf("Seats: ");
    for (i = 0; i < user.ticketCount; i++) {
        printf("%c%d ", getRowLetter(user.row[i]), user.col[i] + 1);
    }
    printf("\nTotal Price: $%.2f\n", user.ticketCount * movies[movieIndex].price);
    printf("Booking Date and Time: %s\n", user.bookingDateTime);
    printf("Confirm purchase (y/n): ");
    scanf(" %c", &confirmation);

    if (confirmation == 'y' || confirmation == 'Y') {
        users[*userCount] = user;
        (*userCount)++;
        printf("Purchase successful! Enjoy your movie.\n");
    } else {
        printf("Purchase canceled.\n");
    }
}

void cancelSeat(Movie movies[], User users[], int *userCount) {
    char phone[11];
    int movieIndex, row, col, i, j, k;
    bool found = false;

    printf("Enter your phone number to cancel seats: ");
    scanf("%s", phone);

    for (i = 0; i < *userCount; i++) {
        if (strcmp(users[i].phone, phone) == 0) {
            movieIndex = users[i].movieIndex;
            printf("Seats booked for %s (Movie: %s):\n", users[i].name, movies[movieIndex].name);
            for (j = 0; j < users[i].ticketCount; j++) {
                printf("%c%d ", getRowLetter(users[i].row[j]), users[i].col[j] + 1);
            }
            printf("\n");
            for (j = 0; j < users[i].ticketCount; j++) {
                row = users[i].row[j];
                col = users[i].col[j];
                movies[movieIndex].seats[row][col] = 0;
            }
            for (k = i; k < (*userCount) - 1; k++) {
                users[k] = users[k + 1];
            }
            (*userCount)--;
            found = true;
            printf("Your booking has been canceled successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("No booking found for the provided phone number.\n");
    }
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

void saveBookingsToFile(User users[], int userCount) {
    FILE *file = fopen("bookings.txt", "w");
    int i, j;

    if (file == NULL) {
        printf("Unable to open file for saving bookings.\n");
        return;
    }

    for (i = 0; i < userCount; i++) {
        fprintf(file, "%s %d %s %d %d", users[i].name, users[i].age, users[i].phone, users[i].movieIndex, users[i].ticketCount);
        for (j = 0; j < users[i].ticketCount; j++) {
            fprintf(file, " %d %d", users[i].row[j], users[i].col[j]);
        }
        fprintf(file, " %s\n", users[i].bookingDateTime);
    }

    fclose(file);
}

void loadBookingsFromFile(User users[], int *userCount) {
    FILE *file = fopen("bookings.txt", "r");
    int i, j;

    if (file == NULL) {
        printf("No previous bookings found.\n");
        return;
    }

    while (fscanf(file, "%s %d %s %d %d", users[*userCount].name, &users[*userCount].age, users[*userCount].phone, 
                  &users[*userCount].movieIndex, &users[*userCount].ticketCount) != EOF) {
        for (j = 0; j < users[*userCount].ticketCount; j++) {
            fscanf(file, "%d %d", &users[*userCount].row[j], &users[*userCount].col[j]);
        }
        fscanf(file, " %[^\n]s", users[*userCount].bookingDateTime);
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
    int r, c;
    printf("Available seats (0: available, 1: booked):\n");
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) {
            if (c == 5) printf(" ");
            if (movies[movieIndex].seats[r][c] == 0) {
                printf("%c%d ", getRowLetter(r), c + 1);
            } else {
                printf("XX ");
            }
        }
        printf("\n");
    }
}