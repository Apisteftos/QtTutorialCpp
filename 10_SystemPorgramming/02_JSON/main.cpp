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
//  10.  Real use case — service config
// ============================================================

using json = nlohmann::json;


int main()
{
    // -------------------------------------------------------
    std::println("=====================================================");
    std::println("Example 1: Create JSON object from scratch");
    std::println("=====================================================");

    json config;
    config["app"]     = "MyApp";
    config["version"] = "2.3.1";
    config["port"]    = 8080;
    config["debug"]   = true;
    config["pi"]      = 3.14159;

    std::println("app:     {}", config["app"].get<std::string>());
    std::println("version: {}", config["version"].get<std::string>());
    std::println("port:    {}", config["port"].get<int>());
    std::println("debug:   {}", config["debug"].get<bool>());
    std::println("pi:      {}", config["pi"].get<double>());

    // Check type
    std::println("");
    std::println("type checks:");
    std::println("  config[port] is_number: {}", config["port"].is_number());
    std::println("  config[app]  is_string: {}", config["app"].is_string());
    std::println("  config[debug] is_bool:  {}", config["debug"].is_boolean());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Parse JSON from string");
    std::println("=====================================================");

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
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Access values — [], at(), value()");
    std::println("=====================================================");

    json data = json::parse(R"({"host":"192.168.1.1","port":8080,"timeout":30})");

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
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Nested objects");
    std::println("=====================================================");

    json service;
    service["id"]              = "SVC-001";
    service["team"]["id"]     = "TEAM-A";
    service["team"]["name"]   = "Platform Team";
    service["team"]["active"] = true;
    service["server"]["host"]  = "192.168.1.100";
    service["server"]["port"]  = 8080;
    service["server"]["tls"]   = false;

    std::println("service id:       {}", service["id"].get<std::string>());
    std::println("team id:          {}", service["team"]["id"].get<std::string>());
    std::println("team name:        {}", service["team"]["name"].get<std::string>());
    std::println("server host:      {}", service["server"]["host"].get<std::string>());
    std::println("server port:      {}", service["server"]["port"].get<int>());
    std::println("server tls:       {}", service["server"]["tls"].get<bool>());

    // Iterate over nested object
    std::println("");
    std::println("server keys:");
    for (auto& [key, value] : service["server"].items())
        std::println("  {} = {}", key, value.dump());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: JSON arrays ↔ std::vector");
    std::println("=====================================================");

    // JSON array from initializer list
    json colors = {"red", "green", "blue"};
    std::println("colors size: {}", colors.size());
    for (const auto& c : colors)
        std::print("  {}", c.get<std::string>());
    std::println("");

    // Array of objects
    json users = json::array();
    users.push_back({{"name","Alice"},{"age",30}});
    users.push_back({{"name","Bob"},  {"age",25}});
    users.push_back({{"name","Carol"},{"age",35}});

    std::println("");
    std::println("users array:");
    for (const auto& u : users)
        std::println("  {} age {}", u["name"].get<std::string>(), u["age"].get<int>());

    // Convert JSON array ↔ std::vector
    json nums = {10, 20, 30, 40, 50};
    std::vector<int> vec = nums.get<std::vector<int>>();
    std::println("");
    std::print("vector from JSON: ");
    for (int n : vec) std::print("{} ", n);
    std::println("");

    // vector → JSON
    std::vector<std::string> tags = {"TEAM-A", "TEAM-B", "TEAM-C"};
    json tagJson = tags;
    std::println("JSON from vector: {}", tagJson.dump());


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: Serialize — dump() and pretty print");
    std::println("=====================================================");

    json obj;
    obj["name"]    = "MyApp";
    obj["port"]    = 8080;
    obj["tags"]  = {"alpha", "beta"};
    obj["enabled"] = true;

    // Compact
    std::println("compact: {}", obj.dump());

    // Pretty print with indent
    std::println("");
    std::println("pretty (2 spaces):\n{}", obj.dump(2));

    // Pretty print with 4 spaces
    std::println("pretty (4 spaces):\n{}", obj.dump(4));


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: Write JSON to file");
    std::println("=====================================================");

    json configFile;
    configFile["app"]             = "MyApp";
    configFile["version"]         = "2.3.1";
    configFile["server"]["host"]  = "192.168.1.100";
    configFile["server"]["port"]  = 8080;
    configFile["server"]["tls"]   = true;
    configFile["tags"]          = {"TEAM-A", "TEAM-B"};
    configFile["debug"]           = false;

    {
        std::ofstream outFile("config.json");
        if (!outFile) {
            std::println(stderr, "ERROR: cannot write config.json");
            return 1;
        }
        outFile << configFile.dump(4);   // pretty print to file
        std::println("Written config.json");
    }

    // Verify by printing file contents
    std::println("");
    std::println("config.json contents:");
    {
        std::ifstream f("config.json");
        std::string line;
        while (std::getline(f, line))
            std::println("  {}", line);
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 8: Read JSON from file");
    std::println("=====================================================");

    {
        std::ifstream inFile("config.json");
        if (!inFile) {
            std::println(stderr, "ERROR: cannot read config.json");
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

        std::print("  tags: ");
        for (const auto& g : loaded["tags"])
            std::print("{} ", g.get<std::string>());
        std::println("");
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 9: Error handling — malformed JSON");
    std::println("=====================================================");

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
    std::println("");
    std::println("=====================================================");
    std::println("Example 10: Real use case — service config");
    std::println("=====================================================");

    // Build the service config
    json appConfig;
    appConfig["service"]["id"]              = "SVC-001";
    appConfig["service"]["team"]           = "TEAM-PLATFORM";
    appConfig["service"]["priority"]        = 1;
    appConfig["service"]["request_timeout"]   = 30;

    appConfig["network"]["host"]            = "192.168.100.1";
    appConfig["network"]["port"]            = 8080;
    appConfig["network"]["protocol"]        = "TCP";
    appConfig["network"]["tls"]             = true;

    appConfig["compression"]["name"]        = "zstd";
    appConfig["compression"]["level"]       = 3;
    appConfig["compression"]["block_size"]  = 65536;

    appConfig["users"] = json::array();
    for (auto& [id, role] : std::vector<std::pair<std::string,std::string>>{
                                                                             {"USER-001","admin"}, {"USER-002","member"}, {"USER-003","member"}})
    {
        appConfig["users"].push_back({{"id",id},{"role",role}});
    }

    // Save to file
    {
        std::ofstream f("app_config.json");
        f << appConfig.dump(4);
    }

    // Load and use
    {
        std::ifstream f("app_config.json");
        json loaded = json::parse(f);

        std::println("Service Config:");
        std::println("  service:    {}", loaded["service"]["id"].get<std::string>());
        std::println("  team:       {}", loaded["service"]["team"].get<std::string>());
        std::println("  host:       {}", loaded["network"]["host"].get<std::string>());
        std::println("  compression:{}", loaded["compression"]["name"].get<std::string>());
        std::println("  level:      {}", loaded["compression"]["level"].get<int>());

        std::println("  users:");
        for (const auto& p : loaded["users"])
            std::println("    {} ({})", p["id"].get<std::string>(), p["role"].get<std::string>());
    }

    // Cleanup
    std::filesystem::remove("config.json");
    std::filesystem::remove("app_config.json");

    std::println("");
    std::println("=== JSON complete ===");
    return 0;
}
