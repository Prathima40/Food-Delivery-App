#include <iostream>
#include <string>
using namespace std;

// Define maximum number of menu items, cart items, and orders
const int MAX_ITEMS = 3;

// Structure to represent a menu item
struct MenuItem {
    int id;
    string name;
    double price;
    bool isAvailable;
};

// Structure to represent a cart item
struct CartItem {
    int id;
    string name;
    double price;
};

// Structure to represent an order
struct Order {
    int orderID;
    double totalPrice;
    int itemCount;
    CartItem items[MAX_ITEMS];
};

// Arrays to store menu items, cart items, and orders
MenuItem menu[MAX_ITEMS];
CartItem cart[MAX_ITEMS];
Order orderQueue[MAX_ITEMS];

// Counters and pointers for managing menu, cart, and orders
int menuCount = 0, cartSize = 0, front = 0, rear = -1, orderCount = 0, nextOrderID = 1;
int top = -1; // Stack top for cart

// Function to add items to the menu
void addItem() {
    int numItems;
    cout << "How many items do you want to enter? Maximum size is " << MAX_ITEMS << ": ";
    cin >> numItems;

    if (numItems <= 0) {
        cout << "Invalid input. Please enter a positive number of items." << endl;
        return;
    }

    if (numItems > MAX_ITEMS) {
        cout << "Cannot add " << numItems << " items. The maximum number of items allowed is " << MAX_ITEMS << "." << endl;
        return;
    }

    if (menuCount + numItems <= MAX_ITEMS) {
        for (int i = 0; i < numItems; ++i) {
            int id;
            string name;
            double price;
            bool isAvailable;

            cout << "Enter item ID: ";
            cin >> id;
            cin.ignore(); // Clear the input buffer to handle strings properly after integer input
            cout << "Enter item name: ";
            getline(cin, name);
            cout << "Enter item price: ";
            cin >> price;
            cout << "Is the item available? (1 for yes, 0 for no): ";
            cin >> isAvailable;

            menu[menuCount++] = { id, name, price, isAvailable };
        }
    }
    else {
        cout << "Menu is full. Cannot add more items." << endl;
    }
}

// Function to display the menu items
void displayMenu() {
    if (menuCount == 0) {
        cout << "Menu is empty." << endl;
        return;
    }

    for (int i = 0; i < menuCount; i++) {
        cout << "ID: " << menu[i].id << " Name: " << menu[i].name
            << " Price: Rs" << menu[i].price
            << " Available: " << (menu[i].isAvailable ? "Yes" : "No") << endl;
    }
}

// Function to remove an item from the menu by name
void removeItem(string name) {
    bool found = false;
    for (int i = 0; i < menuCount; i++) {
        if (menu[i].name == name) {
            // Shift items to the left to overwrite the item being removed
            for (int j = i; j < menuCount - 1; j++) {
                menu[j] = menu[j + 1];
            }
            menuCount--;
            cout << "Item '" << name << "' removed from the menu." << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Item not found in the menu." << endl;
    }
}

// Function to add an item to the cart (push operation)
void addToCart(string name) {
    if (top >= MAX_ITEMS - 1) {
        cout << "Cart is full." << endl;
        return;
    }

    // Search for the item by name in the menu
    for (int i = 0; i < menuCount; i++) {
        if (menu[i].name == name && menu[i].isAvailable) {
            cart[++top] = { menu[i].id, menu[i].name, menu[i].price };
            cout << name << " added to cart." << endl;
            return;
        }
    }

    // If the item is not found or not available
    cout << "Item not found or not available." << endl;
}

// Function to remove an item from the cart (pop operation)
void removeFromCart() {
    if (top == -1) {
        cout << "Cart is empty." << endl;
        return;
    }

    // Remove the top item
    cout << cart[top].name << " removed from cart." << endl;
    top--;
}

// Function to clear all items from the cart
void clearCart() {
    top = -1;
    cout << "Cart has been cleared." << endl;
}

// Function to display the items in the cart
void displayCart() {
    if (top == -1) {
        cout << "Cart is empty." << endl;
        return;
    }

    for (int i = top; i >= 0; i--) {
        cout << "ID: " << cart[i].id << " Name: " << cart[i].name
            << " Price: Rs" << cart[i].price << endl;
    }
}

// Function to enqueue an order
void enqueueOrder() {
    // Check if the cart is empty
    if (top == -1) {
        cout << "Cart is empty. Cannot place order." << endl;
        return;
    }

    // Check if the order queue is full
    if ((rear + 1) % MAX_ITEMS == front && rear != -1) {
        cout << "Order queue is full. Cannot place order." << endl;
        return;
    }

    // Calculate the total price of items in the cart
    double totalPrice = 0.0;
    for (int i = 0; i <= top; i++) {
        totalPrice += cart[i].price;
    }

    // If this is the first item being enqueued, set front to 0
    if (rear == -1) {
        rear = 0;
    }
    else {
        // Move rear to the next position in the circular queue
        rear = (rear + 1) % MAX_ITEMS;
    }

    // Create a new order and add it to the order queue



    orderQueue[rear].orderID = nextOrderID++;
    orderQueue[rear].totalPrice = totalPrice;
    orderQueue[rear].itemCount = top + 1;



    // Copy items from the cart to the order
    for (int i = 0; i <= top; i++) {
        orderQueue[rear].items[i] = cart[i];
    }

    // Display confirmation message
   // cout << "Order placed successfully. Order ID: " << orderQueue[rear].orderID << " Total Price: Rs" << totalPrice << endl; with order id
    cout << "Order placed successfully.  " << " Total Price: Rs" << totalPrice << endl;



    // Clear the cart after placing the order
    clearCart();
}

// Function to dequeue an order
void dequeueOrder() {
    if (rear == -1) {
        cout << "No orders to process." << endl;
        return;
    }

    Order order = orderQueue[front];
    if (front == rear) {
        front = 0;
        rear = -1;
    }
    else {
        front = (front + 1) % MAX_ITEMS;
    }

    cout << "Processing order ID: " << order.orderID << endl;
    cout << "Total Price: Rs" << order.totalPrice << endl;
    cout << "Number of items: " << order.itemCount << endl;
    for (int i = 0; i < order.itemCount; i++) {
        cout << "ID: " << order.items[i].id << " Name: " << order.items[i].name
            << " Price: Rs" << order.items[i].price << endl;
    }
}

// User interface for managing the menu, cart, and orders
void userInterface() {
    int choice, id;
    double price;
    bool isAvailable;
    string name;

    while (true) {
        cout << "\n1. Add Menu Item\n2. Display Menu\n3. Add Item to Cart\n4. Remove Item from Cart\n";
        cout << "5. Display Cart\n6. Clear Cart\n7. Place Order\n8. Process Orders\n9. Update Menu Item\n10. Remove Item from Menu\n11. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addItem();
            break;
        case 2:
            displayMenu();
            break;
        case 3:
            cout << "Enter the name of the item to add to cart: ";
            cin.ignore(); // Clear the input buffer to handle strings properly after integer input
            getline(cin, name); // Use getline to allow names with spaces
            addToCart(name);
            break;
        case 4:
            removeFromCart();
            break;
        case 5:
            displayCart();
            break;
        case 6:
            clearCart();
            break;
        case 7:
            enqueueOrder();
            break;
        case 8:
            dequeueOrder();
            break;


        case 9:
            cout << "Enter the ID of the item to update: ";
            cin >> id;
            cin.ignore();
            cout << "Enter new name: ";
            getline(cin, name);
            cout << "Enter new price: ";
            cin >> price;
            cout << "Is item available? (1 for yes, 0 for no): ";
            cin >> isAvailable;
            for (int i = 0; i < menuCount; i++) {
                if (menu[i].id == id) {
                    menu[i].name = name;
                    menu[i].price = price;
                    menu[i].isAvailable = isAvailable;
                    cout << "Item updated successfully." << endl;
                    break;
                }
            }
            break;
        case 10:
            cout << "Enter the name of the item to remove from the menu: ";
            cin.ignore();
            getline(cin, name);
            removeItem(name);
            break;
        case 11:
            cout << "Exiting program." << endl;
            return;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 11." << endl;
        }
    }
}

// Main function
int main() {
    cout << "Welcome to the MEAL MENU\n";
    userInterface();
    return 0;
}
