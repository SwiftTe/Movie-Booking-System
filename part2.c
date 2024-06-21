#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>

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
    char gender;
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
    int i = 0;
    char c;
    while (i < 19 && (c = getch()) != '\n') {
        if (c == '\r') { // Handle Enter key press
            printf("\n");
            break; // Exit loop when Enter is pressed
        } else if (c == 8) { // Handle backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // Erase the character from console
            }
        } else {
            password[i++] = c;
            printf("*"); // Print asterisk to mask the password
        }
    }
    password[i] = '\0'; // Null terminate the password string

    if (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0) {
        return true;
    } else {
        return false;
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
                                printf("(%d,%d) - %s (Age: %d, Gender: %c)\n", r + 1, c + 1, users[u].name, users[u].age, users[u].gender);
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

    printf("Available Seats for %s at %s (Hall %d):\n", movies[movieIndex].name, movies[movieIndex].time, movies[movieIndex].hallNumber);
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) {
            if (movies[movieIndex].seats[r][c] == 0) {
                printf("(%d,%d) ", r + 1, c + 1);
            }
        }
        printf("\n");
    }

    printf("Enter the number of tickets you want to book: ");
    scanf("%d", &user->ticketCount);
    for (i = 0; i < user->ticketCount; i++) {
        printf("Enter seat row and column (row col): ");
        scanf("%d %d", &user->row[i], &user->col[i]);
        user->row[i]--; // Adjust to 0-based indexing
        user->col[i]--;
        if (user->row[i] >= 0 && user->row[i] < ROWS && user->col[i] >= 0 && user->col[i] < COLS && movies[movieIndex].seats[user->row[i]][user->col[i]] == 0) {
            movies[movieIndex].seats[user->row[i]][user->col[i]] = 1;
            printf("Seat (%d,%d) booked successfully.\n", user->row[i] + 1, user->col[i] + 1);
        } else {
            printf("Seat (%d,%d) is not available. Choose another seat.\n", user->row[i] + 1, user->col[i] + 1);
            i--; // Re-enter seat selection
        }
    }
}

void purchaseTicket(Movie movies[], User users[], int *userCount, int movieCount) {
    int movieChoice;
    User newUser;

    userDetails(&newUser);
    printf("Choose a movie (1-%d): ", movieCount);
    scanf("%d", &movieChoice);
    movieChoice--;
    if (movieChoice < 0 || movieChoice >= movieCount) {
        printf("Invalid movie choice.\n");
        return;
    }
    newUser.movieIndex = movieChoice;

    chooseSeat(movies, &newUser);
    users[*userCount] = newUser;
    (*userCount)++;
    printf("Ticket(s) purchased successfully.\n");

    saveBookingsToFile(users, *userCount); // Save after purchase
}

void cancelSeat(Movie movies[], User users[], int *userCount) {
    int movieChoice, r, c, i, j;
    char userName[MAX_NAME_LENGTH];

    printf("Enter your name: ");
    scanf(" %[^\n]s", userName);
    for (i = 0; i < *userCount; i++) {
        if (strcmp(users[i].name, userName) == 0) {
            printf("Booking found for %s.\n", userName);
            movieChoice = users[i].movieIndex;

            printf("Seats booked by %s for %s at %s:\n", userName, movies[movieChoice].name, movies[movieChoice].time);
            for (j = 0; j < users[i].ticketCount; j++) {
                printf("(%d,%d) ", users[i].row[j] + 1, users[i].col[j] + 1);
            }
            printf("\n");

            for (j = 0; j < users[i].ticketCount; j++) {
                movies[movieChoice].seats[users[i].row[j]][users[i].col[j]] = 0;
            }

            for (j = i; j < *userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            (*userCount)--;
            printf("Booking cancelled successfully.\n");

            saveBookingsToFile(users, *userCount); // Save after cancellation
            return;
        }
    }
    printf("No booking found for %s.\n", userName);
}

void exitSystem() {
    printf("Exiting system...\n");
}

void userDetails(User *user) {
    printf("Enter your name: ");
    scanf(" %[^\n]s", user->name);
    printf("Enter your age: ");
    scanf("%d", &user->age);
    printf("Enter your gender (M/F): ");
    scanf(" %c", &user->gender);
}

void saveBookingsToFile(User users[], int userCount) {
    FILE *file = fopen("bookings.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "User Count: %d\n", userCount);
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "Name: %s, Age: %d, Gender: %c, Movie Index: %d, Ticket Count: %d\n", users[i].name, users[i].age, users[i].gender, users[i].movieIndex, users[i].ticketCount);
        fprintf(file, "Seats: ");
        for (int j = 0; j < users[i].ticketCount; j++) {
            fprintf(file, "(%d,%d) ", users[i].row[j] + 1, users[i].col[j] + 1);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Bookings saved to file successfully.\n");
}

void loadBookingsFromFile(User users[], int *userCount) {
    FILE *file = fopen("bookings.txt", "r");
    if (file == NULL) {
        printf("No previous bookings found.\n");
        return;
    }

    fscanf(file, "User Count: %d\n", userCount);
    for (int i = 0; i < *userCount; i++) {
        fscanf(file, "Name: %[^,], Age: %d, Gender: %c, Movie Index: %d, Ticket Count: %d\n", users[i].name, &users[i].age, &users[i].gender, &users[i].movieIndex, &users[i].ticketCount);
        fscanf(file, "Seats: ");
        for (int j = 0; j < users[i].ticketCount; j++) {
            fscanf(file, "(%d,%d) ", &users[i].row[j], &users[i].col[j]);
            users[i].row[j]--; // Convert to 0-based indexing
            users[i].col[j]--;
        }
        fscanf(file, "\n");
    }

    fclose(file);
    printf("Bookings loaded from file successfully.\n");
}
