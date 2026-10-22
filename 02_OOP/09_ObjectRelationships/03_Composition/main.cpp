#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <print>

// ============================================================
// COMPOSITION — has-a relationship (strong ownership)
// ============================================================
// The whole OWNS the parts. Parts are created by the whole
// and CANNOT exist without it. When the whole is destroyed,
// all parts are destroyed with it.
//
// Key signal: parts stored as MEMBER OBJECTS (by value)
//             or unique_ptr — created inside the whole.
// ============================================================


// ============================================================
// Example 1 — Car has Engine, Gearbox, Wheels (classic)
// ============================================================
class Engine {
private:
    int    m_horsepower;
    double m_displacement;

public:
    Engine(int hp, double disp)
        : m_horsepower(hp), m_displacement(disp)
    {
        std::println("    [Engine {}hp created]", m_horsepower);
    }

    ~Engine() {
        std::println("    [Engine {}hp destroyed]", m_horsepower);
    }

    void start() const {
        std::println("    Engine {}hp started ({}L)", m_horsepower, m_displacement);
    }

    int getHp() const { return m_horsepower; }
};

class Gearbox {
private:
    int  m_gears;
    bool m_isAutomatic;

public:
    Gearbox(int gears, bool automatic)
        : m_gears(gears), m_isAutomatic(automatic)
    {
        std::println("    [Gearbox {}spd created]", m_gears);
    }

    ~Gearbox() {
        std::println("    [Gearbox destroyed]");
    }

    void shiftTo(int gear) const {
        std::println("    Gearbox shifted to gear {}", gear);
    }
};

class Car {
private:
    std::string m_model;
    Engine      m_engine;     // composition — owned member object
    Gearbox     m_gearbox;    // composition — owned member object

public:
    Car(const std::string& model, int hp, double disp, int gears, bool automatic)
        : m_model(model)
        , m_engine(hp, disp)       // engine created WITH the car
        , m_gearbox(gears, automatic)
    {
        std::println("  [Car '{}' created]", m_model);
    }

    ~Car() {
        std::println("  [Car '{}' destroyed — engine and gearbox destroyed too]", m_model);
        // Engine and Gearbox destructors called automatically
    }

    void drive() const {
        std::println("  Driving {}:", m_model);
        m_engine.start();
        m_gearbox.shiftTo(1);
        m_gearbox.shiftTo(2);
    }

    const Engine& getEngine() const { return m_engine; }
};


// ============================================================
// Example 2 — Human Body has Heart, Brain, Lungs
// ============================================================
class Heart {
    int m_bpm;
public:
    explicit Heart(int bpm) : m_bpm(bpm) {
        std::println("    [Heart created, {}bpm]", m_bpm);
    }
    ~Heart() { std::println("    [Heart destroyed]"); }

    void beat() const { std::println("    Heart beating at {}bpm", m_bpm); }
};

class Brain {
    std::string m_dominantSide;
public:
    explicit Brain(const std::string& side) : m_dominantSide(side) {
        std::println("    [Brain created, {}-dominant]", m_dominantSide);
    }
    ~Brain() { std::println("    [Brain destroyed]"); }

    void think() const { std::println("    Brain ({}) is processing", m_dominantSide); }
};

class Human {
private:
    std::string m_name;
    Heart       m_heart;    // composition — owned, same lifetime
    Brain       m_brain;    // composition — owned, same lifetime

public:
    Human(const std::string& name, int bpm, const std::string& side)
        : m_name(name), m_heart(bpm), m_brain(side)
    {
        std::println("  [Human '{}' created]", m_name);
    }

    ~Human() {
        std::println("  [Human '{}' destroyed — heart and brain destroyed too]", m_name);
    }

    void live() const {
        std::println("  {} is alive:", m_name);
        m_heart.beat();
        m_brain.think();
    }
};


// ============================================================
// Example 3 — House has Rooms (unique_ptr composition)
// ============================================================
class Room {
private:
    std::string m_name;
    double      m_areaSqm;

public:
    Room(const std::string& name, double area)
        : m_name(name), m_areaSqm(area)
    {
        std::println("    [Room '{}' ({} m²) created]", m_name, m_areaSqm);
    }

    ~Room() {
        std::println("    [Room '{}' destroyed]", m_name);
    }

    std::string getName()  const { return m_name;     }
    double      getArea()  const { return m_areaSqm;  }

    void describe() const {
        std::println("    Room: {} ({} m²)", m_name, m_areaSqm);
    }
};

class House {
private:
    std::string                           m_address;
    std::vector<std::unique_ptr<Room>>    m_rooms;   // composition via unique_ptr

public:
    explicit House(const std::string& address) : m_address(address) {
        std::println("  [House at '{}' created]", m_address);
    }

    ~House() {
        std::println("  [House at '{}' destroyed — all rooms destroyed]", m_address);
        // unique_ptrs automatically destroy all rooms
    }

    void addRoom(const std::string& name, double area) {
        m_rooms.push_back(std::make_unique<Room>(name, area));
    }

    void describe() const {
        std::println("  House at '{}' ({} rooms):", m_address, m_rooms.size());
        for (const auto& r : m_rooms) r->describe();
    }

    double totalArea() const {
        double total = 0;
        for (const auto& r : m_rooms) total += r->getArea();
        return total;
    }
};


// ============================================================
// Example 4 — WebServer has ServerConfig, Cache
// Config and Cache have no meaning outside the server
// ============================================================
class ServerConfig {
    std::string m_host;
    uint16_t    m_port;
    bool        m_encrypted;

public:
    ServerConfig(const std::string& host, uint16_t port, bool enc)
        : m_host(host), m_port(port), m_encrypted(enc)
    {
        std::println("    [ServerConfig created: {}:{}]", m_host, m_port);
    }

    ~ServerConfig() { std::println("    [ServerConfig destroyed]"); }

    std::string toString() const {
        return m_host + ":" + std::to_string(m_port) +
               (m_encrypted ? " (TLS)" : "");
    }
};

class Cache {
    std::string m_name;
    int         m_sizeMb;

public:
    Cache(const std::string& name, int sizeMb)
        : m_name(name), m_sizeMb(sizeMb)
    {
        std::println("    [Cache {} {}MB created]", m_name, m_sizeMb);
    }

    ~Cache() { std::println("    [Cache {} destroyed]", m_name); }

    void warmUp() const {
        std::println("    {} warming up with {}MB", m_name, m_sizeMb);
    }
};

class WebServer {
private:
    std::string  m_serverId;
    ServerConfig m_config;    // composition — config owned by server
    Cache        m_cache;     // composition — cache owned by server

public:
    WebServer(const std::string& id,
              const std::string& host, uint16_t port, bool enc,
              const std::string& cacheName, int cacheSizeMb)
        : m_serverId(id)
        , m_config(host, port, enc)
        , m_cache(cacheName, cacheSizeMb)
    {
        std::println("  [WebServer '{}' created]", m_serverId);
    }

    ~WebServer() {
        std::println("  [WebServer '{}' destroyed — config and cache destroyed]", m_serverId);
    }

    void start() const {
        std::println("  WebServer '{}' listening on {}", m_serverId, m_config.toString());
        m_cache.warmUp();
    }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    std::println("=====================================================");
    std::println("Example 1: Car — Engine, Gearbox");
    std::println("=====================================================");

    {
        std::println("Creating car:");
        Car bmw("BMW 320d", 190, 2.0, 8, true);
        bmw.drive();
        std::println("");
        std::println("Leaving scope — car destroyed:");
    }
    std::println("Car and all its parts are gone.");
    std::println("");


    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Human — Heart, Brain");
    std::println("=====================================================");

    {
        std::println("Creating human:");
        Human kostas("Kostas", 72, "left");
        kostas.live();
        std::println("");
        std::println("Leaving scope:");
    }
    std::println("Human and all organs are gone.");
    std::println("");


    std::println("");
    std::println("=====================================================");
    std::println("Example 3: House — Rooms (unique_ptr)");
    std::println("=====================================================");

    {
        std::println("Building house:");
        House house("Musterstraße 1, Hannover");
        house.addRoom("Living Room",  35.0);
        house.addRoom("Kitchen",      18.0);
        house.addRoom("Bedroom",      20.0);
        house.addRoom("Bathroom",      8.0);

        house.describe();
        std::println("  Total area: {} m²", house.totalArea());
        std::println("");
        std::println("Demolishing house:");
    }
    std::println("House and all rooms are gone.");
    std::println("");


    std::println("");
    std::println("=====================================================");
    std::println("Example 4: WebServer — ServerConfig, Cache");
    std::println("=====================================================");

    {
        std::println("Creating web server:");
        WebServer server("Web-01",
                         "localhost", 8080, true,
                         "PageCache", 256);
        server.start();
        std::println("");
        std::println("Server going out of scope:");
    }
    std::println("Server and all components are gone.");
    std::println("");

    return 0;
}
