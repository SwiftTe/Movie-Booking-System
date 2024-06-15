#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
    scanf("%s", password);

    if (strcmp(username, "owner") == 0 && strcmp(password, "123456789") == 0) {
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
    for (int i = 0; i < movieCount; i++) {
        printf("\nMovie: %s at %s (Hall %d)\n", movies[i].name, movies[i].time, movies[i].hallNumber);
        printf("Booked seats:\n");
        int booked = 0;
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                if (movies[i].seats[r][c] == 1) {
                    for (int u = 0; u < userCount; u++) {
                        for (int t = 0; t < users[u].ticketCount; t++) {
                            if (users[u].movieIndex == i && users[u].row[t] == r && users[u].col[t] == c) {
                                printf("(%d,%d) - %s ", r + 1, c + 1, users[u].name);
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
    printf("Available Movies:\n");
    for (int i = 0; i < movieCount; i++) {
        printf("+---------------------------------------------+\n");
        printf("| %d. %-41s |\n", i + 1, movies[i].name);
        printf("| Time: %-36s |\n", movies[i].time);
        printf("| Hall: %-2d                               Price: RS %-6.2f |\n", movies[i].hallNumber, movies[i].price);
        printf("+---------------------------------------------+\n");
    }
}

void chooseSeat(Movie movies[], User *user) {
    int movieIndex = user->movieIndex;
    printf("Choosing seats for %s at %s (Hall %d)\n", movies[movieIndex].name, movies[movieIndex].time, movies[movieIndex].hallNumber);
    printf("Available seats:\n");
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (movies[movieIndex].seats[r][c] == 0) {
                printf("(%d,%d) ", r + 1, c + 1);
            } else {
                printf("(X,X) ");  // Indicate booked seats with 'X'
            }
        }
        printf("\n");
    }
    
    for (int i = 0; i < user->ticketCount; i++) {
        printf("Enter row number for ticket %d (1-10): ", i + 1);
        scanf("%d", &user->row[i]);
        printf("Enter column number for ticket %d (1-10): ", i + 1);
        scanf("%d", &user->col[i]);
        user->row[i]--;
        user->col[i]--;
        
        if (user->row[i] < 0 || user->row[i] >= ROWS || user->col[i] < 0 || user->col[i] >= COLS || movies[movieIndex].seats[user->row[i]][user->col[i]] == 1) {
            printf("Invalid or already booked seat for ticket %d. Please choose again.\n", i + 1);
            i--;
        } else {
            movies[movieIndex].seats[user->row[i]][user->col[i]] = 1;
        }
    }

    printf("Seats booked successfully.\n");
}

void purchaseTicket(Movie movies[], User users[], int *userCount, int movieCount) {
    int movieChoice;
    User newUser;
    userDetails(&newUser);

    printf("Enter the movie number to purchase tickets: ");
    scanf("%d", &movieChoice);
    movieChoice--;

    if (movieChoice < 0 || movieChoice >= movieCount) {
        printf("Invalid movie choice.\n");
        return;
    }

    printf("Purchasing ticket for %s at %s (Hall %d)\n", movies[movieChoice].name, movies[movieChoice].time, movies[movieChoice].hallNumber);
    printf("Enter the number of tickets to purchase: ");
    scanf("%d", &newUser.ticketCount);

    if (newUser.ticketCount <= 0 || newUser.ticketCount > 10) {
        printf("Invalid number of tickets. You can purchase up to 10 tickets at a time.\n");
        return;
    }

    newUser.movieIndex = movieChoice;
    chooseSeat(movies, &newUser);

    users[*userCount] = newUser;
    (*userCount)++;

    printf("Tickets purchased successfully for %s at %s (Hall %d).\n", movies[movieChoice].name, movies[movieChoice].time, movies[movieChoice].hallNumber);
    for (int i = 0; i < newUser.ticketCount; i++) {
        printf("Seat %d: (%d, %d), Price: RS %.2f\n", i + 1, newUser.row[i] + 1, newUser.col[i] + 1, movies[movieChoice].price);
    }
}

void cancelSeat(Movie movies[], User users[], int *userCount) {
    char name[MAX_NAME_LENGTH];
    int movieChoice = -1;
    int found = -1;

    printf("Enter your name to cancel tickets: ");
    scanf("%s", name);

    for (int i = 0; i < *userCount; i++) {
        if (strcmp(users[i].name, name) == 0) {
            movieChoice = users[i].movieIndex;
            found = i;
            break;
        }
    }

    if (movieChoice < 0 || found == -1) {
        printf("No booked seats found for the given name.\n");
        return;
    }

    for (int i = 0; i < users[found].ticketCount; i++) {
        int row = users[found].row[i];
        int col = users[found].col[i];
        if (row >= 0 && col >= 0 && movies[movieChoice].seats[row][col] == 1) {
            movies[movieChoice].seats[row][col] = 0;
            printf("Seat (%d,%d) for movie %s at %s (Hall %d) has been canceled.\n", row + 1, col + 1, movies[movieChoice].name, movies[movieChoice].time, movies[movieChoice].hallNumber);
        }
    }

    // Shift remaining users
    for (int i = found; i < *userCount - 1; i++) {
        users[i] = users[i + 1];
    }
    (*userCount)--;
}

void exitSystem() {
    printf("Exiting system...\n");
}

void userDetails(User *user) {
    printf("Enter your name: ");
    scanf("%s", user->name);

    printf("Enter your age: ");
    scanf("%d", &user->age);

    printf("Enter your gender (M/F): ");
    scanf(" %c", &user->gender);

    printf("\nHello, %s! You are %d years old and your gender is %c.\n", user->name, user->age, user->gender);
}
