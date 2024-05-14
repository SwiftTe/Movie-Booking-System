include <stdio.h>
#include <string.h>

// Function prototypes
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
           printf("\n              Simple Movie Booking System\n");
    printf(" ==================================================================\n");
    printf("||             Menu:                                              ||\n");
    printf("||             1- To edit price of ticket (only admin):           ||\n");
    printf("||             2- To view booked tickets (only admin):            ||\n");
    printf("||             3- Show movie and time:                            ||\n");
    printf("||             4- To choose seat:                                 ||\n");
    printf("||             5- To purchase ticket:                             ||\n");
    printf("||             6- To cancel the seat:                             ||\n");
    printf("||             7- Exit system:                                    ||\n");
    printf("||================================================================||\n");
        
        // Get user choice
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        // Process user choice
        switch(choice) {
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
                break;
            case 6:
                cancelSeat();
                break;
            case 7:
                exitSystem();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        
    } while (choice != 7);
    
    return 0;
}

// Function to edit ticket price (admin only)
void editPrice() {
    float newPrice;
    printf("Enter new ticket price: ");
    scanf("%f", &newPrice);
    ticketPrice = newPrice;
    printf("Ticket price updated successfully!\n");
}

// Function to view booked tickets (admin only)
void viewBookedTickets() {
    // Implementation to view booked tickets
    printf("Viewing booked tickets...\n");
}

// Function to show movie and time
void showMovieAndTime(char movie[]) {
    // Implementation to show movie and time
    printf("Movie: %s\n", movie);
    printf("Showing movie and time...\n");
}

// Function to choose seat
void chooseSeat() {
    // Implementation to choose seat
    printf("Choosing seat...\n");
}

// Function to purchase ticket
void purchaseTicket() {
    // Implementation to purchase ticket
    printf("Purchasing ticket...\n");
}

// Function to cancel seat
void cancelSeat() {
    // Implementation to cancel seat
    printf("Canceling seat...\n");
}

// Function to exit the system
void exitSystem() {
    printf("Exiting system...\n");
}
