#include <iostream>
#include <string>

// 🔹 Base class
class Animal {
protected:
    std::string name;

public:
    // Constructor
    Animal(std::string name) {
        this->name = name; // using this pointer
    }

    // Virtual function (for polymorphism)
    virtual void speak() {
        std::cout << name << " makes a sound\n";
    }

    // Const member function
    void info() const {
        std::cout << "Animal name: " << name << "\n";
    }

    // Virtual destructor (important!)
    virtual ~Animal() {
        std::cout << "Animal destroyed\n";
    }
};

// 🔹 Derived class
class Dog : public Animal {
private:
    int age;

public:
    // Static member
    static int dogCount;

    // Constructor
    Dog(std::string name, int age) : Animal(name), age(age) {
        dogCount++;
    }

    // Method override
    void speak() override {
        std::cout << name << " says Woof!\n";
    }

    int getAge() const {
        return age;
    }

    // Friend function
    friend void showDog(const Dog& d);
};

// Initialize static member
int Dog::dogCount = 0;

// Friend function (access private members)
void showDog(const Dog& d) {
    std::cout << "Dog: " << d.name << ", Age: " << d.age << "\n";
}

// 🔹 Another derived class
class Cat : public Animal {
public:
    Cat(std::string name) : Animal(name) {}

    void speak() override {
        std::cout << name << " says Meow!\n";
    }
};

// 🔹 Abstract class (interface style)
class Shape {
public:
    virtual double area() = 0; // pure virtual
};

// 🔹 Concrete class implementing abstract class
class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double area() override {
        return width * height;
    }
};

// 🔹 Struct (default public)
struct Point {
    int x;
    int y;
};

int main() {

    // 🔹 Object creation
    Dog d1("Buddy", 3);
    Cat c1("Whiskers");

    // 🔹 Polymorphism
    Animal* a1 = &d1;
    Animal* a2 = &c1;

    a1->speak(); // Dog version
    a2->speak(); // Cat version

    // 🔹 Const function
    d1.info();

    // 🔹 Friend function
    showDog(d1);

    // 🔹 Static member
    std::cout << "Dog count: " << Dog::dogCount << "\n";

    // 🔹 Abstract class usage
    Rectangle r(4.0, 5.0);
    std::cout << "Area: " << r.area() << "\n";

    // 🔹 Struct usage
    Point p{10, 20};
    std::cout << "Point: (" << p.x << ", " << p.y << ")\n";

    return 0;
}
