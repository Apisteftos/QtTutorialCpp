#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ============================================
// WHAT IS MULTIPLE INHERITANCE?
// ============================================

/*
 * MULTIPLE INHERITANCE:
 * - A class inherits from TWO OR MORE base classes
 * - Syntax: class Derived : public Base1, public Base2 { };
 * - Combines features from multiple parents
 *
 * BENEFITS:
 * - Model complex relationships
 * - Combine functionality from multiple sources
 * - Implement multiple interfaces
 * - Code reuse from multiple classes
 *
 * PROBLEMS:
 * - Ambiguity (same name in multiple bases)
 * - Diamond problem (common ancestor)
 * - Complexity
 * - Increased coupling
 *
 * SOLUTIONS:
 * - Scope resolution (Base1::func)
 * - Virtual inheritance (for diamond)
 * - Interface-based design
 * - Composition (alternative)
 *
 * BEST PRACTICE:
 * - Use sparingly
 * - Prefer composition over inheritance
 * - Use for pure interfaces
 * - Avoid diamond inheritance
 */

// ============================================
// EXAMPLE 1: BASIC MULTIPLE INHERITANCE
// ============================================

class Engine {
protected:
    int horsepower;

public:
    Engine(int hp) : horsepower(hp) {
        cout << "  Engine constructor: " << horsepower << "hp\n";
    }

    ~Engine() {
        cout << "  Engine destructor\n";
    }

    void startEngine() {
        cout << "  Engine started: " << horsepower << "hp\n";
    }

    int getHorsepower() const {
        return horsepower;
    }
};

class Wheels {
protected:
    int numWheels;

public:
    Wheels(int wheels) : numWheels(wheels) {
        cout << "  Wheels constructor: " << numWheels << " wheels\n";
    }

    ~Wheels() {
        cout << "  Wheels destructor\n";
    }

    void roll() {
        cout << "  Rolling on " << numWheels << " wheels\n";
    }

    int getWheelCount() const {
        return numWheels;
    }
};

// Multiple inheritance - inherits from both Engine and Wheels
class Car : public Engine, public Wheels {
private:
    string brand;

public:
    Car(string b, int hp, int wheels)
        : Engine(hp), Wheels(wheels), brand(b) {
        cout << "  Car constructor: " << brand << "\n";
    }

    ~Car() {
        cout << "  Car destructor: " << brand << "\n";
    }

    void drive() {
        cout << "  Driving " << brand << ":\n";
        startEngine();
        roll();
    }

    void displayInfo() {
        cout << "  Brand: " << brand << "\n";
        cout << "  Horsepower: " << getHorsepower() << "\n";
        cout << "  Wheels: " << getWheelCount() << "\n";
    }
};

void demonstrateBasicMultiple() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BASIC MULTIPLE         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Creating car (multiple inheritance) ---\n";
    Car car("Toyota", 150, 4);

    cout << "\n--- Using car ---\n";
    car.displayInfo();
    car.drive();

    cout << "\n✅ Multiple inheritance:\n";
    cout << "   • Car inherits from Engine AND Wheels\n";
    cout << "   • Combines functionality from both\n";
    cout << "   • Constructor order: left to right\n";
    cout << "   • Destructor order: right to left\n";

    cout << "\n--- Destruction order ---\n";
}

// ============================================
// EXAMPLE 2: AMBIGUITY PROBLEM
// ============================================

class ClassA {
public:
    void display() {
        cout << "  ClassA::display()\n";
    }

    void commonFunc() {
        cout << "  ClassA::commonFunc()\n";
    }
};

class ClassB {
public:
    void display() {
        cout << "  ClassB::display()\n";
    }

    void commonFunc() {
        cout << "  ClassB::commonFunc()\n";
    }
};

class MultiDerived : public ClassA, public ClassB {
public:
    void useDisplay() {
        // display();  // ❌ Error! Ambiguous!

        // Must specify which one
        ClassA::display();  // ✅ Use ClassA's version
        ClassB::display();  // ✅ Use ClassB's version
    }

    // Can override to resolve ambiguity
    void display() {
        cout << "  MultiDerived::display() (resolves ambiguity)\n";
        ClassA::display();  // Can still call base versions
        ClassB::display();
    }
};

void demonstrateAmbiguity() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: AMBIGUITY PROBLEM      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    MultiDerived obj;

    cout << "--- Ambiguity problem ---\n";
    // obj.commonFunc();  // ❌ Error! Which commonFunc()?

    cout << "--- Solution 1: Scope resolution ---\n";
    obj.ClassA::commonFunc();
    obj.ClassB::commonFunc();

    cout << "\n--- Solution 2: Override in derived ---\n";
    obj.display();  // Calls MultiDerived version

    cout << "\n⚠️  Ambiguity occurs when:\n";
    cout << "   • Multiple bases have same function name\n";
    cout << "   • Compiler doesn't know which to call\n";
    cout << "   • Must explicitly specify with Base::func()\n";
}

// ============================================
// EXAMPLE 3: DIAMOND PROBLEM (The Problem)
// ============================================

class Device {
protected:
    string deviceName;
    int deviceId;

public:
    Device(string name, int id) : deviceName(name), deviceId(id) {
        cout << "  Device constructor: " << deviceName << " (ID: " << deviceId << ")\n";
    }

    virtual ~Device() {
        cout << "  Device destructor: " << deviceName << "\n";
    }

    void powerOn() {
        cout << "  Device " << deviceName << " powered on\n";
    }

    int getDeviceId() const {
        return deviceId;
    }
};

// Phone inherits from Device
class Phone : public Device {
public:
    Phone(string name, int id) : Device(name, id) {
        cout << "  Phone constructor\n";
    }

    ~Phone() {
        cout << "  Phone destructor\n";
    }

    void makeCall() {
        cout << "  Making a call on " << deviceName << "\n";
    }
};

// Camera inherits from Device
class Camera : public Device {
public:
    Camera(string name, int id) : Device(name, id) {
        cout << "  Camera constructor\n";
    }

    ~Camera() {
        cout << "  Camera destructor\n";
    }

    void takePicture() {
        cout << "  Taking a picture with " << deviceName << "\n";
    }
};

// SmartPhone inherits from both Phone and Camera
// This creates TWO copies of Device! (Diamond Problem)
class SmartPhone : public Phone, public Camera {
public:
    SmartPhone(string name)
        : Phone(name + "-Phone", 1), Camera(name + "-Camera", 2) {
        cout << "  SmartPhone constructor\n";
    }

    ~SmartPhone() {
        cout << "  SmartPhone destructor\n";
    }

    void useFeatures() {
        makeCall();
        takePicture();

        // powerOn();  // ❌ Ambiguous! Which Device?
        Phone::powerOn();   // Must specify
        Camera::powerOn();

        // Two different device IDs!
        cout << "  Phone device ID: " << Phone::getDeviceId() << "\n";
        cout << "  Camera device ID: " << Camera::getDeviceId() << "\n";
    }
};

void demonstrateDiamondProblem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: DIAMOND PROBLEM        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- The Diamond Problem ---\n";
    cout << "       Device\n";
    cout << "       /    \\\n";
    cout << "    Phone  Camera\n";
    cout << "       \\    /\n";
    cout << "     SmartPhone\n";
    cout << "\n";
    cout << "⚠️  SmartPhone contains TWO copies of Device!\n\n";

    cout << "--- Creating SmartPhone ---\n";
    cout << "Notice: TWO Device constructors called!\n";
    SmartPhone phone("iPhone");

    cout << "\n--- Using SmartPhone ---\n";
    phone.useFeatures();

    cout << "\n⚠️  Problems:\n";
    cout << "   • Two Device objects in SmartPhone\n";
    cout << "   • Ambiguity accessing Device members\n";
    cout << "   • Wasted memory\n";
    cout << "   • Inconsistent state\n";
    cout << "   • Solution: Virtual Inheritance (next example)\n";

    cout << "\n--- Destruction (reverse order) ---\n";
}

// ============================================
// EXAMPLE 4: VIRTUAL INHERITANCE (The Solution)
// ============================================

class VDevice {
protected:
    string vDeviceName;
    int vDeviceId;

public:
    VDevice(string name = "Unknown", int id = 0)
        : vDeviceName(name), vDeviceId(id) {
        cout << "  VDevice constructor: " << vDeviceName
             << " (ID: " << vDeviceId << ")\n";
    }

    virtual ~VDevice() {
        cout << "  VDevice destructor: " << vDeviceName << "\n";
    }

    void powerOn() {
        cout << "  VDevice " << vDeviceName << " powered on\n";
    }

    int getVDeviceId() const {
        return vDeviceId;
    }
};

// VIRTUAL inheritance - share single Device instance
class VPhone : virtual public VDevice {
public:
    VPhone(string name = "Unknown", int id = 0) : VDevice(name, id) {
        cout << "  VPhone constructor\n";
    }

    ~VPhone() {
        cout << "  VPhone destructor\n";
    }

    void makeCall() {
        cout << "  Making a call on " << vDeviceName << "\n";
    }
};

// VIRTUAL inheritance - share single Device instance
class VCamera : virtual public VDevice {
public:
    VCamera(string name = "Unknown", int id = 0) : VDevice(name, id) {
        cout << "  VCamera constructor\n";
    }

    ~VCamera() {
        cout << "  VCamera destructor\n";
    }

    void takePicture() {
        cout << "  Taking a picture with " << vDeviceName << "\n";
    }
};

// Now only ONE copy of VDevice!
class VSmartPhone : public VPhone, public VCamera {
public:
    // MUST call VDevice constructor explicitly
    VSmartPhone(string name, int id)
        : VDevice(name, id), VPhone(), VCamera() {
        cout << "  VSmartPhone constructor\n";
    }

    ~VSmartPhone() {
        cout << "  VSmartPhone destructor\n";
    }

    void useFeatures() {
        makeCall();
        takePicture();
        powerOn();  // ✅ No ambiguity now!

        // Only one device ID!
        cout << "  Device ID: " << getVDeviceId() << "\n";
    }
};

void demonstrateVirtualInheritance() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: VIRTUAL INHERITANCE    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Virtual Inheritance Solution ---\n";
    cout << "       VDevice (virtual)\n";
    cout << "       /    \\\n";
    cout << "   VPhone  VCamera\n";
    cout << "       \\    /\n";
    cout << "    VSmartPhone\n";
    cout << "\n";
    cout << "✅ VSmartPhone contains only ONE VDevice!\n\n";

    cout << "--- Creating VSmartPhone ---\n";
    cout << "Notice: Only ONE VDevice constructor!\n";
    VSmartPhone phone("iPhone", 12345);

    cout << "\n--- Using VSmartPhone ---\n";
    phone.useFeatures();

    cout << "\n✅ Virtual inheritance solves:\n";
    cout << "   • Only ONE shared base object\n";
    cout << "   • No ambiguity\n";
    cout << "   • Consistent state\n";
    cout << "   • No wasted memory\n";
    cout << "   • Most derived class calls base constructor\n";

    cout << "\n--- Destruction ---\n";
}

// ============================================
// EXAMPLE 5: MULTIPLE INTERFACE IMPLEMENTATION
// ============================================

// Pure interfaces
class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() = default;
};

class ISerializable {
public:
    virtual string serialize() const = 0;
    virtual void deserialize(const string& data) = 0;
    virtual ~ISerializable() = default;
};

class IClickable {
public:
    virtual void onClick() = 0;
    virtual ~IClickable() = default;
};

// Implements multiple interfaces
class Button : public IDrawable, public ISerializable, public IClickable {
private:
    string label;
    int x, y;

public:
    Button(string lbl, int posX, int posY)
        : label(lbl), x(posX), y(posY) { }

    // Implement IDrawable
    void draw() const override {
        cout << "  Drawing button '" << label << "' at ("
             << x << "," << y << ")\n";
    }

    // Implement ISerializable
    string serialize() const override {
        return "Button:" + label + ":" + to_string(x) + ":" + to_string(y);
    }

    void deserialize(const string& data) override {
        cout << "  Deserializing: " << data << "\n";
    }

    // Implement IClickable
    void onClick() override {
        cout << "  Button '" << label << "' clicked!\n";
    }
};

void demonstrateMultipleInterfaces() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 5: MULTIPLE INTERFACES    ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Button button("Submit", 100, 200);

    cout << "--- Using as different interfaces ---\n";

    IDrawable* drawable = &button;
    drawable->draw();

    ISerializable* serializable = &button;
    cout << "  Serialized: " << serializable->serialize() << "\n";

    IClickable* clickable = &button;
    clickable->onClick();

    cout << "\n✅ Multiple interface inheritance:\n";
    cout << "   • Pure virtual functions only\n";
    cout << "   • No diamond problem (no data)\n";
    cout << "   • Safe multiple inheritance\n";
    cout << "   • Common in design patterns\n";
}

// ============================================
// EXAMPLE 6: MIXIN PATTERN
// ============================================

// Mixin - small class adding specific functionality
template<typename Base>
class Timestamped : public Base {
private:
    string timestamp;

public:
    template<typename... Args>
    Timestamped(Args&&... args) : Base(forward<Args>(args)...) {
        timestamp = "2024-01-15 10:30:00";  // Simplified
    }

    string getTimestamp() const {
        return timestamp;
    }

    void displayWithTimestamp() const {
        cout << "  [" << timestamp << "] ";
        Base::display();
    }
};

template<typename Base>
class Loggable : public Base {
public:
    template<typename... Args>
    Loggable(Args&&... args) : Base(forward<Args>(args)...) { }

    void log(const string& message) const {
        cout << "  LOG: " << message << " - ";
        Base::display();
    }
};

class Message {
protected:
    string content;

public:
    Message(string msg) : content(msg) { }

    void display() const {
        cout << content << "\n";
    }

    virtual ~Message() = default;
};

void demonstrateMixin() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 6: MIXIN PATTERN          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Regular message ---\n";
    Message msg1("Hello World");
    msg1.display();

    cout << "\n--- Message with timestamp ---\n";
    Timestamped<Message> msg2("Important Update");
    msg2.displayWithTimestamp();

    cout << "\n--- Message with logging ---\n";
    Loggable<Message> msg3("Debug Info");
    msg3.log("System event");

    cout << "\n--- Message with timestamp AND logging ---\n";
    Loggable<Timestamped<Message>> msg4("Critical Alert");
    msg4.log("High priority");
    msg4.displayWithTimestamp();

    cout << "\n✅ Mixin pattern:\n";
    cout << "   • Small, reusable functionality\n";
    cout << "   • Compose features flexibly\n";
    cout << "   • Template-based mixing\n";
    cout << "   • Avoid inheritance hierarchy explosion\n";
}

// ============================================
// EXAMPLE 7: CONSTRUCTOR/DESTRUCTOR ORDER
// ============================================

class Base1 {
public:
    Base1() { cout << "  1. Base1 constructor\n"; }
    ~Base1() { cout << "  6. Base1 destructor\n"; }
};

class Base2 {
public:
    Base2() { cout << "  2. Base2 constructor\n"; }
    ~Base2() { cout << "  5. Base2 destructor\n"; }
};

class Base3 {
public:
    Base3() { cout << "  3. Base3 constructor\n"; }
    ~Base3() { cout << "  4. Base3 destructor\n"; }
};

class MultipleBase : public Base1, public Base2, public Base3 {
public:
    MultipleBase() { cout << "  4. MultipleBase constructor\n"; }
    ~MultipleBase() { cout << "  3. MultipleBase destructor\n"; }
};

class VBase1 : virtual public Base1 {
public:
    VBase1() { cout << "  2. VBase1 constructor\n"; }
    ~VBase1() { cout << "  5. VBase1 destructor\n"; }
};

class VBase2 : virtual public Base1 {
public:
    VBase2() { cout << "  3. VBase2 constructor\n"; }
    ~VBase2() { cout << "  4. VBase2 destructor\n"; }
};

class VMultiple : public VBase1, public VBase2 {
public:
    VMultiple() { cout << "  4. VMultiple constructor\n"; }
    ~VMultiple() { cout << "  2. VMultiple destructor\n"; }
};

void demonstrateConstructorOrder() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 7: CONSTRUCTOR ORDER      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Regular multiple inheritance ---\n";
    cout << "Construction order (left to right):\n";
    {
        MultipleBase obj;
    }
    cout << "Destruction order (reverse):\n";

    cout << "\n--- Virtual inheritance ---\n";
    cout << "Construction order (virtual bases first):\n";
    {
        VMultiple obj;
    }
    cout << "Destruction order (reverse):\n";

    cout << "\n✅ Order rules:\n";
    cout << "   Regular: Left to right, then derived\n";
    cout << "   Virtual: Virtual bases first, then order\n";
    cout << "   Destruction: Always reverse of construction\n";
}

// ============================================
// EXAMPLE 8: WHEN TO USE MULTIPLE INHERITANCE
// ============================================

// ❌ BAD: Multiple implementation inheritance
class BadLogger {
    void logToFile() { }
    void logToDatabase() { }
};

class BadValidator {
    bool validate() { return true; }
};

class BadProcessor : public BadLogger, public BadValidator {
    // Tight coupling, complexity
};

// ✅ GOOD: Multiple interface inheritance
class ILogger {
public:
    virtual void log(const string& msg) = 0;
    virtual ~ILogger() = default;
};

class IValidator {
public:
    virtual bool validate() const = 0;
    virtual ~IValidator() = default;
};

class GoodProcessor : public ILogger, public IValidator {
private:
    string data;

public:
    void log(const string& msg) override {
        cout << "  Logging: " << msg << "\n";
    }

    bool validate() const override {
        cout << "  Validating data...\n";
        return true;
    }
};

// ✅ BETTER: Composition
class Logger {
public:
    void log(const string& msg) {
        cout << "  Logging: " << msg << "\n";
    }
};

class Validator {
public:
    bool validate() const {
        cout << "  Validating...\n";
        return true;
    }
};

class BestProcessor {
private:
    Logger logger;       // HAS-A
    Validator validator; // HAS-A

public:
    void process() {
        if (validator.validate()) {
            logger.log("Processing...");
        }
    }
};

void demonstrateWhenToUse() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 8: WHEN TO USE            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Using multiple interfaces (GOOD) ---\n";
    GoodProcessor goodProc;
    goodProc.log("Test message");
    goodProc.validate();

    cout << "\n--- Using composition (BETTER) ---\n";
    BestProcessor bestProc;
    bestProc.process();

    cout << "\n✅ Use multiple inheritance when:\n";
    cout << "   • Implementing multiple INTERFACES (pure virtual)\n";
    cout << "   • No implementation conflicts\n";
    cout << "   • Truly IS-A relationships for all bases\n";
    cout << "\n";
    cout << "❌ Avoid when:\n";
    cout << "   • Multiple implementation inheritance\n";
    cout << "   • Can use composition instead\n";
    cout << "   • Diamond problem likely\n";
    cout << "   • Adds unnecessary complexity\n";
    cout << "\n";
    cout << "💡 General rule: Prefer composition over inheritance!\n";
}

// ============================================
// EXAMPLE 9: REAL-WORLD - EMPLOYEE SYSTEM
// ============================================

// Interfaces
class IManager {
public:
    virtual void manageTeam() = 0;
    virtual int getTeamSize() const = 0;
    virtual ~IManager() = default;
};

class IDeveloper {
public:
    virtual void writeCode() = 0;
    virtual string getPrimaryLanguage() const = 0;
    virtual ~IDeveloper() = default;
};

class IDesigner {
public:
    virtual void createDesign() = 0;
    virtual string getDesignTool() const = 0;
    virtual ~IDesigner() = default;
};

// Base employee class
class Employee {
protected:
    string name;
    int employeeId;

public:
    Employee(string n, int id) : name(n), employeeId(id) { }

    virtual ~Employee() = default;

    void displayInfo() const {
        cout << "  Employee: " << name << " (ID: " << employeeId << ")\n";
    }
};

// Tech Lead - manages AND codes
class TechLead : public Employee, public IManager, public IDeveloper {
private:
    int teamSize;
    string language;

public:
    TechLead(string n, int id, int team, string lang)
        : Employee(n, id), teamSize(team), language(lang) { }

    void manageTeam() override {
        cout << "  Managing team of " << teamSize << " developers\n";
    }

    int getTeamSize() const override {
        return teamSize;
    }

    void writeCode() override {
        cout << "  Writing " << language << " code\n";
    }

    string getPrimaryLanguage() const override {
        return language;
    }
};

// Design Manager - manages designers
class DesignManager : public Employee, public IManager, public IDesigner {
private:
    int teamSize;
    string tool;

public:
    DesignManager(string n, int id, int team, string t)
        : Employee(n, id), teamSize(team), tool(t) { }

    void manageTeam() override {
        cout << "  Managing design team of " << teamSize << "\n";
    }

    int getTeamSize() const override {
        return teamSize;
    }

    void createDesign() override {
        cout << "  Creating designs with " << tool << "\n";
    }

    string getDesignTool() const override {
        return tool;
    }
};

void demonstrateEmployeeSystem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 9: EMPLOYEE SYSTEM        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    TechLead techLead("Alice", 1001, 5, "C++");
    DesignManager designMgr("Bob", 1002, 3, "Figma");

    cout << "--- Tech Lead (Manager + Developer) ---\n";
    techLead.displayInfo();
    techLead.manageTeam();
    techLead.writeCode();

    cout << "\n--- Design Manager (Manager + Designer) ---\n";
    designMgr.displayInfo();
    designMgr.manageTeam();
    designMgr.createDesign();

    cout << "\n--- Using as different interfaces ---\n";
    vector<IManager*> managers = { &techLead, &designMgr };
    cout << "All managers:\n";
    for (IManager* mgr : managers) {
        cout << "  Team size: " << mgr->getTeamSize() << "\n";
    }

    cout << "\n✅ Multiple interfaces enable:\n";
    cout << "   • Flexible role combinations\n";
    cout << "   • Interface-based polymorphism\n";
    cout << "   • Clean separation of concerns\n";
}

// ============================================
// EXAMPLE 10: COMPOSITION ALTERNATIVE
// ============================================

// Using composition instead of multiple inheritance
class AudioPlayer {
public:
    void playAudio() {
        cout << "  Playing audio\n";
    }
};

class VideoPlayer {
public:
    void playVideo() {
        cout << "  Playing video\n";
    }
};

// Multiple inheritance approach
class MediaPlayerInheritance : public AudioPlayer, public VideoPlayer {
public:
    void playMedia() {
        playAudio();
        playVideo();
    }
};

// Composition approach (BETTER!)
class MediaPlayerComposition {
private:
    AudioPlayer audioPlayer;  // HAS-A
    VideoPlayer videoPlayer;  // HAS-A

public:
    void playMedia() {
        audioPlayer.playAudio();
        videoPlayer.playVideo();
    }

    // More control and flexibility
    void playAudioOnly() {
        audioPlayer.playAudio();
    }

    void playVideoOnly() {
        videoPlayer.playVideo();
    }
};

void demonstrateCompositionAlternative() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 10: COMPOSITION vs MI     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "--- Multiple Inheritance approach ---\n";
    MediaPlayerInheritance player1;
    player1.playMedia();

    cout << "\n--- Composition approach (BETTER) ---\n";
    MediaPlayerComposition player2;
    player2.playMedia();
    player2.playAudioOnly();

    cout << "\n✅ Composition advantages:\n";
    cout << "   • Loose coupling\n";
    cout << "   • More flexible\n";
    cout << "   • Easier to test\n";
    cout << "   • Can swap implementations\n";
    cout << "   • Avoids diamond problem\n";
    cout << "   • Runtime flexibility\n";
    cout << "\n";
    cout << "💡 \"Prefer composition over inheritance\"\n";
    cout << "   - Gang of Four, Design Patterns\n";
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    cout << "========================================\n";
    cout << "MULTIPLE INHERITANCE DEMONSTRATION\n";
    cout << "========================================\n";

    demonstrateBasicMultiple();
    demonstrateAmbiguity();
    demonstrateDiamondProblem();
    demonstrateVirtualInheritance();
    demonstrateMultipleInterfaces();
    demonstrateMixin();
    demonstrateConstructorOrder();
    demonstrateWhenToUse();
    demonstrateEmployeeSystem();
    demonstrateCompositionAlternative();

    // ============================================
    // Comprehensive Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "WHAT IS MULTIPLE INHERITANCE?\n";
    cout << "========================================\n";
    cout << "Multiple inheritance allows a class to inherit\n";
    cout << "from TWO OR MORE base classes, combining their\n";
    cout << "features and functionality.\n";
    cout << "\n";
    cout << "Syntax:\n";
    cout << "  class Derived : public Base1, public Base2 { };\n";
    cout << "\n";
    cout << "Key concepts:\n";
    cout << "  • Inherit from multiple parents\n";
    cout << "  • Combine functionality\n";
    cout << "  • Implement multiple interfaces\n";
    cout << "  • Diamond problem and virtual inheritance\n";

    cout << "\n========================================\n";
    cout << "BASIC SYNTAX\n";
    cout << "========================================\n";
    cout << "SIMPLE MULTIPLE INHERITANCE:\n";
    cout << "class Base1 {\n";
    cout << "public:\n";
    cout << "    void func1() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Base2 {\n";
    cout << "public:\n";
    cout << "    void func2() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Derived : public Base1, public Base2 {\n";
    cout << "public:\n";
    cout << "    void useBase() {\n";
    cout << "        func1();  // From Base1\n";
    cout << "        func2();  // From Base2\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "CONSTRUCTOR:\n";
    cout << "Derived() : Base1(args1), Base2(args2) {\n";
    cout << "    // Constructor body\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "CONSTRUCTOR/DESTRUCTOR ORDER\n";
    cout << "========================================\n";
    cout << "CONSTRUCTION (left to right):\n";
    cout << "class D : public A, public B, public C { };\n";
    cout << "\n";
    cout << "Order:\n";
    cout << "  1. A constructor\n";
    cout << "  2. B constructor\n";
    cout << "  3. C constructor\n";
    cout << "  4. D constructor\n";
    cout << "\n";
    cout << "DESTRUCTION (reverse order):\n";
    cout << "  1. D destructor\n";
    cout << "  2. C destructor\n";
    cout << "  3. B destructor\n";
    cout << "  4. A destructor\n";
    cout << "\n";
    cout << "WITH VIRTUAL INHERITANCE:\n";
    cout << "  1. Virtual base constructors first\n";
    cout << "  2. Then non-virtual bases (left to right)\n";
    cout << "  3. Then derived constructor\n";

    cout << "\n========================================\n";
    cout << "AMBIGUITY PROBLEM\n";
    cout << "========================================\n";
    cout << "Problem:\n";
    cout << "class A {\n";
    cout << "    void func() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "class B {\n";
    cout << "    void func() { }  // Same name!\n";
    cout << "};\n";
    cout << "\n";
    cout << "class C : public A, public B {\n";
    cout << "    void use() {\n";
    cout << "        func();  // ❌ Ambiguous! Which func()?\n";
    cout << "    }\n";
    cout << "};\n";
    cout << "\n";
    cout << "SOLUTION 1: Scope resolution\n";
    cout << "A::func();  // Explicitly specify\n";
    cout << "B::func();\n";
    cout << "\n";
    cout << "SOLUTION 2: Override in derived\n";
    cout << "void func() override {\n";
    cout << "    A::func();  // Choose which to call\n";
    cout << "}\n";

    cout << "\n========================================\n";
    cout << "DIAMOND PROBLEM\n";
    cout << "========================================\n";
    cout << "The Problem:\n";
    cout << "       Base\n";
    cout << "       /  \\\n";
    cout << "      A    B\n";
    cout << "       \\  /\n";
    cout << "        C\n";
    cout << "\n";
    cout << "C inherits from A and B,\n";
    cout << "A and B both inherit from Base.\n";
    cout << "Result: C has TWO copies of Base!\n";
    cout << "\n";
    cout << "Problems:\n";
    cout << "  • Duplicate base objects\n";
    cout << "  • Ambiguity accessing Base members\n";
    cout << "  • Wasted memory\n";
    cout << "  • Inconsistent state\n";
    cout << "\n";
    cout << "Example:\n";
    cout << "class Base { int data; };\n";
    cout << "class A : public Base { };\n";
    cout << "class B : public Base { };\n";
    cout << "class C : public A, public B {\n";
    cout << "    // C has TWO Base::data members!\n";
    cout << "    void use() {\n";
    cout << "        data = 10;  // ❌ Ambiguous!\n";
    cout << "    }\n";
    cout << "};\n";

    cout << "\n========================================\n";
    cout << "VIRTUAL INHERITANCE (Solution)\n";
    cout << "========================================\n";
    cout << "Syntax:\n";
    cout << "class A : virtual public Base { };\n";
    cout << "class B : virtual public Base { };\n";
    cout << "class C : public A, public B { };\n";
    cout << "\n";
    cout << "Result: C has only ONE Base!\n";
    cout << "\n";
    cout << "Key points:\n";
    cout << "  • Use 'virtual' keyword\n";
    cout << "  • Shared single base instance\n";
    cout << "  • No ambiguity\n";
    cout << "  • Most derived class calls base constructor\n";
    cout << "\n";
    cout << "Constructor:\n";
    cout << "C() : Base(args), A(), B() {\n";
    cout << "    // C must call Base constructor directly\n";
    cout << "}\n";
    cout << "\n";
    cout << "Visual:\n";
    cout << "       Base (shared)\n";
    cout << "       /  \\\n";
    cout << "      A    B (virtual)\n";
    cout << "       \\  /\n";
    cout << "        C\n";
    cout << "Only ONE Base object!\n";

    cout << "\n========================================\n";
    cout << "MULTIPLE INTERFACES (Best Practice)\n";
    cout << "========================================\n";
    cout << "Interfaces = Pure virtual functions only\n";
    cout << "\n";
    cout << "class IDrawable {\n";
    cout << "public:\n";
    cout << "    virtual void draw() = 0;\n";
    cout << "    virtual ~IDrawable() = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "class ISerializable {\n";
    cout << "public:\n";
    cout << "    virtual string serialize() = 0;\n";
    cout << "    virtual ~ISerializable() = default;\n";
    cout << "};\n";
    cout << "\n";
    cout << "class Widget : public IDrawable, public ISerializable {\n";
    cout << "    // Implement both interfaces\n";
    cout << "};\n";
    cout << "\n";
    cout << "Benefits:\n";
    cout << "  • No diamond problem (no data)\n";
    cout << "  • Safe multiple inheritance\n";
    cout << "  • Clear contracts\n";
    cout << "  • Flexible polymorphism\n";
    cout << "\n";
    cout << "✅ This is the RECOMMENDED use of multiple inheritance!\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE MULTIPLE INHERITANCE\n";
    cout << "========================================\n";
    cout << "✅ USE FOR:\n";
    cout << "  1. Multiple INTERFACES (pure virtual)\n";
    cout << "     class C : public IInterface1, public IInterface2\n";
    cout << "  \n";
    cout << "  2. Mixin pattern (small, reusable features)\n";
    cout << "     template<typename T> class Loggable : public T\n";
    cout << "  \n";
    cout << "  3. True multiple IS-A relationships\n";
    cout << "     (rare in practice)\n";
    cout << "\n";
    cout << "❌ AVOID FOR:\n";
    cout << "  • Multiple implementation inheritance\n";
    cout << "  • Complex hierarchies\n";
    cout << "  • When composition works\n";
    cout << "  • Diamond inheritance\n";
    cout << "  • Just for code reuse\n";
    cout << "\n";
    cout << "⚠️  General rule:\n";
    cout << "   \"Prefer composition over inheritance\"\n";
    cout << "   - Gang of Four\n";

    cout << "\n========================================\n";
    cout << "COMPOSITION vs MULTIPLE INHERITANCE\n";
    cout << "========================================\n";
    cout << "MULTIPLE INHERITANCE:\n";
    cout << "class Logger { };\n";
    cout << "class Validator { };\n";
    cout << "class Processor : public Logger, public Validator {\n";
    cout << "    // IS-A Logger AND IS-A Validator\n";
    cout << "};\n";
    cout << "\n";
    cout << "COMPOSITION (BETTER!):\n";
    cout << "class Processor {\n";
    cout << "    Logger logger;        // HAS-A Logger\n";
    cout << "    Validator validator;  // HAS-A Validator\n";
    cout << "};\n";
    cout << "\n";
    cout << "Composition advantages:\n";
    cout << "  ✅ Loose coupling\n";
    cout << "  ✅ More flexible\n";
    cout << "  ✅ Easier to test\n";
    cout << "  ✅ Can swap implementations\n";
    cout << "  ✅ No diamond problem\n";
    cout << "  ✅ Runtime flexibility\n";
    cout << "  ✅ Clearer relationships\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "✅ DO:\n";
    cout << "  • Use for multiple pure interfaces\n";
    cout << "  • Make destructors virtual\n";
    cout << "  • Use virtual inheritance for diamond\n";
    cout << "  • Document inheritance relationships\n";
    cout << "  • Keep hierarchies shallow\n";
    cout << "  • Consider composition first\n";
    cout << "  • Test thoroughly\n";
    cout << "\n";
    cout << "❌ DON'T:\n";
    cout << "  • Use for implementation reuse (use composition)\n";
    cout << "  • Create diamond inheritance without virtual\n";
    cout << "  • Make deep hierarchies\n";
    cout << "  • Ignore ambiguity errors\n";
    cout << "  • Use when single inheritance suffices\n";
    cout << "  • Forget to call base constructors\n";
    cout << "  • Use just because you can\n";

    cout << "\n========================================\n";
    cout << "COMMON MISTAKES\n";
    cout << "========================================\n";
    cout << "❌ Mistake 1: Not using virtual for diamond\n";
    cout << "class B : public A { };  // Should be virtual!\n";
    cout << "Fix: class B : virtual public A { };\n";
    cout << "\n";
    cout << "❌ Mistake 2: Ambiguous function calls\n";
    cout << "obj.func();  // Which base's func()?\n";
    cout << "Fix: obj.Base1::func();\n";
    cout << "\n";
    cout << "❌ Mistake 3: Not calling virtual base constructor\n";
    cout << "Most derived class must call virtual base!\n";
    cout << "\n";
    cout << "❌ Mistake 4: Using MI when composition better\n";
    cout << "class A : public B, public C { };\n";
    cout << "Fix: class A { B b; C c; };  // HAS-A\n";

    cout << "\n========================================\n";
    cout << "QUICK REFERENCE\n";
    cout << "========================================\n";
    cout << "// Basic multiple inheritance\n";
    cout << "class D : public A, public B {\n";
    cout << "    D() : A(args), B(args) { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Virtual inheritance\n";
    cout << "class A : virtual public Base { };\n";
    cout << "class B : virtual public Base { };\n";
    cout << "class C : public A, public B {\n";
    cout << "    C() : Base(args), A(), B() { }\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Multiple interfaces (RECOMMENDED)\n";
    cout << "class W : public IInterface1, public IInterface2 {\n";
    cout << "    // Implement all pure virtuals\n";
    cout << "};\n";
    cout << "\n";
    cout << "// Resolve ambiguity\n";
    cout << "Base1::func();  // Explicit scope\n";

    cout << "\n========================================\n";
    cout << "KEY TAKEAWAYS\n";
    cout << "========================================\n";
    cout << "1. Multiple inheritance = inherit from 2+ bases\n";
    cout << "2. Can cause ambiguity and diamond problems\n";
    cout << "3. Use virtual inheritance to solve diamond\n";
    cout << "4. Best for multiple pure interfaces\n";
    cout << "5. Constructor order: left to right\n";
    cout << "6. Virtual bases constructed first\n";
    cout << "7. Use scope resolution for ambiguity\n";
    cout << "8. Prefer composition over inheritance\n";
    cout << "9. Keep hierarchies simple\n";
    cout << "10. Document and test thoroughly\n";

    return 0;
}
