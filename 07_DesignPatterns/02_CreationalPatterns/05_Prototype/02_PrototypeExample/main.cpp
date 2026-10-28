#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <print>

// ============================================
// PROTOTYPE INTERFACE
// Declares the cloning method
// ============================================

/**
 * Prototype interface
 * All prototypes must implement the clone method
 */
class Prototype {
public:
    virtual ~Prototype() {}
    virtual std::unique_ptr<Prototype> clone() const = 0;
    virtual void print() const = 0;
};

// ============================================
// EXAMPLE 1: GAME CHARACTERS
// Complex objects that are expensive to create
// ============================================

/**
 * GameCharacter - Complex object with many properties
 */
class GameCharacter : public Prototype {
protected:
    std::string name;
    std::string characterClass;
    int level;
    int health;
    int mana;
    int strength;
    int intelligence;
    int agility;
    std::vector<std::string> inventory;
    std::vector<std::string> skills;
    std::string weapon;
    std::string armor;

public:
    GameCharacter(std::string n, std::string c, int lvl)
        : name(n), characterClass(c), level(lvl),
        health(100), mana(50), strength(10),
        intelligence(10), agility(10) {
        std::println("⚙️  [EXPENSIVE] Creating new character from scratch: {}", name);
    }

    // Copy constructor for cloning
    GameCharacter(const GameCharacter& other)
        : name(other.name), characterClass(other.characterClass),
        level(other.level), health(other.health), mana(other.mana),
        strength(other.strength), intelligence(other.intelligence),
        agility(other.agility), inventory(other.inventory),
        skills(other.skills), weapon(other.weapon), armor(other.armor) {
        std::println("📋 [FAST] Cloning character: {}", name);
    }

    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<GameCharacter>(*this);
    }

    // Setters for customization after cloning
    void setName(const std::string& n) { name = n; }
    void setLevel(int lvl) { level = lvl; }
    void setHealth(int h) { health = h; }
    void addItem(const std::string& item) { inventory.push_back(item); }
    void addSkill(const std::string& skill) { skills.push_back(skill); }
    void setWeapon(const std::string& w) { weapon = w; }
    void setArmor(const std::string& a) { armor = a; }
    void setStrength(int s) { strength = s; }
    void setIntelligence(int i) { intelligence = i; }
    void setAgility(int a) { agility = a; }

    void print() const override {
        std::println("");
        std::println("╔════════════════════════════════════╗");
        std::println("║      CHARACTER SHEET               ║");
        std::println("╚════════════════════════════════════╝");
        std::println("Name:         {}", name);
        std::println("Class:        {}", characterClass);
        std::println("Level:        {}", level);
        std::println("Health:       {} HP", health);
        std::println("Mana:         {} MP", mana);
        std::println("Strength:     {}", strength);
        std::println("Intelligence: {}", intelligence);
        std::println("Agility:      {}", agility);

        if (!weapon.empty())
            std::println("Weapon:       {}", weapon);
        if (!armor.empty())
            std::println("Armor:        {}", armor);

        if (!skills.empty()) {
            std::print("Skills:       ");
            for (size_t i = 0; i < skills.size(); i++) {
                std::print("{}", skills[i]);
                if (i < skills.size() - 1) std::print(", ");
            }
            std::println("");
        }

        if (!inventory.empty()) {
            std::print("Inventory:    ");
            for (size_t i = 0; i < inventory.size(); i++) {
                std::print("{}", inventory[i]);
                if (i < inventory.size() - 1) std::print(", ");
            }
            std::println("");
        }
        std::println("────────────────────────────────────");
    }
};

// ============================================
// SPECIFIC CHARACTER TYPES
// ============================================

class Warrior : public GameCharacter {
public:
    Warrior() : GameCharacter("Warrior", "Warrior", 1) {
        strength = 20;
        intelligence = 5;
        agility = 10;
        health = 150;
        mana = 20;
        weapon = "Iron Sword";
        armor = "Chain Mail";
        skills.push_back("Power Strike");
        skills.push_back("Shield Block");
    }
};

class Mage : public GameCharacter {
public:
    Mage() : GameCharacter("Mage", "Mage", 1) {
        strength = 5;
        intelligence = 20;
        agility = 8;
        health = 80;
        mana = 150;
        weapon = "Wooden Staff";
        armor = "Cloth Robe";
        skills.push_back("Fireball");
        skills.push_back("Ice Blast");
        skills.push_back("Teleport");
    }
};

class Archer : public GameCharacter {
public:
    Archer() : GameCharacter("Archer", "Archer", 1) {
        strength = 12;
        intelligence = 10;
        agility = 20;
        health = 100;
        mana = 50;
        weapon = "Longbow";
        armor = "Leather Armor";
        skills.push_back("Rapid Fire");
        skills.push_back("Precision Shot");
    }
};

// ============================================
// EXAMPLE 2: DOCUMENT SYSTEM
// Documents with complex formatting
// ============================================

class Document : public Prototype {
protected:
    std::string title;
    std::string author;
    std::string content;
    std::string template_type;
    std::vector<std::string> sections;
    std::string formatting;

public:
    Document(std::string t, std::string tmpl)
        : title(t), template_type(tmpl), author("Unknown") {
        std::println("⚙️  [EXPENSIVE] Creating document from template: {}", template_type);
    }

    Document(const Document& other)
        : title(other.title), author(other.author), content(other.content),
        template_type(other.template_type), sections(other.sections),
        formatting(other.formatting) {
        std::println("📋 [FAST] Cloning document: {}", title);
    }

    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<Document>(*this);
    }

    void setTitle(const std::string& t) { title = t; }
    void setAuthor(const std::string& a) { author = a; }
    void setContent(const std::string& c) { content = c; }
    void addSection(const std::string& s) { sections.push_back(s); }

    void print() const override {
        std::println("");
        std::println("╔════════════════════════════════════╗");
        std::println("║          DOCUMENT                  ║");
        std::println("╚════════════════════════════════════╝");
        std::println("Title:    {}", title);
        std::println("Author:   {}", author);
        std::println("Template: {}", template_type);
        std::println("Formatting: {}", formatting);

        if (!sections.empty()) {
            std::println("Sections:");
            for (size_t i = 0; i < sections.size(); i++) {
                std::println("  {}. {}", (i + 1), sections[i]);
            }
        }

        if (!content.empty()) {
            std::print("Content: {}", content.substr(0, 50));
            if (content.length() > 50) std::print("...");
            std::println("");
        }
        std::println("────────────────────────────────────");
    }
};

class BusinessReport : public Document {
public:
    BusinessReport() : Document("Business Report", "Corporate Template") {
        formatting = "Professional, Times New Roman, 12pt";
        sections.push_back("Executive Summary");
        sections.push_back("Introduction");
        sections.push_back("Analysis");
        sections.push_back("Recommendations");
        sections.push_back("Conclusion");
    }
};

class ResearchPaper : public Document {
public:
    ResearchPaper() : Document("Research Paper", "Academic Template") {
        formatting = "Academic, LaTeX style";
        sections.push_back("Abstract");
        sections.push_back("Introduction");
        sections.push_back("Literature Review");
        sections.push_back("Methodology");
        sections.push_back("Results");
        sections.push_back("Discussion");
        sections.push_back("Conclusion");
        sections.push_back("References");
    }
};

// ============================================
// PROTOTYPE REGISTRY (PROTOTYPE MANAGER)
// Manages a collection of prototype instances
// ============================================

class PrototypeRegistry {
private:
    std::unordered_map<std::string, std::unique_ptr<Prototype>> prototypes;

public:
    void registerPrototype(const std::string& key, std::unique_ptr<Prototype> prototype) {
        std::println("📝 Registering prototype: {}", key);
        prototypes[key] = move(prototype);
    }

    std::unique_ptr<Prototype> createClone(const std::string& key) {
        auto it = prototypes.find(key);
        if (it != prototypes.end()) {
            std::println("🔍 Found prototype '{}' - creating clone", key);
            return it->second->clone();
        }
        std::println("❌ Prototype '{}' not found", key);
        return nullptr;
    }

    void listPrototypes() const {
        std::println("");
        std::println("📚 Available Prototypes:");
        for (const auto& pair : prototypes) {
            std::println("  • {}", pair.first);
        }
    }
};

// ============================================
// EXAMPLE 3: SHAPE CLONING (DEEP vs SHALLOW COPY)
// ============================================

class Point {
public:
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    void print() const {
        std::print("({}, {})", x, y);
    }
};

class Shape : public Prototype {
protected:
    std::string color;
    Point* position;  // Pointer to demonstrate deep copy

public:
    Shape(std::string c, int x, int y) : color(c) {
        position = new Point(x, y);
        std::print("⚙️  Creating shape at position ");
        position->print();
        std::println("");
    }

    // Destructor
    virtual ~Shape() {
        delete position;
    }

    // Copy constructor for deep copy
    Shape(const Shape& other) : color(other.color) {
        position = new Point(*other.position);  // Deep copy!
        std::println("📋 Cloning shape (deep copy)");
    }

    std::unique_ptr<Prototype> clone() const override {
        return std::make_unique<Shape>(*this);
    }

    void setPosition(int x, int y) {
        position->x = x;
        position->y = y;
    }

    void print() const override {
        std::print("Shape: color={}, position=", color);
        position->print();
        std::println(", address={}", static_cast<const void*>(position));
    }
};

// ============================================
// CLIENT CODE & DEMONSTRATIONS
// ============================================

void demonstrateGameCharacters() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  GAME CHARACTER PROTOTYPES         ║");
    std::println("╚════════════════════════════════════╝");

    // Create prototype registry
    PrototypeRegistry characterRegistry;

    // Register character prototypes (expensive creation happens once)
    std::println("");
    std::println("--- Registering Character Prototypes ---");
    characterRegistry.registerPrototype("warrior", std::make_unique<Warrior>());
    characterRegistry.registerPrototype("mage", std::make_unique<Mage>());
    characterRegistry.registerPrototype("archer", std::make_unique<Archer>());

    // Now create multiple characters by cloning (fast!)
    std::println("");
    std::println("--- Creating Characters via Cloning ---");

    auto warrior1 = characterRegistry.createClone("warrior");
    if (auto* w = dynamic_cast<GameCharacter*>(warrior1.get())) {
        w->setName("Conan");
        w->setLevel(5);
        w->addItem("Health Potion");
        w->print();
    }

    auto warrior2 = characterRegistry.createClone("warrior");
    if (auto* w = dynamic_cast<GameCharacter*>(warrior2.get())) {
        w->setName("Brutus");
        w->setLevel(3);
        w->setWeapon("Battle Axe");
        w->print();
    }

    auto mage1 = characterRegistry.createClone("mage");
    if (auto* m = dynamic_cast<GameCharacter*>(mage1.get())) {
        m->setName("Gandalf");
        m->setLevel(10);
        m->addSkill("Lightning Bolt");
        m->print();
    }

    std::println("");
    std::println("💡 Notice: Character creation is MUCH faster after the first one!");
}

void demonstrateDocuments() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  DOCUMENT PROTOTYPES               ║");
    std::println("╚════════════════════════════════════╝");

    PrototypeRegistry docRegistry;

    std::println("");
    std::println("--- Registering Document Templates ---");
    docRegistry.registerPrototype("report", std::make_unique<BusinessReport>());
    docRegistry.registerPrototype("paper", std::make_unique<ResearchPaper>());

    std::println("");
    std::println("--- Creating Documents via Cloning ---");

    auto report1 = docRegistry.createClone("report");
    if (auto* doc = dynamic_cast<Document*>(report1.get())) {
        doc->setTitle("Q4 Sales Report");
        doc->setAuthor("John Smith");
        doc->setContent("Sales increased by 25% in Q4...");
        doc->print();
    }

    auto report2 = docRegistry.createClone("report");
    if (auto* doc = dynamic_cast<Document*>(report2.get())) {
        doc->setTitle("Annual Financial Report");
        doc->setAuthor("Jane Doe");
        doc->print();
    }

    auto paper1 = docRegistry.createClone("paper");
    if (auto* doc = dynamic_cast<Document*>(paper1.get())) {
        doc->setTitle("Machine Learning in Healthcare");
        doc->setAuthor("Dr. Smith");
        doc->print();
    }
}

void demonstrateDeepCopy() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  DEEP COPY vs SHALLOW COPY         ║");
    std::println("╚════════════════════════════════════╝");

    std::println("");
    std::println("--- Creating original shape ---");
    Shape original("Red", 10, 20);
    original.print();

    std::println("");
    std::println("--- Cloning shape (deep copy) ---");
    auto cloned = original.clone();
    Shape* clonedShape = dynamic_cast<Shape*>(cloned.get());
    clonedShape->print();

    std::println("");
    std::println("--- Modifying cloned shape ---");
    clonedShape->setPosition(100, 200);

    std::print("Original: ");
    original.print();
    std::print("Cloned:   ");
    clonedShape->print();

    std::println("");
    std::println("💡 Notice: Different memory addresses = Deep copy!");
    std::println("   Original shape position unchanged after modifying clone.");
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("PROTOTYPE PATTERN DEMO");
    std::println("========================================");

    demonstrateGameCharacters();
    demonstrateDocuments();
    demonstrateDeepCopy();

    return 0;
}
