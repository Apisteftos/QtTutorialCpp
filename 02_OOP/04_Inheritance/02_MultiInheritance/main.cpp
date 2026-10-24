#include <print>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

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
    explicit Engine(int hp) : horsepower(hp) {
        std::println("  Engine constructor: {}hp", horsepower);
    }

    ~Engine() {
        std::println("  Engine destructor");
    }

    void startEngine() {
        std::println("  Engine started: {}hp", horsepower);
    }

    int getHorsepower() const {
        return horsepower;
    }
};

class Wheels {
protected:
    int numWheels;

public:
    explicit Wheels(int wheels) : numWheels(wheels) {
        std::println("  Wheels constructor: {} wheels", numWheels);
    }

    ~Wheels() {
        std::println("  Wheels destructor");
    }

    void roll() {
        std::println("  Rolling on {} wheels", numWheels);
    }

    int getWheelCount() const {
        return numWheels;
    }
};

// Multiple inheritance - inherits from both Engine and Wheels
class Car : public Engine, public Wheels {
private:
    std::string brand;

public:
    Car(std::string b, int hp, int wheels)
        : Engine(hp), Wheels(wheels), brand(b) {
        std::println("  Car constructor: {}", brand);
    }

    ~Car() {
        std::println("  Car destructor: {}", brand);
    }

    void drive() {
        std::println("  Driving {}:", brand);
        startEngine();
        roll();
    }

    void displayInfo() {
        std::println("  Brand: {}", brand);
        std::println("  Horsepower: {}", getHorsepower());
        std::println("  Wheels: {}", getWheelCount());
    }
};

void demonstrateBasicMultiple() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BASIC MULTIPLE         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Creating car (multiple inheritance) ---");
    Car car("Toyota", 150, 4);

    std::println("");
    std::println("--- Using car ---");
    car.displayInfo();
    car.drive();
    std::println("");
    std::println("--- Where do the base subobjects live? ---");
    const char* start = reinterpret_cast<const char*>(&car);
    std::println("  Engine subobject at offset {}", reinterpret_cast<const char*>(static_cast<Engine*>(&car)) - start);
    std::println("  Wheels subobject at offset {} (GCC x86-64)", reinterpret_cast<const char*>(static_cast<Wheels*>(&car)) - start);

    std::println("");
    std::println("--- Destruction order ---");
}

// ============================================
// EXAMPLE 2: AMBIGUITY PROBLEM
// ============================================

class ClassA {
public:
    void display() {
        std::println("  ClassA::display()");
    }

    void commonFunc() {
        std::println("  ClassA::commonFunc()");
    }
};

class ClassB {
public:
    void display() {
        std::println("  ClassB::display()");
    }

    void commonFunc() {
        std::println("  ClassB::commonFunc()");
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

    // Declaring display() here hides both base versions
    // (not virtual: this is hiding, not overriding)
    void display() {
        std::println("  MultiDerived::display() (resolves ambiguity)");
        ClassA::display();  // Can still call base versions
        ClassB::display();
    }
};
// Compile-time check: is "t.commonFunc()" a valid expression for T?
template<typename T>
constexpr bool hasCommonFunc = requires(T t) { t.commonFunc(); };
void demonstrateAmbiguity() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: AMBIGUITY PROBLEM      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    MultiDerived obj;

    std::println("--- Ambiguity problem ---");
    // obj.commonFunc();  // ❌ Error! Which commonFunc()?

    std::println("--- Solution 1: Scope resolution ---");
    obj.ClassA::commonFunc();
    obj.ClassB::commonFunc();

    std::println("");
    std::println("--- Solution 2: Override in derived ---");
    obj.display();  // Calls MultiDerived version
    std::println("");
    std::println("--- Does the call compile? ---");
    std::println("  ClassA alone:  a.commonFunc() -> {}", hasCommonFunc<ClassA>);
    std::println("  MultiDerived:  obj.commonFunc() -> {} (ambiguous)", hasCommonFunc<MultiDerived>);
}

// ============================================
// EXAMPLE 3: DIAMOND PROBLEM (The Problem)
// ============================================

class Device {
protected:
    std::string deviceName;
    int deviceId;

public:
    Device(std::string name, int id) : deviceName(name), deviceId(id) {
        std::println("  Device constructor: {} (ID: {})", deviceName, deviceId);
    }

    virtual ~Device() {
        std::println("  Device destructor: {}", deviceName);
    }

    void powerOn() {
        std::println("  Device {} powered on", deviceName);
    }

    int getDeviceId() const {
        return deviceId;
    }
};

// Phone inherits from Device
class Phone : public Device {
public:
    Phone(std::string name, int id) : Device(name, id) {
        std::println("  Phone constructor");
    }

    ~Phone() {
        std::println("  Phone destructor");
    }

    void makeCall() {
        std::println("  Making a call on {}", deviceName);
    }
};

// Camera inherits from Device
class Camera : public Device {
public:
    Camera(std::string name, int id) : Device(name, id) {
        std::println("  Camera constructor");
    }

    ~Camera() {
        std::println("  Camera destructor");
    }

    void takePicture() {
        std::println("  Taking a picture with {}", deviceName);
    }
};

// SmartPhone inherits from both Phone and Camera
// This creates TWO copies of Device! (Diamond Problem)
class SmartPhone : public Phone, public Camera {
public:
    explicit SmartPhone(std::string name)
        : Phone(name + "-Phone", 1), Camera(name + "-Camera", 2) {
        std::println("  SmartPhone constructor");
    }

    ~SmartPhone() {
        std::println("  SmartPhone destructor");
    }

    void useFeatures() {
        makeCall();
        takePicture();

        // powerOn();  // ❌ Ambiguous! Which Device?
        Phone::powerOn();   // Must specify
        Camera::powerOn();

        // Two different device IDs!
        std::println("  Phone device ID: {}", Phone::getDeviceId());
        std::println("  Camera device ID: {}", Camera::getDeviceId());
    }
};

void demonstrateDiamondProblem() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: DIAMOND PROBLEM        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("⚠️  SmartPhone contains TWO copies of Device!");
    std::println("");

    std::println("--- Creating SmartPhone ---");
    std::println("Notice: TWO Device constructors called!");
    SmartPhone phone("iPhone");

    std::println("");
    std::println("--- Using SmartPhone ---");
    phone.useFeatures();
    std::println("");
    std::println("--- Two Device subobjects ---");
    std::println("  SmartPhone* converts to Device*: {} (ambiguous)", std::is_convertible_v<SmartPhone*, Device*>);
    std::println("  sizeof(SmartPhone): {} (GCC x86-64)", sizeof(SmartPhone));

    std::println("");
    std::println("--- Destruction (reverse order) ---");
}

// ============================================
// EXAMPLE 4: VIRTUAL INHERITANCE (The Solution)
// ============================================

class VDevice {
protected:
    std::string vDeviceName;
    int vDeviceId;

public:
    explicit VDevice(std::string name = "Unknown", int id = 0)
        : vDeviceName(name), vDeviceId(id) {
        std::println("  VDevice constructor: {} (ID: {})", vDeviceName, vDeviceId);
    }

    virtual ~VDevice() {
        std::println("  VDevice destructor: {}", vDeviceName);
    }

    void powerOn() {
        std::println("  VDevice {} powered on", vDeviceName);
    }

    int getVDeviceId() const {
        return vDeviceId;
    }
};

// VIRTUAL inheritance - share single Device instance
class VPhone : virtual public VDevice {
public:
    explicit VPhone(std::string name = "Unknown", int id = 0) : VDevice(name, id) {
        std::println("  VPhone constructor");
    }

    ~VPhone() {
        std::println("  VPhone destructor");
    }

    void makeCall() {
        std::println("  Making a call on {}", vDeviceName);
    }
};

// VIRTUAL inheritance - share single Device instance
class VCamera : virtual public VDevice {
public:
    explicit VCamera(std::string name = "Unknown", int id = 0) : VDevice(name, id) {
        std::println("  VCamera constructor");
    }

    ~VCamera() {
        std::println("  VCamera destructor");
    }

    void takePicture() {
        std::println("  Taking a picture with {}", vDeviceName);
    }
};

// Now only ONE copy of VDevice!
class VSmartPhone : public VPhone, public VCamera {
public:
    // MUST call VDevice constructor explicitly
    VSmartPhone(std::string name, int id)
        : VDevice(name, id), VPhone(), VCamera() {
        std::println("  VSmartPhone constructor");
    }

    ~VSmartPhone() {
        std::println("  VSmartPhone destructor");
    }

    void useFeatures() {
        makeCall();
        takePicture();
        powerOn();  // ✅ No ambiguity now!

        // Only one device ID!
        std::println("  Device ID: {}", getVDeviceId());
    }
};

void demonstrateVirtualInheritance() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: VIRTUAL INHERITANCE    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Virtual Inheritance Solution ---");
    std::println("✅ VSmartPhone contains only ONE VDevice!");
    std::println("");

    std::println("--- Creating VSmartPhone ---");
    std::println("Notice: Only ONE VDevice constructor!");
    VSmartPhone phone("iPhone", 12345);

    std::println("");
    std::println("--- Using VSmartPhone ---");
    phone.useFeatures();
    std::println("");
    std::println("--- One shared VDevice ---");
    std::println("  VSmartPhone* converts to VDevice*: {}", std::is_convertible_v<VSmartPhone*, VDevice*>);
    std::println("  sizeof(VSmartPhone): {} (GCC x86-64)", sizeof(VSmartPhone));

    std::println("");
    std::println("--- Destruction ---");
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
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
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
    std::string label;
    int x, y;

public:
    Button(std::string lbl, int posX, int posY)
        : label(lbl), x(posX), y(posY) { }

    // Implement IDrawable
    void draw() const override {
        std::println("  Drawing button '{}' at ({},{})", label, x, y);
    }

    // Implement ISerializable
    std::string serialize() const override {
        return "Button:" + label + ":" + std::to_string(x) + ":" + std::to_string(y);
    }

    void deserialize(const std::string& data) override {
        std::println("  Deserializing: {}", data);
    }

    // Implement IClickable
    void onClick() override {
        std::println("  Button '{}' clicked!", label);
    }
};

void demonstrateMultipleInterfaces() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 5: MULTIPLE INTERFACES    ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Button button("Submit", 100, 200);

    std::println("--- Using as different interfaces ---");

    IDrawable* drawable = &button;
    drawable->draw();

    ISerializable* serializable = &button;
    std::println("  Serialized: {}", serializable->serialize());

    IClickable* clickable = &button;
    clickable->onClick();
}

// ============================================
// EXAMPLE 6: MIXIN PATTERN
// ============================================

// Mixin - small class adding specific functionality
template<typename Base>
class Timestamped : public Base {
private:
    std::string timestamp;

public:
    template<typename... Args>
    explicit Timestamped(Args&&... args) : Base(std::forward<Args>(args)...) {
        timestamp = "2024-01-15 10:30:00";  // Simplified
    }

    std::string getTimestamp() const {
        return timestamp;
    }

    void displayWithTimestamp() const {
        std::print("  [{}] ", timestamp);
        Base::display();
    }
};

template<typename Base>
class Loggable : public Base {
public:
    template<typename... Args>
    explicit Loggable(Args&&... args) : Base(std::forward<Args>(args)...) { }

    void log(const std::string& message) const {
        std::print("  LOG: {} - ", message);
        Base::display();
    }
};

class Message {
protected:
    std::string content;

public:
    explicit Message(std::string msg) : content(msg) { }

    void display() const {
        std::println("{}", content);
    }

    virtual ~Message() = default;
};

void demonstrateMixin() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 6: MIXIN PATTERN          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Regular message ---");
    Message msg1("Hello World");
    msg1.display();

    std::println("");
    std::println("--- Message with timestamp ---");
    Timestamped<Message> msg2("Important Update");
    msg2.displayWithTimestamp();

    std::println("");
    std::println("--- Message with logging ---");
    Loggable<Message> msg3("Debug Info");
    msg3.log("System event");

    std::println("");
    std::println("--- Message with timestamp AND logging ---");
    Loggable<Timestamped<Message>> msg4("Critical Alert");
    msg4.log("High priority");
    msg4.displayWithTimestamp();
}

// ============================================
// EXAMPLE 7: CONSTRUCTOR/DESTRUCTOR ORDER
// ============================================

class Base1 {
public:
    Base1() { std::println("  1. Base1 constructor"); }
    ~Base1() { std::println("  4. Base1 destructor"); }
};

class Base2 {
public:
    Base2() { std::println("  2. Base2 constructor"); }
    ~Base2() { std::println("  3. Base2 destructor"); }
};

class Base3 {
public:
    Base3() { std::println("  3. Base3 constructor"); }
    ~Base3() { std::println("  2. Base3 destructor"); }
};

class MultipleBase : public Base1, public Base2, public Base3 {
public:
    MultipleBase() { std::println("  4. MultipleBase constructor"); }
    ~MultipleBase() { std::println("  1. MultipleBase destructor"); }
};

class VBase1 : virtual public Base1 {
public:
    VBase1() { std::println("  2. VBase1 constructor"); }
    ~VBase1() { std::println("  3. VBase1 destructor"); }
};

class VBase2 : virtual public Base1 {
public:
    VBase2() { std::println("  3. VBase2 constructor"); }
    ~VBase2() { std::println("  2. VBase2 destructor"); }
};

class VMultiple : public VBase1, public VBase2 {
public:
    VMultiple() { std::println("  4. VMultiple constructor"); }
    ~VMultiple() { std::println("  1. VMultiple destructor"); }
};

void demonstrateConstructorOrder() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 7: CONSTRUCTOR ORDER      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Regular multiple inheritance ---");
    std::println("Construction order (left to right):");
    {
        MultipleBase obj;
        std::println("Destruction order (reverse):");
    }

    std::println("");
    std::println("--- Virtual inheritance ---");
    std::println("Construction order (virtual bases first):");
    {
        VMultiple obj;
        std::println("Destruction order (reverse):");
    }
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
    virtual void log(const std::string& msg) = 0;
    virtual ~ILogger() = default;
};

class IValidator {
public:
    virtual bool validate() const = 0;
    virtual ~IValidator() = default;
};

class GoodProcessor : public ILogger, public IValidator {
private:
    std::string data;

public:
    void log(const std::string& msg) override {
        std::println("  Logging: {}", msg);
    }

    bool validate() const override {
        std::println("  Validating data...");
        return true;
    }
};

// ✅ BETTER: Composition
class Logger {
public:
    void log(const std::string& msg) {
        std::println("  Logging: {}", msg);
    }
};

class Validator {
public:
    bool validate() const {
        std::println("  Validating...");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 8: WHEN TO USE            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Using multiple interfaces (GOOD) ---");
    GoodProcessor goodProc;
    goodProc.log("Test message");
    goodProc.validate();

    std::println("");
    std::println("--- Using composition (BETTER) ---");
    BestProcessor bestProc;
    bestProc.process();
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
    virtual std::string getPrimaryLanguage() const = 0;
    virtual ~IDeveloper() = default;
};

class IDesigner {
public:
    virtual void createDesign() = 0;
    virtual std::string getDesignTool() const = 0;
    virtual ~IDesigner() = default;
};

// Base employee class
class Employee {
protected:
    std::string name;
    int employeeId;

public:
    Employee(std::string n, int id) : name(n), employeeId(id) { }

    virtual ~Employee() = default;

    void displayInfo() const {
        std::println("  Employee: {} (ID: {})", name, employeeId);
    }
};

// Tech Lead - manages AND codes
class TechLead : public Employee, public IManager, public IDeveloper {
private:
    int teamSize;
    std::string language;

public:
    TechLead(std::string n, int id, int team, std::string lang)
        : Employee(n, id), teamSize(team), language(lang) { }

    void manageTeam() override {
        std::println("  Managing team of {} developers", teamSize);
    }

    int getTeamSize() const override {
        return teamSize;
    }

    void writeCode() override {
        std::println("  Writing {} code", language);
    }

    std::string getPrimaryLanguage() const override {
        return language;
    }
};

// Design Manager - manages designers
class DesignManager : public Employee, public IManager, public IDesigner {
private:
    int teamSize;
    std::string tool;

public:
    DesignManager(std::string n, int id, int team, std::string t)
        : Employee(n, id), teamSize(team), tool(t) { }

    void manageTeam() override {
        std::println("  Managing design team of {}", teamSize);
    }

    int getTeamSize() const override {
        return teamSize;
    }

    void createDesign() override {
        std::println("  Creating designs with {}", tool);
    }

    std::string getDesignTool() const override {
        return tool;
    }
};

void demonstrateEmployeeSystem() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 9: EMPLOYEE SYSTEM        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    TechLead techLead("Alice", 1001, 5, "C++");
    DesignManager designMgr("Bob", 1002, 3, "Figma");

    std::println("--- Tech Lead (Manager + Developer) ---");
    techLead.displayInfo();
    techLead.manageTeam();
    techLead.writeCode();

    std::println("");
    std::println("--- Design Manager (Manager + Designer) ---");
    designMgr.displayInfo();
    designMgr.manageTeam();
    designMgr.createDesign();

    std::println("");
    std::println("--- Using as different interfaces ---");
    std::vector<IManager*> managers = { &techLead, &designMgr };
    std::println("All managers:");
    for (IManager* mgr : managers) {
        std::println("  Team size: {}", mgr->getTeamSize());
    }
}

// ============================================
// EXAMPLE 10: COMPOSITION ALTERNATIVE
// ============================================

// Using composition instead of multiple inheritance
class AudioPlayer {
public:
    void playAudio() {
        std::println("  Playing audio");
    }
};

class VideoPlayer {
public:
    void playVideo() {
        std::println("  Playing video");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 10: COMPOSITION vs MI     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    std::println("--- Multiple Inheritance approach ---");
    MediaPlayerInheritance player1;
    player1.playMedia();

    std::println("");
    std::println("--- Composition approach (BETTER) ---");
    MediaPlayerComposition player2;
    player2.playMedia();
    player2.playAudioOnly();
}

// ============================================
// MAIN DEMONSTRATION
// ============================================

int main() {
    std::println("========================================");
    std::println("MULTIPLE INHERITANCE DEMONSTRATION");
    std::println("========================================");

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

    return 0;
}

