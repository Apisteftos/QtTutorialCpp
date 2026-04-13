#include <iostream>
#include <string>
#include <memory>
#include <vector>
using namespace std;

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
    virtual string getType() const = 0;
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
        cout << "🚚 Delivering by land in a truck\n";
        cout << "   Route: Using highways and roads\n";
        cout << "   Speed: 80 km/h average\n";
    }

    void loadCargo(int weight) const override {
        cout << "📦 Loading " << weight << " kg into truck cargo bed\n";
    }

    string getType() const override {
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
        cout << "🚢 Delivering by sea in a container ship\n";
        cout << "   Route: Using ocean shipping lanes\n";
        cout << "   Speed: 40 km/h average\n";
    }

    void loadCargo(int weight) const override {
        cout << "📦 Loading " << weight << " kg into shipping containers\n";
    }

    string getType() const override {
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
        cout << "✈️  Delivering by air in a cargo plane\n";
        cout << "   Route: Direct flight path\n";
        cout << "   Speed: 800 km/h average\n";
    }

    void loadCargo(int weight) const override {
        cout << "📦 Loading " << weight << " kg into aircraft cargo hold\n";
    }

    string getType() const override {
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
        cout << "🚂 Delivering by rail in a freight train\n";
        cout << "   Route: Using railway network\n";
        cout << "   Speed: 100 km/h average\n";
    }

    void loadCargo(int weight) const override {
        cout << "📦 Loading " << weight << " kg into rail cars\n";
    }

    string getType() const override {
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
        cout << "🚁 Delivering by air with autonomous drone\n";
        cout << "   Route: Direct GPS coordinates\n";
        cout << "   Speed: 60 km/h average\n";
    }

    void loadCargo(int weight) const override {
        cout << "📦 Loading " << weight << " kg onto drone platform\n";
    }

    string getType() const override {
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
    virtual unique_ptr<Transport> createTransport() const = 0;

    /**
     * Core business logic that uses the factory method
     * Despite the name, product creation is NOT the primary responsibility
     * Usually contains some core business logic that relies on products
     */
    void planDelivery(int cargoWeight) const {
        cout << "\n╔════════════════════════════════════╗\n";
        cout << "║      PLANNING DELIVERY             ║\n";
        cout << "╚════════════════════════════════════╝\n";

        // Call the factory method to create a Transport object
        auto transport = createTransport();

        // Now use the transport object
        cout << "Transport Type: " << transport->getType() << "\n";
        cout << "Capacity: " << transport->getCapacity() << " kg\n";
        cout << "Cargo Weight: " << cargoWeight << " kg\n";

        if (cargoWeight <= transport->getCapacity()) {
            cout << "✅ Cargo fits in " << transport->getType() << "\n\n";
            transport->loadCargo(cargoWeight);
            transport->deliver();
        } else {
            cout << "❌ Cargo too heavy for " << transport->getType() << "\n";
            cout << "   Need " << (cargoWeight - transport->getCapacity())
                 << " kg additional capacity\n";
        }

        cout << "────────────────────────────────────\n";
    }

    void estimateCost(int distance) const {
        auto transport = createTransport();
        cout << "\n💰 Cost Estimate for " << transport->getType() << ":\n";
        cout << "   Distance: " << distance << " km\n";

        // Different cost calculations based on transport type
        if (transport->getType() == "Truck")
            cout << "   Estimated cost: $" << distance * 2 << "\n";
        else if (transport->getType() == "Ship")
            cout << "   Estimated cost: $" << distance * 1 << "\n";
        else if (transport->getType() == "Plane")
            cout << "   Estimated cost: $" << distance * 5 << "\n";
        else if (transport->getType() == "Train")
            cout << "   Estimated cost: $" << distance * 1.5 << "\n";
        else if (transport->getType() == "Drone")
            cout << "   Estimated cost: $" << distance * 10 << "\n";
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
    unique_ptr<Transport> createTransport() const override {
        cout << "🏭 RoadLogistics: Creating Truck transport...\n";
        return make_unique<Truck>();
    }
};

/**
 * SeaLogistics - Creates Ship transport
 */
class SeaLogistics : public Logistics {
public:
    unique_ptr<Transport> createTransport() const override {
        cout << "🏭 SeaLogistics: Creating Ship transport...\n";
        return make_unique<Ship>();
    }
};

/**
 * AirLogistics - Creates Plane transport
 */
class AirLogistics : public Logistics {
public:
    unique_ptr<Transport> createTransport() const override {
        cout << "🏭 AirLogistics: Creating Plane transport...\n";
        return make_unique<Plane>();
    }
};

/**
 * RailLogistics - Creates Train transport
 */
class RailLogistics : public Logistics {
public:
    unique_ptr<Transport> createTransport() const override {
        cout << "🏭 RailLogistics: Creating Train transport...\n";
        return make_unique<Train>();
    }
};

/**
 * DroneLogistics - Creates Drone transport
 */
class DroneLogistics : public Logistics {
public:
    unique_ptr<Transport> createTransport() const override {
        cout << "🏭 DroneLogistics: Creating Drone transport...\n";
        return make_unique<Drone>();
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

unique_ptr<Logistics> selectLogistics(int distance, int weight, bool urgent) {
    cout << "\n🤔 Selecting optimal logistics method...\n";
    cout << "   Distance: " << distance << " km\n";
    cout << "   Weight: " << weight << " kg\n";
    cout << "   Urgent: " << (urgent ? "Yes" : "No") << "\n\n";

    // Decision logic for selecting the right logistics
    if (weight <= 5 && distance <= 50 && urgent) {
        cout << "✓ Decision: Using Drone (small, close, urgent)\n";
        return make_unique<DroneLogistics>();
    }
    else if (urgent && distance < 2000) {
        cout << "✓ Decision: Using Air (urgent, medium distance)\n";
        return make_unique<AirLogistics>();
    }
    else if (weight > 100000) {
        cout << "✓ Decision: Using Ship (very heavy cargo)\n";
        return make_unique<SeaLogistics>();
    }
    else if (distance > 500 && weight > 20000) {
        cout << "✓ Decision: Using Rail (long distance, heavy)\n";
        return make_unique<RailLogistics>();
    }
    else {
        cout << "✓ Decision: Using Road (standard delivery)\n";
        return make_unique<RoadLogistics>();
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
    virtual string getFormat() const = 0;
};

// Concrete products
class PDFDocument : public Document {
public:
    void open() const override {
        cout << "📄 Opening PDF document with PDF reader\n";
    }
    void save() const override {
        cout << "💾 Saving as .pdf file\n";
    }
    string getFormat() const override { return "PDF"; }
};

class WordDocument : public Document {
public:
    void open() const override {
        cout << "📝 Opening Word document with MS Word\n";
    }
    void save() const override {
        cout << "💾 Saving as .docx file\n";
    }
    string getFormat() const override { return "DOCX"; }
};

// Creator
class Application {
public:
    virtual ~Application() {}
    virtual unique_ptr<Document> createDocument() const = 0;

    void newDocument() const {
        auto doc = createDocument();
        cout << "\nCreating new " << doc->getFormat() << " document\n";
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
    unique_ptr<Document> createDocument() const override {
        return make_unique<PDFDocument>();
    }
};

class WordApplication : public Application {
public:
    unique_ptr<Document> createDocument() const override {
        return make_unique<WordDocument>();
    }
};

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "FACTORY METHOD PATTERN DEMO\n";
    cout << "========================================\n";

    // ============================================
    // Example 1: Road Logistics
    // ============================================
    cout << "\n=== EXAMPLE 1: Road Logistics ===\n";
    {
        RoadLogistics roadLogistics;
        clientCode(roadLogistics, 15000, 500);
    }

    // ============================================
    // Example 2: Sea Logistics
    // ============================================
    cout << "\n=== EXAMPLE 2: Sea Logistics ===\n";
    {
        SeaLogistics seaLogistics;
        clientCode(seaLogistics, 150000, 5000);
    }

    // ============================================
    // Example 3: Air Logistics
    // ============================================
    cout << "\n=== EXAMPLE 3: Air Logistics ===\n";
    {
        AirLogistics airLogistics;
        clientCode(airLogistics, 30000, 3000);
    }

    // ============================================
    // Example 4: Rail Logistics
    // ============================================
    cout << "\n=== EXAMPLE 4: Rail Logistics ===\n";
    {
        RailLogistics railLogistics;
        clientCode(railLogistics, 80000, 1200);
    }

    // ============================================
    // Example 5: Drone Logistics
    // ============================================
    cout << "\n=== EXAMPLE 5: Drone Logistics ===\n";
    {
        DroneLogistics droneLogistics;
        clientCode(droneLogistics, 3, 10);
    }

    // ============================================
    // Example 6: Dynamic Selection
    // ============================================
    cout << "\n=== EXAMPLE 6: Dynamic Logistics Selection ===\n";

    struct DeliveryScenario {
        int distance;
        int weight;
        bool urgent;
    };

    vector<DeliveryScenario> scenarios = {
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
        cout << "\n";
    }

    // ============================================
    // Example 7: Document Application
    // ============================================
    cout << "\n=== EXAMPLE 7: Document Application ===\n";

    cout << "\n--- PDF Application ---\n";
    PDFApplication pdfApp;
    pdfApp.newDocument();
    pdfApp.saveDocument();

    cout << "\n--- Word Application ---\n";
    WordApplication wordApp;
    wordApp.newDocument();
    wordApp.saveDocument();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF FACTORY METHOD\n";
    cout << "========================================\n";
    cout << "✅ Avoids tight coupling between creator and concrete products\n";
    cout << "✅ Single Responsibility: Product creation code in one place\n";
    cout << "✅ Open/Closed: Can introduce new products without breaking code\n";
    cout << "✅ Code works with products through common interface\n";
    cout << "✅ Easy to extend with new product types\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE FACTORY METHOD\n";
    cout << "========================================\n";
    cout << "1. Don't know exact types beforehand\n";
    cout << "2. Want to provide library extension points\n";
    cout << "3. Want to save resources by reusing objects\n";
    cout << "4. Localize knowledge of which class to instantiate\n";

    cout << "\n========================================\n";
    cout << "FACTORY METHOD vs ABSTRACT FACTORY\n";
    cout << "========================================\n";
    cout << "Factory Method:\n";
    cout << "  • Creates ONE product\n";
    cout << "  • Uses inheritance (subclass decides which to create)\n";
    cout << "  • Returns different types of a SINGLE product\n";
    cout << "\nAbstract Factory:\n";
    cout << "  • Creates FAMILIES of related products\n";
    cout << "  • Uses composition (factory object has methods)\n";
    cout << "  • Returns MULTIPLE related products\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Logistics/shipping systems (Truck, Ship, Plane)\n";
    cout << "• Document editors (PDF, Word, Excel)\n";
    cout << "• Database connections (MySQL, PostgreSQL, MongoDB)\n";
    cout << "• Payment processors (PayPal, Stripe, Square)\n";
    cout << "• UI element frameworks (Button, Dialog, Menu)\n";
    cout << "• Game character creation (Warrior, Mage, Archer)\n";
    cout << "• Report generators (HTML, PDF, Excel)\n";

    return 0;
}
