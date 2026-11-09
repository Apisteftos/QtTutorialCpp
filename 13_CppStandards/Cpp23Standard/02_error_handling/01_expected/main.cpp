/*
 * std::expected<T, E> (C++23, <expected>)
 *
 * A return type that holds either a value (T) or an error (E). It makes error
 * handling explicit in the function signature, without exceptions.
 *
 * Compile: g++ -std=c++23 -Wall -Wextra main.cpp -o example
 */

#include <charconv>
#include <expected>
#include <print>
#include <string>
#include <string_view>
#include <system_error>

enum class ParseError { Empty, NotANumber, OutOfRange };

std::string_view toString(ParseError error) {
    switch (error) {
    case ParseError::Empty:
        return "empty input";
    case ParseError::NotANumber:
        return "not a number";
    case ParseError::OutOfRange:
        return "out of range";
    }
    return "unknown";
}

std::expected<int, ParseError> parseNumber(std::string_view text) {
    if (text.empty()) {
        return std::unexpected(ParseError::Empty);
    }
    int value = 0;
    auto [end, ec] = std::from_chars(text.data(), text.data() + text.size(), value);
    if (ec == std::errc::result_out_of_range) {
        return std::unexpected(ParseError::OutOfRange);
    }
    if (ec != std::errc{} || end != text.data() + text.size()) {
        return std::unexpected(ParseError::NotANumber);
    }
    return value;
}

std::expected<int, ParseError> checkPort(int value) {
    if (value < 1 || value > 65535) {
        return std::unexpected(ParseError::OutOfRange);
    }
    return value;
}

int main() {
    // === EXAMPLE 1: Value or error ===
    std::println("=== Example 1: Value or error ===");
    for (std::string_view text : {"8080", "", "12ab", "99999999999"}) {
        auto result = parseNumber(text);
        if (result) {
            std::println("'{}' -> value {}", text, *result);
        } else {
            std::println("'{}' -> error: {}", text, toString(result.error()));
        }
    }
    std::println("value_or: {}", parseNumber("oops").value_or(-1));

    // === EXAMPLE 2: Chaining with and_then / transform / or_else ===
    std::println("");
    std::println("=== Example 2: Chaining ===");
    for (std::string_view text : {"443", "70000", "http"}) {
        auto port = parseNumber(text)
                        .and_then(checkPort)                        // runs only on success, may fail itself
                        .transform([](int p) { return p + 1; })     // changes the value, keeps the type of error
                        .or_else([](ParseError e) -> std::expected<int, ParseError> {
                            // runs only on error; here: turn "out of range" into a default
                            if (e == ParseError::OutOfRange) {
                                return 8080;
                            }
                            return std::unexpected(e);
                        });
        if (port) {
            std::println("'{}' -> next port {}", text, *port);
        } else {
            std::println("'{}' -> error: {}", text, toString(port.error()));
        }
    }

    // === EXAMPLE 3: std::unexpect and bad_expected_access ===
    std::println("");
    std::println("=== Example 3: unexpect and bad_expected_access ===");
    std::expected<int, std::string> failed(std::unexpect, "disk not ready");  // build the error in place
    try {
        std::println("{}", failed.value());  // value() on an error throws
    } catch (const std::bad_expected_access<std::string>& ex) {
        std::println("caught bad_expected_access: {}", ex.error());
    }
    return 0;
}
