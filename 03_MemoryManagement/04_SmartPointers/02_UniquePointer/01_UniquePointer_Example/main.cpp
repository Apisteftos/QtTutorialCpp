#include <memory>
#include <print>
#include <string>
#include <vector>

// =============================================================
// unique_ptr — exclusive ownership
// Only ONE owner at a time. Automatically deleted when owner
// goes out of scope. Cannot be copied, only moved.
// =============================================================

class Weapon {
private:
    std::string name;
    int         damage;

public:
    Weapon(const std::string& name, int damage)
        : name(name), damage(damage) {
        std::println("[Weapon] '{}' created", this->name);
    }

    ~Weapon() {
        std::println("[Weapon] '{}' destroyed", name);
    }

    void attack() const {
        std::println("Attacking with '{}' for {} damage!", name, damage);
    }

    std::string getName()   const { return name;   }
    int         getDamage() const { return damage; }
};


class FileHandle {
private:
    std::string filename;
    bool        isOpen;

public:
    FileHandle(const std::string& filename) : filename(filename), isOpen(true) {
        std::println("[FileHandle] Opening '{}'", this->filename);
    }

    ~FileHandle() {
        if (isOpen) close();
    }

    void close() {
        isOpen = false;
        std::println("[FileHandle] Closing '{}'", filename);
    }

    void write(const std::string& data) const {
        if (isOpen)
            std::println("[FileHandle] Writing to '{}': {}", filename, data);
        else
            std::println("[FileHandle] Error — file is closed!");
    }
};


// Function that TRANSFERS ownership (returns unique_ptr)
std::unique_ptr<Weapon> createWeapon(const std::string& name, int damage)
{
    return std::make_unique<Weapon>(name, damage);   // ownership transferred to caller
}

// Function that BORROWS without taking ownership (raw pointer parameter)
void inspectWeapon(const Weapon* weapon)
{
    if (weapon)
        std::println("[inspect] Weapon: {} | damage: {}", weapon->getName(), weapon->getDamage());
    else
        std::println("[inspect] No weapon");
}

// Function that BORROWS via reference (preferred over raw pointer when non-null)
void useWeapon(const Weapon& weapon)
{
    weapon.attack();
}


int main()
{
    // === EXAMPLE 1: Basic unique_ptr — automatic cleanup ===
    std::println("=====================================================");
    std::println("Example 1: Basic unique_ptr");
    std::println("=====================================================");

    {
        std::unique_ptr<Weapon> sword = std::make_unique<Weapon>("Sword", 50);
        sword->attack();
        std::println("Leaving scope...");
    }   // sword automatically destroyed here — no delete needed
    std::println("Back in main — sword is gone");


    // === EXAMPLE 2: Cannot copy — only move ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Cannot Copy — Only Move");
    std::println("=====================================================");

    std::unique_ptr<Weapon> bow = std::make_unique<Weapon>("Bow", 30);

    // std::unique_ptr<Weapon> bow2 = bow;   // ERROR — copy is deleted
    // std::unique_ptr<Weapon> bow2(bow);    // ERROR — same reason

    // Transfer ownership with std::move
    std::unique_ptr<Weapon> bow2 = std::move(bow);   // bow is now EMPTY (nullptr)

    std::println("bow  is: {}", bow  ? "valid" : "nullptr (ownership transferred)");
    std::println("bow2 is: {}", bow2 ? "valid" : "nullptr");
    bow2->attack();


    // === EXAMPLE 3: Transferring ownership via function return ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Transfer Ownership via Function");
    std::println("=====================================================");

    std::unique_ptr<Weapon> axe = createWeapon("Battle Axe", 80);
    axe->attack();

    // Pass to another owner
    std::unique_ptr<Weapon> storedWeapon = std::move(axe);
    std::println("axe is now: {}", axe ? "valid" : "nullptr");
    storedWeapon->attack();


    // === EXAMPLE 4: Borrowing — get() and references ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Borrowing (get() and references)");
    std::println("=====================================================");

    std::unique_ptr<Weapon> spear = std::make_unique<Weapon>("Spear", 60);

    // .get() returns raw pointer — borrow without transferring ownership
    inspectWeapon(spear.get());

    // Pass by reference — clean, no ownership transfer
    useWeapon(*spear);

    std::println("spear still valid: {}", spear ? "YES" : "NO");


    // === EXAMPLE 5: unique_ptr with arrays ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: unique_ptr with Arrays");
    std::println("=====================================================");

    std::unique_ptr<int[]> scores = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i)
        scores[i] = (i + 1) * 100;

    std::print("Scores: ");
    for (int i = 0; i < 5; ++i)
        std::print("{} ", scores[i]);
    std::println("");


    // === EXAMPLE 6: RAII — resource automatically released ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: RAII — Resource Automatically Released");
    std::println("=====================================================");

    {
        std::unique_ptr<FileHandle> file =
            std::make_unique<FileHandle>("report.txt");

        file->write("Line 1: Sales data");
        file->write("Line 2: Q4 results");

        std::println("Leaving scope — file will be closed automatically");
    }   // FileHandle destructor called — file closed, no leak possible
    std::println("File is closed");


    // === EXAMPLE 7: reset() and release() ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: reset() and release()");
    std::println("=====================================================");

    std::unique_ptr<Weapon> dagger = std::make_unique<Weapon>("Dagger", 20);
    std::println("dagger valid: {}", dagger ? "YES" : "NO");

    // reset() — destroys current object, pointer becomes nullptr
    dagger.reset();
    std::println("After reset() — dagger valid: {}", dagger ? "YES" : "NO");

    // reset() with new value — destroys old, takes new ownership
    dagger.reset(new Weapon("Magic Dagger", 45));
    std::println("After reset(new) — dagger valid: {}", dagger ? "YES" : "NO");
    dagger->attack();

    // release() — gives up ownership WITHOUT deleting (you own it now!)
    Weapon* rawWeapon = dagger.release();   // dagger is now nullptr
    std::println("After release() — dagger valid: {}", dagger ? "YES" : "NO");
    rawWeapon->attack();
    delete rawWeapon;   // YOU must delete it now
    rawWeapon = nullptr;


    // === EXAMPLE 8: vector of unique_ptr ===
    std::println("");
    std::println("=====================================================");
    std::println("Example 8: vector of unique_ptr");
    std::println("=====================================================");

    std::vector<std::unique_ptr<Weapon>> arsenal;
    arsenal.push_back(std::make_unique<Weapon>("Crossbow", 40));
    arsenal.push_back(std::make_unique<Weapon>("Hammer",   70));
    arsenal.push_back(std::make_unique<Weapon>("Katana",   65));

    std::println("Arsenal:");
    for (const auto& w : arsenal)
        w->attack();

    std::println("");
    std::println("Leaving main — arsenal destroyed");
    return 0;
}
