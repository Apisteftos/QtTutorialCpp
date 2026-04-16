#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

// ============================================
// WHAT IS CODE STYLE & FORMATTING?
// ============================================

/*
CODE STYLE & FORMATTING:
    Consistent, readable way of writing code that makes it
    easy for humans to read and maintain!

WHY IT MATTERS:

1. READABILITY:
   - Code is read 10x more than written
   - Consistent style = easier to understand
   - Reduces cognitive load

2. MAINTAINABILITY:
   - Easier to spot errors
   - Faster to modify
   - Less context switching

3. COLLABORATION:
   - Team works together smoothly
   - Code reviews are faster
   - Onboarding is easier

4. PROFESSIONALISM:
   - Shows attention to detail
   - Demonstrates discipline
   - Industry standard

KEY ASPECTS:

1. NAMING CONVENTIONS:
   - Variables: camelCase or snake_case
   - Functions: camelCase or snake_case
   - Classes: PascalCase
   - Constants: UPPER_SNAKE_CASE
   - Private members: trailing underscore

2. INDENTATION:
   - Spaces (2 or 4) or tabs
   - Consistency is key!
   - Align code blocks clearly

3. BRACING STYLES:
   - K&R (same line)
   - Allman (next line)
   - Choose one, stick to it

4. SPACING:
   - Around operators
   - After commas
   - Between logical blocks

5. LINE LENGTH:
   - 80-120 characters max
   - Break long lines logically

6. FILE ORGANIZATION:
   - Header order
   - Function order
   - Logical grouping

GOLDEN RULE:
   "Be consistent! Pick a style and use it everywhere!"

TOOLS:
   • clang-format: Auto-format C++ code
   • Style guides: Google, LLVM, Mozilla
   • EditorConfig: Maintain consistency across editors
*/

// ============================================
// EXAMPLE 1: NAMING CONVENTIONS
// ============================================

void example1_naming_conventions() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 1: NAMING CONVENTIONS                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Inconsistent naming
    cout << "\n❌ BAD CODE (Inconsistent):\n";
    cout << "```cpp\n";
    cout << "// Mixing styles randomly\n";
    cout << "int UserAge;            // PascalCase for variable (wrong!)\n";
    cout << "string customer_name;   // snake_case\n";
    cout << "double TotalPrice;      // PascalCase again\n";
    cout << "bool isactive;          // no separators\n\n";
    cout << "void Calculate_Total() { }  // Mixed style\n";
    cout << "void processOrder() { }     // Different style\n\n";
    cout << "class user_account { };     // snake_case for class (wrong!)\n";
    cout << "class OrderManager { };     // PascalCase\n\n";
    cout << "const double tax_rate = 0.19;  // Should be UPPER\n";
    cout << "const int MAX_USERS = 100;     // Correct\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Mixing camelCase, PascalCase, snake_case randomly\n";
    cout << "   • No clear pattern\n";
    cout << "   • Confusing for readers\n";
    cout << "   • Hard to remember which style to use\n";

    // ✅ GOOD CODE: Consistent naming (C++ Standard Library style)
    cout << "\n✅ GOOD CODE (C++ Standard Style):\n";
    cout << "```cpp\n";
    cout << "// Variables and functions: snake_case (like STL)\n";
    cout << "int user_age;\n";
    cout << "string customer_name;\n";
    cout << "double total_price;\n";
    cout << "bool is_active;\n\n";
    cout << "void calculate_total() { }\n";
    cout << "void process_order() { }\n\n";
    cout << "// Classes: PascalCase\n";
    cout << "class UserAccount { };\n";
    cout << "class OrderManager { };\n\n";
    cout << "// Constants: UPPER_SNAKE_CASE\n";
    cout << "const double TAX_RATE = 0.19;\n";
    cout << "const int MAX_USERS = 100;\n\n";
    cout << "// Private members: trailing underscore (optional)\n";
    cout << "class MyClass {\n";
    cout << "private:\n";
    cout << "    int value_;        // Private member\n";
    cout << "    string name_;\n";
    cout << "public:\n";
    cout << "    int getValue() const { return value_; }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✅ ALTERNATIVE (Google C++ Style):\n";
    cout << "```cpp\n";
    cout << "// Variables and functions: snake_case\n";
    cout << "int user_age;\n";
    cout << "void calculate_total() { }\n\n";
    cout << "// Classes: PascalCase\n";
    cout << "class UserAccount { };\n\n";
    cout << "// Constants: kConstantName (Google style)\n";
    cout << "const double kTaxRate = 0.19;\n";
    cout << "const int kMaxUsers = 100;\n\n";
    cout << "// Private members: trailing underscore\n";
    cout << "class MyClass {\n";
    cout << "private:\n";
    cout << "    int value_;\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Consistent style throughout\n";
    cout << "   ✅ Easy to recognize types (class vs variable)\n";
    cout << "   ✅ Follows established conventions\n";
    cout << "   ✅ Clear distinction between public/private\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Pick ONE naming convention and use it consistently!\n";
    cout << "    Follow your team's or project's style guide.\"\n";
}

// ============================================
// EXAMPLE 2: BRACE STYLES
// ============================================

void example2_brace_styles() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 2: BRACE STYLES                        ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Inconsistent bracing
    cout << "\n❌ BAD CODE (Inconsistent Braces):\n";
    cout << "```cpp\n";
    cout << "void function1() {  // K&R style\n";
    cout << "    if (condition) {\n";
    cout << "        doSomething();\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "void function2()    // Allman style\n";
    cout << "{\n";
    cout << "    if (condition)\n";
    cout << "    {\n";
    cout << "        doSomething();\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "void function3() {  // Mixed!\n";
    cout << "    if (condition)\n";
    cout << "    {\n";
    cout << "        doSomething(); }\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Three different brace styles in same file\n";
    cout << "   • Inconsistent and confusing\n";
    cout << "   • Makes code look messy\n";
    cout << "   • Harder to scan visually\n";

    // ✅ GOOD CODE: Consistent K&R style (most common in C++)
    cout << "\n✅ GOOD CODE (K&R Style - Most Popular):\n";
    cout << "```cpp\n";
    cout << "void function1() {\n";
    cout << "    if (condition) {\n";
    cout << "        doSomething();\n";
    cout << "    } else {\n";
    cout << "        doSomethingElse();\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "void function2() {\n";
    cout << "    for (int i = 0; i < n; i++) {\n";
    cout << "        process(i);\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    void method() {\n";
    cout << "        // Implementation\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✅ ALTERNATIVE (Allman Style):\n";
    cout << "```cpp\n";
    cout << "void function1()\n";
    cout << "{\n";
    cout << "    if (condition)\n";
    cout << "    {\n";
    cout << "        doSomething();\n";
    cout << "    }\n";
    cout << "    else\n";
    cout << "    {\n";
    cout << "        doSomethingElse();\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "class MyClass\n";
    cout << "{\n";
    cout << "public:\n";
    cout << "    void method()\n";
    cout << "    {\n";
    cout << "        // Implementation\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Consistent style throughout file\n";
    cout << "   ✅ Easy to scan and read\n";
    cout << "   ✅ Professional appearance\n";
    cout << "   ✅ Matches team/project style\n";

    cout << "\n📋 BRACE STYLE COMPARISON:\n";
    cout << "\n   K&R (Kernighan & Ritchie):\n";
    cout << "   • Opening brace on same line\n";
    cout << "   • Most compact\n";
    cout << "   • Most popular in C++\n";
    cout << "   • Used by Google, LLVM\n";

    cout << "\n   Allman:\n";
    cout << "   • Opening brace on new line\n";
    cout << "   • More vertical space\n";
    cout << "   • Popular in C#, some C++\n";
    cout << "   • Clearer structure visibility\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Pick K&R or Allman, use it everywhere!\n";
    cout << "    Most C++ projects use K&R.\"\n";
}

// ============================================
// EXAMPLE 3: INDENTATION
// ============================================

void example3_indentation() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 3: INDENTATION                         ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Inconsistent indentation
    cout << "\n❌ BAD CODE (Inconsistent Indentation):\n";
    cout << "```cpp\n";
    cout << "void processData() {\n";
    cout << "  if (condition1) {    // 2 spaces\n";
    cout << "      doSomething();   // 6 spaces\n";
    cout << "    if (condition2) {  // 4 spaces\n";
    cout << "       doMore();       // 7 spaces\n";
    cout << "    }\n";
    cout << "  }\n";
    cout << "}\n\n";
    cout << "// Mixing tabs and spaces (disaster!)\n";
    cout << "void mixedIndentation() {\n";
    cout << "→   if (x) {        // Tab + spaces\n";
    cout << "    →   doIt();    // Spaces + tab\n";
    cout << "    }\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Random indentation levels\n";
    cout << "   • Mixing tabs and spaces (worst!)\n";
    cout << "   • Hard to see code structure\n";
    cout << "   • Looks unprofessional\n";
    cout << "   • Editor-dependent appearance\n";

    // ✅ GOOD CODE: Consistent indentation
    cout << "\n✅ GOOD CODE (4 Spaces - Most Common):\n";
    cout << "```cpp\n";
    cout << "void processData() {\n";
    cout << "    if (condition1) {\n";
    cout << "        doSomething();\n";
    cout << "        if (condition2) {\n";
    cout << "            doMore();\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "class MyClass {\n";
    cout << "public:\n";
    cout << "    void method() {\n";
    cout << "        for (int i = 0; i < n; i++) {\n";
    cout << "            process(i);\n";
    cout << "        }\n";
    cout << "    }\n";
    cout << "\n";
    cout << "private:\n";
    cout << "    int value_;\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n✅ ALTERNATIVE (2 Spaces - Google Style):\n";
    cout << "```cpp\n";
    cout << "void processData() {\n";
    cout << "  if (condition1) {\n";
    cout << "    doSomething();\n";
    cout << "    if (condition2) {\n";
    cout << "      doMore();\n";
    cout << "    }\n";
    cout << "  }\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Consistent indentation level\n";
    cout << "   ✅ Clear visual hierarchy\n";
    cout << "   ✅ Easy to spot structure\n";
    cout << "   ✅ Same in all editors\n";

    cout << "\n📊 INDENTATION GUIDELINES:\n";
    cout << "   • Use spaces (not tabs) for consistency\n";
    cout << "   • 4 spaces: Most common (LLVM, Mozilla)\n";
    cout << "   • 2 spaces: More compact (Google)\n";
    cout << "   • Configure editor to insert spaces\n";
    cout << "   • Never mix tabs and spaces!\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Use 2 or 4 spaces consistently. Never tabs!\n";
    cout << "    Configure your editor properly.\"\n";
}

// ============================================
// EXAMPLE 4: SPACING & WHITESPACE
// ============================================

void example4_spacing() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 4: SPACING & WHITESPACE               ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Poor spacing
    cout << "\n❌ BAD CODE (Poor Spacing):\n";
    cout << "```cpp\n";
    cout << "int calculate(int x,int y){  // No spaces!\n";
    cout << "    int result=x+y*2;        // Cramped\n";
    cout << "    if(result>100){          // Hard to read\n";
    cout << "        result=result-10;\n";
    cout << "    }\n";
    cout << "    return result;\n";
    cout << "}\n\n";
    cout << "void processArray(vector<int>&v){\n";
    cout << "    for(int i=0;i<v.size();i++){\n";
    cout << "        v[i]=v[i]*2;\n";
    cout << "    }\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • No spaces around operators\n";
    cout << "   • No spaces after commas\n";
    cout << "   • No space before opening brace\n";
    cout << "   • Cramped and hard to read\n";
    cout << "   • Looks unprofessional\n";

    // ✅ GOOD CODE: Proper spacing
    cout << "\n✅ GOOD CODE (Proper Spacing):\n";
    cout << "```cpp\n";
    cout << "int calculate(int x, int y) {\n";
    cout << "    int result = x + y * 2;\n";
    cout << "    \n";
    cout << "    if (result > 100) {\n";
    cout << "        result = result - 10;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    return result;\n";
    cout << "}\n\n";
    cout << "void processArray(vector<int>& v) {\n";
    cout << "    for (int i = 0; i < v.size(); i++) {\n";
    cout << "        v[i] = v[i] * 2;\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "// Logical grouping with blank lines\n";
    cout << "void complexFunction() {\n";
    cout << "    // Initialize variables\n";
    cout << "    int total = 0;\n";
    cout << "    int count = 0;\n";
    cout << "    \n";
    cout << "    // Process data\n";
    cout << "    for (auto value : data) {\n";
    cout << "        total += value;\n";
    cout << "        count++;\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    // Calculate result\n";
    cout << "    double average = static_cast<double>(total) / count;\n";
    cout << "    return average;\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Space after comma\n";
    cout << "   ✅ Space around operators (=, +, -, *, <, >)\n";
    cout << "   ✅ Space before opening brace\n";
    cout << "   ✅ Space after keywords (if, for, while)\n";
    cout << "   ✅ Blank lines between logical sections\n";

    cout << "\n📋 SPACING RULES:\n";
    cout << "   ✅ Space after comma: foo(a, b, c)\n";
    cout << "   ✅ Space around binary operators: a + b\n";
    cout << "   ✅ Space after keywords: if (condition)\n";
    cout << "   ✅ Space before opening brace: void func() {\n";
    cout << "   ❌ No space after function name: foo()\n";
    cout << "   ❌ No space inside parentheses: if (x)\n";
    cout << "   ✅ Blank line between functions\n";
    cout << "   ✅ Blank line between logical sections\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Breathing room makes code readable!\n";
    cout << "    Space around operators and after commas.\"\n";
}

// ============================================
// EXAMPLE 5: LINE LENGTH
// ============================================

void example5_line_length() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 5: LINE LENGTH                         ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Lines too long
    cout << "\n❌ BAD CODE (Lines Too Long):\n";
    cout << "```cpp\n";
    cout << "void processUserData(string firstName, string lastName, string email, string phoneNumber, string address, string city, string zipCode, string country, int age, bool isActive) { // 180+ characters!\n";
    cout << "    if (firstName.empty() || lastName.empty() || email.empty() || phoneNumber.empty() || address.empty() || city.empty() || zipCode.empty() || country.empty()) { // Impossible to read!\n";
    cout << "        throw invalid_argument(\"All fields are required: firstName, lastName, email, phoneNumber, address, city, zipCode, country\");\n";
    cout << "    }\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Lines over 120 characters\n";
    cout << "   • Horizontal scrolling required\n";
    cout << "   • Hard to read on smaller screens\n";
    cout << "   • Can't see code in split view\n";
    cout << "   • Difficult in code reviews\n";

    // ✅ GOOD CODE: Proper line breaks
    cout << "\n✅ GOOD CODE (Proper Line Breaks):\n";
    cout << "```cpp\n";
    cout << "// Use struct for many parameters\n";
    cout << "struct UserData {\n";
    cout << "    string firstName, lastName;\n";
    cout << "    string email, phoneNumber;\n";
    cout << "    string address, city, zipCode, country;\n";
    cout << "    int age;\n";
    cout << "    bool isActive;\n";
    cout << "};\n\n";
    cout << "void processUserData(const UserData& user) {\n";
    cout << "    if (user.firstName.empty() || \n";
    cout << "        user.lastName.empty() || \n";
    cout << "        user.email.empty()) {\n";
    cout << "        throw invalid_argument(\n";
    cout << "            \"First name, last name, and email are required\"\n";
    cout << "        );\n";
    cout << "    }\n";
    cout << "}\n\n";
    cout << "// Break long chains\n";
    cout << "auto result = data\n";
    cout << "    .filter([](int x) { return x > 0; })\n";
    cout << "    .map([](int x) { return x * 2; })\n";
    cout << "    .reduce(0, [](int a, int b) { return a + b; });\n\n";
    cout << "// Break long conditions\n";
    cout << "if (isValidUser(user) &&\n";
    cout << "    hasPermission(user) &&\n";
    cout << "    isWithinBusinessHours() &&\n";
    cout << "    !isMaintenanceMode()) {\n";
    cout << "    processRequest();\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Lines under 80-100 characters\n";
    cout << "   ✅ No horizontal scrolling\n";
    cout << "   ✅ Readable on all screens\n";
    cout << "   ✅ Works in split view\n";
    cout << "   ✅ Better for code reviews\n";

    cout << "\n📏 LINE LENGTH GUIDELINES:\n";
    cout << "   • 80 characters: Traditional (still common)\n";
    cout << "   • 100 characters: Modern compromise\n";
    cout << "   • 120 characters: Maximum for most teams\n";
    cout << "   • Break at logical points (operators, commas)\n";
    cout << "   • Indent continued lines\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Keep lines under 100 characters.\n";
    cout << "    Break long lines at logical points.\"\n";
}

// ============================================
// EXAMPLE 6: FUNCTION & CLASS ORGANIZATION
// ============================================

void example6_organization() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 6: FUNCTION & CLASS ORGANIZATION      ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Poor organization
    cout << "\n❌ BAD CODE (Poor Organization):\n";
    cout << "```cpp\n";
    cout << "class UserManager {\n";
    cout << "    int count;  // Public data (bad!)\n";
    cout << "public:\n";
    cout << "    void save() { }\n";
    cout << "private:\n";
    cout << "    string name;\n";
    cout << "public:\n";
    cout << "    void load() { }\n";
    cout << "    void delete() { }\n";
    cout << "private:\n";
    cout << "    void helper() { }\n";
    cout << "public:\n";
    cout << "    int getValue() { return count; }\n";
    cout << "};\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • public/private sections mixed randomly\n";
    cout << "   • Related methods separated\n";
    cout << "   • Hard to find methods\n";
    cout << "   • Confusing structure\n";

    // ✅ GOOD CODE: Well-organized class
    cout << "\n✅ GOOD CODE (Well-Organized Class):\n";
    cout << "```cpp\n";
    cout << "class UserManager {\n";
    cout << "public:\n";
    cout << "    // Constructors\n";
    cout << "    UserManager();\n";
    cout << "    ~UserManager();\n";
    cout << "    \n";
    cout << "    // Public interface (grouped logically)\n";
    cout << "    void save();\n";
    cout << "    void load();\n";
    cout << "    void remove();\n";
    cout << "    \n";
    cout << "    // Getters/Setters\n";
    cout << "    int getCount() const { return count_; }\n";
    cout << "    string getName() const { return name_; }\n";
    cout << "    \n";
    cout << "private:\n";
    cout << "    // Helper methods\n";
    cout << "    void validateData();\n";
    cout << "    void notifyObservers();\n";
    cout << "    \n";
    cout << "    // Member variables (at end)\n";
    cout << "    int count_;\n";
    cout << "    string name_;\n";
    cout << "    vector<Observer*> observers_;\n";
    cout << "};\n\n";
    cout << "// Source file organization\n";
    cout << "// 1. Constructors/Destructors\n";
    cout << "UserManager::UserManager() : count_(0) { }\n";
    cout << "UserManager::~UserManager() { }\n\n";
    cout << "// 2. Public methods (in header order)\n";
    cout << "void UserManager::save() { }\n";
    cout << "void UserManager::load() { }\n";
    cout << "void UserManager::remove() { }\n\n";
    cout << "// 3. Private methods\n";
    cout << "void UserManager::validateData() { }\n";
    cout << "void UserManager::notifyObservers() { }\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Clear section separation\n";
    cout << "   ✅ Public interface first\n";
    cout << "   ✅ Related methods grouped\n";
    cout << "   ✅ Easy to navigate\n";
    cout << "   ✅ Logical order\n";

    cout << "\n📋 CLASS ORGANIZATION ORDER:\n";
    cout << "   1. public section:\n";
    cout << "      - Constructors/Destructors\n";
    cout << "      - Public methods (grouped logically)\n";
    cout << "      - Getters/Setters\n";
    cout << "   2. protected section (if needed)\n";
    cout << "   3. private section:\n";
    cout << "      - Helper methods\n";
    cout << "      - Member variables (at end)\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Organize logically: Public first, private last.\n";
    cout << "    Group related methods together.\"\n";
}

// ============================================
// EXAMPLE 7: FILE ORGANIZATION & HEADERS
// ============================================

void example7_file_organization() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 7: FILE ORGANIZATION & HEADERS        ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Random header order
    cout << "\n❌ BAD CODE (Random Header Order):\n";
    cout << "```cpp\n";
    cout << "#include \"MyClass.h\"\n";
    cout << "#include <vector>\n";
    cout << "#include \"Utility.h\"\n";
    cout << "#include <iostream>\n";
    cout << "#include <string>\n";
    cout << "#include \"../other/Helper.h\"\n";
    cout << "#include <algorithm>\n";
    cout << "using namespace std;  // In header (bad!)\n\n";
    cout << "// No logical organization\n";
    cout << "// Hard to spot missing includes\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Random include order\n";
    cout << "   • Mixed system and local headers\n";
    cout << "   • Hard to find duplicates\n";
    cout << "   • Using namespace in header (bad!)\n";

    // ✅ GOOD CODE: Organized headers
    cout << "\n✅ GOOD CODE (Organized Headers):\n";
    cout << "```cpp\n";
    cout << "// ============================================\n";
    cout << "// MyClass.cpp\n";
    cout << "// Description: User management class\n";
    cout << "// Author: Your Name\n";
    cout << "// Date: 2024-11-18\n";
    cout << "// ============================================\n\n";
    cout << "// Related header first\n";
    cout << "#include \"MyClass.h\"\n\n";
    cout << "// C++ standard library (alphabetical)\n";
    cout << "#include <algorithm>\n";
    cout << "#include <iostream>\n";
    cout << "#include <string>\n";
    cout << "#include <vector>\n\n";
    cout << "// Third-party libraries\n";
    cout << "#include <boost/filesystem.hpp>\n\n";
    cout << "// Project headers (alphabetical)\n";
    cout << "#include \"Helper.h\"\n";
    cout << "#include \"Utility.h\"\n\n";
    cout << "// Namespace usage (only in .cpp, never in .h)\n";
    cout << "using namespace std;\n\n";
    cout << "// Constants\n";
    cout << "namespace {\n";
    cout << "    const int MAX_RETRIES = 3;\n";
    cout << "    const double TIMEOUT = 30.0;\n";
    cout << "}\n\n";
    cout << "// Implementation follows...\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Clear header comment\n";
    cout << "   ✅ Logical include order\n";
    cout << "   ✅ Grouped by category\n";
    cout << "   ✅ Alphabetical within groups\n";
    cout << "   ✅ Easy to spot missing includes\n";

    cout << "\n📂 INCLUDE ORDER:\n";
    cout << "   1. Related header (MyClass.h for MyClass.cpp)\n";
    cout << "   2. C standard library (<cstdio>, <cmath>)\n";
    cout << "   3. C++ standard library (<iostream>, <vector>)\n";
    cout << "   4. Third-party libraries (<boost/...>)\n";
    cout << "   5. Project headers (\"MyHeader.h\")\n";
    cout << "   \n";
    cout << "   • Alphabetical within each group\n";
    cout << "   • Blank line between groups\n";
    cout << "   • System headers: <...>\n";
    cout << "   • Local headers: \"...\"\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Organize includes logically and alphabetically.\n";
    cout << "    Never 'using namespace' in headers!\"\n";
}

// ============================================
// EXAMPLE 8: COMMENT STYLE
// ============================================

void example8_comment_style() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 8: COMMENT STYLE                       ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    // ❌ BAD CODE: Poor comments
    cout << "\n❌ BAD CODE (Poor Comments):\n";
    cout << "```cpp\n";
    cout << "// add two numbers\n";
    cout << "int add(int a, int b) {  // function\n";
    cout << "    return a + b;  // return sum\n";
    cout << "}\n\n";
    cout << "//calculate price\n";
    cout << "double calc ( int x , int y )  //x is quantity\n";
    cout << "{//y is price per unit\n";
    cout << "return x*y;//multiply\n";
    cout << "}\n\n";
    cout << "/* This is a comment\n";
    cout << "that explains\n";
    cout << "obvious code */\n";
    cout << "int x = 5;  // Set x to 5\n";
    cout << "```\n";

    cout << "\n🚨 PROBLEMS:\n";
    cout << "   • Comments state the obvious\n";
    cout << "   • Inconsistent spacing\n";
    cout << "   • No capitalization or punctuation\n";
    cout << "   • Comments explain WHAT, not WHY\n";

    // ✅ GOOD CODE: Good comments
    cout << "\n✅ GOOD CODE (Good Comments):\n";
    cout << "```cpp\n";
    cout << "/**\n";
    cout << " * Calculate the total price including bulk discounts.\n";
    cout << " * \n";
    cout << " * Bulk discount rules:\n";
    cout << " * - 10% off for quantities >= 100\n";
    cout << " * - 20% off for quantities >= 500\n";
    cout << " * \n";
    cout << " * @param quantity Number of items\n";
    cout << " * @param pricePerUnit Price of single item\n";
    cout << " * @return Total price after discounts\n";
    cout << " */\n";
    cout << "double calculateBulkPrice(int quantity, double pricePerUnit) {\n";
    cout << "    double total = quantity * pricePerUnit;\n";
    cout << "    \n";
    cout << "    // Apply bulk discounts\n";
    cout << "    if (quantity >= 500) {\n";
    cout << "        total *= 0.8;  // 20% discount\n";
    cout << "    } else if (quantity >= 100) {\n";
    cout << "        total *= 0.9;  // 10% discount\n";
    cout << "    }\n";
    cout << "    \n";
    cout << "    return total;\n";
    cout << "}\n\n";
    cout << "// FIXME: This algorithm is O(n²), needs optimization\n";
    cout << "void slowSort(vector<int>& data) {\n";
    cout << "    // Bubble sort implementation\n";
    cout << "}\n\n";
    cout << "// TODO: Add input validation\n";
    cout << "void processInput(string input) {\n";
    cout << "    // Current implementation\n";
    cout << "}\n\n";
    cout << "// HACK: Workaround for bug in external library v1.2\n";
    cout << "// Remove this when library is updated to v1.3\n";
    cout << "void workaround() {\n";
    cout << "    // Temporary fix\n";
    cout << "}\n";
    cout << "```\n";

    cout << "\n✨ IMPROVEMENTS:\n";
    cout << "   ✅ Doxygen-style documentation\n";
    cout << "   ✅ Explains WHY, not WHAT\n";
    cout << "   ✅ Proper capitalization and punctuation\n";
    cout << "   ✅ Consistent spacing\n";
    cout << "   ✅ Useful markers (TODO, FIXME, HACK)\n";

    cout << "\n📝 COMMENT GUIDELINES:\n";
    cout << "   ✅ Document WHY, not WHAT\n";
    cout << "   ✅ Use Doxygen for public API\n";
    cout << "   ✅ Capitalize and punctuate\n";
    cout << "   ✅ Space after // or /*\n";
    cout << "   ✅ Use TODO, FIXME, HACK markers\n";
    cout << "   ❌ Don't comment obvious code\n";
    cout << "   ❌ Don't leave commented-out code\n";
    cout << "   ❌ Don't use comments to fix bad names\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Good code documents itself. Comments explain WHY,\n";
    cout << "    not WHAT. Use Doxygen for public APIs.\"\n";
}

// ============================================
// EXAMPLE 9: CONSISTENCY IS KEY
// ============================================

void example9_consistency() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 9: CONSISTENCY IS KEY                  ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🎯 WHY CONSISTENCY MATTERS:\n";
    cout << "   • Reduces cognitive load\n";
    cout << "   • Easier to spot patterns and errors\n";
    cout << "   • Faster code reviews\n";
    cout << "   • Team collaboration\n";
    cout << "   • Professional appearance\n";

    cout << "\n📋 CONSISTENCY CHECKLIST:\n";
    cout << "\n   NAMING:\n";
    cout << "   □ One naming convention (not mixed)\n";
    cout << "   □ Variables: snake_case or camelCase\n";
    cout << "   □ Classes: PascalCase\n";
    cout << "   □ Constants: UPPER_SNAKE_CASE\n";
    cout << "   □ Private members: trailing_ or leading m_\n";

    cout << "\n   FORMATTING:\n";
    cout << "   □ One brace style (K&R or Allman)\n";
    cout << "   □ Consistent indentation (2 or 4 spaces)\n";
    cout << "   □ Space after comma, around operators\n";
    cout << "   □ Lines under 100-120 characters\n";
    cout << "   □ Blank lines between functions\n";

    cout << "\n   ORGANIZATION:\n";
    cout << "   □ Consistent include order\n";
    cout << "   □ Public before private\n";
    cout << "   □ Logical grouping of methods\n";
    cout << "   □ Related functions together\n";

    cout << "\n   COMMENTS:\n";
    cout << "   □ Doxygen for public API\n";
    cout << "   □ Consistent comment style\n";
    cout << "   □ Proper capitalization\n";
    cout << "   □ Explain WHY, not WHAT\n";

    cout << "\n🔧 TOOLS TO ENFORCE CONSISTENCY:\n";
    cout << "\n   • clang-format: Auto-format code\n";
    cout << "     $ clang-format -i file.cpp\n";
    cout << "\n   • .clang-format file: Team-wide settings\n";
    cout << "\n   • EditorConfig: Editor settings\n";
    cout << "\n   • Pre-commit hooks: Enforce before commit\n";
    cout << "\n   • CI/CD checks: Verify in pipeline\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Pick a style and stick to it!\n";
    cout << "    Consistency > Personal preference.\"\n";
}

// ============================================
// EXAMPLE 10: MODERN C++ STYLE PREFERENCES
// ============================================

void example10_modern_cpp_style() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 10: MODERN C++ STYLE PREFERENCES      ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✨ MODERN C++ STYLE:\n";

    cout << "\n1. USE auto APPROPRIATELY:\n";
    cout << "   ✅ auto x = 5;                    // Clear type\n";
    cout << "   ✅ auto v = vector<int>{1, 2, 3}; // Obvious\n";
    cout << "   ✅ auto it = map.begin();         // Iterator\n";
    cout << "   ❌ auto x = calculatePrice();     // Not clear!\n";
    cout << "   ✅ double x = calculatePrice();   // Clear\n";

    cout << "\n2. USE nullptr (NOT NULL):\n";
    cout << "   ❌ int* ptr = NULL;   // Old C style\n";
    cout << "   ✅ int* ptr = nullptr; // Modern C++\n";

    cout << "\n3. USE enum class (NOT enum):\n";
    cout << "   ❌ enum Color { RED, GREEN };  // Pollutes namespace\n";
    cout << "   ✅ enum class Color { Red, Green };  // Type-safe\n";

    cout << "\n4. USE range-based for:\n";
    cout << "   ❌ for (int i = 0; i < v.size(); i++)  // Old\n";
    cout << "   ✅ for (const auto& item : v)          // Modern\n";

    cout << "\n5. USE uniform initialization:\n";
    cout << "   ❌ vector<int> v(10);      // May be confusing\n";
    cout << "   ✅ vector<int> v{10};      // Clear\n";
    cout << "   ✅ int x{5};               // Modern\n";

    cout << "\n6. USE smart pointers:\n";
    cout << "   ❌ Widget* w = new Widget();  // Manual management\n";
    cout << "   ✅ auto w = make_unique<Widget>();  // RAII\n";

    cout << "\n7. USE [[nodiscard]] for important returns:\n";
    cout << "   [[nodiscard]] bool save() { /* ... */ }\n";

    cout << "\n8. USE constexpr when possible:\n";
    cout << "   ❌ const double PI = 3.14159;\n";
    cout << "   ✅ constexpr double PI = 3.14159;\n";

    cout << "\n9. USE structured bindings (C++17):\n";
    cout << "   ❌ auto pair = getCoordinates();\n";
    cout << "      int x = pair.first;\n";
    cout << "      int y = pair.second;\n";
    cout << "   ✅ auto [x, y] = getCoordinates();\n";

    cout << "\n10. USE trailing return type (when clearer):\n";
    cout << "    ❌ template<typename T, typename U>\n";
    cout << "       decltype(t + u) add(T t, U u);\n";
    cout << "    ✅ template<typename T, typename U>\n";
    cout << "       auto add(T t, U u) -> decltype(t + u);\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Use modern C++ features for clarity and safety!\n";
    cout << "    Embrace C++11/14/17/20 improvements.\"\n";
}

// ============================================
// EXAMPLE 11: CLANG-FORMAT CONFIGURATION
// ============================================

void example11_clang_format() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 11: CLANG-FORMAT CONFIGURATION        ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n🔧 USING CLANG-FORMAT:\n";

    cout << "\n1. INSTALL:\n";
    cout << "   $ sudo apt install clang-format  # Ubuntu/Debian\n";
    cout << "   $ brew install clang-format      # macOS\n";

    cout << "\n2. FORMAT A FILE:\n";
    cout << "   $ clang-format -i file.cpp  # Format in-place\n";
    cout << "   $ clang-format file.cpp     # Show formatted output\n";

    cout << "\n3. USE PREDEFINED STYLE:\n";
    cout << "   $ clang-format -style=Google -i file.cpp\n";
    cout << "   $ clang-format -style=LLVM -i file.cpp\n";
    cout << "   $ clang-format -style=Mozilla -i file.cpp\n";

    cout << "\n4. CREATE .clang-format FILE:\n";
    cout << "```yaml\n";
    cout << "# .clang-format\n";
    cout << "---\n";
    cout << "BasedOnStyle: LLVM\n";
    cout << "IndentWidth: 4\n";
    cout << "ColumnLimit: 100\n";
    cout << "PointerAlignment: Left\n";
    cout << "BreakBeforeBraces: Attach\n";
    cout << "AllowShortFunctionsOnASingleLine: Empty\n";
    cout << "AllowShortIfStatementsOnASingleLine: Never\n";
    cout << "SpaceAfterCStyleCast: true\n";
    cout << "SpacesInParentheses: false\n";
    cout << "```\n";

    cout << "\n5. INTEGRATE WITH EDITOR:\n";
    cout << "   • VS Code: C/C++ extension\n";
    cout << "   • CLion: Built-in support\n";
    cout << "   • Vim: vim-clang-format plugin\n";
    cout << "   • Format on save\n";

    cout << "\n6. PRE-COMMIT HOOK:\n";
    cout << "```bash\n";
    cout << "#!/bin/bash\n";
    cout << "# .git/hooks/pre-commit\n";
    cout << "FILES=$(git diff --cached --name-only --diff-filter=ACM | grep '\\.cpp$\\|\\.h$')\n";
    cout << "for FILE in $FILES; do\n";
    cout << "    clang-format -i $FILE\n";
    cout << "    git add $FILE\n";
    cout << "done\n";
    cout << "```\n";

    cout << "\n📊 POPULAR STYLE GUIDES:\n";
    cout << "\n   • Google C++ Style:\n";
    cout << "     - 2-space indentation\n";
    cout << "     - K&R bracing\n";
    cout << "     - 80 character limit\n";
    cout << "\n   • LLVM Style:\n";
    cout << "     - 2-space indentation\n";
    cout << "     - K&R bracing\n";
    cout << "     - 80 character limit\n";
    cout << "\n   • Mozilla Style:\n";
    cout << "     - 2-space indentation\n";
    cout << "     - Allman bracing\n";
    cout << "     - 80 character limit\n";

    cout << "\n💡 KEY PRINCIPLE:\n";
    cout << "   \"Automate formatting with clang-format!\n";
    cout << "    Spend time on logic, not formatting.\"\n";
}

// ============================================
// EXAMPLE 12: SUMMARY & BEST PRACTICES
// ============================================

void example12_summary() {
    cout << "\n╔══════════════════════════════════════════════════╗\n";
    cout << "║   EXAMPLE 12: SUMMARY & BEST PRACTICES          ║\n";
    cout << "╚══════════════════════════════════════════════════╝\n";

    cout << "\n✅ CODE STYLE CHECKLIST:\n";

    cout << "\n📝 NAMING:\n";
    cout << "   ✅ Consistent convention (snake_case or camelCase)\n";
    cout << "   ✅ Classes: PascalCase\n";
    cout << "   ✅ Constants: UPPER_SNAKE_CASE\n";
    cout << "   ✅ Private members: trailing underscore\n";
    cout << "   ✅ Descriptive names (no x, tmp, data)\n";

    cout << "\n🎨 FORMATTING:\n";
    cout << "   ✅ One brace style (K&R recommended)\n";
    cout << "   ✅ 4 spaces indentation (or 2)\n";
    cout << "   ✅ Space after comma, around operators\n";
    cout << "   ✅ Lines under 100 characters\n";
    cout << "   ✅ Blank lines between functions\n";
    cout << "   ✅ Blank lines between logical sections\n";

    cout << "\n📂 ORGANIZATION:\n";
    cout << "   ✅ Includes: Related → STL → Third-party → Project\n";
    cout << "   ✅ Alphabetical within groups\n";
    cout << "   ✅ Classes: Public → Private\n";
    cout << "   ✅ Member variables at end\n";

    cout << "\n💬 COMMENTS:\n";
    cout << "   ✅ Doxygen for public API\n";
    cout << "   ✅ Explain WHY, not WHAT\n";
    cout << "   ✅ Proper capitalization\n";
    cout << "   ✅ TODO/FIXME/HACK markers\n";
    cout << "   ❌ No commented-out code\n";

    cout << "\n✨ MODERN C++:\n";
    cout << "   ✅ Use auto (when clear)\n";
    cout << "   ✅ Use nullptr (not NULL)\n";
    cout << "   ✅ Use enum class\n";
    cout << "   ✅ Use range-based for\n";
    cout << "   ✅ Use smart pointers\n";
    cout << "   ✅ Use constexpr\n";

    cout << "\n🔧 AUTOMATION:\n";
    cout << "   ✅ Use clang-format\n";
    cout << "   ✅ .clang-format in repo\n";
    cout << "   ✅ Format on save\n";
    cout << "   ✅ Pre-commit hooks\n";
    cout << "   ✅ CI/CD checks\n";

    cout << "\n🎯 GOLDEN RULES:\n";
    cout << "   1. Consistency > Personal preference\n";
    cout << "   2. Follow team/project style guide\n";
    cout << "   3. Use automation (clang-format)\n";
    cout << "   4. Make code readable\n";
    cout << "   5. Keep it simple\n";
    cout << "   6. Modern C++ over legacy\n";

    cout << "\n📚 RECOMMENDED STYLE GUIDES:\n";
    cout << "   • Google C++ Style Guide\n";
    cout << "   • LLVM Coding Standards\n";
    cout << "   • Mozilla Coding Style\n";
    cout << "   • C++ Core Guidelines\n";

    cout << "\n💡 FINAL WISDOM:\n";
    cout << "   \"Code formatting is like typography.\n";
    cout << "    Done well, you don't notice it.\n";
    cout << "    Done poorly, you can't read anything.\"\n";

    cout << "\n🎓 REMEMBER:\n";
    cout << "   • Formatting shows professionalism\n";
    cout << "   • Consistency helps collaboration\n";
    cout << "   • Tools automate the boring parts\n";
    cout << "   • Good style makes code maintainable\n";
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║       CODE STYLE & FORMATTING - MASTERCLASS           ║\n";
    cout << "║                                                       ║\n";
    cout << "║    Professional Code Formatting Explained!           ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n";

    example1_naming_conventions();
    example2_brace_styles();
    example3_indentation();
    example4_spacing();
    example5_line_length();
    example6_organization();
    example7_file_organization();
    example8_comment_style();
    example9_consistency();
    example10_modern_cpp_style();
    example11_clang_format();
    example12_summary();

    cout << "\n\n";
    cout << "╔═══════════════════════════════════════════════════════╗\n";
    cout << "║                                                       ║\n";
    cout << "║      🎉 CODE STYLE & FORMATTING COMPLETE! 🎉          ║\n";
    cout << "║                                                       ║\n";
    cout << "║  You've mastered professional code formatting!       ║\n";
    cout << "║  Your code will look clean and consistent!           ║\n";
    cout << "║                                                       ║\n";
    cout << "║  Next: Apply these principles with clang-format!     ║\n";
    cout << "║                                                       ║\n";
    cout << "╚═══════════════════════════════════════════════════════╝\n\n";

    return 0;
}
