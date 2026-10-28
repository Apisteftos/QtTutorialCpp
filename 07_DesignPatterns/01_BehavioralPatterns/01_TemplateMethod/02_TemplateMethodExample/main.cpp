#include <string>
#include <vector>
#include <memory>
#include <print>

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
        std::println("");
        std::println("☕ Preparing {}...", getBeverageName());
        std::println("================================");

        boilWater();
        brew();
        pourInCup();

        // Hook method - optional step
        if (customerWantsCondiments()) {
            addCondiments();
        }

        std::println("✅ {} is ready!", getBeverageName());
    }

protected:
    // Concrete method - same for all subclasses
    void boilWater() {
        std::println("1. 💧 Boiling water...");
    }

    // Concrete method - same for all subclasses
    void pourInCup() {
        std::println("3. ☕ Pouring into cup...");
    }

    // Abstract methods - must be implemented by subclasses
    virtual void brew() = 0;
    virtual void addCondiments() = 0;
    virtual std::string getBeverageName() const = 0;

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
        std::println("2. ☕ Brewing coffee grounds...");
    }

    void addCondiments() override {
        std::println("4. 🥛 Adding sugar and milk...");
    }

    std::string getBeverageName() const override {
        return "Coffee";
    }
};

/**
 * Concrete Class: Tea
 */
class Tea : public Beverage {
protected:
    void brew() override {
        std::println("2. 🍵 Steeping tea bag...");
    }

    void addCondiments() override {
        std::println("4. 🍋 Adding lemon...");
    }

    std::string getBeverageName() const override {
        return "Tea";
    }
};

/**
 * Concrete Class: Hot Chocolate
 */
class HotChocolate : public Beverage {
protected:
    void brew() override {
        std::println("2. 🍫 Mixing chocolate powder...");
    }

    void addCondiments() override {
        std::println("4. 🍦 Adding whipped cream and marshmallows...");
    }

    std::string getBeverageName() const override {
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
    std::string getBeverageName() const override {
        return "Black Coffee";
    }

    // Override hook - no condiments
    bool customerWantsCondiments() const override {
        std::println("   ⚠️  No condiments requested");
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
    void mine(const std::string& path) {
        std::println("");
        std::println("⛏️  Starting data mining process...");
        std::println("===================================");

        std::string data = openFile(path);
        std::string rawData = extractData(data);
        std::string parsedData = parseData(rawData);
        std::string analysis = analyzeData(parsedData);
        sendReport(analysis);
        closeFile(path);

        std::println("✅ Data mining complete!");
    }

protected:
    // Concrete methods - common for all miners
    std::string openFile(const std::string& path) {
        std::println("1. 📂 Opening file: {}", path);
        return "file_data";
    }

    void closeFile(const std::string& path) {
        std::println("6. 🔒 Closing file: {}", path);
    }

    // Abstract methods - implemented by subclasses
    virtual std::string extractData(const std::string& data) = 0;
    virtual std::string parseData(const std::string& rawData) = 0;
    virtual std::string analyzeData(const std::string& data) = 0;
    virtual void sendReport(const std::string& analysis) = 0;
};

/**
 * Concrete Class: PDF Data Miner
 */
class PDFDataMiner : public DataMiner {
protected:
    std::string extractData(const std::string& data) override {
        std::println("2. 📄 Extracting data from PDF...");
        return "pdf_raw_data";
    }

    std::string parseData(const std::string& rawData) override {
        std::println("3. 🔍 Parsing PDF data...");
        return "pdf_parsed_data";
    }

    std::string analyzeData(const std::string& data) override {
        std::println("4. 📊 Analyzing PDF data...");
        return "pdf_analysis_results";
    }

    void sendReport(const std::string& analysis) override {
        std::println("5. 📧 Sending PDF report via email...");
    }
};

/**
 * Concrete Class: CSV Data Miner
 */
class CSVDataMiner : public DataMiner {
protected:
    std::string extractData(const std::string& data) override {
        std::println("2. 📊 Extracting data from CSV...");
        return "csv_raw_data";
    }

    std::string parseData(const std::string& rawData) override {
        std::println("3. 🔍 Parsing CSV columns and rows...");
        return "csv_parsed_data";
    }

    std::string analyzeData(const std::string& data) override {
        std::println("4. 📈 Running statistical analysis...");
        return "csv_analysis_results";
    }

    void sendReport(const std::string& analysis) override {
        std::println("5. 💾 Saving CSV report to database...");
    }
};

/**
 * Concrete Class: Word Data Miner
 */
class WordDataMiner : public DataMiner {
protected:
    std::string extractData(const std::string& data) override {
        std::println("2. 📝 Extracting text from Word document...");
        return "word_raw_data";
    }

    std::string parseData(const std::string& rawData) override {
        std::println("3. 🔍 Parsing Word document structure...");
        return "word_parsed_data";
    }

    std::string analyzeData(const std::string& data) override {
        std::println("4. 📖 Analyzing document content...");
        return "word_analysis_results";
    }

    void sendReport(const std::string& analysis) override {
        std::println("5. 📱 Sending report via SMS...");
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
        std::println("");
        std::println("🎮 {} taking turn...", getAIName());
        std::println("================================");

        collectResources();
        buildStructures();
        buildUnits();
        attackEnemy();

        // Hook - optional special ability
        if (canUseSpecialAbility()) {
            useSpecialAbility();
        }

        std::println("✅ Turn complete!");
    }

protected:
    // Concrete method - same for all AI
    void collectResources() {
        std::println("1. 💰 Collecting resources...");
    }

    // Abstract methods - each AI implements differently
    virtual void buildStructures() = 0;
    virtual void buildUnits() = 0;
    virtual void attackEnemy() = 0;
    virtual std::string getAIName() const = 0;

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
        std::println("2. 🏰 Building barracks and armories...");
    }

    void buildUnits() override {
        std::println("3. ⚔️  Training offensive units (soldiers, tanks)...");
    }

    void attackEnemy() override {
        std::println("4. 💥 Launching aggressive attack on enemy base!");
    }

    std::string getAIName() const override {
        return "Aggressive AI";
    }
};

/**
 * Concrete Class: Defensive AI
 */
class DefensiveAI : public GameAI {
protected:
    void buildStructures() override {
        std::println("2. 🏰 Building walls and towers...");
    }

    void buildUnits() override {
        std::println("3. 🛡️  Training defensive units (guards, archers)...");
    }

    void attackEnemy() override {
        std::println("4. 🎯 Defending territory, minimal offense...");
    }

    std::string getAIName() const override {
        return "Defensive AI";
    }
};

/**
 * Concrete Class: Balanced AI with Special Ability
 */
class BalancedAI : public GameAI {
protected:
    void buildStructures() override {
        std::println("2. 🏰 Building balanced mix of structures...");
    }

    void buildUnits() override {
        std::println("3. ⚖️  Training mixed army composition...");
    }

    void attackEnemy() override {
        std::println("4. 🎲 Strategic attack on weak points...");
    }

    std::string getAIName() const override {
        return "Balanced AI";
    }

    bool canUseSpecialAbility() const override {
        return true;
    }

    void useSpecialAbility() override {
        std::println("5. ⭐ Using tactical airstrike!");
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
        std::println("");
        std::println("🏗️  Building {}...", getHouseType());
        std::println("================================");

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

        std::println("✅ {} construction complete!", getHouseType());
    }

protected:
    // Concrete methods - same for all houses
    void layFoundation() {
        std::println("1. 🏗️  Laying foundation...");
    }

    void installWindows() {
        std::println("4. 🪟 Installing windows...");
    }

    void installDoors() {
        std::println("5. 🚪 Installing doors...");
    }

    // Abstract methods - different for each house type
    virtual void buildWalls() = 0;
    virtual void buildRoof() = 0;
    virtual void paintHouse() = 0;
    virtual std::string getHouseType() const = 0;

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
        std::println("2. 🪵 Building wooden walls...");
    }

    void buildRoof() override {
        std::println("3. 🏠 Installing wooden shingle roof...");
    }

    void paintHouse() override {
        std::println("7. 🎨 Applying wood stain (natural finish)...");
    }

    std::string getHouseType() const override {
        return "Wooden House";
    }
};

/**
 * Concrete Class: Brick House
 */
class BrickHouse : public HouseBuilder {
protected:
    void buildWalls() override {
        std::println("2. 🧱 Building brick walls...");
    }

    void buildRoof() override {
        std::println("3. 🏠 Installing clay tile roof...");
    }

    void paintHouse() override {
        std::println("7. 🎨 Painting walls (white color)...");
    }

    std::string getHouseType() const override {
        return "Brick House";
    }
};

/**
 * Concrete Class: Luxury Villa
 */
class LuxuryVilla : public HouseBuilder {
protected:
    void buildWalls() override {
        std::println("2. 🏛️  Building reinforced concrete walls...");
    }

    void buildRoof() override {
        std::println("3. 🏠 Installing premium metal roof...");
    }

    void paintHouse() override {
        std::println("7. 🎨 Professional painting (designer colors)...");
    }

    std::string getHouseType() const override {
        return "Luxury Villa";
    }

    bool hasAdditionalFeatures() const override {
        return true;
    }

    void addFeatures() override {
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateBeveragePreparation() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BEVERAGE PREPARATION   ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Making Different Beverages ---");

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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: DATA MINING            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: GAME AI                ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: HOUSE CONSTRUCTION     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

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
    std::println("========================================");
    std::println("TEMPLATE METHOD PATTERN DEMO");
    std::println("========================================");

    demonstrateBeveragePreparation();
    demonstrateDataMining();
    demonstrateGameAI();
    demonstrateHouseBuilding();

    return 0;
}
