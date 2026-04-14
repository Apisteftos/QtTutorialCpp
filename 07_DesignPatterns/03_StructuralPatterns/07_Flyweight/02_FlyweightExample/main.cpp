// flyweight_example.cpp
// Flyweight Pattern Example - Text Editor with Shared Character Formatting

#include <iostream>
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

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
        std::cout << "Creating new style: " << getDescription() << std::endl;
    }

    void render(char symbol, int x, int y) const {
        std::cout << "Rendering '" << symbol << "' at position (" << x << ", " << y << ") "
                  << "with style: " << getDescription() << std::endl;
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
            std::cout << "  -> Reusing existing style: " << key << std::endl;
            return it->second;
        }

        // Create new style if it doesn't exist
        std::cout << "  -> Creating new style: " << key << std::endl;
        auto newStyle = std::make_shared<CharacterStyle>(font, size, color, bold, italic);
        styles[key] = newStyle;
        return newStyle;
    }

    size_t getStyleCount() const {
        return styles.size();
    }

    void printStatistics() const {
        std::cout << "\n=== Style Factory Statistics ===" << std::endl;
        std::cout << "Total unique styles created: " << styles.size() << std::endl;
        std::cout << "Styles in memory:" << std::endl;
        for (const auto& pair : styles) {
            std::cout << "  - " << pair.first << std::endl;
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
        std::cout << "\n=== Rendering Document ===" << std::endl;
        for (const auto& ch : characters) {
            ch.render();
        }
    }

    void printMemoryStats() const {
        std::cout << "\n=== Memory Statistics ===" << std::endl;
        std::cout << "Total characters in document: " << characters.size() << std::endl;
        std::cout << "Unique styles used: " << styleFactory.getStyleCount() << std::endl;

        // Calculate memory savings
        size_t memoryPerStyleWithoutFlyweight = sizeof(CharacterStyle);
        size_t totalMemoryWithoutFlyweight = characters.size() * memoryPerStyleWithoutFlyweight;
        size_t totalMemoryWithFlyweight = styleFactory.getStyleCount() * memoryPerStyleWithoutFlyweight
                                          + characters.size() * sizeof(Character);

        std::cout << "Memory without Flyweight: ~" << totalMemoryWithoutFlyweight << " bytes" << std::endl;
        std::cout << "Memory with Flyweight: ~" << totalMemoryWithFlyweight << " bytes" << std::endl;
        std::cout << "Memory saved: ~" << (totalMemoryWithoutFlyweight - totalMemoryWithFlyweight)
                  << " bytes ("
                  << ((totalMemoryWithoutFlyweight - totalMemoryWithFlyweight) * 100 / totalMemoryWithoutFlyweight)
                  << "%)" << std::endl;

        styleFactory.printStatistics();
    }
};

// ===== MAIN DEMO =====

int main() {
    std::cout << "=== Flyweight Pattern Demo - Text Editor ===" << std::endl;
    std::cout << "Creating a document with formatted text...\n" << std::endl;

    Document doc;

    // Add title - all characters share the same style
    std::cout << "Adding title 'HELLO':" << std::endl;
    std::string title = "HELLO";
    int x = 10;
    for (char c : title) {
        doc.addCharacter(c, x, 10, "Arial", 24, "Blue", true, false);
        x += 15;
    }

    std::cout << "\nAdding subtitle 'World':" << std::endl;
    std::string subtitle = "World";
    x = 10;
    for (char c : subtitle) {
        doc.addCharacter(c, x, 40, "Arial", 18, "Green", false, true);
        x += 12;
    }

    std::cout << "\nAdding body text 'Hello World Example':" << std::endl;
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

    std::cout << "\n=== Key Observations ===" << std::endl;
    std::cout << "- Characters 'H', 'E', 'L', 'L', 'O' all share the same style object" << std::endl;
    std::cout << "- Each character only stores its unique position (x, y)" << std::endl;
    std::cout << "- Style objects are created once and reused" << std::endl;
    std::cout << "- Significant memory savings with many characters" << std::endl;

    return 0;
}
