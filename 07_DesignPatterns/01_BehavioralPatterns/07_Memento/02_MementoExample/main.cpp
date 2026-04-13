#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stack>
using namespace std;

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
    string text;      // Saved text state
    int cursorPos;    // Saved cursor position



    // Private constructor - only TextEditor can create
    TextMemento(const string& t, int pos) : text(t), cursorPos(pos) {}

public:
    // Getters for saved state
    string getText() const { return text; }
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
    string text;         // Current text
    int cursorPosition;  // Current cursor position

public:
    TextEditor() : text(""), cursorPosition(0) {}

    // Modify the text
    void write(const string& newText) {
        text += newText;
        cursorPosition = text.length();
        cout << "✍️  Wrote: \"" << newText << "\"\n";
        cout << "   Current text: \"" << text << "\"\n";
    }

    // Delete last n characters
    void deleteText(int count) {
        if (count > text.length()) count = text.length();
        text = text.substr(0, text.length() - count);
        cursorPosition = text.length();
        cout << "🗑️  Deleted " << count << " characters\n";
        cout << "   Current text: \"" << text << "\"\n";
    }

    // Set cursor position
    void setCursor(int pos) {
        if (pos >= 0 && pos <= text.length()) {
            cursorPosition = pos;
            cout << "📍 Cursor moved to position " << pos << "\n";
        }
    }

    // Create a memento (save state)
    shared_ptr<TextMemento> save() {
        cout << "💾 Saving state...\n";
        return shared_ptr<TextMemento>(new TextMemento(text, cursorPosition));
    }

    // Restore from memento
    void restore(shared_ptr<TextMemento> memento) {
        text = memento->getText();
        cursorPosition = memento->getCursorPos();
        cout << "⏮️  Restored to: \"" << text << "\"\n";
    }

    // Display current state
    void display() const {
        cout << "📄 Current text: \"" << text << "\"\n";
        cout << "   Cursor at: " << cursorPosition << "\n";
    }
};

/**
 * Caretaker Class: History Manager
 * Manages mementos (undo/redo stacks)
 */
class History {
private:
    stack<shared_ptr<TextMemento>> undoStack;  // Stack for undo
    stack<shared_ptr<TextMemento>> redoStack;  // Stack for redo
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
            cout << "❌ Nothing to undo!\n";
            return;
        }

        cout << "↩️  Performing UNDO...\n";
        // Save current state to redo stack
        redoStack.push(editor->save());
        // Restore previous state
        shared_ptr<TextMemento> memento = undoStack.top();
        undoStack.pop();
        editor->restore(memento);
    }

    // Redo last undone operation
    void redo() {
        if (redoStack.empty()) {
            cout << "❌ Nothing to redo!\n";
            return;
        }

        cout << "↪️  Performing REDO...\n";
        // Save current state to undo stack
        undoStack.push(editor->save());
        // Restore next state
        shared_ptr<TextMemento> memento = redoStack.top();
        redoStack.pop();
        editor->restore(memento);
    }

    // Show history info
    void showHistory() {
        cout << "📚 History: " << undoStack.size() << " undo states, "
             << redoStack.size() << " redo states\n";
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
    string location;

    friend class GameCharacter;

    GameMemento(int h, int m, int l, const string& loc)
        : health(h), mana(m), level(l), location(loc) {}

public:
    int getHealth() const { return health; }
    int getMana() const { return mana; }
    int getLevel() const { return level; }
    string getLocation() const { return location; }
};

/**
 * Originator: Game Character
 */
class GameCharacter {
private:
    string name;
    int health;
    int maxHealth;
    int mana;
    int maxMana;
    int level;
    string currentLocation;

public:
    GameCharacter(const string& n)
        : name(n), health(100), maxHealth(100),
        mana(50), maxMana(50), level(1),
        currentLocation("Starting Village") {}

    // Take damage
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
        cout << "💔 " << name << " took " << damage << " damage! Health: "
             << health << "/" << maxHealth << "\n";
    }

    // Use mana
    void useMana(int amount) {
        mana -= amount;
        if (mana < 0) mana = 0;
        cout << "✨ " << name << " used " << amount << " mana! Mana: "
             << mana << "/" << maxMana << "\n";
    }

    // Level up
    void levelUp() {
        level++;
        maxHealth += 20;
        maxMana += 10;
        health = maxHealth;
        mana = maxMana;
        cout << "⬆️  " << name << " leveled up to level " << level << "!\n";
        cout << "   Health: " << health << ", Mana: " << mana << "\n";
    }

    // Move to new location
    void moveTo(const string& location) {
        currentLocation = location;
        cout << "🗺️  " << name << " moved to " << location << "\n";
    }

    // Create checkpoint (save state)
    shared_ptr<GameMemento> createCheckpoint() {
        cout << "💾 Creating checkpoint at " << currentLocation << "...\n";
        return shared_ptr<GameMemento>(new GameMemento(health, mana, level, currentLocation));
    }

    // Load checkpoint (restore state)
    void loadCheckpoint(shared_ptr<GameMemento> memento) {
        health = memento->getHealth();
        mana = memento->getMana();
        level = memento->getLevel();
        currentLocation = memento->getLocation();
        cout << "⏮️  Checkpoint loaded!\n";
        displayStatus();
    }

    // Display current status
    void displayStatus() const {
        cout << "🎮 " << name << " - Level " << level << "\n";
        cout << "   ❤️  Health: " << health << "/" << maxHealth << "\n";
        cout << "   ✨ Mana: " << mana << "/" << maxMana << "\n";
        cout << "   📍 Location: " << currentLocation << "\n";
    }
};

/**
 * Caretaker: Game Save Manager
 */
class SaveManager {
private:
    vector<shared_ptr<GameMemento>> saveSlots;

public:
    // Save to specific slot
    void saveGame(shared_ptr<GameMemento> memento, int slot) {
        if (slot >= saveSlots.size()) {
            saveSlots.resize(slot + 1);
        }
        saveSlots[slot] = memento;
        cout << "✅ Game saved to slot " << slot << "\n";
    }

    // Load from specific slot
    shared_ptr<GameMemento> loadGame(int slot) {
        if (slot >= saveSlots.size() || !saveSlots[slot]) {
            cout << "❌ No save found in slot " << slot << "\n";
            return nullptr;
        }
        cout << "📂 Loading game from slot " << slot << "...\n";
        return saveSlots[slot];
    }

    // List all saves
    void listSaves() {
        cout << "\n📋 Save Slots:\n";
        for (size_t i = 0; i < saveSlots.size(); i++) {
            if (saveSlots[i]) {
                cout << "   Slot " << i << ": Level "
                     << saveSlots[i]->getLevel() << " at "
                     << saveSlots[i]->getLocation() << "\n";
            } else {
                cout << "   Slot " << i << ": Empty\n";
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
    vector<string> shapes;  // Saved shapes
    string backgroundColor;

    friend class Canvas;

    CanvasMemento(const vector<string>& s, const string& bg)
        : shapes(s), backgroundColor(bg) {}
};

/**
 * Originator: Drawing Canvas
 */
class Canvas {
private:
    vector<string> shapes;
    string backgroundColor;

public:
    Canvas() : backgroundColor("white") {}

    // Add shape to canvas
    void addShape(const string& shape) {
        shapes.push_back(shape);
        cout << "🎨 Added " << shape << " to canvas\n";
    }

    // Remove last shape
    void removeLastShape() {
        if (!shapes.empty()) {
            string removed = shapes.back();
            shapes.pop_back();
            cout << "🗑️  Removed " << removed << "\n";
        }
    }

    // Change background color
    void setBackgroundColor(const string& color) {
        backgroundColor = color;
        cout << "🎨 Background changed to " << color << "\n";
    }

    // Create snapshot
    shared_ptr<CanvasMemento> createSnapshot() {
        cout << "📸 Creating snapshot...\n";
        return shared_ptr<CanvasMemento>(new CanvasMemento(shapes, backgroundColor));
    }

    // Restore from snapshot
    void restoreSnapshot(shared_ptr<CanvasMemento> memento) {
        shapes = memento->shapes;
        backgroundColor = memento->backgroundColor;
        cout << "⏮️  Canvas restored from snapshot\n";
        display();
    }

    // Display canvas
    void display() const {
        cout << "🖼️  Canvas (Background: " << backgroundColor << "):\n";
        if (shapes.empty()) {
            cout << "   [Empty canvas]\n";
        } else {
            for (const string& shape : shapes) {
                cout << "   - " << shape << "\n";
            }
        }
    }
};

/**
 * Caretaker: Snapshot Manager
 */
class SnapshotManager {
private:
    vector<shared_ptr<CanvasMemento>> snapshots;
    Canvas* canvas;

public:
    SnapshotManager(Canvas* c) : canvas(c) {}

    // Take snapshot
    void takeSnapshot() {
        snapshots.push_back(canvas->createSnapshot());
        cout << "✅ Snapshot " << snapshots.size() << " saved\n";
    }

    // Restore specific snapshot
    void restoreSnapshot(int index) {
        if (index < 0 || index >= snapshots.size()) {
            cout << "❌ Invalid snapshot index!\n";
            return;
        }
        canvas->restoreSnapshot(snapshots[index]);
    }

    // List all snapshots
    void listSnapshots() {
        cout << "\n📷 Snapshots: " << snapshots.size() << " total\n";
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
    vector<string> transactionLog;

    friend class BankAccount;

    AccountMemento(double bal, const vector<string>& log)
        : balance(bal), transactionLog(log) {}
};

/**
 * Originator: Bank Account
 */
class BankAccount {
private:
    string accountNumber;
    double balance;
    vector<string> transactionLog;

public:
    BankAccount(const string& accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance) {
        transactionLog.push_back("Account opened with $" + to_string(initialBalance));
    }

    // Deposit money
    void deposit(double amount) {
        balance += amount;
        string transaction = "Deposited $" + to_string(amount);
        transactionLog.push_back(transaction);
        cout << "💰 " << transaction << " | Balance: $" << balance << "\n";
    }

    // Withdraw money
    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "❌ Insufficient funds!\n";
            return false;
        }
        balance -= amount;
        string transaction = "Withdrew $" + to_string(amount);
        transactionLog.push_back(transaction);
        cout << "💸 " << transaction << " | Balance: $" << balance << "\n";
        return true;
    }

    // Begin transaction (save state)
    shared_ptr<AccountMemento> beginTransaction() {
        cout << "🔄 Beginning transaction...\n";
        return shared_ptr<AccountMemento>(new AccountMemento(balance, transactionLog));
    }

    // Rollback transaction
    void rollback(shared_ptr<AccountMemento> memento) {
        balance = memento->balance;
        transactionLog = memento->transactionLog;
        cout << "↩️  Transaction rolled back! Balance: $" << balance << "\n";
    }

    // Display account info
    void displayAccount() const {
        cout << "🏦 Account: " << accountNumber << "\n";
        cout << "   Balance: $" << balance << "\n";
        cout << "   Recent transactions:\n";
        int start = max(0, (int)transactionLog.size() - 3);
        for (int i = start; i < transactionLog.size(); i++) {
            cout << "   - " << transactionLog[i] << "\n";
        }
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateTextEditor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: TEXT EDITOR            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    TextEditor editor;
    History history(&editor);

    cout << "--- Writing Text ---\n";
    history.backup();
    editor.write("Hello ");

    history.backup();
    editor.write("World");

    history.backup();
    editor.write("!");

    cout << "\n--- Undoing ---\n";
    history.undo();  // Remove "!"

    history.undo();  // Remove "World"

    cout << "\n--- Redoing ---\n";
    history.redo();  // Add back "World"

    cout << "\n--- New Action (clears redo) ---\n";
    history.backup();
    editor.write(" from C++");

    history.showHistory();
}

void demonstrateGameCheckpoint() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: GAME CHECKPOINTS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    GameCharacter player("Hero");
    SaveManager saveManager;

    cout << "--- Starting Game ---\n";
    player.displayStatus();

    // Create first checkpoint
    saveManager.saveGame(player.createCheckpoint(), 0);

    cout << "\n--- Progress in Game ---\n";
    player.moveTo("Dark Forest");
    player.takeDamage(30);
    player.useMana(20);
    player.levelUp();

    // Save to slot 1
    saveManager.saveGame(player.createCheckpoint(), 1);

    cout << "\n--- More Progress ---\n";
    player.moveTo("Dragon's Lair");
    player.takeDamage(60);
    player.useMana(30);

    cout << "\n--- Player Dies, Loading Checkpoint ---\n";
    saveManager.listSaves();
    auto save = saveManager.loadGame(1);
    if (save) {
        player.loadCheckpoint(save);
    }
}

void demonstrateCanvas() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: DRAWING CANVAS         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Canvas canvas;
    SnapshotManager snapshotMgr(&canvas);

    cout << "--- Drawing Shapes ---\n";
    canvas.addShape("Circle");
    canvas.addShape("Rectangle");
    snapshotMgr.takeSnapshot();  // Snapshot 0

    canvas.addShape("Triangle");
    canvas.setBackgroundColor("blue");
    canvas.display();
    snapshotMgr.takeSnapshot();  // Snapshot 1

    cout << "\n--- More Changes ---\n";
    canvas.addShape("Star");
    canvas.setBackgroundColor("red");
    canvas.display();

    cout << "\n--- Restore Previous Version ---\n";
    snapshotMgr.restoreSnapshot(0);  // Back to first snapshot
}

void demonstrateTransaction() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: BANK TRANSACTION       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    BankAccount account("ACC-12345", 1000.0);
    account.displayAccount();

    cout << "\n--- Valid Transaction ---\n";
    auto savePoint1 = account.beginTransaction();
    account.deposit(500);
    account.withdraw(200);
    account.displayAccount();

    cout << "\n--- Failed Transaction (Rollback) ---\n";
    auto savePoint2 = account.beginTransaction();
    account.deposit(100);
    account.withdraw(5000);  // This will fail

    cout << "Rolling back failed transaction...\n";
    account.rollback(savePoint2);
    account.displayAccount();
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "MEMENTO PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateTextEditor();
    demonstrateGameCheckpoint();
    demonstrateCanvas();
    demonstrateTransaction();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF MEMENTO PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Preserves encapsulation: Internal state not exposed\n";
    cout << "✅ Simplifies originator: Doesn't track state history\n";
    cout << "✅ Undo/Redo functionality: Easy to implement\n";
    cout << "✅ Rollback support: Transaction-like behavior\n";
    cout << "✅ State snapshots: Save/restore at any point\n";
    cout << "✅ Time travel: Navigate through state history\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE MEMENTO\n";
    cout << "========================================\n";
    cout << "1. Need to save and restore object state\n";
    cout << "2. Implement undo/redo functionality\n";
    cout << "3. Want to preserve encapsulation boundaries\n";
    cout << "4. Direct interface to state would expose implementation\n";
    cout << "5. Need transaction rollback capability\n";
    cout << "6. Create checkpoints or snapshots\n";

    cout << "\n========================================\n";
    cout << "MEMENTO vs COMMAND\n";
    cout << "========================================\n";
    cout << "Memento Pattern:\n";
    cout << "  • Stores complete object state\n";
    cout << "  • Used for undo by restoring state\n";
    cout << "  • Better for complex state\n";
    cout << "  • Example: Save entire document state\n";
    cout << "\nCommand Pattern:\n";
    cout << "  • Stores operations/actions\n";
    cout << "  • Used for undo by reversing operations\n";
    cout << "  • Better for simple operations\n";
    cout << "  • Example: Store 'delete 5 chars' command\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Text editors (undo/redo)\n";
    cout << "• Photo editors (history states)\n";
    cout << "• Video games (save points/checkpoints)\n";
    cout << "• Database transactions (rollback)\n";
    cout << "• Version control systems (commits)\n";
    cout << "• Browser history (back/forward)\n";
    cout << "• Drawing applications (snapshots)\n";
    cout << "• Form wizards (step back/forward)\n";
    cout << "• Simulation software (state rewind)\n";
    cout << "• Configuration management (backup/restore)\n";

    cout << "\n========================================\n";
    cout << "KEY COMPONENTS\n";
    cout << "========================================\n";
    cout << "1. Memento: Stores internal state of originator\n";
    cout << "2. Originator: Creates memento and restores from it\n";
    cout << "3. Caretaker: Manages mementos (history/save manager)\n";
    cout << "4. State: The data being saved/restored\n";

    cout << "\n========================================\n";
    cout << "IMPLEMENTATION VARIATIONS\n";
    cout << "========================================\n";
    cout << "Wide Interface:\n";
    cout << "  • Memento exposes all state to originator\n";
    cout << "  • Used when originator needs full state access\n";
    cout << "  • Example: friend class relationship\n";
    cout << "\nNarrow Interface:\n";
    cout << "  • Memento hides state from caretaker\n";
    cout << "  • Caretaker can't inspect or modify state\n";
    cout << "  • Better encapsulation\n";
    cout << "\nIncremental Memento:\n";
    cout << "  • Stores only changes, not full state\n";
    cout << "  • Saves memory for large objects\n";
    cout << "  • More complex to implement\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "┌──────────────┐\n";
    cout << "│  Caretaker   │\n";
    cout << "│ (History)    │────┐\n";
    cout << "└──────────────┘    │\n";
    cout << "                    │ manages\n";
    cout << "┌──────────────┐    ▼\n";
    cout << "│  Originator  │  ┌──────────────┐\n";
    cout << "│ (Editor)     │  │   Memento    │\n";
    cout << "│ + save()     │──│ (Snapshot)   │\n";
    cout << "│ + restore()  │  └──────────────┘\n";
    cout << "└──────────────┘\n";

    cout << "\n========================================\n";
    cout << "ADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Doesn't violate encapsulation\n";
    cout << "• Simplifies originator code\n";
    cout << "• Easy to implement undo/redo\n";
    cout << "• Can save state at any point\n";
    cout << "• Supports multiple save points\n";
    cout << "• Caretaker doesn't depend on originator internals\n";

    cout << "\n========================================\n";
    cout << "DISADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Can be expensive if state is large\n";
    cout << "• Requires memory for each memento\n";
    cout << "• May need to copy large amounts of data\n";
    cout << "• Caretaker must track memento lifecycle\n";
    cout << "• Can create many mementos if saved frequently\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "• Use smart pointers for automatic memory management\n";
    cout << "• Limit number of mementos to prevent memory issues\n";
    cout << "• Consider incremental saves for large objects\n";
    cout << "• Make mementos immutable\n";
    cout << "• Use friend classes to maintain encapsulation\n";
    cout << "• Compress mementos if they're large\n";
    cout << "• Implement memento expiration/cleanup\n";

    cout << "\n========================================\n";
    cout << "OPTIMIZATION TECHNIQUES\n";
    cout << "========================================\n";
    cout << "Delta Encoding:\n";
    cout << "  • Store only differences between states\n";
    cout << "  • Reduces memory usage significantly\n";
    cout << "  • Example: Store 'insert 5 chars' instead of full text\n";
    cout << "\nCompression:\n";
    cout << "  • Compress memento data before storage\n";
    cout << "  • Trade CPU time for memory savings\n";
    cout << "  • Useful for large state data\n";
    cout << "\nCopy-on-Write:\n";
    cout << "  • Share unchanged parts between mementos\n";
    cout << "  • Only copy modified sections\n";
    cout << "  • Reduces memory and improves performance\n";
    cout << "\nLimited History:\n";
    cout << "  • Keep only N most recent mementos\n";
    cout << "  • Automatically discard old states\n";
    cout << "  • Prevents unbounded memory growth\n";

    cout << "\n========================================\n";
    cout << "COMMON USE CASES\n";
    cout << "========================================\n";
    cout << "Undo/Redo Systems:\n";
    cout << "  • Text/code editors\n";
    cout << "  • Graphic design tools\n";
    cout << "  • Maintain dual stacks (undo/redo)\n";
    cout << "\nGame Saves:\n";
    cout << "  • Save player progress\n";
    cout << "  • Multiple save slots\n";
    cout << "  • Quick save/load functionality\n";
    cout << "\nTransactions:\n";
    cout << "  • Database rollback\n";
    cout << "  • Financial operations\n";
    cout << "  • All-or-nothing execution\n";
    cout << "\nSnapshots:\n";
    cout << "  • Virtual machine states\n";
    cout << "  • Application configuration\n";
    cout << "  • System restore points\n";

    return 0;
}
