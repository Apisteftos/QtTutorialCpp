#include <iostream>
#include <string>
#include <memory>
using namespace std;

// ============================================
// ABSTRACT PRODUCTS
// These define interfaces for product types
// ============================================

/**
 * Abstract Product A: Button
 * Each platform will implement its own button
 */
class AbstractButton {
public:
    virtual ~AbstractButton() {}
    virtual void paint() const = 0;
    virtual void click() const = 0;
};

/**
 * Abstract Product B: Checkbox
 * Each platform will implement its own checkbox
 */
class AbstractCheckbox {
public:
    virtual ~AbstractCheckbox() {}
    virtual void paint() const = 0;
    virtual void toggle() const = 0;

    /**
     * Product B can also interact with Product A
     * This shows how products from same family work together
     */
    virtual void interactWith(const AbstractButton& button) const = 0;
};

// ============================================
// CONCRETE PRODUCTS - WINDOWS FAMILY
// ============================================

class WindowsButton : public AbstractButton {
public:
    void paint() const override {
        cout << "🪟 Rendering button in Windows style (flat, blue)\n";
    }

    void click() const override {
        cout << "🪟 Windows button clicked with left mouse button\n";
    }
};

class WindowsCheckbox : public AbstractCheckbox {
public:
    void paint() const override {
        cout << "🪟 Rendering checkbox in Windows style (square, blue checkmark)\n";
    }

    void toggle() const override {
        cout << "🪟 Windows checkbox toggled\n";
    }

    void interactWith(const AbstractButton& button) const override {
        cout << "🪟 Windows checkbox interacts with ";
        button.paint();
    }
};

// ============================================
// CONCRETE PRODUCTS - MAC FAMILY
// ============================================

class MacButton : public AbstractButton {
public:
    void paint() const override {
        cout << "🍎 Rendering button in macOS style (rounded, gray)\n";
    }

    void click() const override {
        cout << "🍎 macOS button clicked with trackpad gesture\n";
    }
};

class MacCheckbox : public AbstractCheckbox {
public:
    void paint() const override {
        cout << "🍎 Rendering checkbox in macOS style (rounded square, white checkmark)\n";
    }

    void toggle() const override {
        cout << "🍎 macOS checkbox toggled with animation\n";
    }

    void interactWith(const AbstractButton& button) const override {
        cout << "🍎 macOS checkbox interacts with ";
        button.paint();
    }
};

// ============================================
// CONCRETE PRODUCTS - LINUX FAMILY
// ============================================

class LinuxButton : public AbstractButton {
public:
    void paint() const override {
        cout << "🐧 Rendering button in Linux/GTK style (gradient, themed)\n";
    }

    void click() const override {
        cout << "🐧 Linux button clicked\n";
    }
};

class LinuxCheckbox : public AbstractCheckbox {
public:
    void paint() const override {
        cout << "🐧 Rendering checkbox in Linux/GTK style (themed)\n";
    }

    void toggle() const override {
        cout << "🐧 Linux checkbox toggled\n";
    }

    void interactWith(const AbstractButton& button) const override {
        cout << "🐧 Linux checkbox interacts with ";
        button.paint();
    }
};

// ============================================
// ABSTRACT FACTORY
// Declares methods for creating abstract products
// ============================================

/**
 * Abstract Factory interface
 * Declares creation methods for each product type
 */
class GUIFactory {
public:
    virtual ~GUIFactory() {}
    virtual unique_ptr<AbstractButton> createButton() const = 0;
    virtual unique_ptr<AbstractCheckbox> createCheckbox() const = 0;
};

// ============================================
// CONCRETE FACTORIES
// Each factory creates products of one family
// ============================================

/**
 * Windows Factory
 * Creates Windows-style UI components
 */
class WindowsFactory : public GUIFactory {
public:
    unique_ptr<AbstractButton> createButton() const override {
        return make_unique<WindowsButton>();
    }

    unique_ptr<AbstractCheckbox> createCheckbox() const override {
        return make_unique<WindowsCheckbox>();
    }
};

/**
 * Mac Factory
 * Creates macOS-style UI components
 */
class MacFactory : public GUIFactory {
public:
    unique_ptr<AbstractButton> createButton() const override {
        return make_unique<MacButton>();
    }

    unique_ptr<AbstractCheckbox> createCheckbox() const override {
        return make_unique<MacCheckbox>();
    }
};

/**
 * Linux Factory
 * Creates Linux-style UI components
 */
class LinuxFactory : public GUIFactory {
public:
    unique_ptr<AbstractButton> createButton() const override {
        return make_unique<LinuxButton>();
    }

    unique_ptr<AbstractCheckbox> createCheckbox() const override {
        return make_unique<LinuxCheckbox>();
    }
};

// ============================================
// CLIENT CODE
// Works with factories and products through abstract interfaces
// ============================================

/**
 * Client code works with factories and products only through abstract types.
 * This allows you to pass any factory/product to the client code without breaking it.
 */
class Application {
private:
    unique_ptr<AbstractButton> button;
    unique_ptr<AbstractCheckbox> checkbox;

public:
    Application(const GUIFactory& factory) {
        button = factory.createButton();
        checkbox = factory.createCheckbox();
    }

    void paint() {
        cout << "\n--- Painting UI ---\n";
        button->paint();
        checkbox->paint();
    }

    void handleUserInput() {
        cout << "\n--- Handling User Input ---\n";
        button->click();
        checkbox->toggle();
        checkbox->interactWith(*button);
    }
};

/**
 * Application configuration
 * In real app, this would read from config file or environment
 */
string getOperatingSystem() {
    // In real application, you would detect the OS
    // For demo, we'll return different values
    return "Windows";  // or "Mac" or "Linux"
}

unique_ptr<GUIFactory> createFactory(const string& os) {
    if (os == "Windows") {
        cout << "🪟 Initializing Windows Factory\n";
        return make_unique<WindowsFactory>();
    } else if (os == "Mac") {
        cout << "🍎 Initializing macOS Factory\n";
        return make_unique<MacFactory>();
    } else if (os == "Linux") {
        cout << "🐧 Initializing Linux Factory\n";
        return make_unique<LinuxFactory>();
    }
    return nullptr;
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "ABSTRACT FACTORY PATTERN DEMO\n";
    cout << "========================================\n\n";

    // ============================================
    // Example 1: Windows Application
    // ============================================
    cout << "=== EXAMPLE 1: Windows Application ===\n";
    {
        WindowsFactory windowsFactory;
        Application app(windowsFactory);
        app.paint();
        app.handleUserInput();
    }

    // ============================================
    // Example 2: macOS Application
    // ============================================
    cout << "\n=== EXAMPLE 2: macOS Application ===\n";
    {
        MacFactory macFactory;
        Application app(macFactory);
        app.paint();
        app.handleUserInput();
    }

    // ============================================
    // Example 3: Linux Application
    // ============================================
    cout << "\n=== EXAMPLE 3: Linux Application ===\n";
    {
        LinuxFactory linuxFactory;
        Application app(linuxFactory);
        app.paint();
        app.handleUserInput();
    }

    // ============================================
    // Example 4: Dynamic Factory Selection
    // ============================================
    cout << "\n=== EXAMPLE 4: Dynamic OS Detection ===\n";
    {
        string os = getOperatingSystem();
        auto factory = createFactory(os);
        if (factory) {
            Application app(*factory);
            app.paint();
            app.handleUserInput();
        }
    }

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF ABSTRACT FACTORY\n";
    cout << "========================================\n";
    cout << "✅ Products from one family are guaranteed to work together\n";
    cout << "✅ Easy to add new product families (just add new factory)\n";
    cout << "✅ Client code doesn't depend on concrete classes\n";
    cout << "✅ Single Responsibility: Product creation code is separate\n";
    cout << "✅ Open/Closed: Can add new families without changing client\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE ABSTRACT FACTORY\n";
    cout << "========================================\n";
    cout << "1. System needs multiple families of related products\n";
    cout << "2. You want to enforce that products from same family are used together\n";
    cout << "3. You want to hide product implementations from client\n";
    cout << "4. You need to switch between product families at runtime\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Cross-platform UI frameworks (Qt, wxWidgets)\n";
    cout << "• Database abstraction layers (ADO.NET, JDBC)\n";
    cout << "• Game themes (Medieval, Sci-Fi, Fantasy)\n";
    cout << "• Document formats (PDF, Word, HTML)\n";
    cout << "• Operating system services (File I/O, Networking)\n";

    return 0;
}
