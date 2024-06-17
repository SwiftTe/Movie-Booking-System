#include <stdio.h>
#include <string.h>

// Function prototypes
void showMovieAndTime();
void purchaseTicket();
void cancelPurchase();
void editPrice();
void exitSystem();

// Global variables
float ticketPrice = 100; // Default ticket price
char movieName[20] = "KGF chapter 3"; // Default movie name
char name[20] = "Puspa 2";
char movieTime[20] = "10:00 AM"; // Default movie time
char time[20] = "2:00 PM";

int main() {
    int choice;

    do {
        printf("\n              Simple Movie Booking System\n");
        printf(" ==================================================================\n");
        printf("|| ***MAIN MENU***                                                ||\n");
        printf("||             1. Show movie and time:                            ||\n");
        printf("||             2. To purchase ticket:                             ||\n");
        printf("||             3. To cancel the ticket:                           ||\n");
        printf("||             4. To edit price of ticket (only admin):           ||\n");
        printf("||             5. Exit system:                                    ||\n");
        printf("||================================================================||\n");

        // Get user choices
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Process user choice
        switch(choice) {
            case 1:
                showMovieAndTime();
                break;
            case 2:
                purchaseTicket();
                break;
            case 3:
                cancelPurchase();
                break;
            case 4:
                editPrice();
                break;
            case 5:
                exitSystem();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);

    return 0;
}

// Function to show movie and time
void showMovieAndTime() {
    char show;
    // Implementation to show movie and time
    printf("Movies available:\n");
    printf("a. %s (%s)\n", movieName, movieTime);
    printf("b. %s (%s)\n\n", name, time);
    printf("Enter your choice (a or b): ");
    scanf(" %c", &show); 

    if (show == 'a') {
        printf("Movie: %s\n", movieName);
        printf("Time: %s\n", movieTime);
    } else if (show == 'b') {
        printf("Movie: %s\n", name);
        printf("Time: %s\n", time);
    } else {
        printf("Invalid choice. Please try again.\n");
    }
    printf("Movie selected sucessfully");
}

// Function to purchase ticket
void purchaseTicket() {
    int numOfTickets;
    float totalCost;

    printf("Enter the number of tickets you want to purchase: ");
    scanf("%d", &numOfTickets);

    if (numOfTickets <= 0) {
        printf("Invalid number of tickets. Please enter a valid number.\n");
        return;
    }

    totalCost = ticketPrice * numOfTickets;

    printf("Total cost for %d tickets: $%.2f\n", numOfTickets, totalCost);

    // Here you can implement payment gateway integration or any other purchase confirmation logic

    printf("\tTickets purchased successfully!\n");
}

// Function to cancel purchase
void cancelPurchase() {
    // Implementation to cancel purchase
    printf("\tPurchase has been cancell !!!...\n");
}

// Function to edit ticket price (admin only)
void editPrice() {
    float newPrice;
    printf("Enter new ticket price: ");
    scanf("%f", &newPrice);
    ticketPrice = newPrice;
    printf("\tTicket price updated successfully!\n");
}

// Function to exit the system
void exitSystem() {
    printf("\n\t\tSEE YOU SOON !!!...\n");
    printf("donee");
}

