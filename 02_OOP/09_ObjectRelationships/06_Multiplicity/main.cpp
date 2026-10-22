#include <array>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

// ============================================================
// MULTIPLICITY (CARDINALITY) — how many objects are connected
// ============================================================
// The relationship TYPE says what kind of link it is.
// The MULTIPLICITY says how many objects sit on each side.
// It decides the C++ storage type:
//
//   1       -> value member or reference
//   0..1    -> nullable pointer / std::optional
//   0..*    -> std::vector<T>
//   n       -> std::array<T, n>
//   m..n    -> std::vector<T> + validation in constructor/setter
// ============================================================


// ============================================================
// EXAMPLE 1 — One-to-one:  Person "1" -- "1" Passport
// Exactly one: a reference member MUST be bound, so it can't be null
// ============================================================
class Passport {
private:
    std::string m_number;

public:
    explicit Passport(const std::string& number) : m_number(number) {}
    std::string getNumber() const { return m_number; }
};

class Person {
private:
    std::string     m_name;
    const Passport& m_passport;   // multiplicity 1 -> reference, never null

public:
    Person(const std::string& name, const Passport& passport)
        : m_name(name), m_passport(passport) {}

    void describe() const {
        std::println("  {} has passport {}", m_name, m_passport.getNumber());
    }
};


// ============================================================
// EXAMPLE 2 — One-to-many:  Library "1" o-- "0..*" Book
// Zero or more: std::vector
// ============================================================
class Book {
private:
    std::string m_title;

public:
    explicit Book(const std::string& title) : m_title(title) {}
    std::string getTitle() const { return m_title; }
};

class Library {
private:
    std::vector<const Book*> m_books;   // 0..* -> vector (does not own the books)

public:
    void add(const Book* b) { m_books.push_back(b); }

    void list() const {
        std::println("  Library has {} book(s):", m_books.size());
        for (const auto* b : m_books)
            std::println("    - {}", b->getTitle());
    }
};


// ============================================================
// EXAMPLE 3 — Many-to-many:  Student "0..*" -- "0..*" Course
// A vector on BOTH sides
// ============================================================
class Course;

class Student {
private:
    std::string           m_name;
    std::vector<Course*>  m_courses;

public:
    explicit Student(const std::string& name) : m_name(name) {}

    std::string getName()     const { return m_name; }
    size_t      courseCount() const { return m_courses.size(); }
    void        addCourse(Course* c) { m_courses.push_back(c); }
};

class Course {
private:
    std::string            m_title;
    std::vector<Student*>  m_students;

public:
    explicit Course(const std::string& title) : m_title(title) {}

    std::string getTitle()     const { return m_title; }
    size_t      studentCount() const { return m_students.size(); }
    void        addStudent(Student* s) { m_students.push_back(s); }
};

// Link both sides, so both vectors stay consistent
void enroll(Student& s, Course& c) {
    s.addCourse(&c);
    c.addStudent(&s);
    std::println("  {} enrolled in {}", s.getName(), c.getTitle());
}


// ============================================================
// EXAMPLE 4 — Composition with an exact number:  Car "1" *-- "4" Wheel
// Exactly n: std::array<T, n>
// ============================================================
class Wheel {
private:
    int m_position;

public:
    explicit Wheel(int position) : m_position(position) {
        std::println("    [Wheel {} created]", m_position);
    }

    ~Wheel() {
        std::println("    [Wheel {} destroyed]", m_position);
    }
};

class Car {
private:
    std::array<Wheel, 4> m_wheels;   // exactly 4 -> array, owned by value

public:
    Car() : m_wheels{Wheel(1), Wheel(2), Wheel(3), Wheel(4)} {}

    size_t wheelCount() const { return m_wheels.size(); }
};


// ============================================================
// EXAMPLE 5 — Optional and self-association
//             Employee "0..1" manager  /  Employee "0..*" reports
// 0..1 -> nullable pointer, 0..* -> vector (both inside the same class)
// ============================================================
class Employee {
private:
    std::string             m_name;
    Employee*               m_manager = nullptr;   // 0..1 (the boss has none)
    std::vector<Employee*>  m_reports;             // 0..*

public:
    explicit Employee(const std::string& name) : m_name(name) {}

    // this employee becomes the manager of e
    void manage(Employee& e) {
        e.m_manager = this;
        m_reports.push_back(&e);
    }

    void describe() const {
        if (m_manager)
            std::println("  {} reports to {}, manages {} employee(s)",
                         m_name, m_manager->m_name, m_reports.size());
        else
            std::println("  {} has no manager, manages {} employee(s)",
                         m_name, m_reports.size());
    }
};


// ============================================================
// EXAMPLE 6 — Bounded range:  Team "1" o-- "5..30" Player
// m..n: vector + validation (the upper bound is enforced when adding,
//       the lower bound is checked by isValid())
// ============================================================
class Team {
private:
    std::vector<std::string> m_players;   // 5..30

public:
    static constexpr size_t MIN_PLAYERS = 5;
    static constexpr size_t MAX_PLAYERS = 30;

    void addPlayer(const std::string& name) {
        if (m_players.size() >= MAX_PLAYERS)
            throw std::logic_error("Team is full");
        m_players.push_back(name);
    }

    size_t size()    const { return m_players.size(); }
    bool   isValid() const { return m_players.size() >= MIN_PLAYERS; }
};


// ============================================================
// EXAMPLE 7 — Multiplicity for enums
//   single enum value  -> implicit 1, nothing written
//   several values     -> attribute multiplicity [1..*] -> vector
// ============================================================
enum class Color { Red, Green, Blue };

std::string toString(Color c) {
    switch (c) {
    case Color::Red:   return "Red";
    case Color::Green: return "Green";
    case Color::Blue:  return "Blue";
    }
    return "?";
}

class TrafficLight {
private:
    Color m_current = Color::Red;   // exactly one color at a time

public:
    Color current() const { return m_current; }
    void  set(Color c)    { m_current = c; }
};

class Shirt {
private:
    std::vector<Color> m_availableColors;   // [1..*]

public:
    explicit Shirt(std::vector<Color> colors) : m_availableColors(std::move(colors)) {
        if (m_availableColors.empty())
            throw std::invalid_argument("Shirt needs at least one color");
    }

    size_t colorCount() const { return m_availableColors.size(); }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::println("=====================================================");
    std::println("Example 1: Person — Passport (1 to 1)");
    std::println("=====================================================");

    {
        Passport passport("C01X00T47");
        Person   alice("Alice", passport);   // a Person cannot exist without a Passport
        alice.describe();
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Library — Book (1 to 0..*)");
    std::println("=====================================================");

    {
        Book b1("Clean Code"), b2("Effective C++"), b3("The C++ Programming Language");

        Library empty;                // 0 books is allowed
        empty.list();

        Library lib;
        lib.add(&b1);
        lib.add(&b2);
        lib.add(&b3);
        lib.list();
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Student — Course (0..* to 0..*)");
    std::println("=====================================================");

    {
        Student alice("Alice"), bob("Bob");
        Course  cpp("C++"), math("Math");

        enroll(alice, cpp);
        enroll(alice, math);
        enroll(bob,   cpp);

        std::println("  Alice: {} course(s), Bob: {} course(s)", alice.courseCount(), bob.courseCount());
        std::println("  C++: {} student(s), Math: {} student(s)", cpp.studentCount(), math.studentCount());
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Car — Wheel (1 to exactly 4)");
    std::println("=====================================================");

    {
        std::println("  Building car:");
        Car car;
        std::println("  Car has {} wheels", car.wheelCount());
        std::println("  Leaving scope:");
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Employee — Employee (0..1 manager, 0..* reports)");
    std::println("=====================================================");

    {
        Employee boss("Boss"), anna("Anna"), ben("Ben"), carl("Carl");

        boss.manage(anna);
        boss.manage(carl);
        anna.manage(ben);   // every employee has at most ONE manager

        boss.describe();
        anna.describe();
        ben.describe();
        carl.describe();
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 6: Team — Player (1 to 5..30)");
    std::println("=====================================================");

    {
        Team team;
        for (int i = 1; i <= 4; ++i) team.addPlayer("Player" + std::to_string(i));
        std::println("  {} players, valid: {}", team.size(), team.isValid());

        team.addPlayer("Player5");
        std::println("  {} players, valid: {}", team.size(), team.isValid());

        while (team.size() < Team::MAX_PLAYERS) team.addPlayer("Extra");
        std::println("  {} players (maximum)", team.size());

        try {
            team.addPlayer("One too many");
        } catch (const std::logic_error& e) {
            std::println("  Rejected: {}", e.what());
        }
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 7: Enums (exactly one value vs [1..*] values)");
    std::println("=====================================================");

    {
        TrafficLight light;
        std::println("  TrafficLight is {}", toString(light.current()));
        light.set(Color::Green);
        std::println("  TrafficLight is {}", toString(light.current()));

        Shirt shirt({Color::Red, Color::Blue});
        std::println("  Shirt comes in {} color(s)", shirt.colorCount());

        try {
            Shirt none({});
        } catch (const std::invalid_argument& e) {
            std::println("  Rejected: {}", e.what());
        }
    }

    return 0;
}
