#include <iostream>
#include <memory>
#include <vector>
#include <string>

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
        std::cout << "[Weapon] '" << name << "' created\n";
    }

    ~Weapon() {
        std::cout << "[Weapon] '" << name << "' destroyed\n";
    }

    void attack() const {
        std::cout << "Attacking with '" << name
                  << "' for " << damage << " damage!\n";
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
        std::cout << "[FileHandle] Opening '" << filename << "'\n";
    }

    ~FileHandle() {
        if (isOpen) close();
    }

    void close() {
        isOpen = false;
        std::cout << "[FileHandle] Closing '" << filename << "'\n";
    }

    void write(const std::string& data) const {
        if (isOpen)
            std::cout << "[FileHandle] Writing to '" << filename << "': " << data << '\n';
        else
            std::cout << "[FileHandle] Error — file is closed!\n";
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
        std::cout << "[inspect] Weapon: " << weapon->getName()
                  << " | damage: " << weapon->getDamage() << '\n';
    else
        std::cout << "[inspect] No weapon\n";
}

// Function that BORROWS via reference (preferred over raw pointer when non-null)
void useWeapon(const Weapon& weapon)
{
    weapon.attack();
}


int main()
{
    // =================================================
    // BASIC unique_ptr — automatic cleanup
    // =================================================
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Basic unique_ptr\n";
    std::cout << "=====================================================\n";

    {
        std::unique_ptr<Weapon> sword = std::make_unique<Weapon>("Sword", 50);
        sword->attack();
        std::cout << "Leaving scope...\n";
    }   // sword automatically destroyed here — no delete needed
    std::cout << "Back in main — sword is gone\n";


    // =================================================
    // CANNOT COPY — only move
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Cannot Copy — Only Move\n";
    std::cout << "=====================================================\n";

    std::unique_ptr<Weapon> bow = std::make_unique<Weapon>("Bow", 30);

    // std::unique_ptr<Weapon> bow2 = bow;   // ERROR — copy is deleted
    // std::unique_ptr<Weapon> bow2(bow);    // ERROR — same reason

    // Transfer ownership with std::move
    std::unique_ptr<Weapon> bow2 = std::move(bow);   // bow is now EMPTY (nullptr)

    std::cout << "bow  is: " << (bow  ? "valid" : "nullptr (ownership transferred)") << '\n';
    std::cout << "bow2 is: " << (bow2 ? "valid" : "nullptr") << '\n';
    bow2->attack();


    // =================================================
    // TRANSFERRING OWNERSHIP via function return
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Transfer Ownership via Function\n";
    std::cout << "=====================================================\n";

    std::unique_ptr<Weapon> axe = createWeapon("Battle Axe", 80);
    axe->attack();

    // Pass to another owner
    std::unique_ptr<Weapon> storedWeapon = std::move(axe);
    std::cout << "axe is now: " << (axe ? "valid" : "nullptr") << '\n';
    storedWeapon->attack();


    // =================================================
    // BORROWING — get raw pointer without ownership
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Borrowing (get() and references)\n";
    std::cout << "=====================================================\n";

    std::unique_ptr<Weapon> spear = std::make_unique<Weapon>("Spear", 60);

    // .get() returns raw pointer — borrow without transferring ownership
    inspectWeapon(spear.get());

    // Pass by reference — clean, no ownership transfer
    useWeapon(*spear);

    std::cout << "spear still valid: " << (spear ? "YES" : "NO") << '\n';


    // =================================================
    // unique_ptr with ARRAYS
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: unique_ptr with Arrays\n";
    std::cout << "=====================================================\n";

    std::unique_ptr<int[]> scores = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i)
        scores[i] = (i + 1) * 100;

    std::cout << "Scores: ";
    for (int i = 0; i < 5; ++i)
        std::cout << scores[i] << " ";
    std::cout << '\n';


    // =================================================
    // REAL-WORLD: RAII resource management
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: RAII — Resource Automatically Released\n";
    std::cout << "=====================================================\n";

    {
        std::unique_ptr<FileHandle> file =
            std::make_unique<FileHandle>("report.txt");

        file->write("Line 1: Sales data");
        file->write("Line 2: Q4 results");

        std::cout << "Leaving scope — file will be closed automatically\n";
    }   // FileHandle destructor called — file closed, no leak possible
    std::cout << "File is closed\n";


    // =================================================
    // RESETTING and RELEASING
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: reset() and release()\n";
    std::cout << "=====================================================\n";

    std::unique_ptr<Weapon> dagger = std::make_unique<Weapon>("Dagger", 20);
    std::cout << "dagger valid: " << (dagger ? "YES" : "NO") << '\n';

    // reset() — destroys current object, pointer becomes nullptr
    dagger.reset();
    std::cout << "After reset() — dagger valid: " << (dagger ? "YES" : "NO") << '\n';

    // reset() with new value — destroys old, takes new ownership
    dagger.reset(new Weapon("Magic Dagger", 45));
    std::cout << "After reset(new) — dagger valid: " << (dagger ? "YES" : "NO") << '\n';
    dagger->attack();

    // release() — gives up ownership WITHOUT deleting (you own it now!)
    Weapon* rawWeapon = dagger.release();   // dagger is now nullptr
    std::cout << "After release() — dagger valid: " << (dagger ? "YES" : "NO") << '\n';
    rawWeapon->attack();
    delete rawWeapon;   // YOU must delete it now
    rawWeapon = nullptr;


    // =================================================
    // unique_ptr in a vector
    // =================================================
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: vector of unique_ptr\n";
    std::cout << "=====================================================\n";

    std::vector<std::unique_ptr<Weapon>> arsenal;
    arsenal.push_back(std::make_unique<Weapon>("Crossbow", 40));
    arsenal.push_back(std::make_unique<Weapon>("Hammer",   70));
    arsenal.push_back(std::make_unique<Weapon>("Katana",   65));

    std::cout << "Arsenal:\n";
    for (const auto& w : arsenal)
        w->attack();

    std::cout << "\nLeaving main — arsenal destroyed\n";
    return 0;
}
