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
        printf("| Hall: %-2d                               Price: Rs %.2f |\n", movies[i].hallNumber, movies[i].price);
        printf("+---------------------------------------------+\n");
    }
}

void chooseSeat(Movie movies[], User *user) {
    int i, j, seatRow, seatCol, seatNumber;
    char seatLetter;
    printf("Choose your seats (Enter row letter and column number): \n");
    for (i = 0; i < user->ticketCount; i++) {
        printf("Seat %d: ", i + 1);
        scanf(" %c%d", &seatLetter, &seatNumber);
        seatRow = getRowIndex(seatLetter);
        seatCol = seatNumber - 1;

        if (seatRow >= 0 && seatRow < ROWS && seatCol >= 0 && seatCol < COLS && movies[user->movieIndex].seats[seatRow][seatCol] == 0) {
            movies[user->movieIndex].seats[seatRow][seatCol] = 1;
            user->row[i] = seatRow;
            user->col[i] = seatCol;
        } else {
            printf("Invalid seat or seat already booked. Try again.\n");
            i--;
        }
    }
}

void purchaseTicket(Movie movies[], User users[], int *userCount, int movieCount) {
    User user;
    int movieChoice, ticketCount;
    char confirm;

    showMovies(movies, movieCount);

    printf("Select Movie (1-%d): ", movieCount);
    scanf("%d", &movieChoice);
    movieChoice--;

    if (movieChoice < 0 || movieChoice >= movieCount) {
        printf("Invalid movie choice.\n");
        return;
    }

    userDetails(&user);
    user.movieIndex = movieChoice;

    printf("Enter number of tickets: ");
    scanf("%d", &ticketCount);

    if (ticketCount <= 0 || ticketCount > 10) {
        printf("Invalid number of tickets.\n");
        return;
    }

    user.ticketCount = ticketCount;

    printSeats(movies, movieChoice);

    chooseSeat(movies, &user);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(user.bookingDateTime, "%02d-%02d-%04d %02d:%02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    users[*userCount] = user;
    (*userCount)++;

    printf("Confirm booking? (y/n): ");
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        printf("Booking confirmed!\n");
        printf("Booking details:\n");
        printf("Name: %s\n", user.name);
        printf("Age: %d\n", user.age);
        printf("Phone: %s\n", user.phone);
        printf("Movie: %s\n", movies[user.movieIndex].name);
        printf("Time: %s\n", movies[user.movieIndex].time);
        printf("Hall Number: %d\n", movies[user.movieIndex].hallNumber);
        printf("Seats: ");
        for (int i = 0; i < user.ticketCount; i++) {
            printf("%c%d ", getRowLetter(user.row[i]), user.col[i] + 1);
        }
        printf("\nBooked Date and Time: %s\n", user.bookingDateTime);
    } else {
        printf("Booking cancelled.\n");
    }
}

void cancelSeat(Movie movies[], User users[], int *userCount) {
    int i, j, found = 0;
    char phone[11];
    printf("Enter phone number to cancel booking: ");
    scanf("%s", phone);

    for (i = 0; i < *userCount; i++) {
        if (strcmp(users[i].phone, phone) == 0) {
            found = 1;
            printf("Cancelling booking for %s\n", users[i].name);
            for (j = 0; j < users[i].ticketCount; j++) {
                movies[users[i].movieIndex].seats[users[i].row[j]][users[i].col[j]] = 0;
            }
            for (j = i; j < *userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            (*userCount)--;
            printf("Booking cancelled successfully.\n");
            return;
        }
    }

    if (!found) {
        printf("No booking found for this phone number.\n");
    }
}

void exitSystem() {
    printf("Exiting the system...\n");
}

void userDetails(User *user) {
    printf("Enter your details:\n");
    printf("Name: ");
    scanf(" %[^\n]s", user->name);
    printf("Age: ");
    scanf("%d", &user->age);
    printf("Phone: ");
    scanf("%s", user->phone);
}

char getRowLetter(int row) {
    return 'A' + row;
}

int getRowIndex(char letter) {
    return letter - 'A';
}

void printSeats(Movie movies[], int movieIndex) {
    printf("Available seats:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (movies[movieIndex].seats[i][j] == 0) {
                printf("%c%d ", getRowLetter(i), j + 1);
            } else {
                printf("XX ");
            }
        }
        printf("\n");
    }
}

void saveBookingsToFile(User users[], int userCount) {
    FILE *file = fopen("bookings.txt", "w");
    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "Name: %s\n", users[i].name);
        fprintf(file, "Age: %d\n", users[i].age);
        fprintf(file, "Phone: %s\n", users[i].phone);
        fprintf(file, "Movie Index: %d\n", users[i].movieIndex);
        fprintf(file, "Seats: ");
        for (int j = 0; j < users[i].ticketCount; j++) {
            fprintf(file, "%c%d ", getRowLetter(users[i].row[j]), users[i].col[j] + 1);
        }
        fprintf(file, "\nBooking Date and Time: %s\n", users[i].bookingDateTime);
        fprintf(file, "----------------------------------------\n");
    }

    fclose(file);
}

void loadBookingsFromFile(User users[], int *userCount) {
    FILE *file = fopen("bookings.txt", "r");
    if (file == NULL) {
        printf("Unable to open file.\n");
        return;
    }

    while (fscanf(file, "Name: %[^\n]\n", users[*userCount].name) != EOF) {
        fscanf(file, "Age: %d\n", &users[*userCount].age);
        fscanf(file, "Phone: %[^\n]\n", users[*userCount].phone);
        fscanf(file, "Movie Index: %d\n", &users[*userCount].movieIndex);
        fscanf(file, "Seats: ");
        for (int j = 0; j < 10; j++) {
            fscanf(file, "%c%d ", (char*)&users[*userCount].row[j], &users[*userCount].col[j]);
        }
        fscanf(file, "\nBooking Date and Time: %[^\n]\n", users[*userCount].bookingDateTime);
        fscanf(file, "----------------------------------------\n");
        (*userCount)++;
    }

    fclose(file);
}
