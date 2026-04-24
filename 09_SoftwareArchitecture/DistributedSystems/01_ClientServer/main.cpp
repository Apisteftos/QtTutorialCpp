// =============================================================================
// Client-Server Pattern — C++23
//
// Simulates an HTTP-style client/server in-process.
// Real production: replace Server::dispatch() with a TCP socket loop.
// The handler registration, routing, and request/response logic is identical.
//
// Components:
//   Request    — method + path + body
//   Response   — status code + body
//   IHandler   — interface for route handlers
//   Server     — registers handlers, routes requests
//   Client     — sends requests, receives responses
// =============================================================================

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include <functional>

// =============================================================================
// Request / Response
// =============================================================================

struct Request {
    std::string method;   // GET, POST, DELETE
    std::string path;     // /users/42
    std::string body;     // JSON payload for POST/PUT

    std::string toString() const {
        return method + " " + path + (body.empty() ? "" : " body=" + body);
    }
};

struct Response {
    int         statusCode{200};
    std::string body;

    bool ok()      const { return statusCode >= 200 && statusCode < 300; }
    bool notFound() const { return statusCode == 404; }

    std::string toString() const {
        return std::to_string(statusCode) + " " + body;
    }
};

// ── Common responses ──────────────────────────────────────────────────────────
Response ok200(const std::string& body)  { return {200, body}; }
Response created201(const std::string& body) { return {201, body}; }
Response notFound404(const std::string& msg) { return {404, "Not Found: " + msg}; }
Response error500(const std::string& msg)    { return {500, "Error: " + msg}; }
Response badRequest400(const std::string& msg) { return {400, "Bad Request: " + msg}; }

// =============================================================================
// Handler interface
// =============================================================================

class IHandler {
public:
    virtual ~IHandler() = default;
    virtual Response handle(const Request& req) = 0;
    virtual std::string path()   const = 0;
    virtual std::string method() const = 0;
};

// =============================================================================
// Concrete Handlers
// =============================================================================

// In-memory user store
struct User { int id; std::string name; std::string role; };
static std::unordered_map<int, User> user_db = {
    {1, {1, "Kostas",  "admin"}},
    {2, {2, "George",  "user"}},
    {3, {3, "Detlef",  "recruiter"}},
    };
static int next_user_id = 4;

class GetUserHandler : public IHandler {
public:
    std::string path()   const override { return "/users/{id}"; }
    std::string method() const override { return "GET"; }

    Response handle(const Request& req) override {
        // Extract ID from path: /users/42 → 42
        auto pos = req.path.rfind('/');
        if (pos == std::string::npos) return badRequest400("missing id");
        try {
            int id = std::stoi(req.path.substr(pos + 1));
            auto it = user_db.find(id);
            if (it == user_db.end()) return notFound404("user " + std::to_string(id));
            auto& u = it->second;
            return ok200("{\"id\":" + std::to_string(u.id)
                         + ",\"name\":\"" + u.name
                         + "\",\"role\":\"" + u.role + "\"}");
        } catch (...) {
            return badRequest400("invalid id");
        }
    }
};

class ListUsersHandler : public IHandler {
public:
    std::string path()   const override { return "/users"; }
    std::string method() const override { return "GET"; }

    Response handle(const Request&) override {
        std::ostringstream oss;
        oss << "[";
        bool first = true;
        for (const auto& [id, u] : user_db) {
            if (!first) oss << ",";
            oss << "{\"id\":" << u.id << ",\"name\":\"" << u.name << "\"}";
            first = false;
        }
        oss << "]";
        return ok200(oss.str());
    }
};

class CreateUserHandler : public IHandler {
public:
    std::string path()   const override { return "/users"; }
    std::string method() const override { return "POST"; }

    Response handle(const Request& req) override {
        if (req.body.empty()) return badRequest400("body required");
        // Simplified parse: body = "name=Foo"
        auto pos = req.body.find("name=");
        if (pos == std::string::npos) return badRequest400("name field required");
        std::string name = req.body.substr(pos + 5);
        int id = next_user_id++;
        user_db[id] = {id, name, "user"};
        return created201("{\"id\":" + std::to_string(id) + ",\"name\":\"" + name + "\"}");
    }
};

class HealthHandler : public IHandler {
public:
    std::string path()   const override { return "/health"; }
    std::string method() const override { return "GET"; }
    Response handle(const Request&) override {
        return ok200("{\"status\":\"ok\",\"uptime\":42}");
    }
};

// =============================================================================
// Server — routes requests to handlers
// =============================================================================

class Server {
public:
    void registerHandler(std::unique_ptr<IHandler> handler) {
        std::string key = handler->method() + " " + handler->path();
        handlers_[key] = std::move(handler);
        std::cout << "  [Server] registered: " << key << '\n';
    }

    Response dispatch(const Request& req) {
        std::cout << "\n→ " << req.toString() << '\n';

        // Try exact match first
        std::string key = req.method + " " + req.path;
        auto it = handlers_.find(key);
        if (it != handlers_.end()) {
            auto resp = it->second->handle(req);
            std::cout << "← " << resp.toString() << '\n';
            return resp;
        }

        // Try prefix match for parameterised paths (/users/{id})
        for (auto& [pattern, handler] : handlers_) {
            if (handler->method() != req.method) continue;
            auto hpath = handler->path();
            auto brace = hpath.find("/{");
            if (brace != std::string::npos) {
                // Check if path prefix matches
                if (req.path.substr(0, brace) == hpath.substr(0, brace)
                    && req.path.size() > brace + 1) {
                    auto resp = handler->handle(req);
                    std::cout << "← " << resp.toString() << '\n';
                    return resp;
                }
            }
        }

        auto resp = notFound404(req.path);
        std::cout << "← " << resp.toString() << '\n';
        return resp;
    }

    int handlerCount() const { return static_cast<int>(handlers_.size()); }

private:
    std::unordered_map<std::string, std::unique_ptr<IHandler>> handlers_;
};

// =============================================================================
// Client — sends requests to server
// =============================================================================

class Client {
public:
    explicit Client(Server& server, const std::string& name)
        : server_(server), name_(name) {}

    Response get(const std::string& path) {
        return server_.dispatch({.method="GET", .path=path});
    }

    Response post(const std::string& path, const std::string& body) {
        return server_.dispatch({.method="POST", .path=path, .body=body});
    }

    Response del(const std::string& path) {
        return server_.dispatch({.method="DELETE", .path=path});
    }

    const std::string& name() const { return name_; }

private:
    Server&     server_;
    std::string name_;
};

// =============================================================================
// main
// =============================================================================

int main()
{
    std::cout << "=== Client-Server Pattern Demo ===\n\n";

    // ── Setup server ──────────────────────────────────────────────────────────
    std::cout << "--- Server setup ---\n";
    Server server;
    server.registerHandler(std::make_unique<HealthHandler>());
    server.registerHandler(std::make_unique<ListUsersHandler>());
    server.registerHandler(std::make_unique<GetUserHandler>());
    server.registerHandler(std::make_unique<CreateUserHandler>());
    assert(server.handlerCount() == 4);

    // ── Client A: admin operations ────────────────────────────────────────────
    std::cout << "\n--- Client A (admin) ---\n";
    Client admin(server, "admin-client");

    auto health = admin.get("/health");
    assert(health.ok());
    assert(health.statusCode == 200);

    auto users = admin.get("/users");
    assert(users.ok());

    auto user1 = admin.get("/users/1");
    assert(user1.ok());
    assert(user1.body.find("Kostas") != std::string::npos);

    auto missing = admin.get("/users/999");
    assert(missing.notFound());

    // ── Client B: creates a new user ──────────────────────────────────────────
    std::cout << "\n--- Client B (new user registration) ---\n";
    Client app(server, "app-client");

    auto created = app.post("/users", "name=Dieter");
    assert(created.statusCode == 201);
    assert(created.body.find("Dieter") != std::string::npos);

    // Verify new user is retrievable
    auto newUser = admin.get("/users/4");
    assert(newUser.ok());
    assert(newUser.body.find("Dieter") != std::string::npos);

    // ── Error cases ───────────────────────────────────────────────────────────
    std::cout << "\n--- Error cases ---\n";
    auto badPost = app.post("/users", "");   // missing body
    assert(badPost.statusCode == 400);

    auto notFound = app.get("/nonexistent");
    assert(notFound.notFound());

    auto wrongMethod = app.del("/users");   // DELETE /users not registered
    assert(wrongMethod.notFound());

    std::cout << "\n=== Summary ===\n";
    std::cout << "Handlers registered: " << server.handlerCount() << '\n';
    std::cout << "All assertions passed.\n";
    return 0;
}
