#include <string>
#include <vector>
#include <memory>
#include <print>

// ============================================================
// REALIZATION — is-a relationship (implements interface)
// ============================================================
// A class implements an abstract interface — it "realizes"
// the contract defined by the interface. The class promises
// to provide all the behaviours the interface declares.
//
// Key signal: pure virtual base class (= 0),
//             concrete class overrides ALL pure virtuals.
// ============================================================


// ============================================================
// Example 1 — Printable interface
// ============================================================
class Printable {
public:
    virtual void print()    const = 0;   // pure virtual — must implement
    virtual std::string toText() const = 0;
    virtual ~Printable() = default;
};

class Document : public Printable {
private:
    std::string m_title;
    std::string m_content;

public:
    Document(const std::string& title, const std::string& content)
        : m_title(title), m_content(content) {}

    void print() const override {
        std::println("  [Document] '{}': {}", m_title, m_content);
    }

    std::string toText() const override {
        return "Document: " + m_title;
    }
};

class Image : public Printable {
private:
    std::string m_filename;
    int         m_width, m_height;

public:
    Image(const std::string& file, int w, int h)
        : m_filename(file), m_width(w), m_height(h) {}

    void print() const override {
        std::println("  [Image] '{}' ({}x{})", m_filename, m_width, m_height);
    }

    std::string toText() const override {
        return "Image: " + m_filename;
    }
};

class Spreadsheet : public Printable {
private:
    std::string m_name;
    int         m_rows, m_cols;

public:
    Spreadsheet(const std::string& name, int rows, int cols)
        : m_name(name), m_rows(rows), m_cols(cols) {}

    void print() const override {
        std::println("  [Spreadsheet] '{}' ({}x{})", m_name, m_rows, m_cols);
    }

    std::string toText() const override {
        return "Spreadsheet: " + m_name;
    }
};


// ============================================================
// Example 2 — Serializable interface
// ============================================================
class Serializable {
public:
    virtual std::string serialize()   const = 0;
    virtual void        deserialize(const std::string& data) = 0;
    virtual ~Serializable() = default;
};

class UserProfile : public Serializable {
private:
    std::string m_name;
    int         m_age;
    std::string m_email;

public:
    UserProfile(const std::string& name, int age, const std::string& email)
        : m_name(name), m_age(age), m_email(email) {}

    std::string serialize() const override {
        return m_name + ";" + std::to_string(m_age) + ";" + m_email;
    }

    void deserialize(const std::string& data) override {
        // simplified — real code would parse properly
        std::println("  [UserProfile] deserializing: {}", data);
        m_name = "Loaded";
    }

    void print() const {
        std::println("  UserProfile: {} (age {}) <{}>", m_name, m_age, m_email);
    }
};

class AppSettings : public Serializable {
private:
    std::string m_theme;
    std::string m_language;
    bool        m_notifications;

public:
    AppSettings(const std::string& theme, const std::string& lang, bool notifications)
        : m_theme(theme), m_language(lang), m_notifications(notifications) {}

    std::string serialize() const override {
        return m_theme + ";" + m_language + ";" + (m_notifications ? "1" : "0");
    }

    void deserialize(const std::string& data) override {
        std::println("  [AppSettings] deserializing: {}", data);
    }
};


// ============================================================
// Example 3 — Multiple interfaces on one class
// ============================================================
class Drawable {
public:
    virtual void draw()   const = 0;
    virtual void resize(int w, int h) = 0;
    virtual ~Drawable() = default;
};

class Clickable {
public:
    virtual void onClick() = 0;
    virtual bool contains(int x, int y) const = 0;
    virtual ~Clickable() = default;
};

// Button realizes BOTH Drawable AND Clickable
class Button : public Drawable, public Clickable {
private:
    std::string m_label;
    int m_x, m_y, m_w, m_h;

public:
    Button(const std::string& label, int x, int y, int w, int h)
        : m_label(label), m_x(x), m_y(y), m_w(w), m_h(h) {}

    // Realizes Drawable
    void draw() const override {
        std::println("  [Button] '{}' at ({},{}) size {}x{}", m_label, m_x, m_y, m_w, m_h);
    }

    void resize(int w, int h) override {
        m_w = w; m_h = h;
        std::println("  [Button] '{}' resized to {}x{}", m_label, m_w, m_h);
    }

    // Realizes Clickable
    void onClick() override {
        std::println("  [Button] '{}' clicked!", m_label);
    }

    bool contains(int x, int y) const override {
        return x >= m_x && x <= m_x + m_w &&
               y >= m_y && y <= m_y + m_h;
    }
};


// ============================================================
// Example 4 — Compressor interface
// ============================================================
class Compressor {
public:
    virtual std::string getName()  const = 0;
    virtual int         getLevel() const = 0;
    virtual std::string compress(const std::string& data)   const = 0;
    virtual std::string decompress(const std::string& data) const = 0;
    virtual ~Compressor() = default;
};

class ZipCompressor : public Compressor {
public:
    std::string getName()  const override { return "ZIP"; }
    int         getLevel() const override { return 6;     }

    std::string compress(const std::string& data) const override {
        std::println("  [ZIP] compressing: {}", data);
        return "[ZIP compressed]";
    }

    std::string decompress(const std::string& data) const override {
        std::println("  [ZIP] decompressing: {}", data);
        return "[original data]";
    }
};

class GzipCompressor : public Compressor {
public:
    std::string getName()  const override { return "GZIP"; }
    int         getLevel() const override { return 9;      }

    std::string compress(const std::string& data) const override {
        std::println("  [GZIP] compressing: {}", data);
        return "[GZIP compressed]";
    }

    std::string decompress(const std::string& data) const override {
        std::println("  [GZIP] decompressing: {}", data);
        return "[original data]";
    }
};

class ZstdCompressor : public Compressor {
public:
    std::string getName()  const override { return "ZSTD"; }
    int         getLevel() const override { return 3;      }

    std::string compress(const std::string& data) const override {
        std::println("  [ZSTD] compressing: {}", data);
        return "[ZSTD compressed]";
    }

    std::string decompress(const std::string& data) const override {
        std::println("  [ZSTD] decompressing: {}", data);
        return "[original data]";
    }
};

// Client code works through the interface — compressor is interchangeable
void processData(const Compressor& compressor, const std::string& data) {
    std::println("  Using compressor: {} (level {})", compressor.getName(), compressor.getLevel());
    auto packed   = compressor.compress(data);
    auto unpacked = compressor.decompress(packed);
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::println("=====================================================");
    std::println("Example 1: Printable interface");
    std::println("=====================================================");

    {
        std::vector<std::unique_ptr<Printable>> items;
        items.push_back(std::make_unique<Document>("C++23 Guide", "Modern C++ techniques"));
        items.push_back(std::make_unique<Image>("diagram.png", 1920, 1080));
        items.push_back(std::make_unique<Spreadsheet>("Results.xlsx", 100, 10));

        std::println("Printing all Printable objects:");
        for (const auto& item : items)
            item->print();

        std::println("");

        std::println("Converting to text:");
        for (const auto& item : items)
            std::println("  {}", item->toText());
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Serializable interface");
    std::println("=====================================================");

    {
        UserProfile profile("Kostas", 41, "kostas@example.com");
        AppSettings settings("dark", "en", true);

        profile.print();

        std::string serialized = profile.serialize();
        std::println("  Serialized: {}", serialized);

        std::string settingsStr = settings.serialize();
        std::println("  Settings:   {}", settingsStr);

        // Use through interface
        std::vector<Serializable*> objects = {&profile, &settings};
        std::println("");
        std::println("Serializing all objects via interface:");
        for (auto* obj : objects)
            std::println("  {}", obj->serialize());
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Button realizes Drawable AND Clickable");
    std::println("=====================================================");

    {
        Button btn("Connect", 100, 200, 120, 40);

        // Use through Drawable interface
        Drawable& drawable = btn;
        drawable.draw();
        drawable.resize(150, 40);

        // Use through Clickable interface
        Clickable& clickable = btn;
        std::println("  Contains (110,210): {}", clickable.contains(110, 210));
        std::println("  Contains (50, 50):  {}", clickable.contains(50, 50));
        clickable.onClick();
    }


    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Compressor interface — ZIP, GZIP, ZSTD");
    std::println("=====================================================");

    {
        ZipCompressor  zip;
        GzipCompressor gzip;
        ZstdCompressor zstd;

        std::println("Processing with ZIP:");
        processData(zip, "Hello world, this is a test");
        std::println("");

        std::println("Processing with GZIP:");
        processData(gzip, "Hello world, this is a test");
        std::println("");

        std::println("Processing with ZSTD:");
        processData(zstd, "Hello world, this is a test");
    }

    return 0;
}
