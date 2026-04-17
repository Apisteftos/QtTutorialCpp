#include <iostream>
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
// Example 4 — MCX context: Session ↔ Group
// A session is associated with a group — neither owns the other
// ============================================================
class McxGroup {
private:
    std::string m_groupId;
    int         m_priority;

public:
    McxGroup(const std::string& id, int priority)
        : m_groupId(id), m_priority(priority) {}

    std::string getId()       const { return m_groupId; }
    int         getPriority() const { return m_priority; }
};

class McxSession {
private:
    std::string  m_sessionId;
    McxGroup*    m_activeGroup;   // association — session does not own group

public:
    explicit McxSession(const std::string& id)
        : m_sessionId(id), m_activeGroup(nullptr) {}

    void joinGroup(McxGroup* group) {
        m_activeGroup = group;
        std::println("  Session {} joined group {} (priority {})",
                     m_sessionId, group->getId(), group->getPriority());
    }

    void leaveGroup() {
        if (m_activeGroup) {
            std::println("  Session {} left group {}", m_sessionId, m_activeGroup->getId());
            m_activeGroup = nullptr;
        }
    }

    void requestFloor() const {
        if (m_activeGroup)
            std::println("  Session {} requests floor in group {}",
                         m_sessionId, m_activeGroup->getId());
    }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Teacher — Student (basic association)\n";
    std::cout << "=====================================================\n";

    {
        Teacher teacher("Mr. Schmidt", "C++23");
        Student alice("Alice", 9.5);
        Student bob("Bob",   8.0);

        // Teacher and students exist independently
        teacher.teach(alice);
        teacher.teach(bob);
        teacher.gradeStudent(alice);
        teacher.gradeStudent(bob);

        std::cout << "\nBoth teacher and students exist independently.\n";
        std::cout << "Teacher does not own the students.\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Doctor — Patient (bidirectional association)\n";
    std::cout << "=====================================================\n";

    {
        Doctor doctor("Mueller");
        Patient p1("Kostas", &doctor);
        Patient p2("Andre",  &doctor);

        doctor.addPatient(&p1);
        doctor.addPatient(&p2);

        doctor.treatPatient(p1);
        doctor.treatPatient(p2);

        std::cout << "\nBoth Doctor and Patients exist independently.\n";
        std::cout << "If Doctor object is destroyed, Patients still exist.\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Driver — Vehicle (many-to-many)\n";
    std::cout << "=====================================================\n";

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

        std::cout << "\nVehicles and Drivers exist independently.\n";
        std::cout << "Same vehicle can be associated with multiple drivers.\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: MCX Session — Group (domain example)\n";
    std::cout << "=====================================================\n";

    {
        // Groups exist independently of sessions
        McxGroup groupAlpha("GRP-ALPHA", 1);
        McxGroup groupBeta ("GRP-BETA",  2);

        McxSession session1("SES-001");
        McxSession session2("SES-002");

        session1.joinGroup(&groupAlpha);
        session2.joinGroup(&groupAlpha);   // same group, two sessions

        session1.requestFloor();
        session2.requestFloor();

        session1.leaveGroup();
        session1.joinGroup(&groupBeta);    // session switches group
        session1.requestFloor();

        std::cout << "\nGroups and Sessions exist independently.\n";
        std::cout << "Sessions can switch groups. Groups survive without sessions.\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Summary: Association\n";
    std::cout << "=====================================================\n";
    std::cout << "  Relationship:  uses-a\n";
    std::cout << "  Ownership:     none — neither owns the other\n";
    std::cout << "  Lifetime:      both exist independently\n";
    std::cout << "  Storage:       pointer or reference passed from outside\n";
    std::cout << "  Destroy one:   the other still exists\n";
    std::cout << "  UML:           A ————————> B (solid line, open arrow)\n";

    return 0;
}
