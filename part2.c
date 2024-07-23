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
void purchaseTicket(Movie movies[], User **users, int *userCount, int *userCapacity, int *movieCount);
void cancelSeat(Movie movies[], User *users, int *userCount);
void exitSystem();
void userDetails(User *user);
void adminSection(Movie movies[], int *movieCount);
bool authenticateAdmin();
void saveBookingsToFile(User *users, int userCount);
void loadBookingsFromFile(User **users, int *userCount, int *userCapacity);
char getRowLetter(int row);
int getRowIndex(char letter);
void printSeats(Movie movies[], int movieIndex);
void maskPassword(char *password);
void generateReport(User *users, int userCount);

Movie movies[100] = {
    {"Bahubali 3", "9:00 AM", {{0}}, 200, 1},
    {"KGF Chapter 3", "12:00 PM", {{0}}, 150, 1},
    {"RRR 2", "3:00 PM", {{0}}, 180, 1},
    {"Spiderman", "9:00 AM", {{0}}, 250, 2},
    {"Batman", "12:00 PM", {{0}}, 250, 2},
    {"Moonknight", "3:00 PM", {{0}}, 250, 2}
};
int movieCount = 6;

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
                    adminSection(movies, &movieCount);
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
                purchaseTicket(movies, &users, &userCount, &userCapacity, &movieCount);
                break;
            case 6:
                cancelSeat(movies, users, &userCount);
                break;
            case 7:
                saveBookingsToFile(users, userCount);
                exitSystem();
                break;
            case 8:
                generateReport(users, userCount);
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

void adminSection(Movie movies[], int *movieCount) {
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
        printf("|| 4. Add new movie                                               ||\n");
        printf("|| 5. Log out                                                     ||\n");
        printf("||================================================================||\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);

        if (choice == 5) {
            logout = true;
            continue;
        }

        if (choice != 4) {
            showMovies(movies, *movieCount);
            printf("Select Movie (1-%d): ", *movieCount);
            scanf("%d", &movieIndex);
            movieIndex--;

            if (movieIndex < 0 || movieIndex >= *movieCount) {
                printf("Invalid movie choice.\n");
                continue;
            }
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
            case 4:
                printf("Enter new movie name: ");
                scanf(" %[^\n]s", newName);
                printf("Enter new movie time: ");
                scanf("%s", newTime);
                printf("Enter new movie price: ");
                scanf("%f", &newPrice);
                strcpy(movies[*movieCount].name, newName);
                strcpy(movies[*movieCount].time, newTime);
                movies[*movieCount].price = newPrice;
                movies[*movieCount].hallNumber = 3; // Set hall number for new movies
                memset(movies[*movieCount].seats, 0, sizeof(movies[*movieCount].seats));
                (*movieCount)++;
                printf("New movie added successfully.\n");
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
                                printf("Seat %c%d booked by %s\n", getRowLetter(r), c + 1, users[u].name);
                                booked++;
                            }
                        }
                    }
                }
            }
        }
        if (booked == 0) {
            printf("No bookings found for this movie.\n");
        }
    }
}

void showMovies(Movie movies[], int movieCount) {
    int i;
    for (i = 0; i < movieCount; i++) {
        printf("+---------------------------------------------+\n");
        printf("| %d. %-41s |\n", i + 1, movies[i].name);
        printf("| Time: %-36s |\n", movies[i].time);
        printf("| Hall: %-2d                               Price: Rs %.2f |\n", movies[i].hallNumber, movies[i].price);
        printf("+---------------------------------------------+\n");
    }
}

void chooseSeat(Movie movies[], User *user) {
    int movieIndex, row, col, i;
    showMovies(movies, movieCount);
    printf("Select Movie (1-%d): ", movieCount);
    scanf("%d", &movieIndex);
    movieIndex--;

    if (movieIndex < 0 || movieIndex >= movieCount) {
        printf("Invalid movie choice.\n");
        return;
    }

    printSeats(movies, movieIndex);

    printf("Enter the number of tickets: ");
    scanf("%d", &user->ticketCount);

    for (i = 0; i < user->ticketCount; i++) {
        printf("Enter seat row (A-J) and column (1-10) for ticket %d: ", i + 1);
        char rowChar;
        scanf(" %c%d", &rowChar, &col);
        row = getRowIndex(rowChar);
        col--;

               if (row < 0 || row >= ROWS || col < 0 || col >= COLS || movies[movieIndex].seats[row][col] == 1) {
            printf("Invalid or already booked seat. Please choose another seat.\n");
            i--; // Repeat this iteration
        } else {
            user->row[i] = row;
            user->col[i] = col;
            movies[movieIndex].seats[row][col] = 1;
        }
    }
    user->movieIndex = movieIndex;
}

void purchaseTicket(Movie movies[], User **users, int *userCount, int *userCapacity, int *movieCount) {
    if (*userCount >= *userCapacity) {
        *userCapacity *= 2;
        *users = realloc(*users, *userCapacity * sizeof(User));
    }

    User user;
    userDetails(&user);
    chooseSeat(movies, &user);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(user.bookingDateTime, "%02d-%02d-%d %02d:%02d:%02d",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    (*users)[*userCount] = user;
    (*userCount)++;

    float totalAmount = user.ticketCount * movies[user.movieIndex].price;
    printf("Total amount for %d ticket(s): Rs %.2f\n", user.ticketCount, totalAmount);
    printf("Booking confirmed for %s.\n", user.name);
}

void cancelSeat(Movie movies[], User *users, int *userCount) {
    char phone[11];
    int movieIndex, i, found = 0;
    printf("Enter your phone number: ");
    scanf("%s", phone);

    for (i = 0; i < *userCount; i++) {
        if (strcmp(users[i].phone, phone) == 0) {
            found = 1;
            printf("Booking found for %s\n", users[i].name);
            movieIndex = users[i].movieIndex;

            for (int j = 0; j < users[i].ticketCount; j++) {
                printf("Cancelling seat %c%d\n", getRowLetter(users[i].row[j]), users[i].col[j] + 1);
                movies[movieIndex].seats[users[i].row[j]][users[i].col[j]] = 0;
            }

            for (int k = i; k < *userCount - 1; k++) {
                users[k] = users[k + 1];
            }

            (*userCount)--;
            printf("Booking cancelled successfully.\n");
            return;
        }
    }

    if (!found) {
        printf("No booking found for the entered phone number.\n");
    }
}

void exitSystem() {
    printf("Exiting the system. Thank you!\n");
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
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "Name: %s\n", users[i].name);
        fprintf(file, "Age: %d\n", users[i].age);
        fprintf(file, "Phone: %s\n", users[i].phone);
        fprintf(file, "Movie Index: %d\n", users[i].movieIndex);
        fprintf(file, "Ticket Count: %d\n", users[i].ticketCount);
        for (int j = 0; j < users[i].ticketCount; j++) {
            fprintf(file, "Seat: %c%d\n", getRowLetter(users[i].row[j]), users[i].col[j] + 1);
        }
        fprintf(file, "Booking Date and Time: %s\n", users[i].bookingDateTime);
        fprintf(file, "------------------------\n");
    }

    fclose(file);
    printf("Bookings saved to file successfully.\n");
}

void loadBookingsFromFile(User **users, int *userCount, int *userCapacity) {
    FILE *file = fopen("bookings.txt", "r");
    if (!file) {
        printf("No previous bookings found.\n");
        return;
    }

    while (!feof(file)) {
        if (*userCount >= *userCapacity) {
            *userCapacity *= 2;
            *users = realloc(*users, *userCapacity * sizeof(User));
        }

        User user;
        if (fscanf(file, "Name: %[^\n]\n", user.name) == 1) {
            fscanf(file, "Age: %d\n", &user.age);
            fscanf(file, "Phone: %s\n", user.phone);
            fscanf(file, "Movie Index: %d\n", &user.movieIndex);
            fscanf(file, "Ticket Count: %d\n", &user.ticketCount);
            for (int j = 0; j < user.ticketCount; j++) {
                char seatRow;
                int seatCol;
                fscanf(file, "Seat: %c%d\n", &seatRow, &seatCol);
                user.row[j] = getRowIndex(seatRow);
                user.col[j] = seatCol - 1;
                movies[user.movieIndex].seats[user.row[j]][user.col[j]] = 1;
            }
            fscanf(file, "Booking Date and Time: %[^\n]\n", user.bookingDateTime);
            fscanf(file, "------------------------\n");

            (*users)[*userCount] = user;
            (*userCount)++;
        }
    }

    fclose(file);
    printf("Previous bookings loaded successfully.\n");
}

char getRowLetter(int row) {
    return 'A' + row;
}

int getRowIndex(char letter) {
    return letter - 'A';
}

void printSeats(Movie movies[], int movieIndex) {
    printf("Seats available (1 indicates booked, 0 indicates available):\n");
    printf("   1  2  3  4  5  6  7  8  9 10\n");
    for (int r = 0; r < ROWS; r++) {
        printf("%c ", getRowLetter(r));
        for (int c = 0; c < COLS; c++) {
            printf(" %d ", movies[movieIndex].seats[r][c]);
        }
        printf("\n");
    }
}

void generateReport(User *users, int userCount) {
    FILE *file = fopen("report.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    float totalSales = 0;
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "Name: %s\n", users[i].name);
        fprintf(file, "Phone: %s\n", users[i].phone);
        fprintf(file, "Movie: %s\n", movies[users[i].movieIndex].name);
        fprintf(file, "Tickets: %d\n", users[i].ticketCount);
        fprintf(file, "Total Amount: Rs %.2f\n", users[i].ticketCount * movies[users[i].movieIndex].price);
        totalSales += users[i].ticketCount * movies[users[i].movieIndex].price;
        fprintf(file, "Booking Date and Time: %s\n", users[i].bookingDateTime);
        fprintf(file, "------------------------\n");
    }

    fprintf(file, "Total Sales: Rs %.2f\n", totalSales);
    fclose(file);
    printf("Sales report generated successfully in report.txt.\n");
}

