#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>

// ============================================
// Menu Item Class
// ============================================

class MenuItem {
private:
    std::string name;      // Item name
    std::string description; // Item description
    bool vegetarian;       // Is it vegetarian?
    double price;          // Item price

public:
    MenuItem(const std::string& n, const std::string& desc, bool veg, double p)
        : name(n), description(desc), vegetarian(veg), price(p) {}

    // Getters
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    bool isVegetarian() const { return vegetarian; }
    double getPrice() const { return price; }

    // Display menu item
    void display() const {
        std::cout << name << " - $" << price
                  << (vegetarian ? " (V)" : "") << std::endl;
        std::cout << "  " << description << std::endl;
    }
};

// ============================================
// Iterator Interface
// ============================================

class MenuIterator {
public:
    virtual ~MenuIterator() {}
    virtual bool hasNext() const = 0;
    virtual MenuItem next() = 0;
};

// ============================================
// Pancake House Menu (uses std::vector)
// ============================================

class PancakeHouseMenu {
private:
    std::vector<MenuItem> menuItems;  // Dynamic array of items

public:
    PancakeHouseMenu() {
        // Initialize menu items
        addItem("K&B Pancake Breakfast",
                "Pancakes with scrambled eggs and toast",
                true, 2.99);
        addItem("Regular Pancake Breakfast",
                "Pancakes with fried eggs and sausage",
                false, 2.99);
        addItem("Blueberry Pancakes",
                "Pancakes made with fresh blueberries",
                true, 3.49);
        addItem("Waffles",
                "Waffles with your choice of blueberries or strawberries",
                true, 3.59);
    }

    // Add new item to menu
    void addItem(const std::string& name, const std::string& desc,
                 bool vegetarian, double price) {
        menuItems.push_back(MenuItem(name, desc, vegetarian, price));
    }

    // Create iterator for this menu
    std::shared_ptr<MenuIterator> createIterator();

    // Allow iterator to access private members
    friend class PancakeIterator;
};

/**
 * Iterator for PancakeHouseMenu
 * Iterates over std::vector storage
 */
class PancakeIterator : public MenuIterator {
private:
    const PancakeHouseMenu* menu;  // Reference to menu
    size_t position;               // Current position

public:
    PancakeIterator(const PancakeHouseMenu* m) : menu(m), position(0) {}

    bool hasNext() const override {
        return position < menu->menuItems.size();
    }

    MenuItem next() override {
        return menu->menuItems[position++];
    }
};

// Implementation
std::shared_ptr<MenuIterator> PancakeHouseMenu::createIterator() {
    return std::make_shared<PancakeIterator>(this);
}

// ============================================
// Diner Menu (uses fixed-size array)
// ============================================

const int MAX_ITEMS = 6;  // Fixed maximum number of items

class DinerMenu {
private:
    std::array<MenuItem*, MAX_ITEMS> menuItems;  // Fixed array
    int numberOfItems;                           // Current count

public:
    DinerMenu() : numberOfItems(0) {
        // Initialize with nullptr
        menuItems.fill(nullptr);

        // Add menu items
        addItem("Vegetarian BLT",
                "Bacon with lettuce & tomato on whole wheat",
                true, 2.99);
        addItem("BLT",
                "Bacon with lettuce & tomato on whole wheat",
                false, 2.99);
        addItem("Soup of the day",
                "Soup with a side of potato salad",
                false, 3.29);
        addItem("Hotdog",
                "A hotdog with sauerkraut, relish and cheese",
                false, 3.05);
    }

    ~DinerMenu() {
        // Clean up dynamically allocated items
        for (int i = 0; i < numberOfItems; i++) {
            delete menuItems[i];
        }
    }

    // Add new item (if space available)
    void addItem(const std::string& name, const std::string& desc,
                 bool vegetarian, double price) {
        if (numberOfItems >= MAX_ITEMS) {
            std::cout << "Sorry, menu is full!" << std::endl;
            return;
        }
        menuItems[numberOfItems] = new MenuItem(name, desc, vegetarian, price);
        numberOfItems++;
    }

    // Create iterator for this menu
    std::shared_ptr<MenuIterator> createIterator();

    // Allow iterator to access private members
    friend class DinerIterator;
};

/**
 * Iterator for DinerMenu
 * Iterates over fixed array storage
 */
class DinerIterator : public MenuIterator {
private:
    const DinerMenu* menu;  // Reference to menu
    int position;           // Current position

public:
    DinerIterator(const DinerMenu* m) : menu(m), position(0) {}

    bool hasNext() const override {
        // Check position and ensure item exists (not nullptr)
        return position < menu->numberOfItems &&
               menu->menuItems[position] != nullptr;
    }

    MenuItem next() override {
        MenuItem item = *menu->menuItems[position];
        position++;
        return item;
    }
};

// Implementation
std::shared_ptr<MenuIterator> DinerMenu::createIterator() {
    return std::make_shared<DinerIterator>(this);
}

// ============================================
// Waitress - Uses iterators uniformly
// ============================================

class Waitress {
private:
    PancakeHouseMenu* pancakeMenu;  // Breakfast menu
    DinerMenu* dinerMenu;           // Lunch menu

public:
    Waitress(PancakeHouseMenu* pm, DinerMenu* dm)
        : pancakeMenu(pm), dinerMenu(dm) {}

    // Print all menus using iterators
    void printMenu() {
        auto pancakeIterator = pancakeMenu->createIterator();
        auto dinerIterator = dinerMenu->createIterator();

        std::cout << "\n======= BREAKFAST MENU =======" << std::endl;
        printMenu(pancakeIterator);

        std::cout << "\n======== LUNCH MENU ==========" << std::endl;
        printMenu(dinerIterator);
    }

    // Print vegetarian menu only
    void printVegetarianMenu() {
        std::cout << "\n==== VEGETARIAN MENU ====" << std::endl;

        auto pancakeIterator = pancakeMenu->createIterator();
        std::cout << "\nBREAKFAST:" << std::endl;
        printVegetarianMenu(pancakeIterator);

        auto dinerIterator = dinerMenu->createIterator();
        std::cout << "\nLUNCH:" << std::endl;
        printVegetarianMenu(dinerIterator);
    }

private:
    // Helper: print all items using any iterator
    void printMenu(std::shared_ptr<MenuIterator> iterator) {
        while (iterator->hasNext()) {
            MenuItem item = iterator->next();
            std::cout << std::endl;
            item.display();
        }
    }

    // Helper: print only vegetarian items
    void printVegetarianMenu(std::shared_ptr<MenuIterator> iterator) {
        while (iterator->hasNext()) {
            MenuItem item = iterator->next();
            if (item.isVegetarian()) {
                std::cout << std::endl;
                item.display();
            }
        }
    }
};

// ============================================
// Main - Test the Restaurant System
// ============================================

int main() {
    // Create menus with different internal storage
    PancakeHouseMenu pancakeMenu;  // Uses vector
    DinerMenu dinerMenu;           // Uses array

    // Create waitress who uses both menus
    Waitress waitress(&pancakeMenu, &dinerMenu);

    std::cout << "===== RESTAURANT MENU SYSTEM =====" << std::endl;

    // Print all menus (waitress doesn't need to know storage details!)
    waitress.printMenu();

    // Print only vegetarian options
    waitress.printVegetarianMenu();

    return 0;
}
