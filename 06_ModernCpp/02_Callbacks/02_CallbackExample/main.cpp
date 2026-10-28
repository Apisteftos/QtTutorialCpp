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
//  10.  Practical: job event system with callbacks
// ============================================================


// ============================================================
// Example 1 — Function pointer as callback
// ============================================================
void processArray(const int arr[], int size, void (*callback)(int))
{
    for (int i = 0; i < size; ++i)
        callback(arr[i]);
}

void printDouble(int x) { std::print("{} ", x * 2); }
void printSquare(int x) { std::print("{} ", x * x); }
void printHex(int x)    { std::print("{:x} ", x); }


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
        else           std::println("  (no handler registered)");
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
// Example 10 — Job Event System
// ============================================================
class JobEventSystem {
public:
    using JobCallback    = std::function<void(int jobId, const std::string& queueName)>;
    using LockCallback   = std::function<void(int jobId, int userId)>;
    using ErrorCallback   = std::function<void(int code, const std::string& reason)>;
    using StatusCallback  = std::function<void(bool connected)>;

private:
    JobCallback   m_onJobStarted;
    JobCallback   m_onJobEnded;
    LockCallback  m_onLockAcquired;
    LockCallback  m_onLockReleased;
    ErrorCallback  m_onError;
    StatusCallback m_onConnectionStatus;

public:
    // Registration
    void onJobStarted      (JobCallback   cb) { m_onJobStarted    = cb; }
    void onJobEnded        (JobCallback   cb) { m_onJobEnded      = cb; }
    void onLockAcquired     (LockCallback  cb) { m_onLockAcquired   = cb; }
    void onLockReleased    (LockCallback  cb) { m_onLockReleased  = cb; }
    void onError            (ErrorCallback  cb) { m_onError          = cb; }
    void onConnectionStatus (StatusCallback cb) { m_onConnectionStatus = cb; }

    // Simulation — fire events
    void simulateConnect()                            { if (m_onConnectionStatus) m_onConnectionStatus(true);  }
    void simulateJobStart(int cid, const std::string& qname) { if (m_onJobStarted)  m_onJobStarted(cid, qname);  }
    void simulateLockAcquire(int cid, int uid)         { if (m_onLockAcquired)  m_onLockAcquired(cid, uid);  }
    void simulateLockRelease(int cid, int uid)       { if (m_onLockReleased) m_onLockReleased(cid, uid); }
    void simulateJobEnd(int cid, const std::string& qname)   { if (m_onJobEnded)    m_onJobEnded(cid, qname);    }
    void simulateError(int code, const std::string& reason) { if (m_onError)        m_onError(code, reason);    }
};


// ============================================================
// MAIN
// ============================================================
int main()
{
    // -------------------------------------------------------
    std::println("=====================================================");
    std::println("Example 1: Function pointer as callback");
    std::println("=====================================================");

    int numbers[] = {1, 2, 3, 4, 5};

    std::print("printDouble: ");
    processArray(numbers, 5, printDouble);
    std::println("");

    std::print("printSquare: ");
    processArray(numbers, 5, printSquare);
    std::println("");

    std::print("printHex:    ");
    processArray(numbers, 5, printHex);
    std::println("");

    // Store function pointer in a variable
    void (*myCallback)(int) = printDouble;
    std::print("via pointer: ");
    processArray(numbers, 5, myCallback);
    std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: std::function — flexible callback");
    std::println("=====================================================");

    std::vector<int> v = {1, 2, 3, 4, 5};

    // Pass a free function
    std::print("free function: ");
    processVector(v, printSquare);
    std::println("");

    // Pass a lambda
    std::print("lambda:        ");
    processVector(v, [](int x) { std::print("{} ", x * 3); });
    std::println("");

    // Pass a lambda with capture
    int factor = 7;
    std::print("capture(x7):   ");
    processVector(v, [factor](int x) { std::print("{} ", x * factor); });
    std::println("");

    // Store std::function in variable
    std::function<void(int)> fn = [](int x) { std::print("{} ", -x); };
    std::print("stored fn:     ");
    processVector(v, fn);
    std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Lambda captures");
    std::println("=====================================================");

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
    std::print("collected (>3): ");
    for (int x : collected) std::print("{} ", x); std::println("");

    // Mutable lambda — modify captured copy
    int counter = 0;
    auto counting = [counter](int) mutable {
        ++counter;
        std::print("call#{} ", counter);
    };
    std::print("mutable lambda: ");
    processVector(v, counting);
    std::println("");
    std::println("outer counter: {} (unchanged)", counter);


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: Callback with return value");
    std::println("=====================================================");

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
    std::print("squares: ");
    for (int x : squares) std::print("{} ", x); std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: Member function callback");
    std::println("=====================================================");

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

    std::println("broadcasting to {} handlers:", handlers.size());
    for (auto& h : handlers) h("broadcast message");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: Stored callback — deferred execution");
    std::println("=====================================================");

    Button btn("Submit");

    // Register callbacks
    btn.setOnClick([]() {
        std::println("  form submitted!");
    });
    btn.setOnRightClick([](int x, int y) {
        std::println("  context menu at ({},{})", x, y);
    });

    btn.click();
    btn.rightClick(150, 200);

    // Change callback at runtime
    btn.setOnClick([]() { std::println("  new handler!"); });
    btn.click();

    // Button with no handler
    Button btn2("Cancel");
    btn2.click();   // prints "(no handler registered)"


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: Template callback — zero overhead");
    std::println("=====================================================");

    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::print("even numbers doubled: ");
    forEachIf(nums,
              [](int x) { return x % 2 == 0; },
              [](int x) { std::print("{} ", x * 2); });
    std::println("");

    auto tripled = transformAll(nums, [](int x) { return x * 3; });
    std::print("tripled:              ");
    for (int x : tripled) std::print("{} ", x); std::println("");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 8: Callback chain — processing pipeline");
    std::println("=====================================================");

    Pipeline pipeline;
    pipeline
        .addStage([](int x) { return x * 2;    })   // double
        .addStage([](int x) { return x + 10;   })   // add 10
        .addStage([](int x) { return x * x;    })   // square
        .addStage([](int x) { return x / 4;    });  // divide

    std::println("Pipeline result for input=3:");
    int result = pipeline.execute(3);
    std::println("final result: {}", result);


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 9: Success/Error callback pair");
    std::println("=====================================================");

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
    std::println("");
    std::println("=====================================================");
    std::println("Example 10: Job Event System");
    std::println("=====================================================");

    JobEventSystem events;

    // Register all event callbacks
    events.onConnectionStatus([](bool connected) {
        std::println("[events] connection: {}", connected ? "ESTABLISHED" : "LOST");
    });

    events.onJobStarted([](int jobId, const std::string& queueName) {
        std::println("[events] job started — id={} queue={}", jobId, queueName);
    });

    events.onLockAcquired([](int jobId, int userId) {
        std::println("[events] lock ACQUIRED — job={} user={}", jobId, userId);
    });

    events.onLockReleased([](int jobId, int userId) {
        std::println("[events] lock RELEASED — job={} user={}", jobId, userId);
    });

    events.onJobEnded([](int jobId, const std::string& queueName) {
        std::println("[events] job ended — id={} queue={}", jobId, queueName);
    });

    events.onError([](int code, const std::string& reason) {
        std::println("[events] ERROR {} — {}", code, reason);
    });

    // Simulate an event sequence
    std::println("");
    std::println("Simulating a job run:");
    events.simulateConnect();
    events.simulateJobStart(1001, "queue-42");
    events.simulateLockAcquire(1001, 7);
    events.simulateLockRelease(1001, 7);
    events.simulateJobEnd(1001, "queue-42");
    events.simulateError(503, "server unavailable");

    std::println("");
    std::println("=== Callbacks complete ===");
    return 0;
}
