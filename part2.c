#include <stdio.h>
#include <string.h>

// Function prototypes
void userDetails();
void displayMenu();
void editPrice();
void viewBookedTickets();
void showMovieAndTime(char movie[]);
void chooseSeat();
void purchaseTicket();
void cancelSeat();
void exitSystem();

// Global variables
float ticketPrice = 100; // Default ticket price
char movieName[20] = "KGF chapter 3 "; // Default movie name

int main() {
    int choice;

    do {
        displayMenu();
        printf("Enter your choice (1-7): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                editPrice();
                break;
            case 2:
                viewBookedTickets();
                break;
            case 3:
                showMovieAndTime(movieName);
                break;
            case 4:
                chooseSeat();
                break;
            case 5:
                purchaseTicket();
                userDetails(); // Gather user details for purchase
                break;
            case 6:
                cancelSeat();
                userDetails(); // Gather user details for cancellation
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


// Function to display the menu
void displayMenu() {
    printf("\n              Simple Movie Booking System\n");
    printf(" ==================================================================\n");
    printf("||             Menu:                                              ||\n");
    printf("||             1- Edit ticket price (admin)                      ||\n");
    printf("||             2- View booked tickets (admin)                    ||\n");
    printf("||             3- Show movie and time                            ||\n");
    printf("||             4- Choose seat                                    ||\n");
    printf("||             5- Purchase ticket                                ||\n");
    printf("||             6- Cancel the seat                                ||\n");
    printf("||             7- Exit system                                    ||\n");
    printf("||================================================================||\n");
}

// Function to edit ticket price (admin only)
void editPrice() {
    float newPrice;
    printf("Enter the new ticket price: ");
    scanf("%f", &newPrice);
    ticketPrice = newPrice;
    printf("Ticket price updated successfully!\n");
}

// Function to view booked tickets (admin only)
void viewBookedTickets() {
    printf("Viewing booked tickets...\n");
    // Implementation to view booked tickets
}

// Function to show movie and time
void showMovieAndTime(char movie[]) {
    printf("Showing movie: %s\n", movie);
    // Implementation to show movie and time
}

// Function to choose seat
void chooseSeat() {
    printf("Choosing seat...\n");
    // Implementation to choose seat
}

// Function to purchase ticket
void purchaseTicket() {
    printf("Purchasing ticket...\n");
    // Implementation to purchase ticket
}

// Function to cancel seat
void cancelSeat() {
    printf("Canceling seat...\n");
    // Implementation to cancel seat
}

// Function to gather user details
void userDetails() {
    char name[50];
    int age;
    char gender;

    printf("Enter your name: ");
    scanf("%s", name);

    printf("Enter your age: ");
    scanf("%d", &age);

    printf("Enter your gender (M/F): ");
    scanf(" %c", &gender);

    printf("\nHello, %s! You are %d years old and your gender is %c.\n", name, age, gender);
}

// Function to exit the system
void exitSystem() {
    printf("Exiting system...\n");
}
