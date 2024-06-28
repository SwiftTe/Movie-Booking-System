#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h> 

#define MAX_MOVIES 3
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 15
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
    char phone[MAX_PHONE_LENGTH];
    int movieIndex;
    char seatCodes[10][3]; // To store seat codes like A1, B2, etc.
    int ticketCount;
} User;

void displayMenu();
void viewBookedTickets(Movie movies[], User users[], int userCount, int movieCount);
void showMovies(Movie movies[], int movieCount);
void chooseSeat(Movie movies[], User *user);
void purchaseTicket(Movie movies[], User users[], int *userCount, int movieCount);
void cancelBookingByPhone(User users[], int *userCount);
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
    {"Spiderman", "9:00 AM", {{0}}, 250},
    {"Batman", "12:00 PM", {{0}}, 250},
    {"Moonknight", "3:00 PM", {{0}}, 250}
};

int main() {
    int choice;
    User users[100];
    int userCount = 0;

    loadBookingsFromFile(users, &userCount);

    do {
        displayMenu();
        printf("Enter your choice (1-6): ");
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
                purchaseTicket(movies, users, &userCount, MAX_MOVIES);
                break;
            case 5:
                cancelBookingByPhone(users, &userCount);
                break;
            case 6:
                saveBookingsToFile(users, userCount);
                exitSystem();
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    } while (choice != 6);

    return 0;
}

void displayMenu() {
    printf("\n              Simple Movie Booking System\n");
    printf(" ==================================================================\n");
    printf("||             Menu:                                              ||\n");
    printf("||             1- Admin Section (admin)                           ||\n");
    printf("||             2- View booked tickets (admin)                     ||\n");
    printf("||             3- Show available movies                           ||\n");
    printf("||             4- Purchase ticket                                 ||\n");
    printf("||             5- Cancel booking                                  ||\n");
    printf("||             6- Exit system                                     ||\n");
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
        printf("|| 4. View all movies                                             ||\n");
        printf("|| 5. Log out                                                     ||\n");
        printf("||================================================================||\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Select Movie (1-%d): ", movieCount);
                scanf("%d", &movieIndex);
                movieIndex--;

                if (movieIndex < 0 || movieIndex >= movieCount) {
                    printf("Invalid movie choice.\n");
                    continue;
                }

                printf("Enter new movie name: ");
                scanf(" %[^\n]s", newName);
                strcpy(movies[movieIndex].name, newName);
                printf("Movie name updated successfully.\n");
                break;
            case 2:
                printf("Select Movie (1-%d): ", movieCount);
                scanf("%d", &movieIndex);
                movieIndex--;

                if (movieIndex < 0 || movieIndex >= movieCount) {
                    printf("Invalid movie choice.\n");
                    continue;
                }

                printf("Enter new movie time: ");
                scanf("%s", newTime);
                strcpy(movies[movieIndex].time, newTime);
                printf("Movie time updated successfully.\n");
                break;
            case 3:
                printf("Select Movie (1-%d): ", movieCount);
                scanf("%d", &movieIndex);
                movieIndex--;

                if (movieIndex < 0 || movieIndex >= movieCount) {
                    printf("Invalid movie choice.\n");
                    continue;
                }

                printf("Enter new movie price: ");
                scanf("%f", &newPrice);
                movies[movieIndex].price = newPrice;
                printf("Movie price updated successfully.\n");
                break;
            case 4:
                showMovies(movies, movieCount);
                break;
            case 5:
                logout = true;
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }
}

void viewBookedTickets(Movie movies[], User users[], int userCount, int movieCount) {
    int i, u, t, booked;
    char seatCode[3];

    for (i = 0; i < movieCount; i++) {
        printf("\nMovie: %s at %s (Hall %d)\n", movies[i].name, movies[i].time, movies[i].hallNumber);
        printf("Booked seats:\n");
        booked = 0;
        for (u = 0; u < userCount; u++) {
            if (users[u].movieIndex == i) {
                for (t = 0; t < users[u].ticketCount; t++) {
                    printf("(%s) - %s (Phone: %s)\n", users[u].seatCodes[t], users[u].name, users[u].phone);
                    booked++;
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
    int i, r, c;
    char seatCode[3];

    printf("Available Seats for %s at %s (Hall %d):\n", movies[movieIndex].name, movies[movieIndex].time, movies[movieIndex].hallNumber);
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) {
            if (movies[movieIndex].seats[r][c] == 0) {
                printf("(%c%d) ", 'A' + r, c + 1);
            }
        }
        printf("\n");
    }

    printf("Enter the number of tickets you want to book: ");
    scanf("%d", &user->ticketCount);

    printf("Enter the seat codes (e.g., A1, B2) separated by space:\n");
    for (i = 0; i < user->ticketCount; i++) {
        scanf(" %s", seatCode);
        user->seatCodes[i][0] = seatCode[0];
        user->seatCodes[i][1] = seatCode[1];
        user->seatCodes[i][2] = '\0'; // Null terminate the string

        r = seatCode[0] - 'A';
        c = seatCode[1] - '1';

        if (r < 0 || r >= ROWS || c < 0 || c >= COLS || movies[movieIndex].seats[r][c] == 1) {
            printf("Seat (%s) is already booked or invalid. Please choose another seat.\n", seatCode);
            i--;
        } else {
            movies[movieIndex].seats[r][c] = 1;
        }
    }
}

void purchaseTicket(Movie movies[], User users[], int *userCount, int movieCount) {
    int choice;

    printf("Select a movie to purchase ticket (1-%d): ", movieCount);
    scanf("%d", &choice);
    choice--;

    if (choice < 0 || choice >= movieCount) {
        printf("Invalid movie choice.\n");
        return;
    }

    User newUser;
    userDetails(&newUser);

    printf("Enter your phone number: ");
    scanf("%s", newUser.phone); // Collect user's phone number

    newUser.movieIndex = choice;
    chooseSeat(movies, &newUser);

    users[*userCount] = newUser;
    (*userCount)++;

    printf("Ticket purchased successfully.\n");
}

void userDetails(User *user) {
    printf("Enter your name: ");
    scanf(" %[^\n]s", user->name);
}

void cancelBookingByPhone(User users[], int *userCount) {
    char phone[MAX_PHONE_LENGTH];
    int i, j, k;
    bool found = false;

    printf("Enter your phone number to cancel the booking: ");
    scanf("%s", phone);

    for (i = 0; i < *userCount; i++) {
        if (strcmp(users[i].phone, phone) == 0) {
            found = true;
            for (j = 0; j < users[i].ticketCount; j++) {
                int movieIndex = users[i].movieIndex;
                int r = users[i].seatCodes[j][0] - 'A';
                int c = users[i].seatCodes[j][1] - '1';
                movies[movieIndex].seats[r][c] = 0;
            }
            for (k = i; k < *userCount - 1; k++) {
                users[k] = users[k + 1];
            }
            (*userCount)--;
            printf("Booking cancelled successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("No booking found under the phone number %s.\n", phone);
    }
}

void exitSystem() {
    printf("Exiting the system...\n");
}

void saveBookingsToFile(User users[], int userCount) {
    int i, j;
    FILE *file = fopen("bookings.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%d\n", userCount);
    for (i = 0; i < userCount; i++) {
        fprintf(file, "%s %s %d %d", users[i].name, users[i].phone, users[i].movieIndex, users[i].ticketCount);
        for (j = 0; j < users[i].ticketCount; j++) {
            fprintf(file, " %s", users[i].seatCodes[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Bookings saved to file successfully.\n");
}

void loadBookingsFromFile(User users[], int *userCount) {
    int i, j;
    FILE *file = fopen("bookings.txt", "r");
    if (file == NULL) {
        printf("No previous bookings found.\n");
        return;
    }
    fscanf(file, "%d", userCount);
    for (i = 0; i < *userCount; i++) {
        fscanf(file, "%s %s %d %d", users[i].name, users[i].phone, &users[i].movieIndex, &users[i].ticketCount);
        for (j = 0; j < users[i].ticketCount; j++) {
            fscanf(file, "%s", users[i].seatCodes[j]);
        }
    }

    fclose(file);
    printf("Bookings loaded from file successfully.\n");
}
