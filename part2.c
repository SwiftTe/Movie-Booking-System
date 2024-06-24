#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h> // Required for _getch() function

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
    char phone[11];  // 10 digits + null terminator
    int movieIndex;
    int row[10];
    int col[10];
    int ticketCount;
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
void maskPassword(char *password); // Function to mask password input

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
    maskPassword(password); // Mask password input
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
        if (password[i] == '\r') { // Enter key pressed
            password[i] = '\0'; // Null-terminate the string
            break;
        } else if (password[i] == 8 && i > 0) { // Backspace key pressed
            printf("\b \b"); // Erase the character from console
            i--;
        } else {
            printf("*"); // Print asterisk for masking
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
        printf("Enter seat row (A-J) and column (1-10) (e.g., A1, B5): ");
        scanf(" %c%d", &rowLetter, &c);
        rowIndex = getRowIndex(rowLetter);
        c--;

        if (rowIndex >= 0 && rowIndex < ROWS && c >= 0 && c < COLS && movies[movieIndex].seats[rowIndex][c] == 0) {
            movies[movieIndex].seats[rowIndex][c] = 1;
            user->row[i] = rowIndex;
            user->col[i] = c;
        } else {
            printf("Invalid or already booked seat. Please try again.\n");
            i--;
        }
    }
}

void purchaseTicket(Movie movies[], User users[], int *userCount, int movieCount) {
    User newUser;
    userDetails(&newUser);

    showMovies(movies, movieCount);
    printf("Select a movie (1-%d): ", movieCount);
    scanf("%d", &newUser.movieIndex);
    newUser.movieIndex--;

    if (newUser.movieIndex < 0 || newUser.movieIndex >= movieCount) {
        printf("Invalid movie choice.\n");
        return;
    }

    chooseSeat(movies, &newUser);

    users[*userCount] = newUser;
    (*userCount)++;
    printf("Tickets booked successfully!\n");
}

void cancelSeat(Movie movies[], User users[], int *userCount) {
    char name[MAX_NAME_LENGTH];
    char phone[11];
    int movieIndex, i, j;

    printf("Enter the name for ticket cancellation: ");
    scanf(" %[^\n]s", name);
    printf("Enter the phone number for ticket cancellation: ");
    scanf("%s", phone);

    for (i = 0; i < *userCount; i++) {
        if (strcmp(users[i].name, name) == 0 && strcmp(users[i].phone, phone) == 0) {
            movieIndex = users[i].movieIndex;
            for (j = 0; j < users[i].ticketCount; j++) {
                int r = users[i].row[j];
                int c = users[i].col[j];
                movies[movieIndex].seats[r][c] = 0;
            }

            for (j = i; j < *userCount - 1; j++) {
                users[j] = users[j + 1];
            }

            (*userCount)--;
            printf("Tickets cancelled successfully!\n");
            return;
        }
    }

    printf("No booking found under this name and phone number.\n");
}

void exitSystem() {
    printf("Exiting system...\n");
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
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    fprintf(file, "UserCount: %d\n", userCount);
    int i, j;
    for (i = 0; i < userCount; i++) {
        fprintf(file, "Name: %s\n", users[i].name);
        fprintf(file, "Age: %d\n", users[i].age);
        fprintf(file, "Phone: %s\n", users[i].phone);
        fprintf(file, "MovieIndex: %d\n", users[i].movieIndex);
        fprintf(file, "TicketCount: %d\n", users[i].ticketCount);
        for (j = 0; j < users[i].ticketCount; j++) {
            fprintf(file, "Seat: %c%d\n", getRowLetter(users[i].row[j]), users[i].col[j] + 1);
        }
    }
    fclose(file);
    printf("Bookings saved to file.\n");
}

void loadBookingsFromFile(User users[], int *userCount) {
    FILE *file = fopen("bookings.txt", "r");
    if (file == NULL) {
        printf("No previous bookings found.\n");
        return;
    }
    fscanf(file, "UserCount: %d\n", userCount);
    int i, j;
    char rowLetter;
    for (i = 0; i < *userCount; i++) {
        fscanf(file, "Name: %[^\n]s\n", users[i].name);
        fscanf(file, "Age: %d\n", &users[i].age);
        fscanf(file, "Phone: %s\n", users[i].phone);
        fscanf(file, "MovieIndex: %d\n", &users[i].movieIndex);
        fscanf(file, "TicketCount: %d\n", &users[i].ticketCount);
        for (j = 0; j < users[i].ticketCount; j++) {
            fscanf(file, "Seat: %c%d\n", &rowLetter, &users[i].col[j]);
            users[i].row[j] = getRowIndex(rowLetter);
            users[i].col[j]--;
            movies[users[i].movieIndex].seats[users[i].row[j]][users[i].col[j]] = 1;
        }
    }
    fclose(file);
    printf("Bookings loaded from file.\n");
}

char getRowLetter(int row) {
    return 'A' + row;
}

int getRowIndex(char letter) {
    return letter - 'A';
}

void printSeats(Movie movies[], int movieIndex) {
    int r, c;
    printf("Seats for movie: %s at %s (Hall %d)\n", movies[movieIndex].name, movies[movieIndex].time, movies[movieIndex].hallNumber);
    printf("  1 2 3 4 5 6 7 8 9 10\n");
    for (r = 0; r < ROWS; r++) {
        printf("%c ", getRowLetter(r));
        for (c = 0; c < COLS; c++) {
            if (movies[movieIndex].seats[r][c] == 0) {1
                printf("O ");
            } else {
                printf("X ");
            }
        }
        printf("\n");
    }
}
