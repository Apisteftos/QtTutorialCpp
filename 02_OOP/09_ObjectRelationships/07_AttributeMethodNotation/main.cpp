#include <print>
#include <string>
#include <vector>

// ============================================================
// UML MEMBER NOTATION IN C++
// ============================================================
// One small hierarchy that shows the notation from the README:
//
//   # name, baseSalary          protected attributes
//   - employeeId (read-only)    const member + getter, no setter
//   $ employeeCount / getEmployeeCount()   static members
//   + calculateSalary()*        abstract (pure virtual) method
//   / teamSize                  derived attribute (computed)
// ============================================================


// ============================================================
// EXAMPLE 1 — Abstract class, static counter, read-only member
// ============================================================
class Employee {
private:
    const std::string  m_employeeId;              // read-only: set once in the constructor
    static inline int  s_employeeCount = 0;       // $ static: shared by ALL employees

protected:
    std::string m_name;                           // # protected
    double      m_baseSalary;

public:
    Employee(const std::string& id, const std::string& name, double baseSalary)
        : m_employeeId(id), m_name(name), m_baseSalary(baseSalary)
    {
        ++s_employeeCount;
    }

    virtual ~Employee() { --s_employeeCount; }

    virtual double calculateSalary() const = 0;   // * abstract: no body here

    void displayInfo() const {
        std::println("  [{}] {}: {:.2f} EUR", m_employeeId, m_name, calculateSalary());
    }

    std::string getEmployeeId() const { return m_employeeId; }   // getter only, no setter

    static int getEmployeeCount() { return s_employeeCount; }    // $ static method
};

class Developer : public Employee {
public:
    Developer(const std::string& id, const std::string& name, double baseSalary)
        : Employee(id, name, baseSalary) {}

    double calculateSalary() const override { return m_baseSalary; }
};


// ============================================================
// EXAMPLE 2 — Derived attribute (/teamSize)
// ============================================================
class Manager : public Employee {
private:
    std::vector<Employee*> m_directReports;   // the only stored data
    double                 m_bonusPercentage;

public:
    Manager(const std::string& id, const std::string& name,
            double baseSalary, double bonusPercentage)
        : Employee(id, name, baseSalary), m_bonusPercentage(bonusPercentage) {}

    void addReport(Employee* e) { m_directReports.push_back(e); }

    // / derived: computed on demand, so it can never be out of sync with the list
    int teamSize() const { return static_cast<int>(m_directReports.size()); }

    double calculateSalary() const override {
        return m_baseSalary * (1.0 + m_bonusPercentage / 100.0);
    }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::println("=====================================================");
    std::println("Example 1: abstract class, static counter, read-only member");
    std::println("=====================================================");

    {
        // Employee e("E0", "x", 1.0);   // ❌ does not compile: Employee is abstract

        std::println("  Employees before: {}", Employee::getEmployeeCount());

        Developer anna("E1", "Anna", 4500);
        Developer ben ("E2", "Ben",  4200);

        anna.displayInfo();
        ben.displayInfo();

        std::println("  Employees now: {}", Employee::getEmployeeCount());
        std::println("  Anna's id (read-only): {}", anna.getEmployeeId());
    }
    std::println("  Employees after scope: {}", Employee::getEmployeeCount());


    std::println("");
    std::println("=====================================================");
    std::println("Example 2: derived attribute teamSize");
    std::println("=====================================================");

    {
        Manager   boss("M1", "Carla", 6000, 10);
        Developer anna("E1", "Anna",  4500);
        Developer ben ("E2", "Ben",   4200);

        std::println("  teamSize: {}", boss.teamSize());
        boss.addReport(&anna);
        std::println("  teamSize: {}", boss.teamSize());
        boss.addReport(&ben);
        std::println("  teamSize: {}", boss.teamSize());   // no separate field to update

        boss.displayInfo();   // calculateSalary() of Manager is called (virtual)
    }

    return 0;
}
