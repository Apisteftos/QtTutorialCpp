#include <string>
#include <memory>
#include <print>

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
        std::println("🪟 Rendering button in Windows style (flat, blue)");
    }

    void click() const override {
        std::println("🪟 Windows button clicked with left mouse button");
    }
};

class WindowsCheckbox : public AbstractCheckbox {
public:
    void paint() const override {
        std::println("🪟 Rendering checkbox in Windows style (square, blue checkmark)");
    }

    void toggle() const override {
        std::println("🪟 Windows checkbox toggled");
    }

    void interactWith(const AbstractButton& button) const override {
        std::print("🪟 Windows checkbox interacts with ");
        button.paint();
    }
};

// ============================================
// CONCRETE PRODUCTS - MAC FAMILY
// ============================================

class MacButton : public AbstractButton {
public:
    void paint() const override {
        std::println("🍎 Rendering button in macOS style (rounded, gray)");
    }

    void click() const override {
        std::println("🍎 macOS button clicked with trackpad gesture");
    }
};

class MacCheckbox : public AbstractCheckbox {
public:
    void paint() const override {
        std::println("🍎 Rendering checkbox in macOS style (rounded square, white checkmark)");
    }

    void toggle() const override {
        std::println("🍎 macOS checkbox toggled with animation");
    }

    void interactWith(const AbstractButton& button) const override {
        std::print("🍎 macOS checkbox interacts with ");
        button.paint();
    }
};

// ============================================
// CONCRETE PRODUCTS - LINUX FAMILY
// ============================================

class LinuxButton : public AbstractButton {
public:
    void paint() const override {
        std::println("🐧 Rendering button in Linux/GTK style (gradient, themed)");
    }

    void click() const override {
        std::println("🐧 Linux button clicked");
    }
};

class LinuxCheckbox : public AbstractCheckbox {
public:
    void paint() const override {
        std::println("🐧 Rendering checkbox in Linux/GTK style (themed)");
    }

    void toggle() const override {
        std::println("🐧 Linux checkbox toggled");
    }

    void interactWith(const AbstractButton& button) const override {
        std::print("🐧 Linux checkbox interacts with ");
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
    virtual std::unique_ptr<AbstractButton> createButton() const = 0;
    virtual std::unique_ptr<AbstractCheckbox> createCheckbox() const = 0;
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
    std::unique_ptr<AbstractButton> createButton() const override {
        return std::make_unique<WindowsButton>();
    }

    std::unique_ptr<AbstractCheckbox> createCheckbox() const override {
        return std::make_unique<WindowsCheckbox>();
    }
};

/**
 * Mac Factory
 * Creates macOS-style UI components
 */
class MacFactory : public GUIFactory {
public:
    std::unique_ptr<AbstractButton> createButton() const override {
        return std::make_unique<MacButton>();
    }

    std::unique_ptr<AbstractCheckbox> createCheckbox() const override {
        return std::make_unique<MacCheckbox>();
    }
};

/**
 * Linux Factory
 * Creates Linux-style UI components
 */
class LinuxFactory : public GUIFactory {
public:
    std::unique_ptr<AbstractButton> createButton() const override {
        return std::make_unique<LinuxButton>();
    }

    std::unique_ptr<AbstractCheckbox> createCheckbox() const override {
        return std::make_unique<LinuxCheckbox>();
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
    std::unique_ptr<AbstractButton> button;
    std::unique_ptr<AbstractCheckbox> checkbox;

public:
    Application(const GUIFactory& factory) {
        button = factory.createButton();
        checkbox = factory.createCheckbox();
    }

    void paint() {
        std::println("");
        std::println("--- Painting UI ---");
        button->paint();
        checkbox->paint();
    }

    void handleUserInput() {
        std::println("");
        std::println("--- Handling User Input ---");
        button->click();
        checkbox->toggle();
        checkbox->interactWith(*button);
    }
};

/**
 * Application configuration
 * In real app, this would read from config file or environment
 */
std::string getOperatingSystem() {
    // In real application, you would detect the OS
    // For demo, we'll return different values
    return "Windows";  // or "Mac" or "Linux"
}

std::unique_ptr<GUIFactory> createFactory(const std::string& os) {
    if (os == "Windows") {
        std::println("🪟 Initializing Windows Factory");
        return std::make_unique<WindowsFactory>();
    } else if (os == "Mac") {
        std::println("🍎 Initializing macOS Factory");
        return std::make_unique<MacFactory>();
    } else if (os == "Linux") {
        std::println("🐧 Initializing Linux Factory");
        return std::make_unique<LinuxFactory>();
    }
    return nullptr;
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("ABSTRACT FACTORY PATTERN DEMO");
    std::println("========================================");
    std::println("");

    // ============================================
    // Example 1: Windows Application
    // ============================================
    std::println("=== EXAMPLE 1: Windows Application ===");
    {
        WindowsFactory windowsFactory;
        Application app(windowsFactory);
        app.paint();
        app.handleUserInput();
    }

    // ============================================
    // Example 2: macOS Application
    // ============================================
    std::println("");
    std::println("=== EXAMPLE 2: macOS Application ===");
    {
        MacFactory macFactory;
        Application app(macFactory);
        app.paint();
        app.handleUserInput();
    }

    // ============================================
    // Example 3: Linux Application
    // ============================================
    std::println("");
    std::println("=== EXAMPLE 3: Linux Application ===");
    {
        LinuxFactory linuxFactory;
        Application app(linuxFactory);
        app.paint();
        app.handleUserInput();
    }

    // ============================================
    // Example 4: Dynamic Factory Selection
    // ============================================
    std::println("");
    std::println("=== EXAMPLE 4: Dynamic OS Detection ===");
    {
        std::string os = getOperatingSystem();
        auto factory = createFactory(os);
        if (factory) {
            Application app(*factory);
            app.paint();
            app.handleUserInput();
        }
    }

    return 0;
}
