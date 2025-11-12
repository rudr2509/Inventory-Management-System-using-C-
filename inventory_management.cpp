#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Product {
protected:
    int id;
    string name;
    double price;

public:
    const string company;
    static int totalProducts;

    Product() : company("ABC Traders") {
        id = 0;
        name = "";
        price = 0.0;
    }

    Product(int i, string n, double p) : company("ABC Traders") {
        id = i;
        name = n;
        price = p;
        totalProducts++;
    }

    void setData() {
        cout << "Enter Product ID: ";
        cin >> id;
        cout << "Enter Product Name (one word): ";
        cin >> name;
        cout << "Enter Product Price: ";
        cin >> price;
    }

    void setData(int i, string n, double p) {
        id = i;
        name = n;
        price = p;
    }

    virtual void display() {
        cout << "\nID: " << id;
        cout << "\nName: " << name;
        cout << "\nPrice: " << price;
        cout << "\nCompany: " << company << endl;
    }

    int getID() { return id; }
    string getName() { return name; }
    double getPrice() { return price; }
};

int Product::totalProducts = 0;

class InventoryItem : public Product {
    int quantity;

public:
    InventoryItem() : Product() {
        quantity = 0;
    }

    InventoryItem(int i, string n, double p, int q) : Product(i, n, p) {
        quantity = q;
    }

    void setQuantity(int q) {
        quantity = q;
    }

    int getQuantity() {
        return quantity;
    }

    void display() {
        Product::display();
        cout << "Quantity: " << quantity << endl;
    }
};

class InventoryManager {
    void logError(string message) {
        ofstream errorFile("error_log.txt", ios::app);
        if (errorFile) {
            errorFile << message << endl;
            errorFile.close();
        }
    }

public:
    void addItem() {
        InventoryItem item;
        item.setData();
        int q;
        cout << "Enter Quantity: ";
        cin >> q;
        item.setQuantity(q);

        ofstream fout("inventory.txt", ios::app);
        if (!fout) {
            cout << "Error opening file for writing!\n";
            logError("Error: Unable to open inventory.txt while adding an item.");
            return;
        }

        fout << item.getID() << " " << item.getName() << " " << item.getPrice() << " " << item.getQuantity() << endl;
        fout.close();
        cout << "\nItem Added Successfully\n";
    }

    void viewAllItems() {
        ifstream fin("inventory.txt");
        if (!fin) {
            cout << "No Items Found\n";
            logError("Error: inventory.txt not found while viewing items.");
            return;
        }
        cout << "\n--- Inventory List ---\n";
        int id, qty;
        string name;
        double price;
        while (fin >> id >> name >> price >> qty) {
            cout << "ID: " << id << " | Name: " << name << " | Price: " << price << " | Quantity: " << qty << endl;
        }
        fin.close();
    }

    void searchItem() {
        ifstream fin("inventory.txt");
        if (!fin) {
            cout << "No Items Found\n";
            logError("Error: inventory.txt not found while searching item.");
            return;
        }

        int sid;
        cout << "Enter Product ID to Search: ";
        cin >> sid;

        int id, qty;
        string name;
        double price;
        bool found = false;

        while (fin >> id >> name >> price >> qty) {
            if (id == sid) {
                cout << "\nItem Found:\n";
                cout << "ID: " << id << "\nName: " << name << "\nPrice: " << price << "\nQuantity: " << qty << endl;
                found = true;
                break;
            }
        }
        fin.close();

        if (!found) {
            cout << "Item Not Found\n";
            logError("Error: Item with ID " + to_string(sid) + " not found during search.");
        }
    }

    void deleteItem() {
        ifstream fin("inventory.txt");
        ofstream temp("temp.txt");
        if (!fin || !temp) {
            cout << "File Error\n";
            logError("Error: Unable to open file while deleting item.");
            return;
        }

        int did;
        cout << "Enter Product ID to Delete: ";
        cin >> did;
        int id, qty;
        string name;
        double price;
        bool deleted = false;

        while (fin >> id >> name >> price >> qty) {
            if (id != did)
                temp << id << " " << name << " " << price << " " << qty << endl;
            else
                deleted = true;
        }

        fin.close();
        temp.close();
        remove("inventory.txt");
        rename("temp.txt", "inventory.txt");

        if (deleted) cout << "Item Deleted\n";
        else {
            cout << "Item Not Found\n";
            logError("Error: Item with ID " + to_string(did) + " not found during deletion.");
        }
    }
};

int main() {
    InventoryManager manager;
    int ch;
    do {
        cout << "\n===== INVENTORY MANAGEMENT SYSTEM =====\n";
        cout << "1. Add Item\n";
        cout << "2. View All Items\n";
        cout << "3. Search Item\n";
        cout << "4. Delete Item\n";
        cout << "5. View Total Products Created\n";
        cout << "0. Exit\n";
        cout << "Enter Choice: ";
        cin >> ch;

        if (ch == 1) manager.addItem();
        else if (ch == 2) manager.viewAllItems();
        else if (ch == 3) manager.searchItem();
        else if (ch == 4) manager.deleteItem();
        else if (ch == 5) cout << "Total Products Created: " << Product::totalProducts << endl;
        else if (ch == 0) cout << "Exiting...\n";
        else cout << "Invalid Choice\n";
    } while (ch != 0);
    return 0;
}
