#include <string>
#include <vector>
#include <print>

// ============================================================
// ASSOCIATION — uses-a relationship
// ============================================================
// Two classes know about each other but neither owns the other.
// Both exist independently and have their own lifetimes.
//
// Key signal: objects are passed as parameters or stored as
//             references/pointers that were created OUTSIDE
// ============================================================


// ============================================================
// Example 1 — Basic association: Teacher ↔ Student
// ============================================================
class Student {
private:
    std::string m_name;
    double      m_grade;

public:
    Student(const std::string& name, double grade)
        : m_name(name), m_grade(grade) {}

    std::string getName()  const { return m_name;  }
    double      getGrade() const { return m_grade; }

    void learn(const std::string& topic) const {
        std::println("  {} is learning: {}", m_name, topic);
    }
};

class Teacher {
private:
    std::string m_name;
    std::string m_subject;

public:
    Teacher(const std::string& name, const std::string& subject)
        : m_name(name), m_subject(subject) {}

    std::string getName() const { return m_name; }

    // Association: Teacher uses Student — but does NOT own it
    void teach(Student& student) const {
        std::println("  {} teaches {} to {}", m_name, m_subject, student.getName());
        student.learn(m_subject);
    }

    void gradeStudent(const Student& student) const {
        std::println("  {} grades {}: {:.1f}", m_name, student.getName(), student.getGrade());
    }
};


// ============================================================
// Example 2 — Bidirectional association: Doctor ↔ Patient
// ============================================================
class Patient;   // forward declaration

class Doctor {
private:
    std::string              m_name;
    std::vector<Patient*>    m_patients;   // knows about patients, does NOT own

public:
    explicit Doctor(const std::string& name) : m_name(name) {}

    std::string getName() const { return m_name; }

    void addPatient(Patient* p)     { m_patients.push_back(p); }
    void treatPatient(Patient& p);  // defined after Patient
};

class Patient {
private:
    std::string m_name;
    Doctor*     m_doctor;   // knows about doctor, does NOT own

public:
    Patient(const std::string& name, Doctor* doctor = nullptr)
        : m_name(name), m_doctor(doctor) {}

    std::string getName()   const { return m_name; }
    Doctor*     getDoctor() const { return m_doctor; }

    void setDoctor(Doctor* d) {
        m_doctor = d;
        std::println("  {} is now assigned to Dr. {}", m_name, d->getName());
    }

    void describeCondition() const {
        std::println("  {} reports symptoms", m_name);
    }
};

void Doctor::treatPatient(Patient& p) {
    std::println("  Dr. {} is treating {}", m_name, p.getName());
    p.describeCondition();
}


// ============================================================
// Example 3 — Many-to-many association: Driver ↔ Vehicle
// ============================================================
class Vehicle {
private:
    std::string m_plate;
    std::string m_model;

public:
    Vehicle(const std::string& plate, const std::string& model)
        : m_plate(plate), m_model(model) {}

    std::string getPlate() const { return m_plate; }
    std::string getModel() const { return m_model; }
};

class Driver {
private:
    std::string              m_name;
    std::vector<Vehicle*>    m_assignedVehicles;   // association — does not own

public:
    explicit Driver(const std::string& name) : m_name(name) {}

    std::string getName() const { return m_name; }

    void assignVehicle(Vehicle* v) {
        m_assignedVehicles.push_back(v);
        std::println("  {} can now drive {} ({})", m_name, v->getModel(), v->getPlate());
    }

    void listVehicles() const {
        std::println("  {}'s vehicles:", m_name);
        for (const auto* v : m_assignedVehicles)
            std::println("    - {} ({})", v->getModel(), v->getPlate());
    }
};


// ============================================================
// Example 4 — User ↔ ChatRoom
// A user is associated with a chat room — neither owns the other
// ============================================================
class ChatRoom {
private:
    std::string m_name;
    int         m_maxUsers;

public:
    ChatRoom(const std::string& name, int maxUsers)
        : m_name(name), m_maxUsers(maxUsers) {}

    std::string getName()     const { return m_name;     }
    int         getMaxUsers() const { return m_maxUsers; }
};

class User {
private:
    std::string m_name;
    ChatRoom*   m_activeRoom;   // association — user does not own the room

public:
    explicit User(const std::string& name)
        : m_name(name), m_activeRoom(nullptr) {}

    void joinRoom(ChatRoom* room) {
        m_activeRoom = room;
        std::println("  {} joined room {} (max {} users)",
                     m_name, room->getName(), room->getMaxUsers());
    }

    void leaveRoom() {
        if (m_activeRoom) {
            std::println("  {} left room {}", m_name, m_activeRoom->getName());
            m_activeRoom = nullptr;
        }
    }

    void sendMessage(const std::string& text) const {
        if (m_activeRoom)
            std::println("  {} says in {}: {}", m_name, m_activeRoom->getName(), text);
    }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::println("=====================================================");
    std::println("Example 1: Teacher — Student (basic association)");
    std::println("=====================================================");

    {
        Teacher teacher("Mr. Schmidt", "C++23");
        Student alice("Alice", 9.5);
        Student bob("Bob",   8.0);

        // Teacher and students exist independently
        teacher.teach(alice);
        teacher.teach(bob);
        teacher.gradeStudent(alice);
        teacher.gradeStudent(bob);
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Doctor — Patient (bidirectional association)");
    std::println("=====================================================");

    {
        Doctor doctor("Mueller");
        Patient p1("Kostas", &doctor);
        Patient p2("Andre",  &doctor);

        doctor.addPatient(&p1);
        doctor.addPatient(&p2);

        doctor.treatPatient(p1);
        doctor.treatPatient(p2);
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Driver — Vehicle (many-to-many)");
    std::println("=====================================================");

    {
        // Vehicles exist independently
        Vehicle truck("HAN-TRK-01", "Mercedes Actros");
        Vehicle van  ("HAN-VAN-02", "VW Transporter");
        Vehicle car  ("HAN-CAR-03", "BMW 3 Series");

        // Drivers exist independently
        Driver driver1("Hans");
        Driver driver2("Klaus");

        // Association: drivers assigned to vehicles
        driver1.assignVehicle(&truck);
        driver1.assignVehicle(&van);
        driver2.assignVehicle(&van);    // same vehicle, different driver
        driver2.assignVehicle(&car);

        driver1.listVehicles();
        driver2.listVehicles();
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 4: User — ChatRoom");
    std::println("=====================================================");

    {
        // Rooms exist independently of users
        ChatRoom general("general", 100);
        ChatRoom random ("random",   50);

        User alice("Alice");
        User bob  ("Bob");

        alice.joinRoom(&general);
        bob.joinRoom(&general);       // same room, two users

        alice.sendMessage("Hello!");
        bob.sendMessage("Hi Alice!");

        alice.leaveRoom();
        alice.joinRoom(&random);      // user switches room
        alice.sendMessage("Anyone here?");
    }

    return 0;
}
