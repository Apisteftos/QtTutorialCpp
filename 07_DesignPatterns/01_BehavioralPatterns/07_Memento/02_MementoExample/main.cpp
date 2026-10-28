#include <string>
#include <vector>
#include <memory>
#include <stack>
#include <print>

// ============================================
// EXAMPLE 1: TEXT EDITOR WITH UNDO/REDO
// Classic memento pattern example
// ============================================

/**
 * Memento Class
 * Stores the internal state of the Originator
 * Immutable - state cannot be changed after creation
 */
class TextMemento {
private:
    std::string text;      // Saved text state
    int cursorPos;    // Saved cursor position



    // Private constructor - only TextEditor can create
    TextMemento(const std::string& t, int pos) : text(t), cursorPos(pos) {}

public:
    // Getters for saved state
    std::string getText() const { return text; }
    int getCursorPos() const { return cursorPos; }


    // Only Originator can create mementos
    friend class TextEditor;
};

/**
 * Originator Class: Text Editor
 * Creates mementos and can restore from them
 */
class TextEditor {
private:
    std::string text;         // Current text
    int cursorPosition;  // Current cursor position

public:
    TextEditor() : text(""), cursorPosition(0) {}

    // Modify the text
    void write(const std::string& newText) {
        text += newText;
        cursorPosition = text.length();
        std::println("✍️  Wrote: \"{}\"", newText);
        std::println("   Current text: \"{}\"", text);
    }

    // Delete last n characters
    void deleteText(int count) {
        if (count > text.length()) count = text.length();
        text = text.substr(0, text.length() - count);
        cursorPosition = text.length();
        std::println("🗑️  Deleted {} characters", count);
        std::println("   Current text: \"{}\"", text);
    }

    // Set cursor position
    void setCursor(int pos) {
        if (pos >= 0 && pos <= text.length()) {
            cursorPosition = pos;
            std::println("📍 Cursor moved to position {}", pos);
        }
    }

    // Create a memento (save state)
    std::shared_ptr<TextMemento> save() {
        std::println("💾 Saving state...");
        return std::shared_ptr<TextMemento>(new TextMemento(text, cursorPosition));
    }

    // Restore from memento
    void restore(std::shared_ptr<TextMemento> memento) {
        text = memento->getText();
        cursorPosition = memento->getCursorPos();
        std::println("⏮️  Restored to: \"{}\"", text);
    }

    // Display current state
    void display() const {
        std::println("📄 Current text: \"{}\"", text);
        std::println("   Cursor at: {}", cursorPosition);
    }
};

/**
 * Caretaker Class: History Manager
 * Manages mementos (undo/redo stacks)
 */
class History {
private:
    std::stack<std::shared_ptr<TextMemento>> undoStack;  // Stack for undo
    std::stack<std::shared_ptr<TextMemento>> redoStack;  // Stack for redo
    TextEditor* editor;

public:
    History(TextEditor* ed) : editor(ed) {}

    // Save current state for undo
    void backup() {
        undoStack.push(editor->save());
        // Clear redo stack when new action performed
        while (!redoStack.empty()) redoStack.pop();
    }

    // Undo last operation
    void undo() {
        if (undoStack.empty()) {
            std::println("❌ Nothing to undo!");
            return;
        }

        std::println("↩️  Performing UNDO...");
        // Save current state to redo stack
        redoStack.push(editor->save());
        // Restore previous state
        std::shared_ptr<TextMemento> memento = undoStack.top();
        undoStack.pop();
        editor->restore(memento);
    }

    // Redo last undone operation
    void redo() {
        if (redoStack.empty()) {
            std::println("❌ Nothing to redo!");
            return;
        }

        std::println("↪️  Performing REDO...");
        // Save current state to undo stack
        undoStack.push(editor->save());
        // Restore next state
        std::shared_ptr<TextMemento> memento = redoStack.top();
        redoStack.pop();
        editor->restore(memento);
    }

    // Show history info
    void showHistory() {
        std::println("📚 History: {} undo states, {} redo states", undoStack.size(), redoStack.size());
    }
};

// ============================================
// EXAMPLE 2: GAME CHARACTER CHECKPOINT
// Save and restore game state
// ============================================

/**
 * Game State Memento
 */
class GameMemento {
private:
    int health;
    int mana;
    int level;
    std::string location;

    friend class GameCharacter;

    GameMemento(int h, int m, int l, const std::string& loc)
        : health(h), mana(m), level(l), location(loc) {}

public:
    int getHealth() const { return health; }
    int getMana() const { return mana; }
    int getLevel() const { return level; }
    std::string getLocation() const { return location; }
};

/**
 * Originator: Game Character
 */
class GameCharacter {
private:
    std::string name;
    int health;
    int maxHealth;
    int mana;
    int maxMana;
    int level;
    std::string currentLocation;

public:
    GameCharacter(const std::string& n)
        : name(n), health(100), maxHealth(100),
        mana(50), maxMana(50), level(1),
        currentLocation("Starting Village") {}

    // Take damage
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
        std::println("💔 {} took {} damage! Health: {}/{}", name, damage, health, maxHealth);
    }

    // Use mana
    void useMana(int amount) {
        mana -= amount;
        if (mana < 0) mana = 0;
        std::println("✨ {} used {} mana! Mana: {}/{}", name, amount, mana, maxMana);
    }

    // Level up
    void levelUp() {
        level++;
        maxHealth += 20;
        maxMana += 10;
        health = maxHealth;
        mana = maxMana;
        std::println("⬆️  {} leveled up to level {}!", name, level);
        std::println("   Health: {}, Mana: {}", health, mana);
    }

    // Move to new location
    void moveTo(const std::string& location) {
        currentLocation = location;
        std::println("🗺️  {} moved to {}", name, location);
    }

    // Create checkpoint (save state)
    std::shared_ptr<GameMemento> createCheckpoint() {
        std::println("💾 Creating checkpoint at {}...", currentLocation);
        return std::shared_ptr<GameMemento>(new GameMemento(health, mana, level, currentLocation));
    }

    // Load checkpoint (restore state)
    void loadCheckpoint(std::shared_ptr<GameMemento> memento) {
        health = memento->getHealth();
        mana = memento->getMana();
        level = memento->getLevel();
        currentLocation = memento->getLocation();
        std::println("⏮️  Checkpoint loaded!");
        displayStatus();
    }

    // Display current status
    void displayStatus() const {
        std::println("🎮 {} - Level {}", name, level);
        std::println("   ❤️  Health: {}/{}", health, maxHealth);
        std::println("   ✨ Mana: {}/{}", mana, maxMana);
        std::println("   📍 Location: {}", currentLocation);
    }
};

/**
 * Caretaker: Game Save Manager
 */
class SaveManager {
private:
    std::vector<std::shared_ptr<GameMemento>> saveSlots;

public:
    // Save to specific slot
    void saveGame(std::shared_ptr<GameMemento> memento, int slot) {
        if (slot >= saveSlots.size()) {
            saveSlots.resize(slot + 1);
        }
        saveSlots[slot] = memento;
        std::println("✅ Game saved to slot {}", slot);
    }

    // Load from specific slot
    std::shared_ptr<GameMemento> loadGame(int slot) {
        if (slot >= saveSlots.size() || !saveSlots[slot]) {
            std::println("❌ No save found in slot {}", slot);
            return nullptr;
        }
        std::println("📂 Loading game from slot {}...", slot);
        return saveSlots[slot];
    }

    // List all saves
    void listSaves() {
        std::println("");
        std::println("📋 Save Slots:");
        for (size_t i = 0; i < saveSlots.size(); i++) {
            if (saveSlots[i]) {
                std::println("   Slot {}: Level {} at {}",
                    i, saveSlots[i]->getLevel(), saveSlots[i]->getLocation());
            } else {
                std::println("   Slot {}: Empty", i);
            }
        }
    }
};

// ============================================
// EXAMPLE 3: DRAWING CANVAS WITH SNAPSHOTS
// Graphic editor state management
// ============================================

/**
 * Canvas State Memento
 */
class CanvasMemento {
private:
    std::vector<std::string> shapes;  // Saved shapes
    std::string backgroundColor;

    friend class Canvas;

    CanvasMemento(const std::vector<std::string>& s, const std::string& bg)
        : shapes(s), backgroundColor(bg) {}
};

/**
 * Originator: Drawing Canvas
 */
class Canvas {
private:
    std::vector<std::string> shapes;
    std::string backgroundColor;

public:
    Canvas() : backgroundColor("white") {}

    // Add shape to canvas
    void addShape(const std::string& shape) {
        shapes.push_back(shape);
        std::println("🎨 Added {} to canvas", shape);
    }

    // Remove last shape
    void removeLastShape() {
        if (!shapes.empty()) {
            std::string removed = shapes.back();
            shapes.pop_back();
            std::println("🗑️  Removed {}", removed);
        }
    }

    // Change background color
    void setBackgroundColor(const std::string& color) {
        backgroundColor = color;
        std::println("🎨 Background changed to {}", color);
    }

    // Create snapshot
    std::shared_ptr<CanvasMemento> createSnapshot() {
        std::println("📸 Creating snapshot...");
        return std::shared_ptr<CanvasMemento>(new CanvasMemento(shapes, backgroundColor));
    }

    // Restore from snapshot
    void restoreSnapshot(std::shared_ptr<CanvasMemento> memento) {
        shapes = memento->shapes;
        backgroundColor = memento->backgroundColor;
        std::println("⏮️  Canvas restored from snapshot");
        display();
    }

    // Display canvas
    void display() const {
        std::println("🖼️  Canvas (Background: {}):", backgroundColor);
        if (shapes.empty()) {
            std::println("   [Empty canvas]");
        } else {
            for (const std::string& shape : shapes) {
                std::println("   - {}", shape);
            }
        }
    }
};

/**
 * Caretaker: Snapshot Manager
 */
class SnapshotManager {
private:
    std::vector<std::shared_ptr<CanvasMemento>> snapshots;
    Canvas* canvas;

public:
    SnapshotManager(Canvas* c) : canvas(c) {}

    // Take snapshot
    void takeSnapshot() {
        snapshots.push_back(canvas->createSnapshot());
        std::println("✅ Snapshot {} saved", snapshots.size());
    }

    // Restore specific snapshot
    void restoreSnapshot(int index) {
        if (index < 0 || index >= snapshots.size()) {
            std::println("❌ Invalid snapshot index!");
            return;
        }
        canvas->restoreSnapshot(snapshots[index]);
    }

    // List all snapshots
    void listSnapshots() {
        std::println("");
        std::println("📷 Snapshots: {} total", snapshots.size());
    }
};

// ============================================
// EXAMPLE 4: TRANSACTION MANAGER
// Database-like rollback functionality
// ============================================

/**
 * Account State Memento
 */
class AccountMemento {
private:
    double balance;
    std::vector<std::string> transactionLog;

    friend class BankAccount;

    AccountMemento(double bal, const std::vector<std::string>& log)
        : balance(bal), transactionLog(log) {}
};

/**
 * Originator: Bank Account
 */
class BankAccount {
private:
    std::string accountNumber;
    double balance;
    std::vector<std::string> transactionLog;

public:
    BankAccount(const std::string& accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance) {
        transactionLog.push_back("Account opened with $" + std::to_string(initialBalance));
    }

    // Deposit money
    void deposit(double amount) {
        balance += amount;
        std::string transaction = "Deposited $" + std::to_string(amount);
        transactionLog.push_back(transaction);
        std::println("💰 {} | Balance: ${}", transaction, balance);
    }

    // Withdraw money
    bool withdraw(double amount) {
        if (amount > balance) {
            std::println("❌ Insufficient funds!");
            return false;
        }
        balance -= amount;
        std::string transaction = "Withdrew $" + std::to_string(amount);
        transactionLog.push_back(transaction);
        std::println("💸 {} | Balance: ${}", transaction, balance);
        return true;
    }

    // Begin transaction (save state)
    std::shared_ptr<AccountMemento> beginTransaction() {
        std::println("🔄 Beginning transaction...");
        return std::shared_ptr<AccountMemento>(new AccountMemento(balance, transactionLog));
    }

    // Rollback transaction
    void rollback(std::shared_ptr<AccountMemento> memento) {
        balance = memento->balance;
        transactionLog = memento->transactionLog;
        std::println("↩️  Transaction rolled back! Balance: ${}", balance);
    }

    // Display account info
    void displayAccount() const {
        std::println("🏦 Account: {}", accountNumber);
        std::println("   Balance: ${}", balance);
        std::println("   Recent transactions:");
        int start = std::max(0, (int)transactionLog.size() - 3);
        for (int i = start; i < transactionLog.size(); i++) {
            std::println("   - {}", transactionLog[i]);
        }
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateTextEditor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: TEXT EDITOR            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    TextEditor editor;
    History history(&editor);

    std::println("--- Writing Text ---");
    history.backup();
    editor.write("Hello ");

    history.backup();
    editor.write("World");

    history.backup();
    editor.write("!");

    std::println("");
    std::println("--- Undoing ---");
    history.undo();  // Remove "!"

    history.undo();  // Remove "World"

    std::println("");
    std::println("--- Redoing ---");
    history.redo();  // Add back "World"

    std::println("");
    std::println("--- New Action (clears redo) ---");
    history.backup();
    editor.write(" from C++");

    history.showHistory();
}

void demonstrateGameCheckpoint() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: GAME CHECKPOINTS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    GameCharacter player("Hero");
    SaveManager saveManager;

    std::println("--- Starting Game ---");
    player.displayStatus();

    // Create first checkpoint
    saveManager.saveGame(player.createCheckpoint(), 0);

    std::println("");
    std::println("--- Progress in Game ---");
    player.moveTo("Dark Forest");
    player.takeDamage(30);
    player.useMana(20);
    player.levelUp();

    // Save to slot 1
    saveManager.saveGame(player.createCheckpoint(), 1);

    std::println("");
    std::println("--- More Progress ---");
    player.moveTo("Dragon's Lair");
    player.takeDamage(60);
    player.useMana(30);

    std::println("");
    std::println("--- Player Dies, Loading Checkpoint ---");
    saveManager.listSaves();
    auto save = saveManager.loadGame(1);
    if (save) {
        player.loadCheckpoint(save);
    }
}

void demonstrateCanvas() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: DRAWING CANVAS         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Canvas canvas;
    SnapshotManager snapshotMgr(&canvas);

    std::println("--- Drawing Shapes ---");
    canvas.addShape("Circle");
    canvas.addShape("Rectangle");
    snapshotMgr.takeSnapshot();  // Snapshot 0

    canvas.addShape("Triangle");
    canvas.setBackgroundColor("blue");
    canvas.display();
    snapshotMgr.takeSnapshot();  // Snapshot 1

    std::println("");
    std::println("--- More Changes ---");
    canvas.addShape("Star");
    canvas.setBackgroundColor("red");
    canvas.display();

    std::println("");
    std::println("--- Restore Previous Version ---");
    snapshotMgr.restoreSnapshot(0);  // Back to first snapshot
}

void demonstrateTransaction() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: BANK TRANSACTION       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    BankAccount account("ACC-12345", 1000.0);
    account.displayAccount();

    std::println("");
    std::println("--- Valid Transaction ---");
    auto savePoint1 = account.beginTransaction();
    account.deposit(500);
    account.withdraw(200);
    account.displayAccount();

    std::println("");
    std::println("--- Failed Transaction (Rollback) ---");
    auto savePoint2 = account.beginTransaction();
    account.deposit(100);
    account.withdraw(5000);  // This will fail

    std::println("Rolling back failed transaction...");
    account.rollback(savePoint2);
    account.displayAccount();
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("MEMENTO PATTERN DEMO");
    std::println("========================================");

    demonstrateTextEditor();
    demonstrateGameCheckpoint();
    demonstrateCanvas();
    demonstrateTransaction();

    return 0;
}
