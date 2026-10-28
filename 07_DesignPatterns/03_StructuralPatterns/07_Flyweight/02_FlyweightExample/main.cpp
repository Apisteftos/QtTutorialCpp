// flyweight_example.cpp
// Flyweight Pattern Example - Text Editor with Shared Character Formatting

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <print>

// ===== FLYWEIGHT CLASSES =====

// CharacterStyle - Intrinsic state (shared among many characters)
class CharacterStyle {
private:
    std::string fontFamily;
    int fontSize;
    std::string color;
    bool isBold;
    bool isItalic;

public:
    CharacterStyle(const std::string& font, int size, const std::string& col,
                   bool bold, bool italic)
        : fontFamily(font), fontSize(size), color(col),
        isBold(bold), isItalic(italic) {
        std::println("Creating new style: {}", getDescription());
    }

    void render(char symbol, int x, int y) const {
        std::println("Rendering '{}' at position ({}, {}) with style: {}", symbol, x, y, getDescription());
    }

    std::string getDescription() const {
        return fontFamily + "-" + std::to_string(fontSize) + "-" + color +
               (isBold ? "-Bold" : "") + (isItalic ? "-Italic" : "");
    }

    // Key for factory map
    std::string getKey() const {
        return getDescription();
    }
};

// ===== FLYWEIGHT FACTORY =====

class StyleFactory {
private:
    std::unordered_map<std::string, std::shared_ptr<CharacterStyle>> styles;

public:
    std::shared_ptr<CharacterStyle> getStyle(const std::string& font,
                                             int size,
                                             const std::string& color,
                                             bool bold = false,
                                             bool italic = false) {
        // Create temporary object to get the key
        CharacterStyle temp(font, size, color, bold, italic);
        std::string key = temp.getKey();

        // Check if style already exists
        auto it = styles.find(key);
        if (it != styles.end()) {
            std::println("  -> Reusing existing style: {}", key);
            return it->second;
        }

        // Create new style if it doesn't exist
        std::println("  -> Creating new style: {}", key);
        auto newStyle = std::make_shared<CharacterStyle>(font, size, color, bold, italic);
        styles[key] = newStyle;
        return newStyle;
    }

    size_t getStyleCount() const {
        return styles.size();
    }

    void printStatistics() const {
        std::println("");
        std::println("=== Style Factory Statistics ===");
        std::println("Total unique styles created: {}", styles.size());
        std::println("Styles in memory:");
        for (const auto& pair : styles) {
            std::println("  - {}", pair.first);
        }
    }
};

// ===== CHARACTER CLASS (Uses Flyweight) =====

class Character {
private:
    char symbol;                              // Could be part of flyweight
    int x, y;                                  // Extrinsic state (position)
    std::shared_ptr<CharacterStyle> style;    // Shared flyweight

public:
    Character(char sym, int xPos, int yPos, std::shared_ptr<CharacterStyle> characterStyle)
        : symbol(sym), x(xPos), y(yPos), style(characterStyle) {}

    void render() const {
        style->render(symbol, x, y);
    }

    void move(int newX, int newY) {
        x = newX;
        y = newY;
    }
};

// ===== DOCUMENT CLASS =====

class Document {
private:
    std::vector<Character> characters;
    StyleFactory styleFactory;

public:
    void addCharacter(char symbol, int x, int y,
                      const std::string& font, int fontSize,
                      const std::string& color, bool bold = false, bool italic = false) {
        auto style = styleFactory.getStyle(font, fontSize, color, bold, italic);
        characters.emplace_back(symbol, x, y, style);
    }

    void render() const {
        std::println("");
        std::println("=== Rendering Document ===");
        for (const auto& ch : characters) {
            ch.render();
        }
    }

    void printMemoryStats() const {
        std::println("");
        std::println("=== Memory Statistics ===");
        std::println("Total characters in document: {}", characters.size());
        std::println("Unique styles used: {}", styleFactory.getStyleCount());

        // Calculate memory savings
        size_t memoryPerStyleWithoutFlyweight = sizeof(CharacterStyle);
        size_t totalMemoryWithoutFlyweight = characters.size() * memoryPerStyleWithoutFlyweight;
        size_t totalMemoryWithFlyweight = styleFactory.getStyleCount() * memoryPerStyleWithoutFlyweight
                                          + characters.size() * sizeof(Character);

        std::println("Memory without Flyweight: ~{} bytes", totalMemoryWithoutFlyweight);
        std::println("Memory with Flyweight: ~{} bytes", totalMemoryWithFlyweight);
        std::println("Memory saved: ~{} bytes ({}%)",
            (totalMemoryWithoutFlyweight - totalMemoryWithFlyweight), ((totalMemoryWithoutFlyweight - totalMemoryWithFlyweight) * 100 / totalMemoryWithoutFlyweight));

        styleFactory.printStatistics();
    }
};

// ===== MAIN DEMO =====

int main() {
    std::println("=== Flyweight Pattern Demo - Text Editor ===");
    std::println("Creating a document with formatted text...");
    std::println("");

    Document doc;

    // Add title - all characters share the same style
    std::println("Adding title 'HELLO':");
    std::string title = "HELLO";
    int x = 10;
    for (char c : title) {
        doc.addCharacter(c, x, 10, "Arial", 24, "Blue", true, false);
        x += 15;
    }

    std::println("");
    std::println("Adding subtitle 'World':");
    std::string subtitle = "World";
    x = 10;
    for (char c : subtitle) {
        doc.addCharacter(c, x, 40, "Arial", 18, "Green", false, true);
        x += 12;
    }

    std::println("");
    std::println("Adding body text 'Hello World Example':");
    std::string body = "Hello World Example";
    x = 10;
    int y = 70;
    for (size_t i = 0; i < body.length(); ++i) {
        if (body[i] == ' ') {
            x += 8;
            continue;
        }

        // First word in regular style
        if (i < 5) {
            doc.addCharacter(body[i], x, y, "Times", 12, "Black", false, false);
        }
        // Second word in bold
        else if (i >= 6 && i < 11) {
            doc.addCharacter(body[i], x, y, "Times", 12, "Black", true, false);
        }
        // Third word in italic
        else {
            doc.addCharacter(body[i], x, y, "Times", 12, "Black", false, true);
        }
        x += 10;
    }

    // Render the document
    doc.render();

    // Show memory statistics
    doc.printMemoryStats();

    return 0;
}
