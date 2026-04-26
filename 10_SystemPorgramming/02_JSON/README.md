# JSON — C++23 with nlohmann/json

## What is JSON?

**JSON** (JavaScript Object Notation) is a lightweight text format for
data exchange. Despite the name it is language-independent and is the
standard format for config files, REST APIs, and inter-process data.

```json
{
  "name": "MCXApp",
  "port": 5060,
  "tls": true,
  "groups": ["ALPHA", "BETA"]
}
```

---

## Installation

```bash
sudo apt install nlohmann-json3-dev
```

```cmake
find_package(nlohmann_json REQUIRED)
target_link_libraries(MyApp nlohmann_json::nlohmann_json)
```

```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;
```

---

## JSON value types

```mermaid
flowchart TD
    J["JSON value"]
    J --> obj["object\n{ key: value }"]
    J --> arr["array\n[ 1, 2, 3 ]"]
    J --> str["string\n'hello'"]
    J --> num["number\n42 / 3.14"]
    J --> bool_["boolean\ntrue / false"]
    J --> null_["null\nnull"]

    obj --> nested["can contain\nany JSON value"]
    arr --> nested
```

---

## Creating JSON

```mermaid
flowchart LR
    subgraph Ways ["Ways to create JSON"]
        A["json obj;\nobj['key'] = value;\nobj['nested']['x'] = 1;"]
        B["json obj = {\n  {'key', value},\n  {'num', 42}\n};"]
        C["json obj = json::parse(str);\njson obj = json::parse(file);"]
    end
```

```cpp
// Method 1 — assign keys
json config;
config["app"]  = "MCXApp";
config["port"] = 5060;

// Method 2 — initializer list
json config = {{"app","MCXApp"}, {"port",5060}};

// Method 3 — parse string
json config = json::parse(R"({"app":"MCXApp","port":5060})");

// Method 4 — parse file
std::ifstream f("config.json");
json config = json::parse(f);
```

---

## Accessing values

```mermaid
flowchart TD
    access["Access methods"]
    access --> bracket["obj['key']\nNo bounds check\nCreates key if missing"]
    access --> at["obj.at('key')\nThrows out_of_range\nif key missing"]
    access --> value["obj.value('key', default)\nReturns default\nif key missing"]
    access --> contains["obj.contains('key')\nCheck before access"]
```

```cpp
json data = {{"host","192.168.1.1"}, {"port",5060}};

// [] — no bounds check
std::string host = data["host"].get<std::string>();

// at() — throws if missing
int port = data.at("port").get<int>();

// value() — safe with default
int timeout = data.value("timeout", 30);   // 30 if not present

// contains() — check first
if (data.contains("tls")) { ... }
```

---

## Nested objects

```mermaid
flowchart LR
    root["json session"]
    root --> id["session['id'] = 'SES-001'"]
    root --> group["session['group']['id'] = 'GRP-ALPHA'\nsession['group']['name'] = 'Alpha'"]
    root --> server["session['server']['host'] = '192.168.1.1'\nsession['server']['port'] = 5060"]
```

```cpp
json session;
session["id"]             = "SES-001";
session["group"]["id"]    = "GRP-ALPHA";
session["server"]["host"] = "192.168.1.1";
session["server"]["port"] = 5060;

// Access nested
std::string groupId = session["group"]["id"].get<std::string>();

// Iterate nested object
for (auto& [key, val] : session["server"].items())
    std::cout << key << " = " << val.dump() << '\n';
```

---

## Arrays ↔ std::vector

```mermaid
flowchart LR
    subgraph ToJSON ["std::vector → JSON"]
        V1["vector&lt;string&gt; groups\n= {'ALPHA','BETA'}"]
        J1["json groupJson = groups"]
        V1 --> J1
    end

    subgraph FromJSON ["JSON → std::vector"]
        J2["json nums = {10,20,30}"]
        V2["vector&lt;int&gt; vec\n= nums.get&lt;vector&lt;int&gt;&gt;()"]
        J2 --> V2
    end
```

```cpp
// vector → JSON
std::vector<std::string> groups = {"ALPHA", "BETA"};
json j = groups;

// JSON → vector
json nums = {10, 20, 30};
std::vector<int> vec = nums.get<std::vector<int>>();

// Array of objects
json users = json::array();
users.push_back({{"name","Alice"}, {"age",30}});
users.push_back({{"name","Bob"},   {"age",25}});

for (const auto& u : users)
    std::cout << u["name"] << '\n';
```

---

## Serialize — dump()

```cpp
json obj = {{"name","MCXApp"}, {"port",5060}};

obj.dump()    // compact: {"name":"MCXApp","port":5060}
obj.dump(2)   // pretty 2-space indent
obj.dump(4)   // pretty 4-space indent
```

---

## File I/O

```mermaid
sequenceDiagram
    participant App
    participant File as config.json

    App->>App: build json object
    App->>File: ofstream f("config.json")\nf << obj.dump(4)
    Note over File: JSON saved to disk

    App->>File: ifstream f("config.json")
    File-->>App: json::parse(f)
    App->>App: use loaded JSON
```

```cpp
// Write
std::ofstream out("config.json");
out << config.dump(4);

// Read
std::ifstream in("config.json");
json loaded = json::parse(in);
```

---

## Error handling

```mermaid
flowchart TD
    parse["json::parse(str)"]
    parse -->|valid JSON| ok["json object ✅"]
    parse -->|invalid JSON| pe["json::parse_error\ne.what()\ne.byte — position"]

    get["obj.get&lt;T&gt;()"]
    get -->|correct type| val["value ✅"]
    get -->|wrong type| te["json::type_error"]

    at["obj.at('key')"]
    at -->|key exists| val2["value ✅"]
    at -->|key missing| oe["json::out_of_range"]
```

```cpp
// Parse error
try {
    json bad = json::parse("{ invalid }");
} catch (const json::parse_error& e) {
    std::cout << e.what() << " at byte " << e.byte;
}

// Type error
try {
    json val = 42;
    std::string s = val.get<std::string>();  // fails
} catch (const json::type_error& e) { }

// Safe parse — no exception
json result = json::parse(str, nullptr, false);
if (result.is_discarded()) { /* invalid */ }
```

---

## Real use case — MCX config file

```json
{
  "session": {
    "id": "MCX-SES-001",
    "group": "GRP-BOS-Network-ALPHA",
    "priority": 1,
    "floor_timeout": 30
  },
  "network": {
    "host": "192.168.100.1",
    "port": 5060,
    "protocol": "UDP",
    "tls": true
  },
  "codec": {
    "name": "AMR-WB",
    "bitrate": 23850,
    "sample_rate": 16000
  },
  "participants": [
    {"id": "PTT-001", "role": "dispatcher"},
    {"id": "PTT-002", "role": "unit"},
    {"id": "PTT-003", "role": "unit"}
  ]
}
```

---

## Quick reference

| Task | Code |
|------|------|
| Create | `json obj; obj["key"] = val;` |
| Parse string | `json::parse(str)` |
| Parse file | `json::parse(ifstream)` |
| Get value | `obj["key"].get<T>()` |
| Safe get | `obj.value("key", default)` |
| Check key | `obj.contains("key")` |
| Array | `json::array()` + `push_back()` |
| Iterate object | `for (auto& [k,v] : obj.items())` |
| Serialize | `obj.dump()` / `obj.dump(4)` |
| Write file | `ofstream << obj.dump(4)` |
| Read file | `json::parse(ifstream)` |
| Parse error | `catch (json::parse_error& e)` |
| Type error | `catch (json::type_error& e)` |
| Safe parse | `json::parse(s, nullptr, false)` |

---

## When to use JSON

✅ Config files — settings, options, parameters
✅ REST API — send/receive data over HTTP
✅ Inter-process data exchange
✅ Logging structured data
✅ MCX/MCPTT session configuration

❌ Binary data — use binary formats (protobuf, msgpack)
❌ Very large datasets — JSON is verbose, use binary
❌ Real-time high-frequency data — parsing overhead
