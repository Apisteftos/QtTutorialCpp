#include <algorithm>
#include <string>
#include <vector>
#include <print>

// ============================================================
// AGGREGATION — has-a relationship (weak ownership)
// ============================================================
// The whole CONTAINS the parts, but the parts can exist
// WITHOUT the whole. The whole does NOT own the lifetime
// of the parts.
//
// Key signal: parts stored as POINTERS or REFERENCES
//             that were created OUTSIDE the whole.
// ============================================================


// ============================================================
// Example 1 — Department has Employees (classic aggregation)
// ============================================================
class Employee {
private:
    std::string m_name;
    std::string m_role;
    double      m_salary;

public:
    Employee(const std::string& name, const std::string& role, double salary)
        : m_name(name), m_role(role), m_salary(salary) {}

    std::string getName()   const { return m_name;   }
    std::string getRole()   const { return m_role;   }
    double      getSalary() const { return m_salary; }

    void work() const {
        std::println("  {} ({}) is working", m_name, m_role);
    }
};

class Department {
private:
    std::string              m_name;
    std::vector<Employee*>   m_employees;   // aggregation — stores pointers, does NOT own

public:
    explicit Department(const std::string& name) : m_name(name) {}

    // Destructor does NOT delete employees — they exist independently
    ~Department() {
        std::println("  [Department '{}' destroyed — employees still exist]", m_name);
    }

    void addEmployee(Employee* e) {
        m_employees.push_back(e);
        std::println("  {} joined department '{}'", e->getName(), m_name);
    }

    void removeEmployee(Employee* e) {
        auto it = std::find(m_employees.begin(), m_employees.end(), e);
        if (it != m_employees.end()) {
            m_employees.erase(it);
            std::println("  {} left department '{}'", e->getName(), m_name);
        }
    }

    void listEmployees() const {
        std::println("  Department '{}' employees:", m_name);
        for (const auto* e : m_employees)
            std::println("    - {} ({})", e->getName(), e->getRole());
    }

    void allWork() const {
        for (const auto* e : m_employees)
            e->work();
    }

    size_t size() const { return m_employees.size(); }
};


// ============================================================
// Example 2 — University has Courses (aggregation)
// Courses exist before/after the university
// ============================================================
class Course {
private:
    std::string m_title;
    int         m_credits;

public:
    Course(const std::string& title, int credits)
        : m_title(title), m_credits(credits) {}

    std::string getTitle()   const { return m_title;   }
    int         getCredits() const { return m_credits; }
};

class University {
private:
    std::string              m_name;
    std::vector<Course*>     m_courses;    // aggregation — does NOT own courses

public:
    explicit University(const std::string& name) : m_name(name) {}

    ~University() {
        std::println("  [University '{}' destroyed — courses still exist]", m_name);
    }

    void offerCourse(Course* c) {
        m_courses.push_back(c);
        std::println("  '{}' now offers: {} ({} credits)",
                     m_name, c->getTitle(), c->getCredits());
    }

    void listCourses() const {
        std::println("  Courses at '{}':", m_name);
        for (const auto* c : m_courses)
            std::println("    - {} ({} credits)", c->getTitle(), c->getCredits());
    }
};


// ============================================================
// Example 3 — Playlist has Songs (aggregation)
// Songs exist independently — can be in multiple playlists
// ============================================================
class Song {
private:
    std::string m_title;
    std::string m_artist;
    int         m_durationSec;

public:
    Song(const std::string& title, const std::string& artist, int duration)
        : m_title(title), m_artist(artist), m_durationSec(duration) {}

    std::string getTitle()    const { return m_title;       }
    std::string getArtist()   const { return m_artist;      }
    int         getDuration() const { return m_durationSec; }

    void play() const {
        std::println("  ♪ {} — {} ({}s)", m_title, m_artist, m_durationSec);
    }
};

class Playlist {
private:
    std::string           m_name;
    std::vector<Song*>    m_songs;   // aggregation — does NOT own songs

public:
    explicit Playlist(const std::string& name) : m_name(name) {}

    ~Playlist() {
        std::println("  [Playlist '{}' destroyed — songs still exist]", m_name);
    }

    void addSong(Song* s) { m_songs.push_back(s); }

    void play() const {
        std::println("  Playing playlist '{}':", m_name);
        int total = 0;
        for (const auto* s : m_songs) {
            s->play();
            total += s->getDuration();
        }
        std::println("  Total duration: {}s", total);
    }

    size_t count() const { return m_songs.size(); }
};


// ============================================================
// Example 4 — Team has Players
// Players can join/leave teams — they exist independently
// ============================================================
class Player {
private:
    std::string m_id;
    std::string m_nickname;

public:
    Player(const std::string& id, const std::string& nickname)
        : m_id(id), m_nickname(nickname) {}

    std::string getId()       const { return m_id;       }
    std::string getNickname() const { return m_nickname; }

    void play() const {
        std::println("  [{}] {} is playing", m_id, m_nickname);
    }
};

class Team {
private:
    std::string           m_teamName;
    std::vector<Player*>  m_players;   // aggregation — does NOT own players

public:
    explicit Team(const std::string& name) : m_teamName(name) {}

    ~Team() {
        std::println("  [Team '{}' destroyed — players still exist]", m_teamName);
    }

    void signPlayer(Player* p) {
        m_players.push_back(p);
        std::println("  Player {} ({}) signed by team {}",
                     p->getId(), p->getNickname(), m_teamName);
    }

    void releasePlayer(Player* p) {
        auto it = std::find(m_players.begin(), m_players.end(), p);
        if (it != m_players.end()) {
            m_players.erase(it);
            std::println("  Player {} released by team {}", p->getId(), m_teamName);
        }
    }

    void showLineup() const {
        std::println("  Team {} lineup — {} player(s):",
                     m_teamName, m_players.size());
        for (const auto* p : m_players) p->play();
    }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::println("=====================================================");
    std::println("Example 1: Department — Employee");
    std::println("=====================================================");

    {
        // Employees created outside — exist independently
        Employee e1("Kostas", "Senior C++ Dev", 75000);
        Employee e2("Anna",   "Qt Developer",   68000);
        Employee e3("Marco",  "DevOps",         62000);

        {
            Department dev("Development");
            Department ops("Operations");

            dev.addEmployee(&e1);
            dev.addEmployee(&e2);
            ops.addEmployee(&e3);
            ops.addEmployee(&e2);   // Anna is in both departments!

            dev.listEmployees();
            ops.listEmployees();

            // Remove from one department — employee still exists
            dev.removeEmployee(&e2);
            dev.listEmployees();

        }   // departments destroyed here

        // Employees still alive after departments are gone
        std::println("");
        std::println("After departments destroyed:");
        e1.work();
        e2.work();
        e3.work();
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 2: University — Course");
    std::println("=====================================================");

    {
        // Courses exist independently
        Course cpp23("C++23 Programming", 6);
        Course qt6  ("Qt6 Development",   4);
        Course cmake("CMake Build System", 2);

        {
            University uni("Leibniz Universität Hannover");
            uni.offerCourse(&cpp23);
            uni.offerCourse(&qt6);
            uni.offerCourse(&cmake);
            uni.listCourses();
        }   // university destroyed

        // Courses still exist
        std::println("");
        std::println("Courses survive after university destroyed:");
        std::println("  {} ({} credits) still exists", cpp23.getTitle(), cpp23.getCredits());
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Playlist — Song");
    std::println("=====================================================");

    {
        // Songs exist independently
        Song s1("Comfortably Numb",  "Pink Floyd", 382);
        Song s2("Bohemian Rhapsody", "Queen",      354);
        Song s3("Hotel California",  "Eagles",     391);
        Song s4("Stairway to Heaven","Led Zeppelin",482);

        {
            Playlist rock("Classic Rock");
            Playlist favs("Favourites");

            rock.addSong(&s1);
            rock.addSong(&s2);
            rock.addSong(&s3);
            rock.addSong(&s4);

            favs.addSong(&s2);   // Bohemian Rhapsody in both playlists
            favs.addSong(&s4);

            rock.play();
            std::println("");
            favs.play();
        }   // playlists destroyed

        // Songs still exist
        std::println("");
        std::println("Songs survive after playlists destroyed:");
        s1.play();
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Team — Player");
    std::println("=====================================================");

    {
        Player p1("P-001", "Falcon");
        Player p2("P-002", "Viper");
        Player p3("P-003", "Rookie");

        {
            Team red("Red");
            red.signPlayer(&p1);
            red.signPlayer(&p2);
            red.signPlayer(&p3);

            red.showLineup();

            red.releasePlayer(&p3);   // Rookie leaves

            Team blue("Blue");
            blue.signPlayer(&p3);     // Rookie joins a different team

            red.showLineup();
            blue.showLineup();
        }   // teams destroyed

        std::println("");
        std::println("Players survive after teams destroyed:");
        p1.play();
    }

    return 0;
}
