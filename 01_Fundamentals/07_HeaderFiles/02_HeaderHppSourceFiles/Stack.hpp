#pragma once

#include <vector>
#include <stdexcept>
#include <string>

// ============================================================
// Stack.hpp — Template class header (C++ only)
// ============================================================
// USE .hpp WHEN:
//   - Template classes or functions (definition MUST be in header)
//   - C++ only features (classes, templates, std library)
//   - Header-only library — no separate .cpp needed
//   - Signals to readers: "this is C++ only, not C"
// ============================================================

// Templates MUST have their full definition in the header —
// the compiler needs the implementation to generate code for each type T

template<typename T>
class Stack
{
public:
    // ---- Construction ----
    Stack() = default;

    explicit Stack(std::size_t reserve)
    {
        m_data.reserve(reserve);
    }

    // ---- Core operations ----
    void push(const T& value)
    {
        m_data.push_back(value);
    }

    void push(T&& value)                          // move version
    {
        m_data.push_back(std::move(value));
    }

    template<typename... Args>
    void emplace(Args&&... args)                  // construct in place
    {
        m_data.emplace_back(std::forward<Args>(args)...);
    }

    void pop()
    {
        if (empty())
            throw std::underflow_error("Stack::pop — stack is empty");
        m_data.pop_back();
    }

    // ---- Access ----
    T& top()
    {
        if (empty())
            throw std::underflow_error("Stack::top — stack is empty");
        return m_data.back();
    }

    const T& top() const
    {
        if (empty())
            throw std::underflow_error("Stack::top — stack is empty");
        return m_data.back();
    }

    // ---- State ----
    bool        empty() const { return m_data.empty();  }
    std::size_t size()  const { return m_data.size();   }
    void        clear()       { m_data.clear();          }

    // ---- Utility ----
    std::string toString() const
    {
        std::string result = "Stack[";
        for (std::size_t i = 0; i < m_data.size(); ++i) {
            if (i > 0) result += ", ";
            result += std::to_string(m_data[i]);
        }
        return result + "] top=" + (empty() ? "empty" : std::to_string(m_data.back()));
    }

private:
    std::vector<T> m_data;
};

// ============================================================
// WHY the full definition is here in .hpp (not in a .cpp):
//
// Templates are blueprints — the compiler generates actual code
// only when it sees the concrete type (e.g. Stack<int>).
// If the definition were in Stack.cpp, the compiler could not
// see it when compiling main.cpp → linker error:
//   "undefined reference to Stack<int>::push"
//
// Rule: template class/function definitions ALWAYS in header
// ============================================================
