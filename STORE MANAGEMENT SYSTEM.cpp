#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

// Product Classes
class Product {
protected:
    int id;
    string name;
    double price;
    int quantity;

public:
    Product(int id = 0, string name = "", double price = 0.0, int quantity = 0)
        : id(id), name(name), price(price), quantity(quantity) {}

    virtual ~Product() {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void setName(string newName) { name = newName; }
    void setPrice(double newPrice) { price = newPrice; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }

    virtual void display() const {
        cout << "ID: " << id
             << " | Name: " << name
             << " | Price: Rs." << price
             << " | Quantity: " << quantity;
    }

    virtual double calculateTotal(int qty) const { return price * qty; }
};

class DiscountedProduct : public Product {
    double discountPercent;

public:
    DiscountedProduct(int id, string name, double price, int quantity, double discount)
        : Product(id, name, price, quantity), discountPercent(discount) {}

    void display() const override {
        cout << "ID: " << id
             << " | Name: " << name
             << " | Price: Rs." << price
             << " | Quantity: " << quantity
             << " | Discount: " << discountPercent << "%";
    }

    double calculateTotal(int qty) const override {
        double total = price * qty;
        return total - (total * discountPercent / 100);
    }
};

// Inventory
class Inventory {
    vector<Product*> products;

public:
    ~Inventory() {
        for (auto p : products)
            delete p;
    }

    void addProduct(Product* p) {
        products.push_back(p);
        cout << "Product added successfully!\n";
    }

    void displayAll() const {
        if (products.empty()) {
            cout << "No products in inventory.\n";
            return;
        }
        cout << "\n=== PRODUCTS ===\n";
        for (auto p : products) {
            p->display();
            cout << endl;
        }
    }

    Product* findProductById(int id) {
        for (auto p : products)
            if (p->getId() == id)
                return p;
        return nullptr;
    }

    void updateProduct(int id, string name, double price, int qty) {
        Product* p = findProductById(id);
        if (p) {
            p->setName(name);
            p->setPrice(price);
            p->setQuantity(qty);
            cout << "Product updated successfully!\n";
        } else {
            cout << "Product not found!\n";
        }
    }

    void deleteProduct(int id) {
        for (auto it = products.begin(); it != products.end(); ++it) {
            if ((*it)->getId() == id) {
                delete *it;
                products.erase(it);
                cout << "Product deleted successfully!\n";
                return;
            }
        }
        cout << "Product not found!\n";
    }
};

// Bill Item
class BillItem {
    Product* product;
    int quantity;

public:
    BillItem(Product* p, int qty) : product(p), quantity(qty) {}
    double getTotal() const { return product ? product->calculateTotal(quantity) : 0.0; }

    void display() const {
        cout << product->getName() << " x " << quantity
             << " = Rs." << fixed << setprecision(2) << getTotal() << endl;
    }
};

// Bill
class Bill {
    vector<BillItem> items;
    double taxRate;

public:
    Bill(double tax = 5.0) : taxRate(tax) {}

    void addProduct(Product* p, int qty = 1) {
        items.push_back(BillItem(p, qty));
    }

    double getSubtotal() const {
        double total = 0;
        for (auto i : items)
            total += i.getTotal();
        return total;
    }

    double getTax() const { return getSubtotal() * taxRate / 100; }
    double getGrandTotal() const { return getSubtotal() + getTax(); }

    void display() const {
        if (items.empty()) {
            cout << "No items in the bill!\n";
            return;
        }
        cout << "\n=== CUSTOMER BILL ===\n";
        for (auto i : items)
            i.display();

        cout << "Subtotal: Rs." << getSubtotal() << "\n";
        cout << "GST (" << taxRate << "%): Rs." << getTax() << "\n";
        cout << "Grand Total: Rs." << getGrandTotal() << "\n";
    }

    void clear() {
        items.clear();
        cout << "Bill cleared!\n";
    }
};

// Main Menu
void mainMenu(Inventory& inventory) {
    int choice;
    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Add Product\n";
        cout << "2. Update Product\n";
        cout << "3. View Products\n";
        cout << "4. Delete Product\n";
        cout << "5. Customer Billing\n";
        cout << "6. Exit\n";
        cout << "Choose option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int id, qty, type;
                string name;
                double price, discount;
                cout << "Enter ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Price: Rs.";
                cin >> price;
                cout << "Enter Quantity: ";
                cin >> qty;
                cout << "Type (1-Regular, 2-Discounted): ";
                cin >> type;
                if (type == 2) {
                    cout << "Enter Discount %: ";
                    cin >> discount;
                    inventory.addProduct(new DiscountedProduct(id, name, price, qty, discount));
                } else
                    inventory.addProduct(new Product(id, name, price, qty));
                break;
            }
            case 2: {
                int id, qty;
                string name;
                double price;
                cout << "Enter Product ID to update: ";
                cin >> id;
                cin.ignore();
                cout << "Enter New Name: ";
                getline(cin, name);
                cout << "Enter New Price: Rs.";
                cin >> price;
                cout << "Enter New Quantity: ";
                cin >> qty;
                inventory.updateProduct(id, name, price, qty);
                break;
            }
            case 3:
                inventory.displayAll();
                break;
            case 4: {
                int id;
                cout << "Enter Product ID to delete: ";
                cin >> id;
                inventory.deleteProduct(id);
                break;
            }
            case 5: {
                Bill bill;
                int subChoice;
                do {
                    cout << "\n=== CUSTOMER BILLING MENU ===\n";
                    cout << "1. View Products\n";
                    cout << "2. Add Product to Bill\n";
                    cout << "3. View Bill\n";
                    cout << "4. Clear Bill\n";
                    cout << "5. Back to Main Menu\n";
                    cout << "Choose option: ";
                    cin >> subChoice;

                    switch (subChoice) {
                        case 1:
                            inventory.displayAll();
                            break;
                        case 2: {
                            int id, qty;
                            cout << "Enter Product ID: ";
                            cin >> id;
                            Product* p = inventory.findProductById(id);
                            if (!p) {
                                cout << "Product not found!\n";
                                break;
                            }
                            cout << "Enter Quantity: ";
                            cin >> qty;
                            if (qty > p->getQuantity()) {
                                cout << "Insufficient stock!\n";
                                break;
                            }
                            bill.addProduct(p, qty);
                            cout << "Product added to bill!\n";
                            break;
                        }
                        case 3:
                            bill.display();
                            break;
                        case 4:
                            bill.clear();
                            break;
                        case 5:
                            cout << "Returning to Main Menu...\n";
                            break;
                        default:
                            cout << "Invalid choice!\n";
                    }
                } while (subChoice != 5);
                break;
            }
            case 6:
                cout << "Exiting... Goodbye!\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 6);
}

int main() {
    Inventory inventory;
    mainMenu(inventory);
    return 0;
}
