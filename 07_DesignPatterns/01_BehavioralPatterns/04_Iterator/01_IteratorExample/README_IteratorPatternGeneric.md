# Iterator Pattern (Generic) — C++

## Intent

Provide a way to **traverse elements of a collection** sequentially without
exposing the underlying data structure (vector, array, tree, etc.).

---

## Structure from this file

```
Iterator<T, U>        ← generic iterator (template)
  ├── First()         — reset to beginning
  ├── Next()          — advance
  ├── IsDone()        — check if finished
  └── Current()       — access current element

Container<T>          ← generic collection (template)
  ├── Add(element)
  └── CreateIterator() → Iterator<T, Container<T>>*
```

---

## Key code

```cpp
template<typename T, typename U>
class Iterator {
public:
    typedef typename std::vector<T>::iterator iter_type;

    Iterator(U* data) : m_p_data_(data) {
        m_it_ = m_p_data_->m_data_.begin();
    }

    void First()    { m_it_ = m_p_data_->m_data_.begin(); }
    void Next()     { m_it_++; }
    bool IsDone()   { return m_it_ == m_p_data_->m_data_.end(); }
    iter_type Current() { return m_it_; }

private:
    U* m_p_data_;
    iter_type m_it_;
};

template<class T>
class Container {
    friend class Iterator<T, Container>;
    std::vector<T> m_data_;
public:
    void Add(T a) { m_data_.push_back(a); }

    Iterator<T, Container>* CreateIterator() {
        return new Iterator<T, Container>(this);
    }
};
```

---

## Usage — int collection

```cpp
Container<int> cont;
for (int i = 0; i < 10; i++) cont.Add(i);

Iterator<int, Container<int>>* it = cont.CreateIterator();
for (it->First(); !it->IsDone(); it->Next())
    std::cout << *it->Current() << '\n';

delete it;
```

---

## Usage — custom class collection

```cpp
Container<Data> cont2;
cont2.Add(Data(100));
cont2.Add(Data(1000));
cont2.Add(Data(10000));

Iterator<Data, Container<Data>>* it2 = cont2.CreateIterator();
for (it2->First(); !it2->IsDone(); it2->Next())
    std::cout << it2->Current()->data() << '\n';

delete it2;
```

Same iterator interface — regardless of whether T is `int` or a custom class.

---

## Iterator interface summary

| Method | Description |
|--------|-------------|
| `First()` | Reset to first element |
| `Next()` | Advance to next element |
| `IsDone()` | True when past the end |
| `Current()` | Returns iterator to current element |

---

## C++ STL iterators (modern equivalent)

This file implements the classic GoF iterator manually.
In modern C++ you would use STL iterators directly:

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};

// Range-based for (preferred)
for (int x : v) std::cout << x;

// Explicit iterator
for (auto it = v.begin(); it != v.end(); ++it)
    std::cout << *it;
```

---

## When to use Iterator Pattern

✅ Traverse a collection without knowing its internal structure
✅ Provide uniform access to different container types
✅ Support multiple simultaneous traversals of the same collection
✅ Custom containers that need to work with range-based for

❌ For standard containers (vector, list, map) — use STL iterators
