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

## 💡 Access Levels

```
private:   Only this class
protected: This class + derived classes
public:    Anywhere
```

## Build & run

```bash
cmake -S . -B build
cmake --build build
./build/23_AccessSpecifiers
```
