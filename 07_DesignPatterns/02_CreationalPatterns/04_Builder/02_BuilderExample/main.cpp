#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ============================================
// PRODUCT - The complex object being built
// ============================================

/**
 * Computer - The complex product
 * Has many optional components
 */
class Computer {
private:
    // Required components
    string cpu;
    string ram;
    string storage;

    // Optional components
    string gpu;
    string motherboard;
    string powerSupply;
    string coolingSystem;
    string caseType;
    vector<string> peripherals;
    bool hasWifi;
    bool hasBluetooth;
    bool hasRGB;

public:
    Computer() : hasWifi(false), hasBluetooth(false), hasRGB(false) {}

    // Setters for all components
    void setCPU(const string& c) { cpu = c; }
    void setRAM(const string& r) { ram = r; }
    void setStorage(const string& s) { storage = s; }
    void setGPU(const string& g) { gpu = g; }
    void setMotherboard(const string& m) { motherboard = m; }
    void setPowerSupply(const string& p) { powerSupply = p; }
    void setCoolingSystem(const string& c) { coolingSystem = c; }
    void setCaseType(const string& c) { caseType = c; }
    void addPeripheral(const string& p) { peripherals.push_back(p); }
    void setWifi(bool w) { hasWifi = w; }
    void setBluetooth(bool b) { hasBluetooth = b; }
    void setRGB(bool r) { hasRGB = r; }

    // Display the computer configuration
    void showSpecs() const {
        cout << "\n╔════════════════════════════════════╗\n";
        cout << "║      COMPUTER SPECIFICATIONS       ║\n";
        cout << "╚════════════════════════════════════╝\n";

        cout << "CPU:            " << cpu << "\n";
        cout << "RAM:            " << ram << "\n";
        cout << "Storage:        " << storage << "\n";

        if (!gpu.empty())
            cout << "GPU:            " << gpu << "\n";
        if (!motherboard.empty())
            cout << "Motherboard:    " << motherboard << "\n";
        if (!powerSupply.empty())
            cout << "Power Supply:   " << powerSupply << "\n";
        if (!coolingSystem.empty())
            cout << "Cooling:        " << coolingSystem << "\n";
        if (!caseType.empty())
            cout << "Case:           " << caseType << "\n";

        cout << "WiFi:           " << (hasWifi ? "Yes" : "No") << "\n";
        cout << "Bluetooth:      " << (hasBluetooth ? "Yes" : "No") << "\n";
        cout << "RGB Lighting:   " << (hasRGB ? "Yes" : "No") << "\n";

        if (!peripherals.empty()) {
            cout << "Peripherals:    ";
            for (size_t i = 0; i < peripherals.size(); i++) {
                cout << peripherals[i];
                if (i < peripherals.size() - 1) cout << ", ";
            }
            cout << "\n";
        }
        cout << "────────────────────────────────────\n";
    }
};

// ============================================
// BUILDER INTERFACE
// Defines steps for building a product
// ============================================

/**
 * Abstract Builder interface
 * Specifies methods for creating different parts of a Computer
 */
class ComputerBuilder {
protected:
    unique_ptr<Computer> computer;

public:
    ComputerBuilder() { reset(); }
    virtual ~ComputerBuilder() {}

    void reset() { computer = make_unique<Computer>(); }

    // Building steps - pure virtual (must be implemented)
    virtual void buildCPU() = 0;
    virtual void buildRAM() = 0;
    virtual void buildStorage() = 0;
    virtual void buildGPU() = 0;
    virtual void buildMotherboard() = 0;
    virtual void buildPowerSupply() = 0;
    virtual void buildCooling() = 0;
    virtual void buildCase() = 0;
    virtual void buildWifi() = 0;
    virtual void buildBluetooth() = 0;
    virtual void buildRGB() = 0;
    virtual void buildPeripherals() = 0;

    // Return the final product
    unique_ptr<Computer> getResult() {
        return move(computer);
    }
};

// ============================================
// CONCRETE BUILDER 1 - Gaming PC
// ============================================

class GamingPCBuilder : public ComputerBuilder {
public:
    void buildCPU() override {
        computer->setCPU("Intel Core i9-13900K (24-core, 5.8GHz)");
    }

    void buildRAM() override {
        computer->setRAM("64GB DDR5 6000MHz RGB");
    }

    void buildStorage() override {
        computer->setStorage("2TB NVMe SSD + 4TB HDD");
    }

    void buildGPU() override {
        computer->setGPU("NVIDIA RTX 4090 24GB");
    }

    void buildMotherboard() override {
        computer->setMotherboard("ASUS ROG Maximus Z790");
    }

    void buildPowerSupply() override {
        computer->setPowerSupply("1000W 80+ Platinum Modular");
    }

    void buildCooling() override {
        computer->setCoolingSystem("360mm AIO Liquid Cooling");
    }

    void buildCase() override {
        computer->setCaseType("Tempered Glass RGB Gaming Case");
    }

    void buildWifi() override {
        computer->setWifi(true);
    }

    void buildBluetooth() override {
        computer->setBluetooth(true);
    }

    void buildRGB() override {
        computer->setRGB(true);
    }

    void buildPeripherals() override {
        computer->addPeripheral("RGB Mechanical Keyboard");
        computer->addPeripheral("Gaming Mouse 16000 DPI");
        computer->addPeripheral("Gaming Headset 7.1");
        computer->addPeripheral("XXL RGB Mousepad");
    }
};

// ============================================
// CONCRETE BUILDER 2 - Office PC
// ============================================

class OfficePCBuilder : public ComputerBuilder {
public:
    void buildCPU() override {
        computer->setCPU("Intel Core i5-13400 (6-core, 4.6GHz)");
    }

    void buildRAM() override {
        computer->setRAM("16GB DDR4 3200MHz");
    }

    void buildStorage() override {
        computer->setStorage("512GB NVMe SSD");
    }

    void buildGPU() override {
        computer->setGPU("Integrated Intel UHD Graphics");
    }

    void buildMotherboard() override {
        computer->setMotherboard("Business-class B760 Chipset");
    }

    void buildPowerSupply() override {
        computer->setPowerSupply("500W 80+ Bronze");
    }

    void buildCooling() override {
        computer->setCoolingSystem("Stock CPU Cooler");
    }

    void buildCase() override {
        computer->setCaseType("Compact Office Tower");
    }

    void buildWifi() override {
        computer->setWifi(true);
    }

    void buildBluetooth() override {
        computer->setBluetooth(true);
    }

    void buildRGB() override {
        computer->setRGB(false);  // No RGB for office
    }

    void buildPeripherals() override {
        computer->addPeripheral("Standard Keyboard");
        computer->addPeripheral("Wireless Mouse");
    }
};

// ============================================
// CONCRETE BUILDER 3 - Budget PC
// ============================================

class BudgetPCBuilder : public ComputerBuilder {
public:
    void buildCPU() override {
        computer->setCPU("AMD Ryzen 5 5600G (6-core, 4.4GHz)");
    }

    void buildRAM() override {
        computer->setRAM("8GB DDR4 3200MHz");
    }

    void buildStorage() override {
        computer->setStorage("256GB SATA SSD");
    }

    void buildGPU() override {
        computer->setGPU("Integrated Radeon Graphics");
    }

    void buildMotherboard() override {
        computer->setMotherboard("MSI B450M Micro-ATX");
    }

    void buildPowerSupply() override {
        computer->setPowerSupply("450W 80+ Standard");
    }

    void buildCooling() override {
        computer->setCoolingSystem("Stock AMD Cooler");
    }

    void buildCase() override {
        computer->setCaseType("Basic Micro-ATX Case");
    }

    void buildWifi() override {
        computer->setWifi(false);  // Use ethernet
    }

    void buildBluetooth() override {
        computer->setBluetooth(false);
    }

    void buildRGB() override {
        computer->setRGB(false);
    }

    void buildPeripherals() override {
        computer->addPeripheral("Basic Keyboard & Mouse Combo");
    }
};

// ============================================
// CONCRETE BUILDER 4 - Workstation
// ============================================

class WorkstationBuilder : public ComputerBuilder {
public:
    void buildCPU() override {
        computer->setCPU("AMD Threadripper 7970X (32-core, 5.3GHz)");
    }

    void buildRAM() override {
        computer->setRAM("128GB DDR5 ECC Memory");
    }

    void buildStorage() override {
        computer->setStorage("4TB NVMe SSD (RAID 0) + 8TB HDD");
    }

    void buildGPU() override {
        computer->setGPU("NVIDIA RTX A6000 48GB (Workstation GPU)");
    }

    void buildMotherboard() override {
        computer->setMotherboard("ASUS Pro WS TRX50-SAGE WIFI");
    }

    void buildPowerSupply() override {
        computer->setPowerSupply("1600W 80+ Titanium");
    }

    void buildCooling() override {
        computer->setCoolingSystem("Custom Water Cooling Loop");
    }

    void buildCase() override {
        computer->setCaseType("Professional Full Tower Workstation");
    }

    void buildWifi() override {
        computer->setWifi(true);
    }

    void buildBluetooth() override {
        computer->setBluetooth(true);
    }

    void buildRGB() override {
        computer->setRGB(false);  // Professional, no RGB
    }

    void buildPeripherals() override {
        computer->addPeripheral("Mechanical Keyboard");
        computer->addPeripheral("Precision Mouse");
        computer->addPeripheral("Graphics Tablet");
        computer->addPeripheral("Color Calibrated Monitor");
    }
};

// ============================================
// DIRECTOR
// Defines the order of building steps
// ============================================

/**
 * Director class controls the building process
 * It knows which building steps to execute and in what order
 */
class ComputerDirector {
private:
    ComputerBuilder* builder;

public:
    void setBuilder(ComputerBuilder* b) {
        builder = b;
    }

    // Build a minimal computer (only required parts)
    void buildMinimalComputer() {
        builder->buildCPU();
        builder->buildRAM();
        builder->buildStorage();
    }

    // Build a full-featured computer (all parts)
    void buildFullComputer() {
        builder->buildCPU();
        builder->buildRAM();
        builder->buildStorage();
        builder->buildGPU();
        builder->buildMotherboard();
        builder->buildPowerSupply();
        builder->buildCooling();
        builder->buildCase();
        builder->buildWifi();
        builder->buildBluetooth();
        builder->buildRGB();
        builder->buildPeripherals();
    }

    // Custom build process
    void buildCustomComputer() {
        builder->buildCPU();
        builder->buildRAM();
        builder->buildStorage();
        builder->buildGPU();
        builder->buildPeripherals();
    }
};

// ============================================
// FLUENT BUILDER (Alternative Approach)
// Method chaining for easier building
// ============================================

class FluentComputerBuilder {
private:
    unique_ptr<Computer> computer;

public:
    FluentComputerBuilder() {
        computer = make_unique<Computer>();
    }

    FluentComputerBuilder& setCPU(const string& cpu) {
        computer->setCPU(cpu);
        return *this;
    }

    FluentComputerBuilder& setRAM(const string& ram) {
        computer->setRAM(ram);
        return *this;
    }

    FluentComputerBuilder& setStorage(const string& storage) {
        computer->setStorage(storage);
        return *this;
    }

    FluentComputerBuilder& setGPU(const string& gpu) {
        computer->setGPU(gpu);
        return *this;
    }

    FluentComputerBuilder& addPeripheral(const string& peripheral) {
        computer->addPeripheral(peripheral);
        return *this;
    }

    FluentComputerBuilder& enableWifi() {
        computer->setWifi(true);
        return *this;
    }

    FluentComputerBuilder& enableBluetooth() {
        computer->setBluetooth(true);
        return *this;
    }

    FluentComputerBuilder& enableRGB() {
        computer->setRGB(true);
        return *this;
    }

    unique_ptr<Computer> build() {
        return move(computer);
    }
};

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "BUILDER PATTERN DEMO\n";
    cout << "========================================\n";

    ComputerDirector director;

    // ============================================
    // Example 1: Gaming PC
    // ============================================
    cout << "\n=== EXAMPLE 1: Gaming PC ===\n";
    {
        GamingPCBuilder gamingBuilder;
        director.setBuilder(&gamingBuilder);
        director.buildFullComputer();

        auto gamingPC = gamingBuilder.getResult();
        gamingPC->showSpecs();
    }

    // ============================================
    // Example 2: Office PC
    // ============================================
    cout << "\n=== EXAMPLE 2: Office PC ===\n";
    {
        OfficePCBuilder officeBuilder;
        director.setBuilder(&officeBuilder);
        director.buildFullComputer();

        auto officePC = officeBuilder.getResult();
        officePC->showSpecs();
    }

    // ============================================
    // Example 3: Budget PC (Minimal Build)
    // ============================================
    cout << "\n=== EXAMPLE 3: Budget PC (Minimal) ===\n";
    {
        BudgetPCBuilder budgetBuilder;
        director.setBuilder(&budgetBuilder);
        director.buildMinimalComputer();  // Only essential parts

        auto budgetPC = budgetBuilder.getResult();
        budgetPC->showSpecs();
    }

    // ============================================
    // Example 4: Workstation
    // ============================================
    cout << "\n=== EXAMPLE 4: Professional Workstation ===\n";
    {
        WorkstationBuilder workstationBuilder;
        director.setBuilder(&workstationBuilder);
        director.buildFullComputer();

        auto workstation = workstationBuilder.getResult();
        workstation->showSpecs();
    }

    // ============================================
    // Example 5: Manual Building (Without Director)
    // ============================================
    cout << "\n=== EXAMPLE 5: Manual Custom Build ===\n";
    {
        GamingPCBuilder customBuilder;

        // Client controls the building process
        customBuilder.buildCPU();
        customBuilder.buildRAM();
        customBuilder.buildStorage();
        customBuilder.buildGPU();
        customBuilder.buildRGB();
        // Skipping other parts for a custom build

        auto customPC = customBuilder.getResult();
        customPC->showSpecs();
    }

    // ============================================
    // Example 6: Fluent Builder (Method Chaining)
    // ============================================
    cout << "\n=== EXAMPLE 6: Fluent Builder (Method Chaining) ===\n";
    {
        auto fluentPC = FluentComputerBuilder()
                            .setCPU("Intel Core i7-13700K")
                            .setRAM("32GB DDR5")
                            .setStorage("1TB NVMe SSD")
                            .setGPU("NVIDIA RTX 4070")
                            .addPeripheral("Keyboard")
                            .addPeripheral("Mouse")
                            .enableWifi()
                            .enableRGB()
                            .build();

        fluentPC->showSpecs();
    }

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF BUILDER PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Construct objects step-by-step\n";
    cout << "✅ Use same construction code for different representations\n";
    cout << "✅ Isolate complex construction code from business logic\n";
    cout << "✅ Avoid 'telescoping constructor' antipattern\n";
    cout << "✅ Build objects that require multi-step construction\n";
    cout << "✅ Create immutable objects with many optional fields\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE BUILDER\n";
    cout << "========================================\n";
    cout << "1. Object has many optional parameters\n";
    cout << "2. Construction process is complex\n";
    cout << "3. Need different representations of same object\n";
    cout << "4. Want to avoid telescoping constructors\n";
    cout << "5. Building immutable objects\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• StringBuilder in Java/C#\n";
    cout << "• SQL Query builders (SELECT, WHERE, JOIN)\n";
    cout << "• HTML/XML document builders\n";
    cout << "• HTTP request builders\n";
    cout << "• Configuration objects\n";
    cout << "• Fast food meal builders\n";
    cout << "• Game character creators\n";

    return 0;
}
