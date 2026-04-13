# Default Parameters — C++

A function parameter can have a **default value** — the caller may omit it
and the default is used automatically.

---

## Example from this file

```cpp
void myFunction(std::string country = "Norway") {
    std::cout << country << '\n';
}

myFunction("Sweden");   // Sweden
myFunction("India");    // India
myFunction();           // Norway  ← default used
myFunction("USA");      // USA
```

---

## Rules

### 1. Defaults must be at the END of the parameter list

```cpp
void fn(int a, int b = 10, int c = 20);   // OK
void fn(int a = 5, int b, int c);         // ERROR — non-default after default
```

### 2. Default in declaration, NOT in definition (when declared separately)

```cpp
// CORRECT — default in declaration (header / top of file)
void myFunction(std::string country = "Norway");

// definition — no default here
void myFunction(std::string country) {
    std::cout << country << '\n';
}
```

The comment in the file explains this:
> *"If I declare twice the function it won't work with the default parameter.
> So then I have to declare my function at the header and not at the bottom"*

If the function is defined directly (no separate declaration), the default
goes in the definition itself — which is what the working version does.

---

## Multiple default parameters

```cpp
void connect(const std::string& host,
             int  port    = 5060,
             bool secure  = false) {
    // ...
}

connect("192.168.1.1");              // port=5060, secure=false
connect("192.168.1.1", 443);         // port=443,  secure=false
connect("192.168.1.1", 443, true);   // all specified
```

---

## When to use default parameters

✅ Optional configuration with a sensible default
✅ Backwards compatibility — add new param without breaking callers
✅ Reduce overloaded functions

❌ When the default value is unclear — use overloading instead
❌ Don't mix with function overloading on the same name — can cause ambiguity
