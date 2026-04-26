#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <print>

#include <nlohmann/json.hpp>

// ============================================================
// JSON in C++23 — nlohmann/json
// ============================================================
// Install: sudo apt install nlohmann-json3-dev
//
// Topics covered:
//   1.  Create JSON object from scratch
//   2.  Parse JSON from string
//   3.  Access values — [], at(), value()
//   4.  Nested objects
//   5.  JSON arrays → std::vector
//   6.  Serialize to string / pretty print
//   7.  Read JSON from file
//   8.  Write JSON to file
//   9.  Error handling — malformed JSON
//  10.  Real use case — MCX session config
// ============================================================

using json = nlohmann::json;


int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Create JSON object from scratch\n";
    std::cout << "=====================================================\n";

    json config;
    config["app"]     = "MCXApp";
    config["version"] = "2.3.1";
    config["port"]    = 5060;
    config["debug"]   = true;
    config["pi"]      = 3.14159;

    std::println("app:     {}", config["app"].get<std::string>());
    std::println("version: {}", config["version"].get<std::string>());
    std::println("port:    {}", config["port"].get<int>());
    std::println("debug:   {}", config["debug"].get<bool>());
    std::println("pi:      {}", config["pi"].get<double>());

    // Check type
    std::println("\ntype checks:");
    std::println("  config[port] is_number: {}", config["port"].is_number());
    std::println("  config[app]  is_string: {}", config["app"].is_string());
    std::println("  config[debug] is_bool:  {}", config["debug"].is_boolean());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: Parse JSON from string\n";
    std::cout << "=====================================================\n";

    std::string jsonStr = R"({
        "name": "Kostas",
        "age": 41,
        "city": "Hannover",
        "active": true
    })";

    json person = json::parse(jsonStr);

    std::println("name:   {}", person["name"].get<std::string>());
    std::println("age:    {}", person["age"].get<int>());
    std::println("city:   {}", person["city"].get<std::string>());
    std::println("active: {}", person["active"].get<bool>());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Access values — [], at(), value()\n";
    std::cout << "=====================================================\n";

    json data = json::parse(R"({"host":"192.168.1.1","port":5060,"timeout":30})");

    // [] — no bounds check, creates key if missing
    std::println("host ([]): {}", data["host"].get<std::string>());

    // at() — throws std::out_of_range if key missing
    try {
        std::println("port (at): {}", data.at("port").get<int>());
        std::println("missing:   {}", data.at("nonexistent").get<int>());
    } catch (const json::out_of_range& e) {
        std::println("at() error: {}", e.what());
    }

    // value() — returns default if key missing
    std::println("timeout (value): {}", data.value("timeout", 0));
    std::println("retries (value): {}", data.value("retries", 3));  // default 3

    // contains() — check before access
    std::println("contains 'host':    {}", data.contains("host"));
    std::println("contains 'missing': {}", data.contains("missing"));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Nested objects\n";
    std::cout << "=====================================================\n";

    json session;
    session["id"]              = "SES-001";
    session["group"]["id"]     = "GRP-ALPHA";
    session["group"]["name"]   = "Alpha Team";
    session["group"]["active"] = true;
    session["server"]["host"]  = "192.168.1.100";
    session["server"]["port"]  = 5060;
    session["server"]["tls"]   = false;

    std::println("session id:       {}", session["id"].get<std::string>());
    std::println("group id:         {}", session["group"]["id"].get<std::string>());
    std::println("group name:       {}", session["group"]["name"].get<std::string>());
    std::println("server host:      {}", session["server"]["host"].get<std::string>());
    std::println("server port:      {}", session["server"]["port"].get<int>());
    std::println("server tls:       {}", session["server"]["tls"].get<bool>());

    // Iterate over nested object
    std::cout << "\nserver keys:\n";
    for (auto& [key, value] : session["server"].items())
        std::println("  {} = {}", key, value.dump());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: JSON arrays ↔ std::vector\n";
    std::cout << "=====================================================\n";

    // JSON array from initializer list
    json colors = {"red", "green", "blue"};
    std::println("colors size: {}", colors.size());
    for (const auto& c : colors)
        std::print("  {}", c.get<std::string>());
    std::cout << '\n';

    // Array of objects
    json users = json::array();
    users.push_back({{"name","Alice"},{"age",30}});
    users.push_back({{"name","Bob"},  {"age",25}});
    users.push_back({{"name","Carol"},{"age",35}});

    std::cout << "\nusers array:\n";
    for (const auto& u : users)
        std::println("  {} age {}", u["name"].get<std::string>(), u["age"].get<int>());

    // Convert JSON array ↔ std::vector
    json nums = {10, 20, 30, 40, 50};
    std::vector<int> vec = nums.get<std::vector<int>>();
    std::cout << "\nvector from JSON: ";
    for (int n : vec) std::print("{} ", n);
    std::cout << '\n';

    // vector → JSON
    std::vector<std::string> groups = {"GRP-ALPHA", "GRP-BETA", "GRP-GAMMA"};
    json groupJson = groups;
    std::println("JSON from vector: {}", groupJson.dump());


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Serialize — dump() and pretty print\n";
    std::cout << "=====================================================\n";

    json obj;
    obj["name"]    = "MCXApp";
    obj["port"]    = 5060;
    obj["groups"]  = {"ALPHA", "BETA"};
    obj["enabled"] = true;

    // Compact
    std::println("compact: {}", obj.dump());

    // Pretty print with indent
    std::println("\npretty (2 spaces):\n{}", obj.dump(2));

    // Pretty print with 4 spaces
    std::println("pretty (4 spaces):\n{}", obj.dump(4));


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Write JSON to file\n";
    std::cout << "=====================================================\n";

    json configFile;
    configFile["app"]             = "MCXApp";
    configFile["version"]         = "2.3.1";
    configFile["server"]["host"]  = "192.168.1.100";
    configFile["server"]["port"]  = 5060;
    configFile["server"]["tls"]   = true;
    configFile["groups"]          = {"GRP-ALPHA", "GRP-BETA"};
    configFile["debug"]           = false;

    {
        std::ofstream outFile("config.json");
        if (!outFile) {
            std::cerr << "ERROR: cannot write config.json\n";
            return 1;
        }
        outFile << configFile.dump(4);   // pretty print to file
        std::println("Written config.json");
    }

    // Verify by printing file contents
    std::cout << "\nconfig.json contents:\n";
    {
        std::ifstream f("config.json");
        std::string line;
        while (std::getline(f, line))
            std::println("  {}", line);
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Read JSON from file\n";
    std::cout << "=====================================================\n";

    {
        std::ifstream inFile("config.json");
        if (!inFile) {
            std::cerr << "ERROR: cannot read config.json\n";
            return 1;
        }

        json loaded = json::parse(inFile);

        std::println("Loaded from file:");
        std::println("  app:     {}", loaded["app"].get<std::string>());
        std::println("  version: {}", loaded["version"].get<std::string>());
        std::println("  host:    {}", loaded["server"]["host"].get<std::string>());
        std::println("  port:    {}", loaded["server"]["port"].get<int>());
        std::println("  tls:     {}", loaded["server"]["tls"].get<bool>());
        std::println("  debug:   {}", loaded["debug"].get<bool>());

        std::cout << "  groups: ";
        for (const auto& g : loaded["groups"])
            std::print("{} ", g.get<std::string>());
        std::cout << '\n';
    }


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Error handling — malformed JSON\n";
    std::cout << "=====================================================\n";

    // Parse error
    try {
        json bad = json::parse("{ invalid json }");
    } catch (const json::parse_error& e) {
        std::println("parse_error: {}", e.what());
        std::println("byte position: {}", e.byte);
    }

    // Type error — wrong get<T>
    try {
        json val = 42;
        std::string s = val.get<std::string>();  // int → string fails
    } catch (const json::type_error& e) {
        std::println("type_error: {}", e.what());
    }

    // Missing key with at()
    try {
        json obj2 = {{"a", 1}};
        int x = obj2.at("missing").get<int>();
        (void)x;
    } catch (const json::out_of_range& e) {
        std::println("out_of_range: {}", e.what());
    }

    // Safe parsing with error check
    std::string maybeJson = "not valid json";
    json result = json::parse(maybeJson, nullptr, false);  // no_throw = false → returns discarded
    if (result.is_discarded())
        std::println("safe parse: JSON was invalid — got discarded value");


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: Real use case — MCX session config\n";
    std::cout << "=====================================================\n";

    // Build MCX config
    json mcxConfig;
    mcxConfig["session"]["id"]              = "MCX-SES-001";
    mcxConfig["session"]["group"]           = "GRP-BOS-Network-ALPHA";
    mcxConfig["session"]["priority"]        = 1;
    mcxConfig["session"]["floor_timeout"]   = 30;

    mcxConfig["network"]["host"]            = "192.168.100.1";
    mcxConfig["network"]["port"]            = 5060;
    mcxConfig["network"]["protocol"]        = "UDP";
    mcxConfig["network"]["tls"]             = true;

    mcxConfig["codec"]["name"]              = "AMR-WB";
    mcxConfig["codec"]["bitrate"]           = 23850;
    mcxConfig["codec"]["sample_rate"]       = 16000;

    mcxConfig["participants"] = json::array();
    for (auto& [id, role] : std::vector<std::pair<std::string,std::string>>{
                                                                             {"PTT-001","dispatcher"}, {"PTT-002","unit"}, {"PTT-003","unit"}})
    {
        mcxConfig["participants"].push_back({{"id",id},{"role",role}});
    }

    // Save to file
    {
        std::ofstream f("mcx_config.json");
        f << mcxConfig.dump(4);
    }

    // Load and use
    {
        std::ifstream f("mcx_config.json");
        json loaded = json::parse(f);

        std::println("MCX Session Config:");
        std::println("  session:    {}", loaded["session"]["id"].get<std::string>());
        std::println("  group:      {}", loaded["session"]["group"].get<std::string>());
        std::println("  host:       {}", loaded["network"]["host"].get<std::string>());
        std::println("  codec:      {}", loaded["codec"]["name"].get<std::string>());
        std::println("  bitrate:    {} bps", loaded["codec"]["bitrate"].get<int>());

        std::cout << "  participants:\n";
        for (const auto& p : loaded["participants"])
            std::println("    {} ({})", p["id"].get<std::string>(), p["role"].get<std::string>());
    }

    // Cleanup
    std::filesystem::remove("config.json");
    std::filesystem::remove("mcx_config.json");

    std::cout << "\n=== JSON complete ===\n";
    return 0;
}
