
#include <iostream>        // Include the iostream library for input and output operations
#include <fstream>         // Include the fstream library for file operations
#include <string>          // Include the string library for handling strings
#include <vector>          // Include the vector library to use dynamic arrays
#include <cstdlib>         // Include the cstdlib library for random number generation and other utilities
#include <ctime>           // Include the ctime library to use time functions
#include <algorithm>       // Include the algorithm library for functions like all_of
#define ADMIN_PIN 1234     // Define a constant for the admin PIN
using namespace std;       // Use the standard namespace to simplify code

// Structure to represent an item in the menu
struct Item {
    double price;            // Price of the food item
    string foodName;         // Name of the food item
    int quantity = 0;        // Quantity of the food item available
};

// Structure to represent an item in the user's cart
struct CartItem {
    string foodName;         // Name of the food item in the cart
    double price;            // Price of the food item in the cart
    int quantity;            // Quantity of the food item in the cart
};

// Class to handle the entire food delivery system
class Food {
public:
    void loginPage();                                    // Function to display the login or register page
    bool login();                                       // Function to handle user login
    void registerUser();                                // Function to handle user registration
    void placeOrder(vector<CartItem> &cart);            // Function to place an order
    void startPage();                                   // Function to display the main menu
    void viewMenu();                                    // Function to display the menu and allow users to order
    void view();                                        // Function to allow admin to view and restock items
    void authAdmin(Item *foods, int size);              // Function to authenticate the admin
    void restockItem(Item *foods, int size);            // Function to restock a specific item
    void saveMenu(Item *foods, int size);               // Function to save the menu to a file
    void loadMenu(Item *foods, int size);               // Function to load the menu from a file
    void trackDelivery(string location);                // Function to track the delivery status
    void paymentProcess(double totalCost);              // Function to handle the payment process
};

// Function to display the login or registration page
void Food::loginPage() {
    int choice;  // Variable to store user's choice

    do {
        cout << "*************************************************************\n";
        cout << "*****          Welcome to the Food Delivery System      *****\n";
        cout << "*************************************************************\n";
        cout << "1. Login\n";               // Option to log in
        cout << "2. Register\n";            // Option to register
        cout << "0. Exit\n";                // Option to exit the program
        cout << "Enter your choice: ";
        cin >> choice;              // Get user's choice
        cin.ignore();               // Ignore newline character

        switch (choice) {
        case 1:
            if (login()) {          // Call the login function
                startPage();        // If login is successful, go to the main menu
            } else {
                cout << "Invalid credentials. Please try again.\n";
            }
            break;
        case 2:
            registerUser();         // Call the register function
            break;
        case 0:
            exit(0);                // Exit the program
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}

// Function to handle user login
bool Food::login() {
    string username, password, storedUser, storedPass;
    ifstream userFile("users.txt"); // Open the file containing user credentials

    cout << "Enter username: ";
    getline(cin, username);         // Get the username
    cout << "Enter password: ";
    getline(cin, password);         // Get the password

    if (!userFile) {                // Check if the file cannot be opened
        cout << "Error: Unable to open user file.\n";
        return false;
    }

    while (userFile >> storedUser >> storedPass) { // Read each username and password pair
        if (username == storedUser && password == storedPass) { // Check if credentials match
            cout << "Login successful!\n";
            userFile.close();       // Close the file
            return true;            // Return true for successful login
        }
    }

    userFile.close();               // Close the file
    return false;                   // Return false for failed login
}

// Function to handle user registration
void Food::registerUser() {
    string username, password;
    ofstream userFile("users.txt", ios::app); // Open the file in append mode

    if (!userFile) {                // Check if the file cannot be opened
        cout << "Error: Unable to open user file.\n";
        return;
    }

    cout << "Enter a username: ";
    getline(cin, username);         // Get the username
    cout << "Enter a password: ";
    getline(cin, password);         // Get the password

    userFile << username << " " << password << "\n"; // Save the credentials to the file
    userFile.close();               // Close the file
    cout << "Registration successful! You can now log in.\n";
}
void Food::authAdmin(Item *foods, int size) {
    int pin;                               // Variable to store the admin PIN entered by the user
    cout << "Hello, Welcome to admin page. Use your pin code to access\n";
    cin >> pin;                            // Get the admin PIN from the user
    cin.ignore();                          // Ignore the newline character left in the input buffer

    if (pin == ADMIN_PIN) {                // Check if the entered PIN matches the defined ADMIN_PIN
        cout << "Admin authenticated successfully.\n";
        view();                            // Call the view function to allow the admin to manage the menu
    } else {
        cout << "Invalid PIN. Try again later.\n"; // Display an error if the PIN is incorrect
    }
}
void Food::placeOrder(vector<CartItem> &cart) {
    double totalCost = 0;                  // Variable to store the total cost of the order
    string location;                       // Variable to store the user's delivery location

    cout << "Enter your delivery location: ";
    getline(cin, location);                // Get the delivery location from the user

    cout << "\nHello, your order is placed\n";
    for (const auto &item : cart) {        // Loop through all items in the cart
        cout << "Ordered Food: " << item.foodName << "\n";
        cout << "Quantity: " << item.quantity << "\n";
        cout << "Price: " << item.price * item.quantity << " Birr\n";
        totalCost += item.price * item.quantity; // Add the cost of the current item to the total cost
    }

    cout << "\nTotal Price: " << totalCost << " Birr\n"; // Display the total cost
    cout << "Delivery Location: " << location << "\n";   // Display the delivery location

    paymentProcess(totalCost);             // Call the payment process function to handle payment

    cout << "Delivery Status: Pending\n";  // Notify the user about the delivery status
    cout << "*************************************************************\n";
    cout << "*************************************************************\n";
    cout << "***              THANK YOU FOR CHOOSING US!          ********\n";
    cout << "*************************************************************\n";
    cout << "*************************************************************\n";

    startPage();                           // Return to the main menu
    trackDelivery(location);               // Call the function to track delivery
}
void Food::trackDelivery(string location) {
    cout << "\nTracking Delivery:\n";
    cout << "Your order is being prepared and will be delivered to " << location << " soon.\n"; // Notify the user of the delivery status
    cout << "Current Status: Pending\n";   // Notify the user that the delivery is pending
}
void Food::paymentProcess(double totalCost) {
    int paymentMethod;                     // Variable to store the user's chosen payment method
    string accountNumber;                  // Variable to store the user's account or phone number
    srand(time(0));                        // Seed the random number generator with the current time
    int deliveryCode = rand() % 9000 + 1000; // Generate a random 4-digit delivery verification code

    cout << "\nChoose your payment method:\n";
    cout << "1. Mobile Banking\n";         // Option for Mobile Banking
    cout << "2. Cash on Delivery\n";       // Option for Cash on Delivery
    cout << "3. TeleBirr Payment";         // Option for TeleBirr Payment
    cout << ">> ";
    cin >> paymentMethod;                  // Get the payment method from the user
    cin.ignore();                          // Ignore the newline character left in the input buffer

    switch (paymentMethod) {               // Handle different payment methods based on the user's choice
    case 1:
        cout << "Our account number is 100025234652347:\n ";
        cout << "Enter your mobile banking account number: ";
        getline(cin, accountNumber);       // Get the user's mobile banking account number

        // Validate input to allow only digits
        if (!std::all_of(accountNumber.begin(), accountNumber.end(), ::isdigit)) {
            cout << "Invalid account number. Only digits are allowed. Try again.\n";
            paymentProcess(totalCost);     // Restart the payment process if validation fails
            return;
        }

        cout << "Payment of " << totalCost << " Birr is successful from account " << accountNumber << ".\n"; // Confirm successful payment
        break;

    case 2:
        cout << "Our account number is 100025234652347:\n";
        cout << "You chose Cash on Delivery. Please prepare " << totalCost << " Birr for payment.\n"; // Confirm cash payment
        break;

    case 3:
        cout << "Enter your phone number: ";
        getline(cin, accountNumber);       // Get the user's phone number for TeleBirr payment

        // Validate input to allow only digits
        if (!std::all_of(accountNumber.begin(), accountNumber.end(), ::isdigit)) {
            cout << "Invalid card number. Only digits are allowed. Try again.\n";
            paymentProcess(totalCost);     // Restart the payment process if validation fails
            return;
        }

        cout << "Payment of " << totalCost << " Birr is successful using TeleBirr ending in " << accountNumber.substr(accountNumber.length() - 4) << ".\n"; // Confirm TeleBirr payment
        break;

    default:
        cout << "Invalid payment method. Please try again.\n";
        paymentProcess(totalCost);         // Restart the payment process if an invalid method is chosen
        return;
    }

    cout << "Your delivery verification code is: " << deliveryCode << "\n"; // Display the delivery verification code
    cout << "Please provide this code upon receiving your order.\n";
}
void Food::startPage() {
    int choose;                           // Variable to store the user's choice
    Item foods[10];                       // Array to hold the menu items
    loadMenu(foods, 10);                  // Load the menu from the file or use default values if the file is unavailable

    do {
        cout << "*************************************************************\n";
        cout << "*************************************************************\n";
        cout << "*****                                                ********\n";
        cout << "====>     Hello, WELCOME TO OUR DELIVERY SYSTEM      <===****\n";
        cout << "====>                                                <===****\n";
        cout << "*****                                                ********\n";
        cout << "*************************************************************\n";
        cout << "*************************************************************\n";
        cout << "1. View Menu\n";                     // Option to view the menu and place an order
        cout << "2. Login to admin\n";               // Option to log in as admin
        cout << "0. Exit the program\n";             // Option to exit the program
        cout << " Enter your choice >> ";
        cin >> choose;                    // Get the user's choice
        cin.ignore();                     // Ignore the newline character left in the input buffer
    } while (choose != 0 && choose != 1 && choose != 2); // Repeat until the user selects a valid option

    switch (choose) {
    case 0:
        exit(0);                          // Exit the program
    case 1:
        viewMenu();                       // Call the function to view the menu
        break;
    case 2:
        authAdmin(foods, 10);             // Call the function to authenticate the admin
        break;
    }
}
void Food::view() {
    int size = 10;                        // Size of the menu array
    Item foods[10];                       // Array to hold the menu items
    loadMenu(foods, size);                // Load the menu from the file

    cout << "ID\t\tFOOD NAME\t\tPRICE\t\tAVAILABLE QUANTITY\n";
    cout << "==\t\t=========\t\t=====\t\t==================\n";
    for (size_t i = 0; i < size; i++) {   // Loop through all menu items
        cout << i << "\t\t" << foods[i].foodName << "\t\t" << foods[i].price << "Birr\t\t" << foods[i].quantity << "\n";
    }

    restockItem(foods, size);             // Call the function to restock items
    saveMenu(foods, size);                // Save the updated menu to the file
    startPage();                          // Return to the main menu
}
void Food::viewMenu() {
    int itemId, quantity;                 // Variables for selected item ID and quantity
    vector<CartItem> cart;                // Vector to store the user's cart
    Item foods[10];                       // Array to hold the menu items
    loadMenu(foods, 10);                  // Load the menu from the file

    cout << "*************************************************************\n";
    cout << "*************************************************************\n";
    cout << "*********                                        ************\n";
    cout << "*********                                        ************\n";
    cout << "=========  Our system has the following foods    =======*****\n";
    cout << "=========                                        =======*****\n";
    cout << "=========                                        =======*****\n";
    cout << "*************************************************************\n";
    cout << "*************************************************************\n";
    cout << "ID\t\t FOOD NAME\t\tPRICE\t\tAVAILABLE QUANTITY\n";
    cout << "==\t\t=========\t\t=====\t\t==================\n";
    for (size_t i = 0; i < 10; i++) {     // Display all menu items
        cout << i << "\t\t" << foods[i].foodName << "\t\t" << foods[i].price << " Birr\t\t" << foods[i].quantity << endl;
    }
    cout << "==\t\t=========\t\t=====\t\t==================\n";
    cout << "*************************************************************\n";

    while (true) {
        cout << "Enter item ID to buy (-1 to 9): ";
        cin >> itemId;                    // Get the item ID from the user
        cin.ignore();                     // Ignore the newline character

        if (itemId == -1) {               // If the user enters -1, exit the loop
            break;
        }

        if (itemId < 0 || itemId >= 10) { // Check if the item ID is invalid
            cout << "Invalid item ID. Please try again.\n";
            continue;
        }

        Item &selectedItem = foods[itemId]; // Reference to the selected item

        if (selectedItem.quantity == 0) { // Check if the item is out of stock
            cout << "Sorry, the item is out of stock. Admin needs to restock.\n";
            continue;
        }

        do {
            cout << "Enter quantity: ";
            cin >> quantity;             // Get the quantity from the user
            cin.ignore();                // Ignore the newline character

            if (quantity > selectedItem.quantity) { // Check if the requested quantity exceeds the available quantity
                cout << "Sorry, we only have " << selectedItem.quantity << " available.\n";
            }
        } while (quantity <= 0 || quantity > selectedItem.quantity); // Repeat until a valid quantity is entered

        selectedItem.quantity -= quantity; // Reduce the item's quantity
        cart.push_back({selectedItem.foodName, selectedItem.price, quantity}); // Add the item to the cart
    }

    saveMenu(foods, 10);                  // Save the updated menu to the file

    if (!cart.empty()) {
        placeOrder(cart);                 // Call the function to place an order if the cart is not empty
    } else {
        cout << "No items were selected. Returning to main menu.\n";
        startPage();                      // Return to the main menu if no items were selected
    }
}
void Food::restockItem(Item *foods, int size) {
    int itemId, restockQty;               // Variables for item ID and quantity to restock

    cout << "Enter the item ID to restock: ";
    cin >> itemId;                        // Get the item ID from the admin
    cin.ignore();                         // Ignore the newline character

    if (itemId < 0 || itemId >= size) {   // Check if the item ID is invalid
        cout << "Invalid item ID.\n";
        return;
    }

    cout << "Enter the quantity to add: ";
    cin >> restockQty;                    // Get the quantity to restock
    cin.ignore();                         // Ignore the newline character

    if (restockQty <= 0) {                // Check if the quantity is invalid
        cout << "Invalid quantity. Please try again.\n";
        return;
    }

    foods[itemId].quantity += restockQty; // Add the quantity to the item's stock
    cout << "Item " << foods[itemId].foodName << " has been restocked. New quantity: " << foods[itemId].quantity << "\n";
}
void Food::saveMenu(Item *foods, int size) {
    ofstream file("menu.txt");            // Open the file for writing
    if (!file) {                          // Check if the file cannot be opened
        cout << "Error: Unable to save menu.\n";
        return;
    }

    for (int i = 0; i < size; i++) {      // Write each menu item's details to the file
        file << foods[i].price << " " << foods[i].foodName << " " << foods[i].quantity << "\n";
    }

    file.close();                         // Close the file
}
void Food::loadMenu(Item *foods, int size) {
    ifstream file("menu.txt");            // Open the file for reading
    if (!file) {                          // If the file cannot be opened, use default values
        cout << "Error: Unable to load menu. Using default values.\n";
        Item defaultFoods[10] = {         // Array of default menu items
            {100, "FIRFIR", 50},
            {300, "DORO_W", 60},
            {150, "Enkula", 20},
            {400, "BURGER", 25},
            {120, "PASTAA", 35},
            {150, "PIZZAA", 55},
            {900, "TIRE_S", 10},
            {100, "MIRNDA", 80},
            {100, "SPRITE", 80},
            {100, "COCA_C", 90},
        };
        for (int i = 0; i < size; i++) {
            foods[i] = defaultFoods[i];   // Assign default items to the menu array
        }
        return;
    }

    for (int i = 0; i < size; i++) {      // Read each item's details from the file
        if (!(file >> foods[i].price >> foods[i].foodName >> foods[i].quantity)) {
            cout << "Error reading menu data. Loading default values.\n";
            break;
        }
    }

    file.close();                         // Close the file
}
int main() {
    Food food;                            // Create an instance of the Food class
    food.loginPage();                     // Start the program by displaying the login page
    return 0;                             // End the program
}
