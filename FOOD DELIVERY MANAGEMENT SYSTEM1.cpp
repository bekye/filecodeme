#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#define ADMIN_PIN 1234
using namespace std;

// Structure to represent an item in the menu
struct Item {
    double price;
    string foodName;
    int quantity = 0;
};

// Class to handle food delivery system operations
class Food {
public:
    void placeOrder(Item);                // Place an order for a food item
    void startPage();                     // Display the main menu of the program
    void viewMenu();                      // Display the list of food items
    void view();                          // Admin view for restocking
    void authAdmin(Item *foods, int size); // Authenticate admin and allow restocking
    void restockItem(Item *foods, int size); // Restock a specific food item
    void saveMenu(Item *foods, int size); // Save the current menu to a file
    void loadMenu(Item *foods, int size); // Load the menu from a file
    void trackDelivery(string location);  // Track delivery status
    void paymentProcess(double totalCost); // Handle payment process
};

void Food::authAdmin(Item *foods, int size) {
    int pin;
    cout << "Hello, Welcome to admin page. Use your pin code to access\n";
    cin >> pin;
    cin.ignore();

    if (pin == ADMIN_PIN) {
        cout << "Admin authenticated successfully.\n";
        view();
    } else {
        cout << "Invalid PIN. Try again later.\n";
    }
}

void Food::placeOrder(Item foodItem) {
    double totalCost = foodItem.price * foodItem.quantity;
    string location;

    cout << "Enter your delivery location: ";
    getline(cin, location);

    cout << "Hello, your order is placed\n";
    cout << "Ordered Food: " << foodItem.foodName;
    cout << "\nQuantity: " << foodItem.quantity;
    cout << "\nTotal Price: " << totalCost << " Birr\n";
    cout << "Delivery Location: " << location << "\n";

    paymentProcess(totalCost);

    cout << "Delivery Status: Pending\n";
    cout << "THANK YOU FOR CHOOSING US!\n";

    startPage();
    trackDelivery(location);
}

void Food::trackDelivery(string location) {
    cout << "\nTracking Delivery:\n";
    cout << "Your order is being prepared and will be delivered to " << location << " soon.\n";
    cout << "Current Status: Pending\n";
}

void Food::paymentProcess(double totalCost) {
    int paymentMethod;
    string accountNumber;
    srand(time(0));
    int deliveryCode = rand() % 9000 + 1000; // Generate a random 4-digit code

    cout << "\nChoose your payment method:\n";
    cout << "1. Mobile Banking\n";
    cout << "2. Cash on Delivery\n";
    cout << "3. Card Payment\n";
    cout << ">> ";
    cin >> paymentMethod;
    cin.ignore();

    switch (paymentMethod) {
    case 1:
        cout << "Enter your mobile banking account number: ";
        getline(cin, accountNumber);
        cout << "Payment of " << totalCost << " Birr is successful from account " << accountNumber << ".\n";
        break;
    case 2:
        cout << "You chose Cash on Delivery. Please prepare " << totalCost << " Birr for payment.\n";
        break;
    case 3:
        cout << "Enter your card number: ";
        getline(cin, accountNumber);
        cout << "Payment of " << totalCost << " Birr is successful using card ending in " << accountNumber.substr(accountNumber.length() - 4) << ".\n";
        break;
    default:
        cout << "Invalid payment method. Please try again.\n";
        paymentProcess(totalCost);
        return;
    }

    cout << "Your delivery verification code is: " << deliveryCode << "\n";
    cout << "Please provide this code upon receiving your order.\n";
}

void Food::startPage() {
    int choose;
    Item foods[6];
    loadMenu(foods, 6);

    do {
        cout << "*************************************************************\n";
        cout << "====>     Hello, WELCOME TO OUR DELIVERY SYSTEM     <===***\n";
        cout << "====>     Our system has the following options       <===***\n";
        cout << "*************************************************************\n";
        cout << "1. View Menu\n";
        cout << "2. Login to admin\n";
        cout << "0. Exit the program\n";
        cout << ">> ";
        cin >> choose;
        cin.ignore();
    } while (choose != 0 && choose != 1 && choose != 2);

    switch (choose) {
    case 0:
        exit(0);
    case 1:
        viewMenu();
        break;
    case 2:
        authAdmin(foods, 6);
        break;
    }
}

void Food::view() {
    int size = 6;
    Item foods[6];
    loadMenu(foods, size);

    for (size_t i = 0; i < size; i++) {
        cout << i << ". " << foods[i].foodName << " " << foods[i].price << " Birr (Available: " << foods[i].quantity << ")\n";
    }
    restockItem(foods, size);
    saveMenu(foods, size);
    startPage();
}

void Food::viewMenu() {
    int itemId, quantity;
    Item foods[6];
    loadMenu(foods, 6);

    cout << "*************************************************************\n";
    cout << "=========Our system has the following foods============****\n";
    cout << "*************************************************************\n";
    for (size_t i = 0; i < 6; i++) {
        cout << i << ". " << foods[i].foodName << " " << foods[i].price << " Birr (Available: " << foods[i].quantity << ")\n";
    }
    cout << "*************************************************************\n";

    cout << "Enter item ID to buy: ";
    cin >> itemId;
    cin.ignore();

    if (itemId < 0 || itemId >= 6) {
        cout << "Invalid item ID. Please try again.\n";
        return;
    }

    Item &selectedItem = foods[itemId];

    if (selectedItem.quantity == 0) {
        cout << "Sorry, the item is out of stock. Admin needs to restock.\n";
        return;
    }

    do {
        cout << "Enter quantity: ";
        cin >> quantity;
        cin.ignore();

        if (quantity > selectedItem.quantity) {
            cout << "Sorry, we only have " << selectedItem.quantity << " available.\n";
        }
    } while (quantity <= 0 || quantity > selectedItem.quantity);

    selectedItem.quantity -= quantity;
    saveMenu(foods, 6);

    selectedItem.quantity = quantity;
    placeOrder(selectedItem);
}

void Food::restockItem(Item *foods, int size) {
    int itemId, restockQty;

    cout << "Enter the item ID to restock: ";
    cin >> itemId;
    cin.ignore();

    if (itemId < 0 || itemId >= size) {
        cout << "Invalid item ID.\n";
        return;
    }

    cout << "Enter the quantity to add: ";
    cin >> restockQty;
    cin.ignore();

    if (restockQty <= 0) {
        cout << "Invalid quantity. Please try again.\n";
        return;
    }

    foods[itemId].quantity += restockQty;
    cout << "Item " << foods[itemId].foodName << " has been restocked. New quantity: " << foods[itemId].quantity << "\n";
}

void Food::saveMenu(Item *foods, int size) {
    ofstream file("menu.txt");
    if (!file) {
        cout << "Error: Unable to save menu.\n";
        return;
    }

    for (int i = 0; i < size; i++) {
        file << foods[i].price << " " << foods[i].foodName << " " << foods[i].quantity << "\n";
    }

    file.close();
}

void Food::loadMenu(Item *foods, int size) {
    ifstream file("menu.txt");
    if (!file) {
        cout << "Error: Unable to load menu. Using default values.\n";
        Item defaultFoods[6] = {
            {80, "FIRFIR", 50},
            {200, "DOROWOT", 60},
            {80, "ENKULALIFIRFIR", 20},
            {400, "BURGER", 25},
            {120, "PASTA", 35},
            {150, "PIZZA", 55},
        };
        for (int i = 0; i < size; i++) {
            foods[i] = defaultFoods[i];
        }
        return;
    }

    for (int i = 0; i < size; i++) {
        if (!(file >> foods[i].price >> foods[i].foodName >> foods[i].quantity)) {
            cout << "Error reading menu data. Loading default values.\n";
            Item defaultFoods[6] = {
                {80, "FIRFIR", 50},
                {200, "DOROWOT", 60},
                {80, "ENKULALIFIRFIR", 20},
                {400, "BURGER", 25},
                {120, "PASTA", 35},
                {150, "PIZZA", 55},
            };
            for (int j = 0; j < size; j++) {
                foods[j] = defaultFoods[j];
            }
            break;
        }
    }

    file.close();
}

int main() {
    Food food;
    food.startPage();
    return 0;
}
