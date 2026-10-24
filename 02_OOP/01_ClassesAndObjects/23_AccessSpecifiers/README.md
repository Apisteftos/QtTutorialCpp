# 23_AccessSpecifiers

Part of `02_OOP / 01_ClassesAndObjects`.

## 🔒 Private

### Definition
**Only accessible inside the defining class.** Not visible to outside code,
not even to derived classes.

```cpp
private:
    int privateData;

    void privateMethod() {
        cout << "   Private method called\n";
    }
```

## 🛡️ Protected

### Definition
**Accessible in this class and derived classes** — for inheritance.

```cpp
protected:
    int protectedData;

    void protectedMethod() {
        cout << "   Protected method called\n";
    }
```

## 🌐 Public

### Definition
**Accessible from anywhere** the object is visible.

```cpp
public:
    int publicData;

    void publicMethod() {
        cout << "   Public method called\n";
        cout << "   Accessing private: " << privateData << "\n";
        cout << "   Accessing protected: " << protectedData << "\n";
        cout << "   Accessing public: " << publicData << "\n";

        privateMethod();
        protectedMethod();
    }
```

Inside the class itself, all three are fair game — `publicMethod()` freely
reads `privateData`, `protectedData`, and `publicData`, and calls both
`privateMethod()` and `protectedMethod()`.

## From outside the class

```cpp
AccessSpecifierDemo obj;

✅ Can access public:
   obj.publicData = 100;
   obj.publicMethod();

❌ Cannot access private:
   // obj.privateData = 100;  // Error!
   // obj.privateMethod();     // Error!

❌ Cannot access protected (from outside):
   // obj.protectedData = 100; // Error!
   // obj.protectedMethod();    // Error!
```

## From a derived class

`main.cpp` now shows the "derived classes" part of the table too (the old demo never created a derived class):

```cpp
class DerivedDemo : public AccessSpecifierDemo {
public:
    void showInherited() {
        protectedData;        // ✅ OK
        protectedMethod();    // ✅ OK
        // privateData;       // ❌ Error: private in the base class
        // privateMethod();   // ❌ Error: private in the base class
    }
};
```

✅ tested (GCC 13): the four commented-out lines from "From outside the class" fail with `'int AccessSpecifierDemo::privateData' is private within this context` (and the same for the other three, with "protected"), and the two derived-class errors above fail the same way.

## 💡 Access Levels

```
private:   Only this class
protected: This class + derived classes
public:    Anywhere
```

## 🧾 Notes moved from main.cpp

- The "💡 Access Levels" lines that `main()` printed are exactly the block above, so they were removed from the program.
- **Access is per class, not per object** ✅ tested: a member function may read the private members of *another* object of the same class (`bool same(const A& o) const { return priv == o.priv; }`).
- **Protected is checked on the object type** ✅ tested: inside `DerivedDemo`, `protectedData` of `*this` is fine, but `other.protectedData` through a plain `AccessSpecifierDemo&` is an error (`is protected within this context`).
- **Default access** ✅ tested: members of a `class` are private by default, members of a `struct` are public by default (`c.y` on a class fails, `s.x` on a struct works).
- **Code changes in main.cpp:** `cout` replaced by `std::println`; `using namespace std;` removed (nothing else needed it); `DerivedDemo` added.
- The CMake commands under "Build & run" refer to a `CMakeLists.txt` that was not attached, so I could not run them; I compiled `main.cpp` directly with `-Wall -Wextra` and the sanitizers (clean).

## Build & run

```bash
cmake -S . -B build
cmake --build build
./build/23_AccessSpecifiers
```
