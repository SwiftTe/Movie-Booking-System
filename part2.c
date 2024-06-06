#include <stdio.h>
#include <string.h>

// Define constants for maximum values
#define MAX_MOVIES 3
#define MAX_NAME_LENGTH 50
#define MAX_SEATS 50

// Define structure to store movie details
typedef struct {
    char name[MAX_NAME_LENGTH];   // Movie name
    char time[20];                // Movie showtime
    int seats[MAX_SEATS];         // Seat availability
    float price;                  // Ticket price
} Movie;

// Function prototypes for various operations
void displayMenu();                 // Display the menu options
void editPrice();                   // Edit the ticket price
void viewBookedTickets();           // View booked tickets
void showMovies(Movie movies[]);    // Show available movies
void chooseSeat(Movie movies[]);    // Choose a seat for a movie
void purchaseTicket(Movie movies[]);// Purchase a ticket
void cancelSeat();                  // Cancel a booked seat
void exitSystem();                  // Exit the booking system
void userDetails();                 // Gather user details

// Global variable for ticket price
float ticketPrice = 100; // Default ticket price

int main() {
    int choice;  // Variable to store user's menu choice

    // Initialize movies with default values
    Movie movies[MAX_MOVIES] = {
        {"KGF Chapter 3", "12:00 PM", {0}, 150},
        {"Bahubali 3", "03:00 PM", {0}, 200},
        {"RRR 2", "06:00 PM", {0}, 180}
    };

    // Loop to display the menu and process user's choice
    do {
        displayMenu();  // Show menu options
        printf("Enter your choice (1-7): ");
        scanf("%d", &choice);  // Get user's choice

        // Process user's choice
        switch (choice) {
            case 1:
                editPrice();  // Call function to edit ticket price
                break;
            case 2:
                viewBookedTickets();  // Call function to view booked tickets
                break;
            case 3:
                showMovies(movies);  // Call function to show available movies
                break;
            case 4:
                chooseSeat(movies);  // Call function to choose a seat
                break;
            case 5:
                purchaseTicket(movies);  // Call function to purchase a ticket
                userDetails();  // Call function to gather user details
                break;
            case 6:
                cancelSeat();  // Call function to cancel a seat
                userDetails();  // Call function to gather user details
                break;
            case 7:
                exitSystem();  // Call function to exit the system
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 7.\n");
        }
    } while (choice != 7);  // Repeat until user chooses to exit

    return 0;
}

// Function to display the menu options
void displayMenu() {
    printf("\n              Simple Movie Booking System\n");
    printf(" ==================================================================\n");
    printf("||             Menu:                                              ||\n");
    printf("||             1- Edit ticket price (admin)                       ||\n");
    printf("||             2- View booked tickets (admin)                     ||\n");
    printf("||             3- Show available movies                           ||\n");
    printf("||             4- Choose seat                                     ||\n");
    printf("||             5- Purchase ticket                                 ||\n");
    printf("||             6- Cancel the seat                                 ||\n");
    printf("||             7- Exit system                                     ||\n");
    printf("||================================================================||\n");
}

// Function to edit the ticket price (admin only)
void editPrice() {
    float newPrice;  // Variable to store the new ticket price
    printf("Enter the new ticket price (in RS): ");
    scanf("%f", &newPrice);  // Get new price from user
    ticketPrice = newPrice;  // Update the global ticket price
    printf("Ticket price updated successfully to RS %.2f!\n", ticketPrice);
}

// Function to view booked tickets (admin only)
void viewBookedTickets() {
    printf("Viewing booked tickets...\n");
    // Implementation to view booked tickets
}

// Function to show available movies
void showMovies(Movie movies[]) {
    printf("Available Movies:\n");
    for (int i = 0; i < MAX_MOVIES; i++) {
        printf("%d. %s at %s, Price: RS %.2f\n", i + 1, movies[i].name, movies[i].time, movies[i].price);
    }
}

// Function to choose a seat for a movie
void chooseSeat(Movie movies[]) {
    int movieChoice;  // Variable to store the selected movie number
    printf("Enter the movie number to choose a seat: ");
    scanf("%d", &movieChoice);
    movieChoice--;  // Adjust for array indexing

    // Check if the chosen movie number is valid
    if (movieChoice < 0 || movieChoice >= MAX_MOVIES) {
        printf("Invalid movie choice.\n");
        return;
    }

    printf("Choosing seat for %s at %s\n", movies[movieChoice].name, movies[movieChoice].time);
    // Implementation to choose seat
}

// Function to purchase a ticket
void purchaseTicket(Movie movies[]) {
    int movieChoice;  // Variable to store the selected movie number
    printf("Enter the movie number to purchase ticket: ");
    scanf("%d", &movieChoice);
    movieChoice--;  // Adjust for array indexing

    // Check if the chosen movie number is valid
    if (movieChoice < 0 || movieChoice >= MAX_MOVIES) {
        printf("Invalid movie choice.\n");
        return;
    }

    printf("Purchasing ticket for %s at %s\n", movies[movieChoice].name, movies[movieChoice].time);
    // Implementation to purchase ticket
}

// Function to cancel a booked seat
void cancelSeat() {
    printf("Canceling seat...\n");
    // Implementation to cancel seat
}

// Function to gather user details
void userDetails() {
    char name[50];  // Variable to store user's name
    int age;  // Variable to store user's age
    char gender;  // Variable to store user's gender

    printf("Enter your name: ");
    scanf("%s", name);  // Get user's name

    printf("Enter your age: ");
    scanf("%d", &age);  // Get user's age

    printf("Enter your gender (M/F): ");
    scanf(" %c", &gender);  // Get user's gender

    printf("\nHello, %s! You are %d years old and your gender is %c.\n", name, age, gender);
}

// Function to exit the booking system
void exitSystem() {
    printf("Exiting system...\n");
}
