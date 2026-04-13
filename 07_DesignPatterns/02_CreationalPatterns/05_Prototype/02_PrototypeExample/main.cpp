#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
using namespace std;

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
    virtual unique_ptr<Prototype> clone() const = 0;
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
    string name;
    string characterClass;
    int level;
    int health;
    int mana;
    int strength;
    int intelligence;
    int agility;
    vector<string> inventory;
    vector<string> skills;
    string weapon;
    string armor;

public:
    GameCharacter(string n, string c, int lvl)
        : name(n), characterClass(c), level(lvl),
        health(100), mana(50), strength(10),
        intelligence(10), agility(10) {
        cout << "⚙️  [EXPENSIVE] Creating new character from scratch: " << name << "\n";
        cout << "   Loading character model...\n";
        cout << "   Initializing stats...\n";
        cout << "   Loading textures...\n";
        cout << "   Setting up animations...\n";
    }

    // Copy constructor for cloning
    GameCharacter(const GameCharacter& other)
        : name(other.name), characterClass(other.characterClass),
        level(other.level), health(other.health), mana(other.mana),
        strength(other.strength), intelligence(other.intelligence),
        agility(other.agility), inventory(other.inventory),
        skills(other.skills), weapon(other.weapon), armor(other.armor) {
        cout << "📋 [FAST] Cloning character: " << name << "\n";
    }

    unique_ptr<Prototype> clone() const override {
        return make_unique<GameCharacter>(*this);
    }

    // Setters for customization after cloning
    void setName(const string& n) { name = n; }
    void setLevel(int lvl) { level = lvl; }
    void setHealth(int h) { health = h; }
    void addItem(const string& item) { inventory.push_back(item); }
    void addSkill(const string& skill) { skills.push_back(skill); }
    void setWeapon(const string& w) { weapon = w; }
    void setArmor(const string& a) { armor = a; }
    void setStrength(int s) { strength = s; }
    void setIntelligence(int i) { intelligence = i; }
    void setAgility(int a) { agility = a; }

    void print() const override {
        cout << "\n╔════════════════════════════════════╗\n";
        cout << "║      CHARACTER SHEET               ║\n";
        cout << "╚════════════════════════════════════╝\n";
        cout << "Name:         " << name << "\n";
        cout << "Class:        " << characterClass << "\n";
        cout << "Level:        " << level << "\n";
        cout << "Health:       " << health << " HP\n";
        cout << "Mana:         " << mana << " MP\n";
        cout << "Strength:     " << strength << "\n";
        cout << "Intelligence: " << intelligence << "\n";
        cout << "Agility:      " << agility << "\n";

        if (!weapon.empty())
            cout << "Weapon:       " << weapon << "\n";
        if (!armor.empty())
            cout << "Armor:        " << armor << "\n";

        if (!skills.empty()) {
            cout << "Skills:       ";
            for (size_t i = 0; i < skills.size(); i++) {
                cout << skills[i];
                if (i < skills.size() - 1) cout << ", ";
            }
            cout << "\n";
        }

        if (!inventory.empty()) {
            cout << "Inventory:    ";
            for (size_t i = 0; i < inventory.size(); i++) {
                cout << inventory[i];
                if (i < inventory.size() - 1) cout << ", ";
            }
            cout << "\n";
        }
        cout << "────────────────────────────────────\n";
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
    string title;
    string author;
    string content;
    string template_type;
    vector<string> sections;
    string formatting;

public:
    Document(string t, string tmpl)
        : title(t), template_type(tmpl), author("Unknown") {
        cout << "⚙️  [EXPENSIVE] Creating document from template: " << template_type << "\n";
        cout << "   Loading template...\n";
        cout << "   Applying formatting...\n";
        cout << "   Setting up sections...\n";
    }

    Document(const Document& other)
        : title(other.title), author(other.author), content(other.content),
        template_type(other.template_type), sections(other.sections),
        formatting(other.formatting) {
        cout << "📋 [FAST] Cloning document: " << title << "\n";
    }

    unique_ptr<Prototype> clone() const override {
        return make_unique<Document>(*this);
    }

    void setTitle(const string& t) { title = t; }
    void setAuthor(const string& a) { author = a; }
    void setContent(const string& c) { content = c; }
    void addSection(const string& s) { sections.push_back(s); }

    void print() const override {
        cout << "\n╔════════════════════════════════════╗\n";
        cout << "║          DOCUMENT                  ║\n";
        cout << "╚════════════════════════════════════╝\n";
        cout << "Title:    " << title << "\n";
        cout << "Author:   " << author << "\n";
        cout << "Template: " << template_type << "\n";
        cout << "Formatting: " << formatting << "\n";

        if (!sections.empty()) {
            cout << "Sections:\n";
            for (size_t i = 0; i < sections.size(); i++) {
                cout << "  " << (i + 1) << ". " << sections[i] << "\n";
            }
        }

        if (!content.empty()) {
            cout << "Content: " << content.substr(0, 50);
            if (content.length() > 50) cout << "...";
            cout << "\n";
        }
        cout << "────────────────────────────────────\n";
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
    unordered_map<string, unique_ptr<Prototype>> prototypes;

public:
    void registerPrototype(const string& key, unique_ptr<Prototype> prototype) {
        cout << "📝 Registering prototype: " << key << "\n";
        prototypes[key] = move(prototype);
    }

    unique_ptr<Prototype> createClone(const string& key) {
        auto it = prototypes.find(key);
        if (it != prototypes.end()) {
            cout << "🔍 Found prototype '" << key << "' - creating clone\n";
            return it->second->clone();
        }
        cout << "❌ Prototype '" << key << "' not found\n";
        return nullptr;
    }

    void listPrototypes() const {
        cout << "\n📚 Available Prototypes:\n";
        for (const auto& pair : prototypes) {
            cout << "  • " << pair.first << "\n";
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
        cout << "(" << x << ", " << y << ")";
    }
};

class Shape : public Prototype {
protected:
    string color;
    Point* position;  // Pointer to demonstrate deep copy

public:
    Shape(string c, int x, int y) : color(c) {
        position = new Point(x, y);
        cout << "⚙️  Creating shape at position ";
        position->print();
        cout << "\n";
    }

    // Destructor
    virtual ~Shape() {
        delete position;
    }

    // Copy constructor for deep copy
    Shape(const Shape& other) : color(other.color) {
        position = new Point(*other.position);  // Deep copy!
        cout << "📋 Cloning shape (deep copy)\n";
    }

    unique_ptr<Prototype> clone() const override {
        return make_unique<Shape>(*this);
    }

    void setPosition(int x, int y) {
        position->x = x;
        position->y = y;
    }

    void print() const override {
        cout << "Shape: color=" << color << ", position=";
        position->print();
        cout << ", address=" << position << "\n";
    }
};

// ============================================
// CLIENT CODE & DEMONSTRATIONS
// ============================================

void demonstrateGameCharacters() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  GAME CHARACTER PROTOTYPES         ║\n";
    cout << "╚════════════════════════════════════╝\n";

    // Create prototype registry
    PrototypeRegistry characterRegistry;

    // Register character prototypes (expensive creation happens once)
    cout << "\n--- Registering Character Prototypes ---\n";
    characterRegistry.registerPrototype("warrior", make_unique<Warrior>());
    characterRegistry.registerPrototype("mage", make_unique<Mage>());
    characterRegistry.registerPrototype("archer", make_unique<Archer>());

    // Now create multiple characters by cloning (fast!)
    cout << "\n--- Creating Characters via Cloning ---\n";

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

    cout << "\n💡 Notice: Character creation is MUCH faster after the first one!\n";
}

void demonstrateDocuments() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  DOCUMENT PROTOTYPES               ║\n";
    cout << "╚════════════════════════════════════╝\n";

    PrototypeRegistry docRegistry;

    cout << "\n--- Registering Document Templates ---\n";
    docRegistry.registerPrototype("report", make_unique<BusinessReport>());
    docRegistry.registerPrototype("paper", make_unique<ResearchPaper>());

    cout << "\n--- Creating Documents via Cloning ---\n";

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
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  DEEP COPY vs SHALLOW COPY         ║\n";
    cout << "╚════════════════════════════════════╝\n";

    cout << "\n--- Creating original shape ---\n";
    Shape original("Red", 10, 20);
    original.print();

    cout << "\n--- Cloning shape (deep copy) ---\n";
    auto cloned = original.clone();
    Shape* clonedShape = dynamic_cast<Shape*>(cloned.get());
    clonedShape->print();

    cout << "\n--- Modifying cloned shape ---\n";
    clonedShape->setPosition(100, 200);

    cout << "Original: ";
    original.print();
    cout << "Cloned:   ";
    clonedShape->print();

    cout << "\n💡 Notice: Different memory addresses = Deep copy!\n";
    cout << "   Original shape position unchanged after modifying clone.\n";
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "PROTOTYPE PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateGameCharacters();
    demonstrateDocuments();
    demonstrateDeepCopy();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF PROTOTYPE PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Avoids expensive object creation\n";
    cout << "✅ Reduces subclassing (no need for Factory hierarchy)\n";
    cout << "✅ Add/remove products at runtime\n";
    cout << "✅ Clone complex objects easily\n";
    cout << "✅ Alternative to inheritance for configuration\n";
    cout << "✅ Hide complexity of object creation\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE PROTOTYPE\n";
    cout << "========================================\n";
    cout << "1. Object creation is expensive (DB queries, network)\n";
    cout << "2. Want to avoid factory/builder hierarchy\n";
    cout << "3. Few state combinations (reuse configured objects)\n";
    cout << "4. Need to create objects at runtime\n";
    cout << "5. Want to reduce number of classes\n";

    cout << "\n========================================\n";
    cout << "SHALLOW vs DEEP COPY\n";
    cout << "========================================\n";
    cout << "Shallow Copy:\n";
    cout << "  • Copies object fields\n";
    cout << "  • Pointers still point to same memory\n";
    cout << "  • Changes affect both objects\n";
    cout << "\nDeep Copy:\n";
    cout << "  • Copies object AND pointed-to objects\n";
    cout << "  • Completely independent copies\n";
    cout << "  • Changes don't affect original\n";

    cout << "\n========================================\n";
    cout << "IMPLEMENTATION APPROACHES\n";
    cout << "========================================\n";
    cout << "1. Clone method in base class\n";
    cout << "2. Copy constructor\n";
    cout << "3. Prototype registry/manager\n";
    cout << "4. Prototype factory\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Game character templates (spawn enemies)\n";
    cout << "• Document templates (Word, Photoshop)\n";
    cout << "• Database record caching\n";
    cout << "• UI widget templates\n";
    cout << "• Configuration presets\n";
    cout << "• Cell division in biology\n";
    cout << "• Object pooling systems\n";

    return 0;
}
