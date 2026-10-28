#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
#include <print>

// ============================================
// READABILITY PRINCIPLE
// ============================================

/*
READABILITY PRINCIPLE:
    "Code should be written to be easily read and understood
     by humans, not just executed by machines."
    — Software Engineering Best Practice

WHAT IT MEANS:
    • Code is read 10x more than written
    • Clear code reduces bugs
    • Easy to understand = easy to maintain
    • Self-documenting code is best
    • Think about the next person reading it

WHY IT MATTERS:
    ✅ Reduces cognitive load
    ✅ Faster onboarding for new developers
    ✅ Fewer bugs from misunderstanding
    ✅ Easier code reviews
    ✅ Lower maintenance costs

KEY ASPECTS:
    • Meaningful names - Variables, functions, classes
    • Clear structure - Logical organization
    • Consistent formatting - Predictable layout
    • Appropriate comments - Why, not what
    • Simple expressions - Avoid complexity
    • Low nesting - Flatten control flow
    • No magic numbers - Use named constants
    • Good whitespace - Visual breathing room

READABILITY FACTORS:
    1. Naming conventions
    2. Code formatting
    3. Function size
    4. Nesting depth
    5. Expression clarity
    6. Comment quality
    7. Consistent style
    8. Logical flow

VIOLATION SIGNS:
    🚨 Need to re-read code multiple times
    🚨 Can't understand intent
    🚨 Cryptic variable names (a, x, tmp)
    🚨 Deep nesting (>3 levels)
    🚨 Magic numbers everywhere
    🚨 Inconsistent formatting
*/

// ============================================
// EXAMPLE 1: MEANINGFUL NAMES
// ============================================

namespace bad_example1 {
// ❌ BAD: Cryptic, unclear names

double calc(double a, double b, int t, bool f) {
    // 🚨 What are these variables?
    // 🚨 What does this function do?
    double r = a * b;
    if (t == 1) {
        r *= 1.2;
    }
    if (f) {
        r -= 50;
    }
    return r;
}

void proc(std::vector<int>& v) {
    // 🚨 What does this process?
    int n = v.size();
    for (int i = 0; i < n; i++) {
        int x = v[i];
        if (x > 100) {
            v[i] = 100;
        }
    }
}

class C {
    // 🚨 What is C? What does it represent?
    int d;
    std::string s;
public:
    void p() { }  // 🚨 What does p do?
};
}

namespace good_example1 {
// ✅ GOOD: Clear, descriptive names

enum EmployeeType {
    FULL_TIME,
    PART_TIME,
    CONTRACTOR
};

double calculateSalary(
    double hourlyRate,
    double hoursWorked,
    EmployeeType type,
    bool hasHealthBenefits
    ) {
    double baseSalary = hourlyRate * hoursWorked;

    // Apply full-time bonus
    if (type == FULL_TIME) {
        const double FULL_TIME_BONUS_MULTIPLIER = 1.2;
        baseSalary *= FULL_TIME_BONUS_MULTIPLIER;
    }

    // Deduct health benefits cost
    if (hasHealthBenefits) {
        const double HEALTH_BENEFITS_COST = 50.0;
        baseSalary -= HEALTH_BENEFITS_COST;
    }

    return baseSalary;
}

void limitScoresToMaximum(std::vector<int>& scores) {
    const int MAX_SCORE = 100;

    for (int& score : scores) {
        if (score > MAX_SCORE) {
            score = MAX_SCORE;
        }
    }
}

class Customer {
private:
    int customerId;
    std::string customerName;

public:
    void processOrder() {
        // Clear intent from name
    }
};
}

void example1_meaningful_names() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: MEANINGFUL NAMES                    ║");
    std::println("╚══════════════════════════════════════════════════╝");

    double salary = good_example1::calculateSalary(
        25.0,           // hourlyRate
        160.0,          // hoursWorked (full month)
        good_example1::FULL_TIME,
        true            // hasHealthBenefits
        );
    std::println("");
    std::println("   Calculated salary: ${}", salary);

    std::vector<int> testScores = {85, 95, 105, 110, 90};
    std::println("");
    std::print("   Before limiting: ");
    for (int score : testScores) std::print("{} ", score);

    good_example1::limitScoresToMaximum(testScores);
    std::println("");
    std::print("   After limiting:  ");
    for (int score : testScores) std::print("{} ", score);
    std::println("");

}

// ============================================
// EXAMPLE 2: AVOID MAGIC NUMBERS
// ============================================

namespace bad_example2 {
// ❌ BAD: Magic numbers everywhere

double calculateDiscount(double price, int customerType) {
    // 🚨 What do these numbers mean?
    if (customerType == 1) {
        return price * 0.1;   // 🚨 Why 0.1?
    } else if (customerType == 2) {
        return price * 0.15;  // 🚨 Why 0.15?
    } else if (customerType == 3) {
        return price * 0.25;  // 🚨 Why 0.25?
    }
    return 0;
}

bool isValidAge(int age) {
    // 🚨 Why 18 and 120?
    return age >= 18 && age <= 120;
}

double calculateArea(double radius) {
    // 🚨 Where does 3.14159 come from?
    return 3.14159 * radius * radius;
}
}

namespace good_example2 {
// ✅ GOOD: Named constants

enum CustomerType {
    REGULAR = 1,
    PREMIUM = 2,
    VIP = 3
};

const double REGULAR_DISCOUNT_RATE = 0.10;  // 10% for regular customers
const double PREMIUM_DISCOUNT_RATE = 0.15;  // 15% for premium customers
const double VIP_DISCOUNT_RATE = 0.25;      // 25% for VIP customers

double calculateDiscount(double price, CustomerType type) {
    switch (type) {
    case REGULAR:
        return price * REGULAR_DISCOUNT_RATE;
    case PREMIUM:
        return price * PREMIUM_DISCOUNT_RATE;
    case VIP:
        return price * VIP_DISCOUNT_RATE;
    default:
        return 0.0;
    }
}

const int MINIMUM_AGE = 18;
const int MAXIMUM_AGE = 120;

bool isValidAge(int age) {
    return age >= MINIMUM_AGE && age <= MAXIMUM_AGE;
}

const double PI = 3.14159265359;

double calculateCircleArea(double radius) {
    return PI * radius * radius;
}
}

void example2_magic_numbers() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: AVOID MAGIC NUMBERS                 ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    std::println("   ");
    std::println("   ");

    double price = 100.0;
    std::println("");
    std::println("   Regular discount: ${}",
        good_example2::calculateDiscount(price, good_example2::REGULAR));
    std::println("   Premium discount: ${}", good_example2::calculateDiscount(price, good_example2::PREMIUM));
    std::println("   VIP discount:     ${}", good_example2::calculateDiscount(price, good_example2::VIP));

    std::println("");
    std::println("   Age 17 valid? {}", (good_example2::isValidAge(17) ? "Yes" : "No"));
    std::println("   Age 25 valid? {}", (good_example2::isValidAge(25) ? "Yes" : "No"));

}

// ============================================
// EXAMPLE 3: REDUCE NESTING
// ============================================

namespace bad_example3 {
// ❌ BAD: Deep nesting (hard to read)

void processOrder(int orderId, bool userLoggedIn, bool hasInventory, bool paymentValid) {
    // 🚨 Nested 4 levels deep!
    if (orderId > 0) {
        if (userLoggedIn) {
            if (hasInventory) {
                if (paymentValid) {
                    std::println("   ✅ Order processed");
                } else {
                    std::println("   ❌ Invalid payment");
                }
            } else {
                std::println("   ❌ Out of stock");
            }
        } else {
            std::println("   ❌ User not logged in");
        }
    } else {
        std::println("   ❌ Invalid order ID");
    }
}
}

namespace good_example3 {
// ✅ GOOD: Flat structure with early returns

void processOrder(int orderId, bool userLoggedIn, bool hasInventory, bool paymentValid) {
    // Guard clauses - fail fast
    if (orderId <= 0) {
        std::println("   ❌ Invalid order ID");
        return;
    }

    if (!userLoggedIn) {
        std::println("   ❌ User not logged in");
        return;
    }

    if (!hasInventory) {
        std::println("   ❌ Out of stock");
        return;
    }

    if (!paymentValid) {
        std::println("   ❌ Invalid payment");
        return;
    }

    // Happy path at the end - clear and readable
    std::println("   ✅ Order processed");
}
}

void example3_reduce_nesting() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: REDUCE NESTING                      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    good_example3::processOrder(123, true, true, true);

    std::println("");
    std::println("   Test 2 - User not logged in:");
    good_example3::processOrder(123, false, true, true);

    std::println("");
    std::println("   Test 3 - Out of stock:");
    good_example3::processOrder(123, true, false, true);

}

// ============================================
// EXAMPLE 4: SIMPLIFY EXPRESSIONS
// ============================================

namespace bad_example4 {
// ❌ BAD: Complex, hard-to-read expressions

bool validate(int age, bool employed, double income, bool citizen) {
    // 🚨 One giant expression - what does this check?
    return ((age >= 18 && age <= 65) || (age > 65 && employed)) &&
           (income > 30000 || (income > 20000 && citizen)) &&
           (employed || income > 50000);
}

double calculate(double a, double b, double c) {
    // 🚨 Complex calculation - hard to understand
    return ((a * b) / (c + 1.5)) * (1 - (a / (b + c))) +
           (b * c / (a + 2.0)) - (c / (a * b + 0.5));
}
}

namespace good_example4 {
// ✅ GOOD: Broken down into clear steps

const int WORKING_AGE_MIN = 18;
const int WORKING_AGE_MAX = 65;
const int RETIREMENT_AGE = 65;
const double MIN_INCOME = 30000.0;
const double CITIZEN_MIN_INCOME = 20000.0;
const double UNEMPLOYED_MIN_INCOME = 50000.0;

bool isWorkingAge(int age) {
    return age >= WORKING_AGE_MIN && age <= WORKING_AGE_MAX;
}

bool isRetiredAndEmployed(int age, bool employed) {
    return age > RETIREMENT_AGE && employed;
}

bool hasAdequateIncome(double income, bool citizen) {
    if (income > MIN_INCOME) {
        return true;
    }
    if (income > CITIZEN_MIN_INCOME && citizen) {
        return true;
    }
    return false;
}

bool hasEmploymentOrIncome(bool employed, double income) {
    return employed || income > UNEMPLOYED_MIN_INCOME;
}

bool validate(int age, bool employed, double income, bool citizen) {
    // Clear, step-by-step validation
    bool ageValid = isWorkingAge(age) || isRetiredAndEmployed(age, employed);
    bool incomeValid = hasAdequateIncome(income, citizen);
    bool employmentValid = hasEmploymentOrIncome(employed, income);

    return ageValid && incomeValid && employmentValid;
}

double calculate(double a, double b, double c) {
    // Break complex formula into understandable parts
    double firstTerm = (a * b) / (c + 1.5);
    double adjustmentFactor = 1 - (a / (b + c));
    double firstPart = firstTerm * adjustmentFactor;

    double secondPart = (b * c) / (a + 2.0);
    double thirdPart = c / (a * b + 0.5);

    return firstPart + secondPart - thirdPart;
}
}

void example4_simplify_expressions() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: SIMPLIFY EXPRESSIONS                ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");

    bool result1 = good_example4::validate(30, true, 35000.0, true);
    std::println("");
    std::println("   Age 30, employed, $35K, citizen: {}", (result1 ? "Valid" : "Invalid"));

    bool result2 = good_example4::validate(70, true, 25000.0, false);
    std::println("   Age 70, employed, $25K, not citizen: {}", (result2 ? "Valid" : "Invalid"));

}

// ============================================
// EXAMPLE 5: CLEAR CONTROL FLOW
// ============================================

namespace bad_example5 {
// ❌ BAD: Unclear control flow

int processData(const std::vector<int>& data) {
    int result = 0;
    bool flag = false;

    for (int i = 0; i < data.size(); i++) {
        if (data[i] > 0) {
            if (!flag) {
                result += data[i];
                if (data[i] > 10) {
                    flag = true;
                    continue;
                }
            } else {
                result -= data[i];
            }
        }
    }
    return result;
    // 🚨 Hard to follow the logic!
}
}

namespace good_example5 {
// ✅ GOOD: Clear, readable control flow

const int THRESHOLD_VALUE = 10;

int processData(const std::vector<int>& data) {
    int sum = 0;
    bool thresholdExceeded = false;

    for (int value : data) {
        // Skip negative values
        if (value <= 0) {
            continue;
        }

        if (!thresholdExceeded) {
            // Before threshold: add values
            sum += value;

            // Check if we exceeded threshold
            if (value > THRESHOLD_VALUE) {
                thresholdExceeded = true;
            }
        } else {
            // After threshold: subtract values
            sum -= value;
        }
    }

    return sum;
}
}

void example5_control_flow() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: CLEAR CONTROL FLOW                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("   ");
    std::println("       ");

    std::vector<int> testData = {5, 8, 12, 3, 7, -2, 6};
    std::println("");
    std::print("   Data: ");
    for (int v : testData) std::print("{} ", v);

    int result = good_example5::processData(testData);
    std::println("");
    std::println("   Result: {}", result);
}

// ============================================
// EXAMPLE 6: PROPER FORMATTING
// ============================================

namespace bad_example6 {
// ❌ BAD: Poor formatting

class user{private:std::string n;int a;public:void setn(std::string x){n=x;}
    int geta(){return a;}void setage(int x){if(x>0&&x<150)a=x;}};

void process(std::vector<int>&v){for(int i=0;i<v.size();i++){if(v[i]>100)
            v[i]=100;else if(v[i]<0)v[i]=0;}}
}

namespace good_example6 {
// ✅ GOOD: Clean, consistent formatting

class User {
private:
    std::string name;
    int age;

public:
    void setName(const std::string& newName) {
        name = newName;
    }

    int getAge() const {
        return age;
    }

    void setAge(int newAge) {
        const int MIN_AGE = 0;
        const int MAX_AGE = 150;

        if (newAge > MIN_AGE && newAge < MAX_AGE) {
            age = newAge;
        }
    }
};

void normalizeValues(std::vector<int>& values) {
    const int MIN_VALUE = 0;
    const int MAX_VALUE = 100;

    for (int& value : values) {
        if (value > MAX_VALUE) {
            value = MAX_VALUE;
        } else if (value < MIN_VALUE) {
            value = MIN_VALUE;
        }
    }
}
}

void example6_formatting() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: PROPER FORMATTING                   ║");
    std::println("╚══════════════════════════════════════════════════╝");

    std::println("       ");

}

// ============================================
// EXAMPLE 7: FUNCTION LENGTH
// ============================================

namespace bad_example7 {
// ❌ BAD: Giant function (100+ lines)

void processUserRegistration(std::string name, std::string email, std::string password) {
    // Validate name
    // ... 10 lines

    // Validate email
    // ... 15 lines

    // Validate password
    // ... 20 lines

    // Check if user exists
    // ... 10 lines

    // Hash password
    // ... 15 lines

    // Save to database
    // ... 20 lines

    // Send welcome email
    // ... 15 lines

    // Log registration
    // ... 5 lines

    // 🚨 Way too long! Hard to understand!
}
}

namespace good_example7 {
// ✅ GOOD: Small, focused functions

bool isValidName(const std::string& name) {
    return !name.empty() && name.length() <= 50;
}

bool isValidEmail(const std::string& email) {
    return email.find('@') != std::string::npos;
}

bool isValidPassword(const std::string& password) {
    const int MIN_PASSWORD_LENGTH = 8;
    return password.length() >= MIN_PASSWORD_LENGTH;
}

bool userExists(const std::string& email) {
    // Database check simulation
    return false;
}

std::string hashPassword(const std::string& password) {
    // Simple hash simulation
    return "hashed_" + password;
}

void saveUser(const std::string& name, const std::string& email, const std::string& hashedPassword) {
    std::println("   💾 User saved: {} ({})", name, email);
}

void sendWelcomeEmail(const std::string& email) {
    std::println("   📧 Welcome email sent to: {}", email);
}

void logRegistration(const std::string& email) {
    std::println("   📝 Registration logged for: {}", email);
}

bool processUserRegistration(const std::string& name, const std::string& email, const std::string& password) {
    // Each validation is clear and separate
    if (!isValidName(name)) {
        std::println("   ❌ Invalid name");
        return false;
    }

    if (!isValidEmail(email)) {
        std::println("   ❌ Invalid email");
        return false;
    }

    if (!isValidPassword(password)) {
        std::println("   ❌ Invalid password (min 8 characters)");
        return false;
    }

    if (userExists(email)) {
        std::println("   ❌ User already exists");
        return false;
    }

    // Process registration
    std::string hashedPassword = hashPassword(password);
    saveUser(name, email, hashedPassword);
    sendWelcomeEmail(email);
    logRegistration(email);

    std::println("   ✅ Registration successful!");
    return true;
}
}

void example7_function_length() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: FUNCTION LENGTH                     ║");
    std::println("╚══════════════════════════════════════════════════╝");

    good_example7::processUserRegistration("John Doe", "john@example.com", "SecurePass123");

}

// ============================================
// EXAMPLE 8: READABILITY CHECKLIST
// ============================================

void example8_checklist() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: READABILITY CHECKLIST               ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║              READABILITY PRINCIPLE                    ║");
    std::println("║                                                       ║");
    std::println("║  \"Code should be written to be easily read and       ║");
    std::println("║   understood by humans, not just executed by          ║");
    std::println("║   machines.\"                                         ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_meaningful_names();
    example2_magic_numbers();
    example3_reduce_nesting();
    example4_simplify_expressions();
    example5_control_flow();
    example6_formatting();
    example7_function_length();
    example8_checklist();

    return 0;
}
