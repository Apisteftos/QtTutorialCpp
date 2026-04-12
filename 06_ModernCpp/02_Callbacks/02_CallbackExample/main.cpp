#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <print>

// ============================================================
// CALLBACKS in C++23
// ============================================================
// Topics covered:
//   1.  Function pointer as callback
//   2.  std::function — flexible callback wrapper
//   3.  Lambda as callback (inline, capture by value/ref)
//   4.  Callback with return value
//   5.  Member function callback
//   6.  Stored callback — deferred execution
//   7.  Template callback (most efficient)
//   8.  Callback chains / middleware pipeline
//   9.  Success/Error callback pair pattern
//  10.  Practical: MCX event system with callbacks
// ============================================================


// ============================================================
// Example 1 — Function pointer as callback
// ============================================================
void processArray(const int arr[], int size, void (*callback)(int))
{
    for (int i = 0; i < size; ++i)
        callback(arr[i]);
}

void printDouble(int x) { std::cout << x * 2 << ' '; }
void printSquare(int x) { std::cout << x * x << ' '; }
void printHex(int x)    { std::cout << std::hex << x << std::dec << ' '; }


// ============================================================
// Example 2 — std::function — accepts anything callable
// ============================================================
void processVector(const std::vector<int>& vec,
                   std::function<void(int)> callback)
{
    for (int num : vec) callback(num);
}


// ============================================================
// Example 4 — Callback with return value
// ============================================================
int reduce(const std::vector<int>& v, int init,
           std::function<int(int, int)> combiner)
{
    int result = init;
    for (int x : v) result = combiner(result, x);
    return result;
}


// ============================================================
// Example 5 — Member function callback
// ============================================================
class Logger {
private:
    std::string m_prefix;
    int         m_callCount{0};

public:
    explicit Logger(const std::string& prefix) : m_prefix(prefix) {}

    void log(const std::string& msg) {
        ++m_callCount;
        std::println("[{}] #{}: {}", m_prefix, m_callCount, msg);
    }

    void logUpperCase(const std::string& msg) {
        std::string upper = msg;
        for (auto& c : upper) c = std::toupper(c);
        std::println("[{}] {}", m_prefix, upper);
    }

    int callCount() const { return m_callCount; }
};


// ============================================================
// Example 6 — Stored callback / deferred execution
// ============================================================
class Button {
private:
    std::string                   m_label;
    std::function<void()>         m_onClick;
    std::function<void(int, int)> m_onRightClick;

public:
    explicit Button(const std::string& label) : m_label(label) {}

    void setOnClick(std::function<void()> cb)          { m_onClick = cb; }
    void setOnRightClick(std::function<void(int,int)> cb) { m_onRightClick = cb; }

    void click() {
        std::println("[Button '{}'] clicked", m_label);
        if (m_onClick) m_onClick();
        else           std::cout << "  (no handler registered)\n";
    }

    void rightClick(int x, int y) {
        std::println("[Button '{}'] right-clicked at ({},{})", m_label, x, y);
        if (m_onRightClick) m_onRightClick(x, y);
    }
};


// ============================================================
// Example 7 — Template callback (zero overhead)
// ============================================================
template<typename Callback>
void forEachIf(const std::vector<int>& v,
               std::function<bool(int)> predicate,
               Callback action)
{
    for (int x : v)
        if (predicate(x)) action(x);
}

template<typename T, typename Transformer>
std::vector<T> transformAll(const std::vector<T>& v, Transformer fn)
{
    std::vector<T> result;
    result.reserve(v.size());
    for (const auto& x : v) result.push_back(fn(x));
    return result;
}


// ============================================================
// Example 8 — Callback chain / middleware pipeline
// ============================================================
class Pipeline {
private:
    std::vector<std::function<int(int)>> m_stages;

public:
    Pipeline& addStage(std::function<int(int)> stage) {
        m_stages.push_back(stage);
        return *this;   // fluent interface
    }

    int execute(int input) const {
        int val = input;
        for (const auto& stage : m_stages) {
            int prev = val;
            val = stage(val);
            std::println("  stage: {} -> {}", prev, val);
        }
        return val;
    }
};


// ============================================================
// Example 9 — Success/Error callback pair
// ============================================================
struct NetworkResult {
    bool        success;
    std::string data;
    std::string error;
};

void fetchData(const std::string& url,
               std::function<void(const std::string&)> onSuccess,
               std::function<void(const std::string&)> onError)
{
    // Simulate network call
    if (url.find("valid") != std::string::npos) {
        std::string data = "{ \"status\": \"ok\", \"url\": \"" + url + "\" }";
        onSuccess(data);
    } else {
        onError("404 Not Found: " + url);
    }
}


// ============================================================
// Example 10 — MCX Event System
// ============================================================
class McxEventSystem {
public:
    using CallCallback    = std::function<void(int callId, const std::string& groupId)>;
    using FloorCallback   = std::function<void(int callId, int userId)>;
    using ErrorCallback   = std::function<void(int code, const std::string& reason)>;
    using StatusCallback  = std::function<void(bool connected)>;

private:
    CallCallback   m_onCallStarted;
    CallCallback   m_onCallEnded;
    FloorCallback  m_onFloorGranted;
    FloorCallback  m_onFloorReleased;
    ErrorCallback  m_onError;
    StatusCallback m_onConnectionStatus;

public:
    // Registration
    void onCallStarted      (CallCallback   cb) { m_onCallStarted    = cb; }
    void onCallEnded        (CallCallback   cb) { m_onCallEnded      = cb; }
    void onFloorGranted     (FloorCallback  cb) { m_onFloorGranted   = cb; }
    void onFloorReleased    (FloorCallback  cb) { m_onFloorReleased  = cb; }
    void onError            (ErrorCallback  cb) { m_onError          = cb; }
    void onConnectionStatus (StatusCallback cb) { m_onConnectionStatus = cb; }

    // Simulation — fire events
    void simulateConnect()                            { if (m_onConnectionStatus) m_onConnectionStatus(true);  }
    void simulateCallStart(int cid, const std::string& gid) { if (m_onCallStarted)  m_onCallStarted(cid, gid);  }
    void simulateFloorGrant(int cid, int uid)         { if (m_onFloorGranted)  m_onFloorGranted(cid, uid);  }
    void simulateFloorRelease(int cid, int uid)       { if (m_onFloorReleased) m_onFloorReleased(cid, uid); }
    void simulateCallEnd(int cid, const std::string& gid)   { if (m_onCallEnded)    m_onCallEnded(cid, gid);    }
    void simulateError(int code, const std::string& reason) { if (m_onError)        m_onError(code, reason);    }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Function pointer as callback\n";
    std::cout << "=====================================================\n";

    int numbers[] = {1, 2, 3, 4, 5};

    std::cout << "printDouble: ";
    processArray(numbers, 5, printDouble);
    std::cout << '\n';

    std::cout << "printSquare: ";
    processArray(numbers, 5, printSquare);
    std::cout << '\n';

    std::cout << "printHex:    ";
    processArray(numbers, 5, printHex);
    std::cout << '\n';

    // Store function pointer in a variable
    void (*myCallback)(int) = printDouble;
    std::cout << "via pointer: ";
    processArray(numbers, 5, myCallback);
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: std::function — flexible callback\n";
    std::cout << "=====================================================\n";

    std::vector<int> v = {1, 2, 3, 4, 5};

    // Pass a free function
    std::cout << "free function: ";
    processVector(v, printSquare);
    std::cout << '\n';

    // Pass a lambda
    std::cout << "lambda:        ";
    processVector(v, [](int x) { std::cout << x * 3 << ' '; });
    std::cout << '\n';

    // Pass a lambda with capture
    int factor = 7;
    std::cout << "capture(x7):   ";
    processVector(v, [factor](int x) { std::cout << x * factor << ' '; });
    std::cout << '\n';

    // Store std::function in variable
    std::function<void(int)> fn = [](int x) { std::cout << -x << ' '; };
    std::cout << "stored fn:     ";
    processVector(v, fn);
    std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Lambda captures\n";
    std::cout << "=====================================================\n";

    int sum = 0;
    int count = 0;

    // Capture by reference — modifies outer variables
    processVector(v, [&sum, &count](int x) {
        sum   += x;
        count += 1;
    });
    std::println("sum={} count={} avg={:.1f}", sum, count, (double)sum/count);

    // Capture by value — owns a copy
    std::vector<int> collected;
    int threshold = 3;
    processVector(v, [threshold, &collected](int x) {
        if (x > threshold) collected.push_back(x);
    });
    std::cout << "collected (>3): ";
    for (int x : collected) std::cout << x << ' '; std::cout << '\n';

    // Mutable lambda — modify captured copy
    int counter = 0;
    auto counting = [counter](int) mutable {
        ++counter;
        std::cout << "call#" << counter << ' ';
    };
    std::cout << "mutable lambda: ";
    processVector(v, counting);
    std::cout << '\n';
    std::cout << "outer counter: " << counter << " (unchanged)\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Callback with return value\n";
    std::cout << "=====================================================\n";

    std::vector<int> data = {1, 2, 3, 4, 5};

    int total  = reduce(data, 0, [](int acc, int x) { return acc + x; });
    int product= reduce(data, 1, [](int acc, int x) { return acc * x; });
    int maxVal = reduce(data, INT_MIN, [](int acc, int x) { return std::max(acc, x); });

    std::println("sum:     {}", total);
    std::println("product: {}", product);
    std::println("max:     {}", maxVal);

    // Callback that transforms and filters
    auto transform = [](const std::vector<int>& v,
                        std::function<int(int)> fn) -> std::vector<int> {
        std::vector<int> result;
        for (int x : v) result.push_back(fn(x));
        return result;
    };

    auto squares = transform(data, [](int x) { return x * x; });
    std::cout << "squares: ";
    for (int x : squares) std::cout << x << ' '; std::cout << '\n';


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: Member function callback\n";
    std::cout << "=====================================================\n";

    Logger logger("APP");

    // Wrap member function in lambda (most common approach)
    std::function<void(const std::string&)> logCb =
        [&logger](const std::string& msg) { logger.log(msg); };

    logCb("server started");
    logCb("connection established");
    logCb("data received");
    std::println("log call count: {}", logger.callCount());

    // Using std::bind (older style — lambdas preferred)
    auto boundLog = std::bind(&Logger::logUpperCase, &logger, std::placeholders::_1);
    boundLog("bind example");

    // Store member function callback and call later
    std::vector<std::function<void(const std::string&)>> handlers;
    handlers.push_back([&logger](const std::string& m) { logger.log(m); });
    handlers.push_back([](const std::string& m) { std::println("[CONSOLE] {}", m); });

    std::cout << "broadcasting to " << handlers.size() << " handlers:\n";
    for (auto& h : handlers) h("broadcast message");


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 6: Stored callback — deferred execution\n";
    std::cout << "=====================================================\n";

    Button btn("Submit");

    // Register callbacks
    btn.setOnClick([]() {
        std::cout << "  form submitted!\n";
    });
    btn.setOnRightClick([](int x, int y) {
        std::println("  context menu at ({},{})", x, y);
    });

    btn.click();
    btn.rightClick(150, 200);

    // Change callback at runtime
    btn.setOnClick([]() { std::cout << "  new handler!\n"; });
    btn.click();

    // Button with no handler
    Button btn2("Cancel");
    btn2.click();   // prints "(no handler registered)"


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 7: Template callback — zero overhead\n";
    std::cout << "=====================================================\n";

    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "even numbers doubled: ";
    forEachIf(nums,
              [](int x) { return x % 2 == 0; },
              [](int x) { std::cout << x * 2 << ' '; });
    std::cout << '\n';

    auto tripled = transformAll(nums, [](int x) { return x * 3; });
    std::cout << "tripled:              ";
    for (int x : tripled) std::cout << x << ' '; std::cout << '\n';

    // Template vs std::function — template is inlined by compiler
    std::cout << "\nTemplate callback: inlined by compiler — no virtual call\n";
    std::cout << "std::function:     type-erased — small overhead per call\n";
    std::cout << "Use template when: performance-critical tight loops\n";
    std::cout << "Use std::function: storing, returning, or runtime dispatch\n";


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 8: Callback chain — processing pipeline\n";
    std::cout << "=====================================================\n";

    Pipeline pipeline;
    pipeline
        .addStage([](int x) { return x * 2;    })   // double
        .addStage([](int x) { return x + 10;   })   // add 10
        .addStage([](int x) { return x * x;    })   // square
        .addStage([](int x) { return x / 4;    });  // divide

    std::cout << "Pipeline result for input=3:\n";
    int result = pipeline.execute(3);
    std::println("final result: {}", result);


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 9: Success/Error callback pair\n";
    std::cout << "=====================================================\n";

    fetchData("https://valid.api/data",
              [](const std::string& data)  { std::println("SUCCESS: {}", data); },
              [](const std::string& error) { std::println("ERROR:   {}", error); }
              );

    fetchData("https://broken.api/missing",
              [](const std::string& data)  { std::println("SUCCESS: {}", data); },
              [](const std::string& error) { std::println("ERROR:   {}", error); }
              );

    // Retry pattern with error callback
    int retries = 0;
    std::function<void()> attempt = [&]() {
        fetchData("https://broken.api/resource",
                  [](const std::string& d) { std::println("got data: {}", d); },
                  [&](const std::string& e) {
                      std::println("attempt {} failed: {}", retries+1, e);
                      if (++retries < 3) attempt();
                  }
                  );
    };
    attempt();


    // -------------------------------------------------------
    std::cout << "\n=====================================================\n";
    std::cout << "Example 10: MCX Event System\n";
    std::cout << "=====================================================\n";

    McxEventSystem mcx;

    // Register all event callbacks
    mcx.onConnectionStatus([](bool connected) {
        std::println("[MCX] connection: {}", connected ? "ESTABLISHED" : "LOST");
    });

    mcx.onCallStarted([](int callId, const std::string& groupId) {
        std::println("[MCX] call started — id={} group={}", callId, groupId);
    });

    mcx.onFloorGranted([](int callId, int userId) {
        std::println("[MCX] floor GRANTED — call={} user={}", callId, userId);
    });

    mcx.onFloorReleased([](int callId, int userId) {
        std::println("[MCX] floor RELEASED — call={} user={}", callId, userId);
    });

    mcx.onCallEnded([](int callId, const std::string& groupId) {
        std::println("[MCX] call ended — id={} group={}", callId, groupId);
    });

    mcx.onError([](int code, const std::string& reason) {
        std::println("[MCX] ERROR {} — {}", code, reason);
    });

    // Simulate MCX event sequence
    std::cout << "\nSimulating MCX group call:\n";
    mcx.simulateConnect();
    mcx.simulateCallStart(1001, "GRP-42");
    mcx.simulateFloorGrant(1001, 7);
    mcx.simulateFloorRelease(1001, 7);
    mcx.simulateCallEnd(1001, "GRP-42");
    mcx.simulateError(503, "server unavailable");

    std::cout << "\n=== Callbacks complete ===\n";
    return 0;
}
