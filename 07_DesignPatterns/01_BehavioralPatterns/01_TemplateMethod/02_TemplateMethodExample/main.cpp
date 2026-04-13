#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// ============================================
// EXAMPLE 1: BEVERAGE PREPARATION
// Classic template method example
// ============================================

/**
 * Abstract Class: Beverage Template
 * Defines the template method and abstract steps
 */
class Beverage {
public:
    virtual ~Beverage() {}

    /**
     * Template Method
     * Defines the skeleton of the algorithm
     * Final - cannot be overridden by subclasses
     */
    void prepareBeverage() {
        cout << "\n☕ Preparing " << getBeverageName() << "...\n";
        cout << "================================\n";

        boilWater();
        brew();
        pourInCup();

        // Hook method - optional step
        if (customerWantsCondiments()) {
            addCondiments();
        }

        cout << "✅ " << getBeverageName() << " is ready!\n";
    }

protected:
    // Concrete method - same for all subclasses
    void boilWater() {
        cout << "1. 💧 Boiling water...\n";
    }

    // Concrete method - same for all subclasses
    void pourInCup() {
        cout << "3. ☕ Pouring into cup...\n";
    }

    // Abstract methods - must be implemented by subclasses
    virtual void brew() = 0;
    virtual void addCondiments() = 0;
    virtual string getBeverageName() const = 0;

    // Hook method - subclasses can override if needed
    virtual bool customerWantsCondiments() const {
        return true;  // Default behavior
    }
};

/**
 * Concrete Class: Coffee
 */
class Coffee : public Beverage {
protected:
    void brew() override {
        cout << "2. ☕ Brewing coffee grounds...\n";
    }

    void addCondiments() override {
        cout << "4. 🥛 Adding sugar and milk...\n";
    }

    string getBeverageName() const override {
        return "Coffee";
    }
};

/**
 * Concrete Class: Tea
 */
class Tea : public Beverage {
protected:
    void brew() override {
        cout << "2. 🍵 Steeping tea bag...\n";
    }

    void addCondiments() override {
        cout << "4. 🍋 Adding lemon...\n";
    }

    string getBeverageName() const override {
        return "Tea";
    }
};

/**
 * Concrete Class: Hot Chocolate
 */
class HotChocolate : public Beverage {
protected:
    void brew() override {
        cout << "2. 🍫 Mixing chocolate powder...\n";
    }

    void addCondiments() override {
        cout << "4. 🍦 Adding whipped cream and marshmallows...\n";
    }

    string getBeverageName() const override {
        return "Hot Chocolate";
    }

    // Override hook to always add condiments
    bool customerWantsCondiments() const override {
        return true;
    }
};

/**
 * Concrete Class: Black Coffee (uses hook)
 */
class BlackCoffee : public Coffee {
protected:
    string getBeverageName() const override {
        return "Black Coffee";
    }

    // Override hook - no condiments
    bool customerWantsCondiments() const override {
        cout << "   ⚠️  No condiments requested\n";
        return false;
    }
};

// ============================================
// EXAMPLE 2: DATA MINING
// Abstract data processing algorithm
// ============================================

/**
 * Abstract Class: Data Miner Template
 */
class DataMiner {
public:
    virtual ~DataMiner() {}

    /**
     * Template Method
     * Defines data mining process
     */
    void mine(const string& path) {
        cout << "\n⛏️  Starting data mining process...\n";
        cout << "===================================\n";

        string data = openFile(path);
        string rawData = extractData(data);
        string parsedData = parseData(rawData);
        string analysis = analyzeData(parsedData);
        sendReport(analysis);
        closeFile(path);

        cout << "✅ Data mining complete!\n";
    }

protected:
    // Concrete methods - common for all miners
    string openFile(const string& path) {
        cout << "1. 📂 Opening file: " << path << "\n";
        return "file_data";
    }

    void closeFile(const string& path) {
        cout << "6. 🔒 Closing file: " << path << "\n";
    }

    // Abstract methods - implemented by subclasses
    virtual string extractData(const string& data) = 0;
    virtual string parseData(const string& rawData) = 0;
    virtual string analyzeData(const string& data) = 0;
    virtual void sendReport(const string& analysis) = 0;
};

/**
 * Concrete Class: PDF Data Miner
 */
class PDFDataMiner : public DataMiner {
protected:
    string extractData(const string& data) override {
        cout << "2. 📄 Extracting data from PDF...\n";
        return "pdf_raw_data";
    }

    string parseData(const string& rawData) override {
        cout << "3. 🔍 Parsing PDF data...\n";
        return "pdf_parsed_data";
    }

    string analyzeData(const string& data) override {
        cout << "4. 📊 Analyzing PDF data...\n";
        return "pdf_analysis_results";
    }

    void sendReport(const string& analysis) override {
        cout << "5. 📧 Sending PDF report via email...\n";
    }
};

/**
 * Concrete Class: CSV Data Miner
 */
class CSVDataMiner : public DataMiner {
protected:
    string extractData(const string& data) override {
        cout << "2. 📊 Extracting data from CSV...\n";
        return "csv_raw_data";
    }

    string parseData(const string& rawData) override {
        cout << "3. 🔍 Parsing CSV columns and rows...\n";
        return "csv_parsed_data";
    }

    string analyzeData(const string& data) override {
        cout << "4. 📈 Running statistical analysis...\n";
        return "csv_analysis_results";
    }

    void sendReport(const string& analysis) override {
        cout << "5. 💾 Saving CSV report to database...\n";
    }
};

/**
 * Concrete Class: Word Data Miner
 */
class WordDataMiner : public DataMiner {
protected:
    string extractData(const string& data) override {
        cout << "2. 📝 Extracting text from Word document...\n";
        return "word_raw_data";
    }

    string parseData(const string& rawData) override {
        cout << "3. 🔍 Parsing Word document structure...\n";
        return "word_parsed_data";
    }

    string analyzeData(const string& data) override {
        cout << "4. 📖 Analyzing document content...\n";
        return "word_analysis_results";
    }

    void sendReport(const string& analysis) override {
        cout << "5. 📱 Sending report via SMS...\n";
    }
};

// ============================================
// EXAMPLE 3: GAME AI
// AI behavior template
// ============================================

/**
 * Abstract Class: Game AI Template
 */
class GameAI {
public:
    virtual ~GameAI() {}

    /**
     * Template Method
     * Defines AI turn sequence
     */
    void takeTurn() {
        cout << "\n🎮 " << getAIName() << " taking turn...\n";
        cout << "================================\n";

        collectResources();
        buildStructures();
        buildUnits();
        attackEnemy();

        // Hook - optional special ability
        if (canUseSpecialAbility()) {
            useSpecialAbility();
        }

        cout << "✅ Turn complete!\n";
    }

protected:
    // Concrete method - same for all AI
    void collectResources() {
        cout << "1. 💰 Collecting resources...\n";
    }

    // Abstract methods - each AI implements differently
    virtual void buildStructures() = 0;
    virtual void buildUnits() = 0;
    virtual void attackEnemy() = 0;
    virtual string getAIName() const = 0;

    // Hook methods
    virtual bool canUseSpecialAbility() const {
        return false;
    }

    virtual void useSpecialAbility() {}
};

/**
 * Concrete Class: Aggressive AI
 */
class AggressiveAI : public GameAI {
protected:
    void buildStructures() override {
        cout << "2. 🏰 Building barracks and armories...\n";
    }

    void buildUnits() override {
        cout << "3. ⚔️  Training offensive units (soldiers, tanks)...\n";
    }

    void attackEnemy() override {
        cout << "4. 💥 Launching aggressive attack on enemy base!\n";
    }

    string getAIName() const override {
        return "Aggressive AI";
    }
};

/**
 * Concrete Class: Defensive AI
 */
class DefensiveAI : public GameAI {
protected:
    void buildStructures() override {
        cout << "2. 🏰 Building walls and towers...\n";
    }

    void buildUnits() override {
        cout << "3. 🛡️  Training defensive units (guards, archers)...\n";
    }

    void attackEnemy() override {
        cout << "4. 🎯 Defending territory, minimal offense...\n";
    }

    string getAIName() const override {
        return "Defensive AI";
    }
};

/**
 * Concrete Class: Balanced AI with Special Ability
 */
class BalancedAI : public GameAI {
protected:
    void buildStructures() override {
        cout << "2. 🏰 Building balanced mix of structures...\n";
    }

    void buildUnits() override {
        cout << "3. ⚖️  Training mixed army composition...\n";
    }

    void attackEnemy() override {
        cout << "4. 🎲 Strategic attack on weak points...\n";
    }

    string getAIName() const override {
        return "Balanced AI";
    }

    bool canUseSpecialAbility() const override {
        return true;
    }

    void useSpecialAbility() override {
        cout << "5. ⭐ Using tactical airstrike!\n";
    }
};

// ============================================
// EXAMPLE 4: HOUSE BUILDING
// Construction process template
// ============================================

/**
 * Abstract Class: House Builder Template
 */
class HouseBuilder {
public:
    virtual ~HouseBuilder() {}

    /**
     * Template Method
     * Defines house construction process
     */
    void buildHouse() {
        cout << "\n🏗️  Building " << getHouseType() << "...\n";
        cout << "================================\n";

        layFoundation();
        buildWalls();
        buildRoof();
        installWindows();
        installDoors();

        // Hook for additional features
        if (hasAdditionalFeatures()) {
            addFeatures();
        }

        paintHouse();

        cout << "✅ " << getHouseType() << " construction complete!\n";
    }

protected:
    // Concrete methods - same for all houses
    void layFoundation() {
        cout << "1. 🏗️  Laying foundation...\n";
    }

    void installWindows() {
        cout << "4. 🪟 Installing windows...\n";
    }

    void installDoors() {
        cout << "5. 🚪 Installing doors...\n";
    }

    // Abstract methods - different for each house type
    virtual void buildWalls() = 0;
    virtual void buildRoof() = 0;
    virtual void paintHouse() = 0;
    virtual string getHouseType() const = 0;

    // Hook methods
    virtual bool hasAdditionalFeatures() const {
        return false;
    }

    virtual void addFeatures() {}
};

/**
 * Concrete Class: Wooden House
 */
class WoodenHouse : public HouseBuilder {
protected:
    void buildWalls() override {
        cout << "2. 🪵 Building wooden walls...\n";
    }

    void buildRoof() override {
        cout << "3. 🏠 Installing wooden shingle roof...\n";
    }

    void paintHouse() override {
        cout << "7. 🎨 Applying wood stain (natural finish)...\n";
    }

    string getHouseType() const override {
        return "Wooden House";
    }
};

/**
 * Concrete Class: Brick House
 */
class BrickHouse : public HouseBuilder {
protected:
    void buildWalls() override {
        cout << "2. 🧱 Building brick walls...\n";
    }

    void buildRoof() override {
        cout << "3. 🏠 Installing clay tile roof...\n";
    }

    void paintHouse() override {
        cout << "7. 🎨 Painting walls (white color)...\n";
    }

    string getHouseType() const override {
        return "Brick House";
    }
};

/**
 * Concrete Class: Luxury Villa
 */
class LuxuryVilla : public HouseBuilder {
protected:
    void buildWalls() override {
        cout << "2. 🏛️  Building reinforced concrete walls...\n";
    }

    void buildRoof() override {
        cout << "3. 🏠 Installing premium metal roof...\n";
    }

    void paintHouse() override {
        cout << "7. 🎨 Professional painting (designer colors)...\n";
    }

    string getHouseType() const override {
        return "Luxury Villa";
    }

    bool hasAdditionalFeatures() const override {
        return true;
    }

    void addFeatures() override {
        cout << "6. ⭐ Adding luxury features:\n";
        cout << "   • 🏊 Swimming pool\n";
        cout << "   • 🌴 Garden with fountain\n";
        cout << "   • 🚗 Underground garage\n";
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateBeveragePreparation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BEVERAGE PREPARATION   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Making Different Beverages ---\n";

    // Coffee
    Beverage* coffee = new Coffee();
    coffee->prepareBeverage();

    // Tea
    Beverage* tea = new Tea();
    tea->prepareBeverage();

    // Hot Chocolate
    Beverage* hotChoco = new HotChocolate();
    hotChoco->prepareBeverage();

    // Black Coffee (uses hook)
    Beverage* blackCoffee = new BlackCoffee();
    blackCoffee->prepareBeverage();

    delete coffee;
    delete tea;
    delete hotChoco;
    delete blackCoffee;
}

void demonstrateDataMining() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: DATA MINING            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // PDF Mining
    DataMiner* pdfMiner = new PDFDataMiner();
    pdfMiner->mine("document.pdf");

    // CSV Mining
    DataMiner* csvMiner = new CSVDataMiner();
    csvMiner->mine("data.csv");

    // Word Mining
    DataMiner* wordMiner = new WordDataMiner();
    wordMiner->mine("report.docx");

    delete pdfMiner;
    delete csvMiner;
    delete wordMiner;
}

void demonstrateGameAI() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: GAME AI                ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Aggressive AI
    GameAI* aggressive = new AggressiveAI();
    aggressive->takeTurn();

    // Defensive AI
    GameAI* defensive = new DefensiveAI();
    defensive->takeTurn();

    // Balanced AI with special ability
    GameAI* balanced = new BalancedAI();
    balanced->takeTurn();

    delete aggressive;
    delete defensive;
    delete balanced;
}

void demonstrateHouseBuilding() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: HOUSE CONSTRUCTION     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Wooden House
    HouseBuilder* wooden = new WoodenHouse();
    wooden->buildHouse();

    // Brick House
    HouseBuilder* brick = new BrickHouse();
    brick->buildHouse();

    // Luxury Villa
    HouseBuilder* villa = new LuxuryVilla();
    villa->buildHouse();

    delete wooden;
    delete brick;
    delete villa;
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "TEMPLATE METHOD PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateBeveragePreparation();
    demonstrateDataMining();
    demonstrateGameAI();
    demonstrateHouseBuilding();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF TEMPLATE METHOD PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Code Reuse: Common algorithm in base class\n";
    cout << "✅ Control: Parent controls algorithm structure\n";
    cout << "✅ Flexibility: Subclasses customize specific steps\n";
    cout << "✅ Maintainability: Algorithm changes in one place\n";
    cout << "✅ Inversion of Control: Framework calls subclass methods\n";
    cout << "✅ Enforces Consistency: All subclasses follow same structure\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE TEMPLATE METHOD\n";
    cout << "========================================\n";
    cout << "1. Multiple classes have similar algorithms\n";
    cout << "2. Want to control algorithm structure from parent\n";
    cout << "3. Common behavior should be in base class\n";
    cout << "4. Want to avoid code duplication\n";
    cout << "5. Need to let subclasses extend specific steps\n";
    cout << "6. Want to enforce algorithm sequence\n";

    cout << "\n========================================\n";
    cout << "TEMPLATE METHOD vs STRATEGY\n";
    cout << "========================================\n";
    cout << "Template Method:\n";
    cout << "  • Uses inheritance (class-level)\n";
    cout << "  • Defines algorithm skeleton in base class\n";
    cout << "  • Compile-time binding\n";
    cout << "  • Changes part of algorithm\n";
    cout << "  • Example: Beverage preparation steps\n";
    cout << "\nStrategy Pattern:\n";
    cout << "  • Uses composition (object-level)\n";
    cout << "  • Encapsulates entire algorithm\n";
    cout << "  • Runtime binding\n";
    cout << "  • Replaces entire algorithm\n";
    cout << "  • Example: Different payment methods\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Beverage/food preparation (coffee, tea, recipes)\n";
    cout << "• Data mining and processing pipelines\n";
    cout << "• Game AI behavior sequences\n";
    cout << "• House/building construction processes\n";
    cout << "• Document generation (reports, invoices)\n";
    cout << "• Test frameworks (setUp, test, tearDown)\n";
    cout << "• Web page rendering (header, content, footer)\n";
    cout << "• Database connection workflow\n";
    cout << "• File parsers (XML, JSON, CSV)\n";
    cout << "• Network protocols (handshake, data, close)\n";
    cout << "• Compilation process (parse, compile, link)\n";
    cout << "• Authentication flows (validate, authorize, log)\n";

    cout << "\n========================================\n";
    cout << "KEY COMPONENTS\n";
    cout << "========================================\n";
    cout << "1. Abstract Class: Defines template method and steps\n";
    cout << "2. Template Method: Defines algorithm skeleton (final)\n";
    cout << "3. Abstract Methods: Must be implemented by subclasses\n";
    cout << "4. Concrete Methods: Common implementation for all\n";
    cout << "5. Hook Methods: Optional override points\n";
    cout << "6. Concrete Classes: Implement abstract steps\n";

    cout << "\n========================================\n";
    cout << "METHOD TYPES IN TEMPLATE\n";
    cout << "========================================\n";
    cout << "Template Method:\n";
    cout << "  • Defines algorithm structure\n";
    cout << "  • Calls other methods in sequence\n";
    cout << "  • Should not be overridden (final)\n";
    cout << "  • Example: prepareBeverage(), buildHouse()\n";
    cout << "\nAbstract Methods:\n";
    cout << "  • Must be implemented by subclasses\n";
    cout << "  • Different for each subclass\n";
    cout << "  • Pure virtual (= 0)\n";
    cout << "  • Example: brew(), addCondiments()\n";
    cout << "\nConcrete Methods:\n";
    cout << "  • Common implementation\n";
    cout << "  • Same for all subclasses\n";
    cout << "  • Not overridden\n";
    cout << "  • Example: boilWater(), pourInCup()\n";
    cout << "\nHook Methods:\n";
    cout << "  • Optional override points\n";
    cout << "  • Default implementation provided\n";
    cout << "  • Subclasses can customize\n";
    cout << "  • Example: customerWantsCondiments()\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "┌─────────────────────────┐\n";
    cout << "│   AbstractClass         │\n";
    cout << "│   (Beverage)            │\n";
    cout << "│                         │\n";
    cout << "│ + templateMethod() {    │  ← Defines algorithm\n";
    cout << "│     step1();            │    structure\n";
    cout << "│     step2();            │\n";
    cout << "│     step3();            │\n";
    cout << "│   }                     │\n";
    cout << "│                         │\n";
    cout << "│ # step1()               │  ← Concrete method\n";
    cout << "│ # step2() = 0           │  ← Abstract method\n";
    cout << "│ # step3()               │  ← Hook method\n";
    cout << "└─────────────────────────┘\n";
    cout << "            △\n";
    cout << "            │ inherits\n";
    cout << "    ┌───────┴───────┐\n";
    cout << "    │               │\n";
    cout << "┌───────────┐  ┌───────────┐\n";
    cout << "│ ConcreteA │  │ ConcreteB │\n";
    cout << "│ (Coffee)  │  │ (Tea)     │\n";
    cout << "│           │  │           │\n";
    cout << "│ # step2() │  │ # step2() │  ← Override\n";
    cout << "│ # step3() │  │           │    abstract\n";
    cout << "└───────────┘  └───────────┘    methods\n";

    cout << "\n========================================\n";
    cout << "ADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Reuses common code in base class\n";
    cout << "• Controls algorithm structure\n";
    cout << "• Easy to add new implementations\n";
    cout << "• Enforces consistent algorithm sequence\n";
    cout << "• Reduces code duplication\n";
    cout << "• Follows Don't Repeat Yourself (DRY)\n";
    cout << "• Follows Hollywood Principle (Don't call us, we'll call you)\n";
    cout << "• Clear separation of variant and invariant parts\n";

    cout << "\n========================================\n";
    cout << "DISADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Requires inheritance (less flexible than composition)\n";
    cout << "• Difficult to add new steps to algorithm\n";
    cout << "• Can lead to class proliferation\n";
    cout << "• Subclasses depend on base class implementation\n";
    cout << "• Harder to understand flow (jumping between classes)\n";
    cout << "• Violates Liskov Substitution if not careful\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "• Make template method final/non-virtual\n";
    cout << "• Minimize number of abstract methods\n";
    cout << "• Use meaningful names for steps\n";
    cout << "• Document algorithm flow clearly\n";
    cout << "• Provide sensible defaults in hooks\n";
    cout << "• Keep template method simple\n";
    cout << "• Use protected access for step methods\n";
    cout << "• Consider final keyword in C++ (virtual final)\n";

    cout << "\n========================================\n";
    cout << "HOOK METHODS\n";
    cout << "========================================\n";
    cout << "What are Hooks:\n";
    cout << "  • Optional steps in algorithm\n";
    cout << "  • Have default implementation\n";
    cout << "  • Subclasses can override if needed\n";
    cout << "  • Provide flexibility without forcing implementation\n";
    cout << "\nWhen to Use Hooks:\n";
    cout << "  • Step is optional\n";
    cout << "  • Default behavior exists\n";
    cout << "  • Not all subclasses need customization\n";
    cout << "  • Want to provide extension points\n";
    cout << "\nExamples:\n";
    cout << "  • customerWantsCondiments() in Beverage\n";
    cout << "  • hasAdditionalFeatures() in HouseBuilder\n";
    cout << "  • canUseSpecialAbility() in GameAI\n";

    cout << "\n========================================\n";
    cout << "HOLLYWOOD PRINCIPLE\n";
    cout << "========================================\n";
    cout << "\"Don't call us, we'll call you\"\n";
    cout << "\nHow it applies:\n";
    cout << "  • High-level component (template) controls flow\n";
    cout << "  • Low-level components (subclasses) are called\n";
    cout << "  • Prevents dependency rot\n";
    cout << "  • Inversion of control\n";
    cout << "\nExample:\n";
    cout << "  • prepareBeverage() calls brew()\n";
    cout << "  • Subclass doesn't call parent methods\n";
    cout << "  • Parent orchestrates entire process\n";

    cout << "\n========================================\n";
    cout << "IMPLEMENTATION VARIATIONS\n";
    cout << "========================================\n";
    cout << "Classic Template Method:\n";
    cout << "  • Abstract base class\n";
    cout << "  • Pure virtual methods\n";
    cout << "  • Protected inheritance\n";
    cout << "  • Example: All examples above\n";
    cout << "\nNon-Virtual Template Interface (NVI):\n";
    cout << "  • Public non-virtual template method\n";
    cout << "  • Private virtual implementation methods\n";
    cout << "  • Better encapsulation\n";
    cout << "  • Separates interface from implementation\n";
    cout << "\nCRTP (Curiously Recurring Template Pattern):\n";
    cout << "  • Compile-time polymorphism\n";
    cout << "  • No virtual functions\n";
    cout << "  • Better performance\n";
    cout << "  • Example: template<typename T> class Base\n";

    cout << "\n========================================\n";
    cout << "COMMON ISSUES & SOLUTIONS\n";
    cout << "========================================\n";
    cout << "Too Many Abstract Methods:\n";
    cout << "  • Issue: Subclasses forced to implement many methods\n";
    cout << "  • Solution: Use hooks with defaults, split template\n";
    cout << "\nRigid Algorithm:\n";
    cout << "  • Issue: Hard to vary algorithm structure\n";
    cout << "  • Solution: Consider Strategy pattern instead\n";
    cout << "\nBase Class Changes:\n";
    cout << "  • Issue: Changing base affects all subclasses\n";
    cout << "  • Solution: Careful design, versioning, deprecation\n";
    cout << "\nTesting Difficulties:\n";
    cout << "  • Issue: Hard to test individual steps\n";
    cout << "  • Solution: Extract steps to separate methods\n";

    cout << "\n========================================\n";
    cout << "RELATED PATTERNS\n";
    cout << "========================================\n";
    cout << "Strategy:\n";
    cout << "  • Template Method uses inheritance\n";
    cout << "  • Strategy uses composition\n";
    cout << "  • Strategy more flexible at runtime\n";
    cout << "\nFactory Method:\n";
    cout << "  • Special case of Template Method\n";
    cout << "  • Focuses on object creation\n";
    cout << "  • Uses same inheritance structure\n";
    cout << "\nBuilder:\n";
    cout << "  • Template Method defines construction algorithm\n";
    cout << "  • Builder provides fluent interface\n";
    cout << "  • Can be combined\n";

    cout << "\n========================================\n";
    cout << "TESTING TEMPLATE METHODS\n";
    cout << "========================================\n";
    cout << "Unit Testing:\n";
    cout << "  • Test each concrete class independently\n";
    cout << "  • Verify each step is called\n";
    cout << "  • Test hook method overrides\n";
    cout << "\nIntegration Testing:\n";
    cout << "  • Test complete algorithm flow\n";
    cout << "  • Verify step sequence\n";
    cout << "  • Test with real data\n";
    cout << "\nMocking:\n";
    cout << "  • Mock abstract methods\n";
    cout << "  • Verify template method calls them\n";
    cout << "  • Check call order and count\n";

    cout << "\n========================================\n";
    cout << "DESIGN GUIDELINES\n";
    cout << "========================================\n";
    cout << "Do:\n";
    cout << "  • Keep template method simple\n";
    cout << "  • Use clear, descriptive names\n";
    cout << "  • Document the algorithm flow\n";
    cout << "  • Provide good default implementations\n";
    cout << "  • Make template method non-virtual\n";
    cout << "\nDon't:\n";
    cout << "  • Put business logic in template method\n";
    cout << "  • Make too many steps abstract\n";
    cout << "  • Change algorithm structure frequently\n";
    cout << "  • Use when composition would work better\n";
    cout << "  • Create deep inheritance hierarchies\n";

    cout << "\n========================================\n";
    cout << "TEMPLATE METHOD IN FRAMEWORKS\n";
    cout << "========================================\n";
    cout << "Testing Frameworks:\n";
    cout << "  • setUp() - prepare test environment\n";
    cout << "  • test() - run actual test\n";
    cout << "  • tearDown() - cleanup\n";
    cout << "  • Example: JUnit, Google Test\n";
    cout << "\nWeb Frameworks:\n";
    cout << "  • initialize() - setup request\n";
    cout << "  • processRequest() - handle logic\n";
    cout << "  • renderResponse() - send output\n";
    cout << "  • Example: Servlet lifecycle\n";
    cout << "\nGUI Frameworks:\n";
    cout << "  • paintComponent() template\n";
    cout << "  • Event handling sequences\n";
    cout << "  • Widget lifecycle methods\n";

    cout << "\n========================================\n";
    cout << "REFACTORING TO TEMPLATE METHOD\n";
    cout << "========================================\n";
    cout << "Signs You Need Template Method:\n";
    cout << "  • Similar code across multiple classes\n";
    cout << "  • Same algorithm with minor variations\n";
    cout << "  • Copy-pasted code with small changes\n";
    cout << "  • Parallel class hierarchies\n";
    cout << "\nRefactoring Steps:\n";
    cout << "  1. Identify common algorithm\n";
    cout << "  2. Create abstract base class\n";
    cout << "  3. Extract template method\n";
    cout << "  4. Identify variant steps → abstract methods\n";
    cout << "  5. Identify invariant steps → concrete methods\n";
    cout << "  6. Identify optional steps → hooks\n";
    cout << "  7. Move subclasses to inherit from base\n";

    cout << "\n========================================\n";
    cout << "PERFORMANCE CONSIDERATIONS\n";
    cout << "========================================\n";
    cout << "Virtual Function Overhead:\n";
    cout << "  • Virtual calls have small performance cost\n";
    cout << "  • Usually negligible\n";
    cout << "  • Consider CRTP for hot paths\n";
    cout << "\nMemory:\n";
    cout << "  • Each object has vtable pointer\n";
    cout << "  • Minimal memory overhead\n";
    cout << "  • Shared vtable across instances\n";
    cout << "\nOptimizations:\n";
    cout << "  • Use final keyword for leaf classes\n";
    cout << "  • Enable compiler optimizations\n";
    cout << "  • Profile before optimizing\n";

    return 0;
}
