import java.io.*;
import java.util.*;

// Structure to represent an item in the menu
class Item {
    double price;
    String foodName;
    int quantity = 0;

    Item(double price, String foodName, int quantity) {
        this.price = price;
        this.foodName = foodName;
        this.quantity = quantity;
    }
}

// Structure to represent an item in the user's cart
class CartItem {
    String foodName;
    double price;
    int quantity;

    CartItem(String foodName, double price, int quantity) {
        this.foodName = foodName;
        this.price = price;
        this.quantity = quantity;
    }
}

// Class to handle the entire food delivery system
class Food {
    private static final int ADMIN_PIN = 1234;

    public void loginPage() {
        Scanner scanner = new Scanner(System.in);
        int choice;

        do {
            System.out.println("*************************************************************");
            System.out.println("*****          Welcome to the Food Delivery System      *****");
            System.out.println("*************************************************************");
            System.out.println("1. Login");
            System.out.println("2. Register");
            System.out.println("0. Exit");
            System.out.print("Enter your choice: ");
            choice = scanner.nextInt();
            scanner.nextLine(); // Consume newline

            switch (choice) {
                case 1:
                    if (login()) {
                        startPage();
                    } else {
                        System.out.println("Invalid credentials. Please try again.");
                    }
                    break;
                case 2:
                    registerUser();
                    break;
                case 0:
                    System.exit(0);
                default:
                    System.out.println("Invalid choice. Please try again.");
            }
        } while (choice != 0);
    }

    public boolean login() {
        Scanner scanner = new Scanner(System.in);
        String username, password, storedUser, storedPass;

        System.out.print("Enter username: ");
        username = scanner.nextLine();
        System.out.print("Enter password: ");
        password = scanner.nextLine();

        try (BufferedReader userFile = new BufferedReader(new FileReader("users.txt"))) {
            String line;
            while ((line = userFile.readLine()) != null) {
                String[] parts = line.split(" ");
                storedUser = parts[0];
                storedPass = parts[1];
                if (username.equals(storedUser) && password.equals(storedPass)) {
                    System.out.println("Login successful!");
                    return true;
                }
            }
        } catch (IOException e) {
            System.out.println("Error: Unable to open user file.");
        }
        return false;
    }

    public void registerUser() {
        Scanner scanner = new Scanner(System.in);
        String username, password;

        System.out.print("Enter a username: ");
        username = scanner.nextLine();
        System.out.print("Enter a password: ");
        password = scanner.nextLine();

        try (BufferedWriter userFile = new BufferedWriter(new FileWriter("users.txt", true))) {
            userFile.write(username + " " + password + "\n");
            System.out.println("Registration successful! You can now log in.");
        } catch (IOException e) {
            System.out.println("Error: Unable to open user file.");
        }
    }

    public void authAdmin(Item[] foods, int size) {
        Scanner scanner = new Scanner(System.in);
        int pin;

        System.out.println("Hello, Welcome to admin page. Use your pin code to access");
        pin = scanner.nextInt();
        scanner.nextLine(); // Consume newline

        if (pin == ADMIN_PIN) {
            System.out.println("Admin authenticated successfully.");
            view(foods, size);
        } else {
            System.out.println("Invalid PIN. Try again later.");
        }
    }

    public void placeOrder(ArrayList<CartItem> cart) {
        Scanner scanner = new Scanner(System.in);
        double totalCost = 0;
        String location;

        System.out.print("Enter your delivery location: ");
        location = scanner.nextLine();

        System.out.println("\nHello, your order is placed");
        for (CartItem item : cart) {
            System.out.println("Ordered Food: " + item.foodName);
            System.out.println("Quantity: " + item.quantity);
            System.out.println("Price: " + item.price * item.quantity + " Birr");
            totalCost += item.price * item.quantity;
        }

        System.out.println("\nTotal Price: " + totalCost + " Birr");
        System.out.println("Delivery Location: " + location);

        paymentProcess(totalCost);

        System.out.println("Delivery Status: Pending");
        System.out.println("*************************************************************");
        System.out.println("*************************************************************");
        System.out.println("***              THANK YOU FOR CHOOSING US!          ********");
        System.out.println("*************************************************************");
        System.out.println("*************************************************************");

        startPage();
        trackDelivery(location);
    }

    public void trackDelivery(String location) {
        System.out.println("\nTracking Delivery:");
        System.out.println("Your order is being prepared and will be delivered to " + location + " soon.");
        System.out.println("Current Status: Pending");
    }

    public void paymentProcess(double totalCost) {
        Scanner scanner = new Scanner(System.in);
        int paymentMethod;
        String accountNumber;
        Random rand = new Random();
        int deliveryCode = rand.nextInt(9000) + 1000;

        System.out.println("\nChoose your payment method:");
        System.out.println("1. Mobile Banking");
        System.out.println("2. Cash on Delivery");
        System.out.println("3. TeleBirr Payment");
        System.out.print(">> ");
        paymentMethod = scanner.nextInt();
        scanner.nextLine(); // Consume newline

        switch (paymentMethod) {
            case 1:
                System.out.println("Our account number is 100025234652347:");
                System.out.print("Enter your mobile banking account number: ");
                accountNumber = scanner.nextLine();

                if (!accountNumber.matches("\\d+")) {
                    System.out.println("Invalid account number. Only digits are allowed. Try again.");
                    paymentProcess(totalCost);
                    return;
                }

                System.out.println("Payment of " + totalCost + " Birr is successful from account " + accountNumber + ".");
                break;

            case 2:
                System.out.println("Our account number is 100025234652347:");
                System.out.println("You chose Cash on Delivery. Please prepare " + totalCost + " Birr for payment.");
                break;

            case 3:
                System.out.print("Enter your phone number: ");
                accountNumber = scanner.nextLine();

                if (!accountNumber.matches("\\d+")) {
                    System.out.println("Invalid card number. Only digits are allowed. Try again.");
                    paymentProcess(totalCost);
                    return;
                }

                System.out.println("Payment of " + totalCost + " Birr is successful using TeleBirr ending in " + accountNumber.substring(accountNumber.length() - 4) + ".");
                break;

            default:
                System.out.println("Invalid payment method. Please try again.");
                paymentProcess(totalCost);
                return;
        }

        System.out.println("Your delivery verification code is: " + deliveryCode);
        System.out.println("Please provide this code upon receiving your order.");
    }

    public void startPage() {
        Scanner scanner = new Scanner(System.in);
        int choose;
        Item[] foods = new Item[10];
        loadMenu(foods, 10);

        do {
            System.out.println("*************************************************************");
            System.out.println("*************************************************************");
            System.out.println("*****                                                ********");
            System.out.println("====>     Hello, WELCOME TO OUR DELIVERY SYSTEM      <===****");
            System.out.println("====>                                                <===****");
            System.out.println("*****                                                ********");
            System.out.println("*************************************************************");
            System.out.println("*************************************************************");
            System.out.println("1. View Menu");
            System.out.println("2. Login to admin");
            System.out.println("0. Exit the program");
            System.out.print(" Enter your choice >> ");
            choose = scanner.nextInt();
            scanner.nextLine(); // Consume newline
        } while (choose != 0 && choose != 1 && choose != 2);

        switch (choose) {
            case 0:
                System.exit(0);
            case 1:
                viewMenu(foods);
                break;
            case 2:
                authAdmin(foods, 10);
                break;
        }
    }

    public void view(Item[] foods, int size) {
        System.out.println("ID\t\tFOOD NAME\t\tPRICE\t\tAVAILABLE QUANTITY");
        System.out.println("==\t\t=========\t\t=====\t\t==================");
        for (int i = 0; i < size; i++) {
            System.out.println(i + "\t\t" + foods[i].foodName + "\t\t" + foods[i].price + "Birr\t\t" + foods[i].quantity);
        }

        restockItem(foods, size);
        saveMenu(foods, size);
        startPage();
    }

    public void viewMenu(Item[] foods) {
        Scanner scanner = new Scanner(System.in);
        int itemId, quantity;
        ArrayList<CartItem> cart = new ArrayList<>();

        System.out.println("*************************************************************");
        System.out.println("*************************************************************");
        System.out.println("*********                                        ************");
        System.out.println("*********                                        ************");
        System.out.println("=========  Our system has the following foods    =======*****");
        System.out.println("=========                                        =======*****");
        System.out.println("=========                                        =======*****");
        System.out.println("*************************************************************");
        System.out.println("*************************************************************");
        System.out.println("ID\t\t FOOD NAME\t\tPRICE\t\tAVAILABLE QUANTITY");
        System.out.println("==\t\t=========\t\t=====\t\t==================");
        for (int i = 0; i < 10; i++) {
            System.out.println(i + "\t\t" + foods[i].foodName + "\t\t" + foods[i].price + " Birr\t\t" + foods[i].quantity);
        }
        System.out.println("==\t\t=========\t\t=====\t\t==================");
        System.out.println("*************************************************************");

        while (true) {
            System.out.print("Enter item ID to buy (-1 to 9): ");
            itemId = scanner.nextInt();
            scanner.nextLine(); // Consume newline

            if (itemId == -1) {
                break;
            }

            if (itemId < 0 || itemId >= 10) {
                System.out.println("Invalid item ID. Please try again.");
                continue;
            }

            Item selectedItem = foods[itemId];

            if (selectedItem.quantity == 0) {
                System.out.println("Sorry, the item is out of stock. Admin needs to restock.");
                continue;
            }

            do {
                System.out.print("Enter quantity: ");
                quantity = scanner.nextInt();
                scanner.nextLine(); // Consume newline

                if (quantity > selectedItem.quantity) {
                    System.out.println("Sorry, we only have " + selectedItem.quantity + " available.");
                }
            } while (quantity <= 0 || quantity > selectedItem.quantity);

            selectedItem.quantity -= quantity;
            cart.add(new CartItem(selectedItem.foodName, selectedItem.price, quantity));
        }

        saveMenu(foods, 10);

        if (!cart.isEmpty()) {
            placeOrder(cart);
        } else {
            System.out.println("No items were selected. Returning to main menu.");
            startPage();
        }
    }

    public void restockItem(Item[] foods, int size) {
        Scanner scanner = new Scanner(System.in);
        int itemId, restockQty;

        System.out.print("Enter the item ID to restock: ");
        itemId = scanner.nextInt();
        scanner.nextLine(); // Consume newline

        if (itemId < 0 || itemId >= size) {
            System.out.println("Invalid item ID.");
            return;
        }

        System.out.print("Enter the quantity to add: ");
        restockQty = scanner.nextInt();
        scanner.nextLine(); // Consume newline

        if (restockQty <= 0) {
            System.out.println("Invalid quantity. Please try again.");
            return;
        }

        foods[itemId].quantity += restockQty;
        System.out.println("Item " + foods[itemId].foodName + " has been restocked. New quantity: " + foods[itemId].quantity);
    }

    public void saveMenu(Item[] foods, int size) {
        try (BufferedWriter file = new BufferedWriter(new FileWriter("menu.txt"))) {
            for (int i = 0; i < size; i++) {
                file.write(foods[i].price + " " + foods[i].foodName + " " + foods[i].quantity + "\n");
            }
        } catch (IOException e) {
            System.out.println("Error: Unable to save menu.");
        }
    }

    public void loadMenu(Item[] foods, int size) {
        try (BufferedReader file = new BufferedReader(new FileReader("menu.txt"))) {
            for (int i = 0; i < size; i++) {
                String line = file.readLine();
                if (line == null) break;
                String[] parts = line.split(" ");
                foods[i] = new Item(Double.parseDouble(parts[0]), parts[1], Integer.parseInt(parts[2]));
            }
        } catch (IOException e) {
            System.out.println("Error: Unable to load menu. Using default values.");
            Item[] defaultFoods = {
                new Item(100, "FIRFIR", 50),
                new Item(300, "DORO_W", 60),
                new Item(150, "Enkula", 20),
                new Item(400, "BURGER", 25),
                new Item(120, "PASTAA", 35),
                new Item(150, "PIZZAA", 55),
                new Item(900, "TIRE_S", 10),
                new Item(100, "MIRNDA", 80),
                new Item(100, "SPRITE", 80),
                new Item(100, "COCA_C", 90)
            };
            for (int i = 0; i < size; i++) {
                foods[i] = defaultFoods[i];
            }
        }
    }

    public static void main(String[] args) {
        Food food = new Food();
        food.loginPage();
    }
}