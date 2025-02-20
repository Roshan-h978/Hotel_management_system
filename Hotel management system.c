#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_ROOMS 50
#define MAX_RESERVATIONS 100

// Enum for User Roles
typedef enum {
    ADMINISTRATOR,
    HOTEL_STAFF,
    CUSTOMER,
    GUEST
} Role;

// Structure for User
typedef struct {
    char username[50];
    char password[50];
    Role role;
    int isActive; // 1 if active, 0 if inactive
} User;

// Structure for Room
typedef struct {
    int roomNumber;
    int isBooked;
    float rate; // Rate for the room
} Room;

// Structure for Reservation
typedef struct {
    char username[50];
    int roomNumber;
} Reservation;

User users[MAX_USERS];
Room rooms[MAX_ROOMS];
Reservation reservations[MAX_RESERVATIONS];
int userCount = 0, roomCount = 0, reservationCount = 0;

// Function to initialize a new data file if it doesn't exist
void initializeFile(const char *filename) {
    FILE *file = fopen(filename, "ab");  // Try to open in append mode
    if (file) {
        fclose(file);
    }
}

// Function to load users from file
void loadUsers()
{
    initializeFile("users.txt");
    FILE *file = fopen("users.txt", "r");
    if (file)
    {
        fscanf(file, "%d", &userCount);
        for (int i = 0; i < userCount; i++)
        {
            fscanf(file, "%s %s %d %d", users[i].username, users[i].password, &users[i].role, &users[i].isActive);
        }
        fclose(file);
    }
    else
    {
        printf("Critical error: Cannot access users file\n");
    }
}

// Function to load rooms from file
void loadRooms()
{
    initializeFile("rooms.txt");
    FILE *file = fopen("rooms.txt", "r");
    if (file)
    {
        fscanf(file, "%d", &roomCount);
        for (int i = 0; i < roomCount; i++)
        {
            fscanf(file, "%d %f %d", &rooms[i].roomNumber, &rooms[i].rate, &rooms[i].isBooked);
        }
        fclose(file);
    }
    else
    {
        printf("Critical error: Cannot access rooms file\n");
    }
}

// Function to load reservations from file
void loadReservations() {
    initializeFile("reservations.dat");
    FILE *file = fopen("reservations.dat", "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        if (ftell(file) > 0) {
            fseek(file, 0, SEEK_SET);
            fread(&reservationCount, sizeof(int), 1, file);
            fread(reservations, sizeof(Reservation), reservationCount, file);
        } else {
            reservationCount = 0;
        }
        fclose(file);
    } else {
        printf("Critical error: Cannot access reservations file\n");
    }
}

// Function to save users to file
void saveUsers()
{
    FILE *file = fopen("users.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", userCount); // Save user count
        for (int i = 0; i < userCount; i++)
        {
            fprintf(file, "%s %s %d %d\n", users[i].username, users[i].password, users[i].role, users[i].isActive);
        }
        fclose(file);
    }
    else
    {
        printf("Error saving users file\n");
    }
}

// Function to save rooms to file
void saveRooms()
{
    FILE *file = fopen("rooms.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", roomCount); // Save room count
        for (int i = 0; i < roomCount; i++)
        {
            fprintf(file, "%d %.2f %d\n", rooms[i].roomNumber, rooms[i].rate, rooms[i].isBooked);
        }
        fclose(file);
    }
    else
    {
        printf("Error saving rooms file\n");
    }
}

// Function to save reservations to file
void saveReservations()
{
    FILE *file = fopen("reservations.txt", "w");
    if (file)
    {
        fprintf(file, "%d\n", reservationCount);
        for (int i = 0; i < reservationCount; i++)
        {
            fprintf(file, "%s %d\n", reservations[i].username, reservations[i].roomNumber);
        }
        fclose(file);
    }
    else
    {
        printf("Error saving reservations file\n");
    }
}

// Function to login a user
int loginUser(User *currentUser) {
    char username[50], password[50];
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    strtok(username, "\n");  // Remove newline character if any
    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    strtok(password, "\n");  // Remove newline character if any
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login successful! Welcome, %s!\n", username);
            *currentUser = users[i]; // Set the current user
            return 1;
        }
    }
    printf("Invalid credentials!\n");
    return 0;
}

// Function to register a user
void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached!\n");
        return;
    }
    printf("Enter username: ");
    fgets(users[userCount].username, sizeof(users[userCount].username), stdin);
    strtok(users[userCount].username, "\n");  // Remove newline character if any
    
    printf("Enter password: ");
    fgets(users[userCount].password, sizeof(users[userCount].password), stdin);
    strtok(users[userCount].password, "\n");  // Remove newline character if any

    printf("Select Role: 0 = Administrator, 1 = Hotel Staff, 2 = Customer, 3 = Guest\n");
    scanf("%d", &users[userCount].role);
    getchar();  // To handle newline character after role input
    users[userCount].isActive = 1;  // New user is active by default
    userCount++;
    saveUsers();
    printf("User registered successfully!\n");
}

// Function to activate/deactivate users
void manageUserAccount() {
    char username[50];
    printf("Enter username to manage: ");
    fgets(username, sizeof(username), stdin);
    strtok(username, "\n");
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("User %s found. Current status: %s\n", users[i].username, users[i].isActive ? "Active" : "Inactive");
            printf("Toggle account status (1 = Activate, 0 = Deactivate): ");
            scanf("%d", &users[i].isActive);
            getchar();  // Handle newline character after input
            saveUsers();
            printf("Account status updated.\n");
            return;
        }
    }
    printf("User not found!\n");
}

// Function to add a new room
void addRoom() {
    if (roomCount >= MAX_ROOMS) {
        printf("Room limit reached!\n");
        return;
    }
    printf("Enter room number: ");
    scanf("%d", &rooms[roomCount].roomNumber);
    printf("Enter room rate: ");
    scanf("%f", &rooms[roomCount].rate);
    rooms[roomCount].isBooked = 0;  // New room is available by default
    roomCount++;
    saveRooms();
    printf("Room added successfully!\n");
}

// Function to view room availability
void viewRoomAvailability() {
    printf("Available Rooms:\n");
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].isBooked == 0) {
            printf("Room %d is available, Rate: %.2f\n", rooms[i].roomNumber, rooms[i].rate);
        }
    }
}

// Function to search for room details
void searchRoomDetails() {
    int roomNumber;
    printf("Enter room number to search: ");
    scanf("%d", &roomNumber);
    getchar();  // To handle newline character after input
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].roomNumber == roomNumber) {
            printf("Room %d - Rate: %.2f\n", rooms[i].roomNumber, rooms[i].rate);
            return;
        }
    }
    printf("Room not found!\n");
}

// Function to make a reservation
void makeReservation() {
    char username[50];
    int roomNumber;
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    strtok(username, "\n");
    printf("Enter room number to reserve: ");
    scanf("%d", &roomNumber);
    getchar();  // To handle newline character after input
    
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].roomNumber == roomNumber && rooms[i].isBooked == 0) {
            rooms[i].isBooked = 1;
            strcpy(reservations[reservationCount].username, username);
            reservations[reservationCount].roomNumber = roomNumber;
            reservationCount++;
            saveRooms();
            saveReservations();
            printf("Reservation successful!\n");
            return;
        }
    }
    printf("Room not available!\n");
}

// Function to cancel a reservation
void cancelReservation() {
    char username[50];
    int roomNumber;
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    strtok(username, "\n");
    printf("Enter room number to cancel: ");
    scanf("%d", &roomNumber);
    getchar();  // To handle newline character after input
    
    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].username, username) == 0 && reservations[i].roomNumber == roomNumber) {
            reservations[i] = reservations[reservationCount - 1];  // Remove reservation
            reservationCount--;
            for (int j = 0; j < roomCount; j++) {
                if (rooms[j].roomNumber == roomNumber) {
                    rooms[j].isBooked = 0;  // Mark room as available
                    break;
                }
            }
            saveRooms();
            saveReservations();
            printf("Reservation canceled!\n");
            return;
        }
    }
    printf("Reservation not found!\n");
}

// Function to view reservation history
void viewReservationHistory() {
    char username[50];
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    strtok(username, "\n");
    printf("Reservation History for %s:\n", username);
    for (int i = 0; i < reservationCount; i++) {
        if (strcmp(reservations[i].username, username) == 0) {
            printf("Room Number: %d\n", reservations[i].roomNumber);
        }
    }
}

// Function to check-in guest
void checkInGuest() {
    char username[50];
    int roomNumber;
    printf("Enter guest username: ");
    fgets(username, sizeof(username), stdin);
    strtok(username, "\n");
    printf("Enter room number to check in: ");
    scanf("%d", &roomNumber);
    getchar();  // Handle newline character after input
    
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].roomNumber == roomNumber && rooms[i].isBooked == 1) {
            printf("%s checked into room %d.\n", username, roomNumber);
            return;
        }
    }
    printf("Room not found or not booked!\n");
}

// Function to check-out guest
void checkOutGuest() {
    char username[50];
    int roomNumber;
    printf("Enter guest username: ");
    fgets(username, sizeof(username), stdin);
    strtok(username, "\n");
    printf("Enter room number to check out: ");
    scanf("%d", &roomNumber);
    getchar();  // Handle newline character after input
    
    for (int i = 0; i < roomCount; i++) {
        if (rooms[i].roomNumber == roomNumber && rooms[i].isBooked == 1) {
            rooms[i].isBooked = 0;  // Mark room as available after checkout
            printf("%s checked out from room %d.\n", username, roomNumber);
            return;
        }
    }
    printf("Room not found or not booked!\n");
}

// Main function
int main() {
    loadUsers();
    loadRooms();
    loadReservations();
    
    int choice;
    User currentUser;

    while (1) {
        printf("\nWelcome to the Hotel Management System\n");
        printf("1. Administrator\n");
        printf("2. Hotel Staff\n");
        printf("3. Customer\n");
        printf("4. Guest\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Handle the newline character after input
        
        switch (choice) {
            case 1:
                printf("Administrator Menu:\n");
                printf("1. Register New User\n");
                printf("2. Manage User Accounts\n");
                printf("3. Add New Room\n");
                printf("4. Exit to Home Page\n");
                printf("Enter choice: ");
                scanf("%d", &choice);
                getchar();
                
                switch (choice) {
                    case 1:
                        registerUser();
                        break;
                    case 2:
                        manageUserAccount();
                        break;
                    case 3:
                        addRoom();
                        break;
                    case 4:
                        break;
                    default:
                        printf("Invalid choice.\n");
                }
                break;
            case 2:
                printf("Hotel Staff Menu:\n");
                printf("1. View Room Availability\n");
                printf("2. Search Room Details\n");
                printf("3. Make Reservation\n");
                printf("4. Cancel Reservation\n");
                printf("5. Check-in Guest\n");
                printf("6. Check-out Guest\n");
                printf("7. Logout \n");
                printf("Enter choice: ");
                scanf("%d", &choice);
                getchar();
                
                switch (choice) {
                    case 1:
                        viewRoomAvailability();
                        break;
                    case 2:
                        searchRoomDetails();
                        break;
                    case 3:
                        makeReservation();
                        break;
                    case 4:
                        cancelReservation();
                        break;
                    case 5:
                        checkInGuest();
                        break;
                    case 6:
                        checkOutGuest();
                        break;
                    case 7:
                        break;
                        
                    default:
                        printf("Invalid choice.\n");
                }
                break;
            case 3:
                printf("Customer Menu:\n");
                printf("1. View Room Availability\n");
                printf("2. Search Room Details\n");
                printf("3. Make Reservation\n");
                printf("4. Cancel Reservation\n");
                printf("5. View Reservation History\n");
                printf("6. Logout \n");
                printf("Enter choice: ");
                scanf("%d", &choice);
                getchar();
                
                switch (choice) {
                    case 1:
                        viewRoomAvailability();
                        break;
                    case 2:
                        searchRoomDetails();
                        break;
                    case 3:
                        makeReservation();
                        break;
                    case 4:
                        cancelReservation();
                        break;
                    case 5:
                        viewReservationHistory();
                        break;
                    case 6:
                        
                        break;
                    
                    default:
                        printf("Invalid choice.\n");
                }
                break;
            case 4:
                printf("Guest Menu:\n");
                printf("1. View Room Availability\n");
                printf("2. Search Room Details\n");
                printf("3. logout\n");
                printf("Enter choice: ");
                scanf("%d", &choice);
                getchar();
                
                switch (choice) {
                    case 1:
                        viewRoomAvailability();
                        break;
                    case 2:
                        searchRoomDetails();
                        break;
                    case 3:
                       
                        break;
                    default:
                        printf("Invalid choice.\n");
                }
                break;
            case 5:
                saveUsers();
                saveRooms();
                saveReservations();
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
