#include <string>
#include <memory>
#include <vector>
#include <print>

// ============================================
// PRODUCT INTERFACE
// Defines the interface for objects the factory method creates
// ============================================

/**
 * Transport - The product interface
 * All concrete products must implement this interface
 */
class Transport {
public:
    virtual ~Transport() {}

    // All transports must implement these methods
    virtual void deliver() const = 0;
    virtual void loadCargo(int weight) const = 0;
    virtual std::string getType() const = 0;
    virtual int getCapacity() const = 0;
};

// ============================================
// CONCRETE PRODUCTS
// Different implementations of the Transport interface
// ============================================

/**
 * Truck - Concrete product for road transport
 */
class Truck : public Transport {
public:
    void deliver() const override {
    }

    void loadCargo(int weight) const override {
        std::println("📦 Loading {} kg into truck cargo bed", weight);
    }

    std::string getType() const override {
        return "Truck";
    }

    int getCapacity() const override {
        return 20000;  // 20 tons
    }
};

/**
 * Ship - Concrete product for sea transport
 */
class Ship : public Transport {
public:
    void deliver() const override {
    }

    void loadCargo(int weight) const override {
        std::println("📦 Loading {} kg into shipping containers", weight);
    }

    std::string getType() const override {
        return "Ship";
    }

    int getCapacity() const override {
        return 200000;  // 200 tons
    }
};

/**
 * Plane - Concrete product for air transport
 */
class Plane : public Transport {
public:
    void deliver() const override {
    }

    void loadCargo(int weight) const override {
        std::println("📦 Loading {} kg into aircraft cargo hold", weight);
    }

    std::string getType() const override {
        return "Plane";
    }

    int getCapacity() const override {
        return 50000;  // 50 tons
    }
};

/**
 * Train - Concrete product for rail transport
 */
class Train : public Transport {
public:
    void deliver() const override {
    }

    void loadCargo(int weight) const override {
        std::println("📦 Loading {} kg into rail cars", weight);
    }

    std::string getType() const override {
        return "Train";
    }

    int getCapacity() const override {
        return 100000;  // 100 tons
    }
};

/**
 * Drone - Concrete product for drone transport
 */
class Drone : public Transport {
public:
    void deliver() const override {
    }

    void loadCargo(int weight) const override {
        std::println("📦 Loading {} kg onto drone platform", weight);
    }

    std::string getType() const override {
        return "Drone";
    }

    int getCapacity() const override {
        return 5;  // 5 kg
    }
};

// ============================================
// CREATOR (Abstract Factory)
// Declares the factory method that returns Transport objects
// ============================================

/**
 * Logistics - The creator class
 * Declares the factory method that must return a Transport object
 * The creator's code works with products through the abstract interface
 */
class Logistics {
public:
    virtual ~Logistics() {}

    /**
     * Factory Method - creates products
     * This is the key method that subclasses override
     */
    virtual std::unique_ptr<Transport> createTransport() const = 0;

    /**
     * Core business logic that uses the factory method
     * Despite the name, product creation is NOT the primary responsibility
     * Usually contains some core business logic that relies on products
     */
    void planDelivery(int cargoWeight) const {
        std::println("");
        std::println("╔════════════════════════════════════╗");
        std::println("║      PLANNING DELIVERY             ║");
        std::println("╚════════════════════════════════════╝");

        // Call the factory method to create a Transport object
        auto transport = createTransport();

        // Now use the transport object
        std::println("Transport Type: {}", transport->getType());
        std::println("Capacity: {} kg", transport->getCapacity());
        std::println("Cargo Weight: {} kg", cargoWeight);

        if (cargoWeight <= transport->getCapacity()) {
            std::println("✅ Cargo fits in {}", transport->getType());
            std::println("");
            transport->loadCargo(cargoWeight);
            transport->deliver();
        } else {
            std::println("❌ Cargo too heavy for {}", transport->getType());
            std::println("   Need {} kg additional capacity", (cargoWeight - transport->getCapacity()));
        }

        std::println("────────────────────────────────────");
    }

    void estimateCost(int distance) const {
        auto transport = createTransport();
        std::println("");
        std::println("💰 Cost Estimate for {}:", transport->getType());
        std::println("   Distance: {} km", distance);

        // Different cost calculations based on transport type
        if (transport->getType() == "Truck")
            std::println("   Estimated cost: ${}", distance * 2);
        else if (transport->getType() == "Ship")
            std::println("   Estimated cost: ${}", distance * 1);
        else if (transport->getType() == "Plane")
            std::println("   Estimated cost: ${}", distance * 5);
        else if (transport->getType() == "Train")
            std::println("   Estimated cost: ${}", distance * 1.5);
        else if (transport->getType() == "Drone")
            std::println("   Estimated cost: ${}", distance * 10);
    }
};

// ============================================
// CONCRETE CREATORS
// Override the factory method to change the resulting product type
// ============================================

/**
 * RoadLogistics - Creates Truck transport
 */
class RoadLogistics : public Logistics {
public:
    std::unique_ptr<Transport> createTransport() const override {
        std::println("🏭 RoadLogistics: Creating Truck transport...");
        return std::make_unique<Truck>();
    }
};

/**
 * SeaLogistics - Creates Ship transport
 */
class SeaLogistics : public Logistics {
public:
    std::unique_ptr<Transport> createTransport() const override {
        std::println("🏭 SeaLogistics: Creating Ship transport...");
        return std::make_unique<Ship>();
    }
};

/**
 * AirLogistics - Creates Plane transport
 */
class AirLogistics : public Logistics {
public:
    std::unique_ptr<Transport> createTransport() const override {
        std::println("🏭 AirLogistics: Creating Plane transport...");
        return std::make_unique<Plane>();
    }
};

/**
 * RailLogistics - Creates Train transport
 */
class RailLogistics : public Logistics {
public:
    std::unique_ptr<Transport> createTransport() const override {
        std::println("🏭 RailLogistics: Creating Train transport...");
        return std::make_unique<Train>();
    }
};

/**
 * DroneLogistics - Creates Drone transport
 */
class DroneLogistics : public Logistics {
public:
    std::unique_ptr<Transport> createTransport() const override {
        std::println("🏭 DroneLogistics: Creating Drone transport...");
        return std::make_unique<Drone>();
    }
};

// ============================================
// CLIENT CODE
// Works with an instance of a concrete creator through the base interface
// ============================================

void clientCode(const Logistics& logistics, int cargoWeight, int distance) {
    // Client code works with any creator through the abstract interface
    logistics.planDelivery(cargoWeight);
    logistics.estimateCost(distance);
}

// ============================================
// FACTORY SELECTOR
// Dynamic factory selection based on conditions
// ============================================

std::unique_ptr<Logistics> selectLogistics(int distance, int weight, bool urgent) {
    std::println("");
    std::println("🤔 Selecting optimal logistics method...");
    std::println("   Distance: {} km", distance);
    std::println("   Weight: {} kg", weight);
    std::println("   Urgent: {}", (urgent ? "Yes" : "No"));
    std::println("");

    // Decision logic for selecting the right logistics
    if (weight <= 5 && distance <= 50 && urgent) {
        std::println("✓ Decision: Using Drone (small, close, urgent)");
        return std::make_unique<DroneLogistics>();
    }
    else if (urgent && distance < 2000) {
        std::println("✓ Decision: Using Air (urgent, medium distance)");
        return std::make_unique<AirLogistics>();
    }
    else if (weight > 100000) {
        std::println("✓ Decision: Using Ship (very heavy cargo)");
        return std::make_unique<SeaLogistics>();
    }
    else if (distance > 500 && weight > 20000) {
        std::println("✓ Decision: Using Rail (long distance, heavy)");
        return std::make_unique<RailLogistics>();
    }
    else {
        std::println("✓ Decision: Using Road (standard delivery)");
        return std::make_unique<RoadLogistics>();
    }
}

// ============================================
// REAL-WORLD EXAMPLE 2: Document Creator
// ============================================

// Product interface
class Document {
public:
    virtual ~Document() {}
    virtual void open() const = 0;
    virtual void save() const = 0;
    virtual std::string getFormat() const = 0;
};

// Concrete products
class PDFDocument : public Document {
public:
    void open() const override {
        std::println("📄 Opening PDF document with PDF reader");
    }
    void save() const override {
        std::println("💾 Saving as .pdf file");
    }
    std::string getFormat() const override { return "PDF"; }
};

class WordDocument : public Document {
public:
    void open() const override {
        std::println("📝 Opening Word document with MS Word");
    }
    void save() const override {
        std::println("💾 Saving as .docx file");
    }
    std::string getFormat() const override { return "DOCX"; }
};

// Creator
class Application {
public:
    virtual ~Application() {}
    virtual std::unique_ptr<Document> createDocument() const = 0;

    void newDocument() const {
        auto doc = createDocument();
        std::println("");
        std::println("Creating new {} document", doc->getFormat());
        doc->open();
    }

    void saveDocument() const {
        auto doc = createDocument();
        doc->save();
    }
};

// Concrete creators
class PDFApplication : public Application {
public:
    std::unique_ptr<Document> createDocument() const override {
        return std::make_unique<PDFDocument>();
    }
};

class WordApplication : public Application {
public:
    std::unique_ptr<Document> createDocument() const override {
        return std::make_unique<WordDocument>();
    }
};

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("FACTORY METHOD PATTERN DEMO");
    std::println("========================================");

    // ============================================
    // Example 1: Road Logistics
    // ============================================
    std::println("");
    std::println("=== EXAMPLE 1: Road Logistics ===");
    {
        RoadLogistics roadLogistics;
        clientCode(roadLogistics, 15000, 500);
    }

    // ============================================
    // Example 2: Sea Logistics
    // ============================================
    std::println("");
    std::println("=== EXAMPLE 2: Sea Logistics ===");
    {
        SeaLogistics seaLogistics;
        clientCode(seaLogistics, 150000, 5000);
    }

    // ============================================
    // Example 3: Air Logistics
    // ============================================
    std::println("");
    std::println("=== EXAMPLE 3: Air Logistics ===");
    {
        AirLogistics airLogistics;
        clientCode(airLogistics, 30000, 3000);
    }

    // ============================================
    // Example 4: Rail Logistics
    // ============================================
    std::println("");
    std::println("=== EXAMPLE 4: Rail Logistics ===");
    {
        RailLogistics railLogistics;
        clientCode(railLogistics, 80000, 1200);
    }

    // ============================================
    // Example 5: Drone Logistics
    // ============================================
    std::println("");
    std::println("=== EXAMPLE 5: Drone Logistics ===");
    {
        DroneLogistics droneLogistics;
        clientCode(droneLogistics, 3, 10);
    }

    // ============================================
    // Example 6: Dynamic Selection
    // ============================================
    std::println("");
    std::println("=== EXAMPLE 6: Dynamic Logistics Selection ===");

    struct DeliveryScenario {
        int distance;
        int weight;
        bool urgent;
    };

    std::vector<DeliveryScenario> scenarios = {
        {20, 2, true},          // Small urgent package
        {1500, 10000, true},    // Medium urgent cargo
        {5000, 150000, false},  // Very heavy cargo
        {800, 50000, false},    // Heavy long-distance
        {300, 10000, false}     // Standard delivery
    };

    for (const auto& scenario : scenarios) {
        auto logistics = selectLogistics(scenario.distance,
                                         scenario.weight,
                                         scenario.urgent);
        logistics->planDelivery(scenario.weight);
        std::println("");
    }

    // ============================================
    // Example 7: Document Application
    // ============================================
    std::println("");
    std::println("=== EXAMPLE 7: Document Application ===");

    std::println("");
    std::println("--- PDF Application ---");
    PDFApplication pdfApp;
    pdfApp.newDocument();
    pdfApp.saveDocument();

    std::println("");
    std::println("--- Word Application ---");
    WordApplication wordApp;
    wordApp.newDocument();
    wordApp.saveDocument();

    return 0;
}
