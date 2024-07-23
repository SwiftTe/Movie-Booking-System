#include <stdio.h>

void userDetails() {
    char name;
    int age;
    char gender;
    #include <stdio.h>

void userDetails() {
    char name[50];
    int age;
    char gender[2];
    char phone[15];

    printf("Enter your name: ");
    scanf("%s", name);

    printf("Enter your age: ");
    scanf("%d", &age);

    printf("Enter your gender (M/F): ");
    scanf(" %s", gender);

    printf("Enter your phone number: ");
    scanf("%s", phone);

    printf("\nHello, %s! You are %d years old, your gender is %s, and your phone number is %s.\n", name, age, gender, phone);
}

int main() {
    int choice;

    printf("                                                           Simple Movie Booking System                    \n");
    printf("============================================================================================================================================================\n");
    printf


    printf("Enter your name: ");
    scanf(" %c", &name);

    printf("Enter your age: ");
    scanf("%d", &age);

    printf("Enter your gender (M/F): ");
    scanf(" %c", &gender);

    printf("\nHello, %c! You are %d years old and your gender is %c.\n", name, age, gender);
}

int main() {
    int choice;

    printf("                                                           Simple Movie Booking System                    \n");
    printf("============================================================================================================================================================\n");
    printf("||                                                         1- To edit price of ticket (only admin):                                                        ||\n");
    printf("||                                                         2- To view booked tickets (only admin):                                                         ||\n");
    printf("||                                                         3- Show movie and time:                                                                         ||\n");
    printf("||                                                         4- To choose seat:                                                                              ||\n");
    printf("||                                                         5- To purchase ticket:                                                                          ||\n");
    printf("||                                                         6- To cancel the seat:                                                                          ||\n");
    printf("||                                                         7- Exit system:                                                                                 ||\n");
    printf("||==========================================================================================================================================================\n");
    printf("||                                                               Wel-come                                                                                                                             ||\n");
    do {
        printf("Enter your choice (1-7): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Edit ticket price\n");
                break;
            case 2:
                printf("View booked tickets\n");
                break;
            case 3:
                printf("Show movie and time\n");
                break;
            case 4:
                printf("Choose seat\n");
                break;
            case 5:
                printf("Purchase ticket\n");
                userDetails(); // Gather user details for purchase
                break;
            case 6:
                printf("Cancel seat\n");
                userDetails(); // Gather user details for cancellation
                break;
            case 7:
                printf("Exiting system\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 7);

    return 0;
}
