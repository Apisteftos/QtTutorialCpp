#include <iostream>
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

class SessionData : public Serializable {
private:
    std::string m_sessionId;
    std::string m_groupId;
    bool        m_active;

public:
    SessionData(const std::string& sid, const std::string& gid, bool active)
        : m_sessionId(sid), m_groupId(gid), m_active(active) {}

    std::string serialize() const override {
        return m_sessionId + ";" + m_groupId + ";" + (m_active ? "1" : "0");
    }

    void deserialize(const std::string& data) override {
        std::println("  [SessionData] deserializing: {}", data);
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
// Example 4 — MCX context: McxCodec interface
// ============================================================
class McxCodec {
public:
    virtual std::string getName()         const = 0;
    virtual int         getBitrate()      const = 0;
    virtual std::string encode(const std::string& audio) const = 0;
    virtual std::string decode(const std::string& data)  const = 0;
    virtual ~McxCodec() = default;
};

class AmrNbCodec : public McxCodec {
public:
    std::string getName()    const override { return "AMR-NB"; }
    int         getBitrate() const override { return 12200;    }

    std::string encode(const std::string& audio) const override {
        std::println("  [AMR-NB] encoding: {}", audio);
        return "[AMR-NB encoded]";
    }

    std::string decode(const std::string& data) const override {
        std::println("  [AMR-NB] decoding: {}", data);
        return "[decoded audio]";
    }
};

class AmrWbCodec : public McxCodec {
public:
    std::string getName()    const override { return "AMR-WB"; }
    int         getBitrate() const override { return 23850;    }

    std::string encode(const std::string& audio) const override {
        std::println("  [AMR-WB] encoding: {}", audio);
        return "[AMR-WB encoded]";
    }

    std::string decode(const std::string& data) const override {
        std::println("  [AMR-WB] decoding: {}", data);
        return "[decoded audio]";
    }
};

class OpusCodec : public McxCodec {
public:
    std::string getName()    const override { return "OPUS";  }
    int         getBitrate() const override { return 32000;   }

    std::string encode(const std::string& audio) const override {
        std::println("  [OPUS] encoding: {}", audio);
        return "[OPUS encoded]";
    }

    std::string decode(const std::string& data) const override {
        std::println("  [OPUS] decoding: {}", data);
        return "[decoded audio]";
    }
};

// Client code works through the interface — codec is interchangeable
void processAudio(const McxCodec& codec, const std::string& audio) {
    std::println("  Using codec: {} ({}bps)", codec.getName(), codec.getBitrate());
    auto encoded = codec.encode(audio);
    auto decoded = codec.decode(encoded);
}


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Printable interface\n";
    std::cout << "=====================================================\n";

    {
        std::vector<std::unique_ptr<Printable>> items;
        items.push_back(std::make_unique<Document>("C++23 Guide", "Modern C++ techniques"));
        items.push_back(std::make_unique<Image>("diagram.png", 1920, 1080));
        items.push_back(std::make_unique<Spreadsheet>("Results.xlsx", 100, 10));

        std::println("Printing all Printable objects:");
        for (const auto& item : items)
            item->print();

        std::println("\nConverting to text:");
        for (const auto& item : items)
            std::println("  {}", item->toText());

        std::cout << "\nAll three classes realize the same Printable interface.\n";
        std::cout << "Client works through interface — doesn't know concrete type.\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Serializable interface\n";
    std::cout << "=====================================================\n";

    {
        UserProfile profile("Kostas", 41, "kostas@example.com");
        SessionData session("SES-001", "GRP-ALPHA", true);

        profile.print();

        std::string serialized = profile.serialize();
        std::println("  Serialized: {}", serialized);

        std::string sessionStr = session.serialize();
        std::println("  Session:    {}", sessionStr);

        // Use through interface
        std::vector<Serializable*> objects = {&profile, &session};
        std::println("\nSerializing all objects via interface:");
        for (auto* obj : objects)
            std::println("  {}", obj->serialize());
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Button realizes Drawable AND Clickable\n";
    std::cout << "=====================================================\n";

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

        std::cout << "\nSame Button object viewed through two different interfaces.\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: McxCodec interface — AMR-NB, AMR-WB, OPUS\n";
    std::cout << "=====================================================\n";

    {
        AmrNbCodec amrNb;
        AmrWbCodec amrWb;
        OpusCodec  opus;

        std::println("Processing with AMR-NB:");
        processAudio(amrNb, "Hello ALPHA, this is BRAVO");
        std::cout << '\n';

        std::println("Processing with AMR-WB:");
        processAudio(amrWb, "Hello ALPHA, this is BRAVO");
        std::cout << '\n';

        std::println("Processing with OPUS:");
        processAudio(opus,  "Hello ALPHA, this is BRAVO");

        std::cout << "\nAll three codecs realize the same McxCodec interface.\n";
        std::cout << "processAudio() works with any codec — no changes needed.\n";
    }


    std::cout << "\n=====================================================\n";
    std::cout << "Summary: Realization\n";
    std::cout << "=====================================================\n";
    std::cout << "  Relationship:  is-a (implements contract)\n";
    std::cout << "  Base class:    pure abstract — no data, all = 0\n";
    std::cout << "  Derived class: must implement ALL pure virtuals\n";
    std::cout << "  Benefit:       interchangeable implementations\n";
    std::cout << "  Multiple:      one class can realize many interfaces\n";
    std::cout << "  UML:           A <— — — — —B (dashed, hollow triangle)\n";
    std::cout << "\n  Realization vs Inheritance:\n";
    std::cout << "    Realization  — implements pure interface (no data, no impl)\n";
    std::cout << "    Inheritance  — extends concrete class (inherits data + impl)\n";

    return 0;
}
