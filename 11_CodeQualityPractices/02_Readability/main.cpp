#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
using namespace std;

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

void proc(vector<int>& v) {
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
    string s;
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

void limitScoresToMaximum(vector<int>& scores) {
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
    string customerName;

public:
    void processOrder() {
        // Clear intent from name
    }
};
}

void example1_meaningful_names() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: MEANINGFUL NAMES                    ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Cryptic Names):\n";
    cout << "```cpp\n";
    cout << "double calc(double a, double b, int t, bool f) {\n";
    cout << "    double r = a * b;\n";
    cout << "    if (t == 1) r *= 1.2;\n";
    cout << "    if (f) r -= 50;\n";
    cout << "    // What are a, b, t, f, r?\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Single-letter variables\n";
    cout << "   • Unclear parameter purpose\n";
    cout << "   • Magic numbers (1.2, 50)\n";
    cout << "   • Function name doesn't describe action\n";

    cout << "\n✅ GOOD CODE (Clear Names):\n";
    cout << "```cpp\n";
    cout << "double calculateSalary(\n";
    cout << "    double hourlyRate,\n";
    cout << "    double hoursWorked,\n";
    cout << "    EmployeeType type,\n";
    cout << "    bool hasHealthBenefits\n";
    cout << ") {\n";
    cout << "    // Intent is crystal clear!\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ DEMONSTRATION:\n";

    double salary = good_example1::calculateSalary(
        25.0,           // hourlyRate
        160.0,          // hoursWorked (full month)
        good_example1::FULL_TIME,
        true            // hasHealthBenefits
        );
    cout << "\n   Calculated salary: $" << salary << endl;

    vector<int> testScores = {85, 95, 105, 110, 90};
    cout << "\n   Before limiting: ";
    for (int score : testScores) cout << score << " ";

    good_example1::limitScoresToMaximum(testScores);
    cout << "\n   After limiting:  ";
    for (int score : testScores) cout << score << " ";
    cout << endl;

    cout << "\n💡 NAMING GUIDELINES:\n";
    cout << "   ✅ Use descriptive names\n";
    cout << "   ✅ Names should reveal intent\n";
    cout << "   ✅ Avoid single letters (except i, j in loops)\n";
    cout << "   ✅ Use domain terminology\n";
    cout << "   ✅ Be consistent with conventions\n";
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: AVOID MAGIC NUMBERS                 ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   if (customerType == 2) {\n";
    cout << "       return price * 0.15;  // 🚨 What is 2? Why 0.15?\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   return age >= 18 && age <= 120;  // 🚨 Why these numbers?\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   const double PREMIUM_DISCOUNT_RATE = 0.15;\n";
    cout << "   const int MINIMUM_AGE = 18;\n";
    cout << "   \n";
    cout << "   if (type == PREMIUM) {\n";
    cout << "       return price * PREMIUM_DISCOUNT_RATE;\n";
    cout << "   }\n";
    cout << "   \n";
    cout << "   return age >= MINIMUM_AGE && age <= MAXIMUM_AGE;\n";

    cout << "\n✨ DEMONSTRATION:\n";

    double price = 100.0;
    cout << "\n   Regular discount: $"
         << good_example2::calculateDiscount(price, good_example2::REGULAR) << endl;
    cout << "   Premium discount: $"
         << good_example2::calculateDiscount(price, good_example2::PREMIUM) << endl;
    cout << "   VIP discount:     $"
         << good_example2::calculateDiscount(price, good_example2::VIP) << endl;

    cout << "\n   Age 17 valid? "
         << (good_example2::isValidAge(17) ? "Yes" : "No") << endl;
    cout << "   Age 25 valid? "
         << (good_example2::isValidAge(25) ? "Yes" : "No") << endl;

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Numbers have clear meaning\n";
    cout << "   • Easy to change values\n";
    cout << "   • Self-documenting code\n";
    cout << "   • No confusion about meaning\n";
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
                    cout << "   ✅ Order processed" << endl;
                } else {
                    cout << "   ❌ Invalid payment" << endl;
                }
            } else {
                cout << "   ❌ Out of stock" << endl;
            }
        } else {
            cout << "   ❌ User not logged in" << endl;
        }
    } else {
        cout << "   ❌ Invalid order ID" << endl;
    }
}
}

namespace good_example3 {
// ✅ GOOD: Flat structure with early returns

void processOrder(int orderId, bool userLoggedIn, bool hasInventory, bool paymentValid) {
    // Guard clauses - fail fast
    if (orderId <= 0) {
        cout << "   ❌ Invalid order ID" << endl;
        return;
    }

    if (!userLoggedIn) {
        cout << "   ❌ User not logged in" << endl;
        return;
    }

    if (!hasInventory) {
        cout << "   ❌ Out of stock" << endl;
        return;
    }

    if (!paymentValid) {
        cout << "   ❌ Invalid payment" << endl;
        return;
    }

    // Happy path at the end - clear and readable
    cout << "   ✅ Order processed" << endl;
}
}

void example3_reduce_nesting() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: REDUCE NESTING                      ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE (Deep Nesting):\n";
    cout << "   if (orderId > 0) {\n";
    cout << "       if (userLoggedIn) {\n";
    cout << "           if (hasInventory) {\n";
    cout << "               if (paymentValid) {\n";
    cout << "                   // 4 levels deep!\n";
    cout << "               }\n";
    cout << "           }\n";
    cout << "       }\n";
    cout << "   }\n";

    cout << "\n✅ GOOD CODE (Early Returns):\n";
    cout << "   if (orderId <= 0) return;\n";
    cout << "   if (!userLoggedIn) return;\n";
    cout << "   if (!hasInventory) return;\n";
    cout << "   if (!paymentValid) return;\n";
    cout << "   \n";
    cout << "   // Process order - no nesting!\n";

    cout << "\n✨ DEMONSTRATION:\n";

    cout << "\n   Test 1 - All valid:\n";
    good_example3::processOrder(123, true, true, true);

    cout << "\n   Test 2 - User not logged in:\n";
    good_example3::processOrder(123, false, true, true);

    cout << "\n   Test 3 - Out of stock:\n";
    good_example3::processOrder(123, true, false, true);

    cout << "\n💡 TECHNIQUES:\n";
    cout << "   ✅ Use early returns (guard clauses)\n";
    cout << "   ✅ Extract nested logic to functions\n";
    cout << "   ✅ Invert conditions\n";
    cout << "   ✅ Keep nesting depth ≤ 2 levels\n";
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: SIMPLIFY EXPRESSIONS                ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   return ((age >= 18 && age <= 65) || (age > 65 && emp)) &&\n";
    cout << "          (inc > 30000 || (inc > 20000 && cit)) &&\n";
    cout << "          (emp || inc > 50000);\n";
    cout << "   // 🚨 What is this checking?\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   bool ageValid = isWorkingAge(age) ||\n";
    cout << "                   isRetiredAndEmployed(age, employed);\n";
    cout << "   bool incomeValid = hasAdequateIncome(income, citizen);\n";
    cout << "   bool employmentValid = hasEmploymentOrIncome(employed, income);\n";
    cout << "   \n";
    cout << "   return ageValid && incomeValid && employmentValid;\n";

    cout << "\n✨ DEMONSTRATION:\n";

    bool result1 = good_example4::validate(30, true, 35000.0, true);
    cout << "\n   Age 30, employed, $35K, citizen: "
         << (result1 ? "Valid" : "Invalid") << endl;

    bool result2 = good_example4::validate(70, true, 25000.0, false);
    cout << "   Age 70, employed, $25K, not citizen: "
         << (result2 ? "Valid" : "Invalid") << endl;

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Easy to understand each part\n";
    cout << "   • Easy to test individually\n";
    cout << "   • Self-documenting with names\n";
    cout << "   • Can reuse helper functions\n";
}

// ============================================
// EXAMPLE 5: CLEAR CONTROL FLOW
// ============================================

namespace bad_example5 {
// ❌ BAD: Unclear control flow

int processData(const vector<int>& data) {
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

int processData(const vector<int>& data) {
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: CLEAR CONTROL FLOW                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   bool flag = false;\n";
    cout << "   for (int i = 0; i < data.size(); i++) {\n";
    cout << "       if (data[i] > 0) {\n";
    cout << "           if (!flag) {\n";
    cout << "               result += data[i];\n";
    cout << "               if (data[i] > 10) {\n";
    cout << "                   flag = true;\n";
    cout << "   // 🚨 Hard to follow!\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   bool thresholdExceeded = false;\n";
    cout << "   \n";
    cout << "   for (int value : data) {\n";
    cout << "       if (value <= 0) continue;\n";
    cout << "       \n";
    cout << "       if (!thresholdExceeded) {\n";
    cout << "           sum += value;\n";
    cout << "           // Clear intent with comments\n";

    cout << "\n✨ DEMONSTRATION:\n";

    vector<int> testData = {5, 8, 12, 3, 7, -2, 6};
    cout << "\n   Data: ";
    for (int v : testData) cout << v << " ";

    int result = good_example5::processData(testData);
    cout << "\n   Result: " << result << endl;
    cout << "   (Sum before threshold, subtract after)\n";

    cout << "\n💡 GUIDELINES:\n";
    cout << "   ✅ Use descriptive variable names for flags\n";
    cout << "   ✅ Add comments explaining control flow\n";
    cout << "   ✅ Use early continue/return\n";
    cout << "   ✅ Keep logic linear when possible\n";
}

// ============================================
// EXAMPLE 6: PROPER FORMATTING
// ============================================

namespace bad_example6 {
// ❌ BAD: Poor formatting

class user{private:string n;int a;public:void setn(string x){n=x;}
    int geta(){return a;}void setage(int x){if(x>0&&x<150)a=x;}};

void process(vector<int>&v){for(int i=0;i<v.size();i++){if(v[i]>100)
            v[i]=100;else if(v[i]<0)v[i]=0;}}
}

namespace good_example6 {
// ✅ GOOD: Clean, consistent formatting

class User {
private:
    string name;
    int age;

public:
    void setName(const string& newName) {
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

void normalizeValues(vector<int>& values) {
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
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: PROPER FORMATTING                   ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD CODE:\n";
    cout << "   class user{private:string n;int a;public:void setn\n";
    cout << "   (string x){n=x;}int geta(){return a;}};\n";
    cout << "   // 🚨 Everything crammed together!\n";

    cout << "\n✅ GOOD CODE:\n";
    cout << "   class User {\n";
    cout << "   private:\n";
    cout << "       string name;\n";
    cout << "       int age;\n";
    cout << "       \n";
    cout << "   public:\n";
    cout << "       void setName(const string& newName) {\n";
    cout << "           name = newName;\n";
    cout << "       }\n";
    cout << "   };\n";

    cout << "\n📐 FORMATTING RULES:\n";
    cout << "   ✅ Consistent indentation (4 spaces)\n";
    cout << "   ✅ One statement per line\n";
    cout << "   ✅ Blank lines between sections\n";
    cout << "   ✅ Space after keywords (if, for, while)\n";
    cout << "   ✅ Space around operators (=, +, -, etc.)\n";
    cout << "   ✅ Aligned braces\n";

    cout << "\n💡 BENEFITS:\n";
    cout << "   • Code is scannable\n";
    cout << "   • Easy to spot errors\n";
    cout << "   • Professional appearance\n";
    cout << "   • Consistent team style\n";
}

// ============================================
// EXAMPLE 7: FUNCTION LENGTH
// ============================================

namespace bad_example7 {
// ❌ BAD: Giant function (100+ lines)

void processUserRegistration(string name, string email, string password) {
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

bool isValidName(const string& name) {
    return !name.empty() && name.length() <= 50;
}

bool isValidEmail(const string& email) {
    return email.find('@') != string::npos;
}

bool isValidPassword(const string& password) {
    const int MIN_PASSWORD_LENGTH = 8;
    return password.length() >= MIN_PASSWORD_LENGTH;
}

bool userExists(const string& email) {
    // Database check simulation
    return false;
}

string hashPassword(const string& password) {
    // Simple hash simulation
    return "hashed_" + password;
}

void saveUser(const string& name, const string& email, const string& hashedPassword) {
    cout << "   💾 User saved: " << name << " (" << email << ")" << endl;
}

void sendWelcomeEmail(const string& email) {
    cout << "   📧 Welcome email sent to: " << email << endl;
}

void logRegistration(const string& email) {
    cout << "   📝 Registration logged for: " << email << endl;
}

bool processUserRegistration(const string& name, const string& email, const string& password) {
    // Each validation is clear and separate
    if (!isValidName(name)) {
        cout << "   ❌ Invalid name" << endl;
        return false;
    }

    if (!isValidEmail(email)) {
        cout << "   ❌ Invalid email" << endl;
        return false;
    }

    if (!isValidPassword(password)) {
        cout << "   ❌ Invalid password (min 8 characters)" << endl;
        return false;
    }

    if (userExists(email)) {
        cout << "   ❌ User already exists" << endl;
        return false;
    }

    // Process registration
    string hashedPassword = hashPassword(password);
    saveUser(name, email, hashedPassword);
    sendWelcomeEmail(email);
    logRegistration(email);

    cout << "   ✅ Registration successful!" << endl;
    return true;
}
}

void example7_function_length() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: FUNCTION LENGTH                     ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n❌ BAD: One giant function\n";
    cout << "   • 100+ lines\n";
    cout << "   • Multiple responsibilities\n";
    cout << "   • Hard to test\n";
    cout << "   • Hard to understand\n";

    cout << "\n✅ GOOD: Multiple small functions\n";
    cout << "   • isValidName() - 3 lines\n";
    cout << "   • isValidEmail() - 3 lines\n";
    cout << "   • isValidPassword() - 3 lines\n";
    cout << "   • processUserRegistration() - orchestrates\n";

    cout << "\n✨ DEMONSTRATION:\n";

    good_example7::processUserRegistration("John Doe", "john@example.com", "SecurePass123");

    cout << "\n💡 FUNCTION SIZE GUIDELINES:\n";
    cout << "   ✅ Ideal: 5-20 lines\n";
    cout << "   ✅ Acceptable: 20-50 lines\n";
    cout << "   ✅ Warning: 50-100 lines\n";
    cout << "   🚨 Refactor: 100+ lines\n";

    cout << "\n   Rule of thumb: If you can't see the whole\n";
    cout << "   function on one screen, it's too long!\n";
}

// ============================================
// EXAMPLE 8: READABILITY CHECKLIST
// ============================================

void example8_checklist() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: READABILITY CHECKLIST               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ READABILITY CHECKLIST:\n";

    cout << "\n📝 NAMING:\n";
    cout << "   □ Variable names are descriptive\n";
    cout << "   □ Function names describe action\n";
    cout << "   □ Class names are nouns\n";
    cout << "   □ Constants are UPPER_CASE\n";
    cout << "   □ No single-letter names (except i, j)\n";

    cout << "\n🔢 NUMBERS:\n";
    cout << "   □ No magic numbers\n";
    cout << "   □ Constants are named\n";
    cout << "   □ Numbers have context\n";

    cout << "\n📐 STRUCTURE:\n";
    cout << "   □ Functions < 20 lines\n";
    cout << "   □ Nesting depth ≤ 2\n";
    cout << "   □ One purpose per function\n";
    cout << "   □ Early returns used\n";

    cout << "\n🎨 FORMATTING:\n";
    cout << "   □ Consistent indentation\n";
    cout << "   □ Proper spacing\n";
    cout << "   □ Aligned braces\n";
    cout << "   □ Blank lines between sections\n";

    cout << "\n💭 EXPRESSIONS:\n";
    cout << "   □ Simple, clear expressions\n";
    cout << "   □ Complex logic broken down\n";
    cout << "   □ No deeply nested conditions\n";

    cout << "\n📖 COMMENTS:\n";
    cout << "   □ Explain WHY, not WHAT\n";
    cout << "   □ Document business rules\n";
    cout << "   □ No obvious comments\n";
    cout << "   □ No outdated comments\n";

    cout << "\n🎯 THE ULTIMATE TEST:\n";
    cout << "   \"Can you understand this code in 30 seconds?\"\n";
    cout << "   If not, it needs to be more readable!\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║              READABILITY PRINCIPLE                    ║\n";
    cout << "║                                                       ║\n";
    cout << "║  \"Code should be written to be easily read and        ║\n";
    cout << "║   understood by humans, not just executed by          ║\n";
    cout << "║   machines.\"                                         ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_meaningful_names();
    example2_magic_numbers();
    example3_reduce_nesting();
    example4_simplify_expressions();
    example5_control_flow();
    example6_formatting();
    example7_function_length();
    example8_checklist();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║         🎉 READABILITY PRINCIPLE COMPLETE! 🎉         ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Key Takeaway: Code is read 10x more than written.    ║\n";
    cout << "║  Write for humans, not just computers!                ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
