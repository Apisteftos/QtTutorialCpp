#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <functional>
using namespace std;

// ============================================
// EXAMPLE 1: WEATHER STATION
// Classic observer pattern example
// ============================================

// Forward declaration
class Observer;

/**
 * Subject Interface (Observable)
 * Maintains list of observers and notifies them of changes
 */
class Subject {
private:
    vector<Observer*> observers;  // List of observers

public:
    virtual ~Subject() {}

    // Attach an observer
    void attach(Observer* observer) {
        observers.push_back(observer);
        cout << "✅ Observer attached (Total: " << observers.size() << ")\n";
    }

    // Detach an observer
    void detach(Observer* observer) {
        auto it = find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
            cout << "❌ Observer detached (Total: " << observers.size() << ")\n";
        }
    }

    // Notify all observers
    void notify();
};

/**
 * Observer Interface
 * Defines update interface for objects that should be notified
 */
class Observer {
public:
    virtual ~Observer() {}
    virtual void update(Subject* subject) = 0;
};


void Subject::notify() {
    cout << "📢 Notifying " << observers.size() << " observers...\n";
    for (Observer* observer : observers) {
        observer->update(this);
    }
}



/**
 * Concrete Subject: Weather Station
 * Maintains weather data and notifies observers when it changes
 */
class WeatherStation : public Subject {
private:
    float temperature;
    float humidity;
    float pressure;

public:
    WeatherStation() : temperature(0), humidity(0), pressure(0) {}

    // Setters that trigger notification
    void setMeasurements(float temp, float hum, float press) {
        cout << "\n🌡️  Weather Station: New measurements received\n";
        temperature = temp;
        humidity = hum;
        pressure = press;
        notify();  // Notify all observers
    }

    // Getters for observers to pull data
    float getTemperature() const { return temperature; }
    float getHumidity() const { return humidity; }
    float getPressure() const { return pressure; }
};

/**
 * Concrete Observer: Current Conditions Display
 * Shows current weather conditions
 */
class CurrentConditionsDisplay : public Observer {
private:
    float temperature;
    float humidity;
    string name;

public:
    CurrentConditionsDisplay(const string& n) : name(n), temperature(0), humidity(0) {}

    void update(Subject* subject) override {
        WeatherStation* station = dynamic_cast<WeatherStation*>(subject);
        if (station) {
            temperature = station->getTemperature();
            humidity = station->getHumidity();
            display();
        }
    }

    void display() {
        cout << "📺 [" << name << "] Current conditions: "
             << temperature << "°C and " << humidity << "% humidity\n";
    }
};

/**
 * Concrete Observer: Statistics Display
 * Shows statistical data (avg, min, max)
 */
class StatisticsDisplay : public Observer {
private:
    vector<float> temperatureHistory;
    string name;

public:
    StatisticsDisplay(const string& n) : name(n) {}

    void update(Subject* subject) override {
        WeatherStation* station = dynamic_cast<WeatherStation*>(subject);
        if (station) {
            temperatureHistory.push_back(station->getTemperature());
            display();
        }
    }

    void display() {
        if (temperatureHistory.empty()) return;

        float sum = 0;
        float minTemp = temperatureHistory[0];
        float maxTemp = temperatureHistory[0];

        for (float temp : temperatureHistory) {
            sum += temp;
            if (temp < minTemp) minTemp = temp;
            if (temp > maxTemp) maxTemp = temp;
        }

        float avg = sum / temperatureHistory.size();
        cout << "📊 [" << name << "] Avg: " << avg << "°C, Min: "
             << minTemp << "°C, Max: " << maxTemp << "°C\n";
    }
};

/**
 * Concrete Observer: Forecast Display
 * Predicts weather based on pressure changes
 */
class ForecastDisplay : public Observer {
private:
    float lastPressure;
    float currentPressure;
    string name;

public:
    ForecastDisplay(const string& n) : name(n), lastPressure(0), currentPressure(0) {}

    void update(Subject* subject) override {
        WeatherStation* station = dynamic_cast<WeatherStation*>(subject);
        if (station) {
            lastPressure = currentPressure;
            currentPressure = station->getPressure();
            display();
        }
    }

    void display() {
        cout << "🔮 [" << name << "] Forecast: ";
        if (currentPressure > lastPressure) {
            cout << "Improving weather on the way!\n";
        } else if (currentPressure < lastPressure) {
            cout << "Watch out for cooler, rainy weather\n";
        } else {
            cout << "More of the same\n";
        }
    }
};

// ============================================
// EXAMPLE 2: STOCK MARKET
// Stock price monitoring system
// ============================================

/**
 * Concrete Subject: Stock
 * Represents a stock whose price can change
 */
class Stock : public Subject {
private:
    string symbol;
    double price;

public:
    Stock(const string& sym, double initialPrice)
        : symbol(sym), price(initialPrice) {}

    void setPrice(double newPrice) {
        if (newPrice != price) {
            cout << "\n📈 Stock " << symbol << " price changed: $"
                 << price << " → $" << newPrice << "\n";
            price = newPrice;
            notify();
        }
    }

    string getSymbol() const { return symbol; }
    double getPrice() const { return price; }
};

/**
 * Concrete Observer: Investor
 * Monitors stock prices
 */
class Investor : public Observer {
private:
    string name;
    double buyThreshold;
    double sellThreshold;

public:
    Investor(const string& n, double buyAt, double sellAt)
        : name(n), buyThreshold(buyAt), sellThreshold(sellAt) {}

    void update(Subject* subject) override {
        Stock* stock = dynamic_cast<Stock*>(subject);
        if (stock) {
            double price = stock->getPrice();
            cout << "👤 [" << name << "] notified: " << stock->getSymbol()
                 << " is now $" << price;

            if (price <= buyThreshold) {
                cout << " - 💰 BUY SIGNAL!\n";
            } else if (price >= sellThreshold) {
                cout << " - 💸 SELL SIGNAL!\n";
            } else {
                cout << " - ⏸️  HOLD\n";
            }
        }
    }
};

/**
 * Concrete Observer: Stock Display Board
 * Shows stock information on display
 */
class StockDisplayBoard : public Observer {
private:
    string location;

public:
    StockDisplayBoard(const string& loc) : location(loc) {}

    void update(Subject* subject) override {
        Stock* stock = dynamic_cast<Stock*>(subject);
        if (stock) {
            cout << "🖥️  [Display @ " << location << "] "
                 << stock->getSymbol() << ": $" << stock->getPrice() << "\n";
        }
    }
};

// ============================================
// EXAMPLE 3: NEWS AGENCY & SUBSCRIBERS
// Push-based notification system
// ============================================

/**
 * Concrete Subject: News Agency
 * Publishes news to subscribers
 */
class NewsAgency : public Subject {
private:
    string latestNews;
    string category;

public:
    NewsAgency(const string& cat) : category(cat) {}

    void publishNews(const string& news) {
        cout << "\n📰 [" << category << "] Publishing: \"" << news << "\"\n";
        latestNews = news;
        notify();
    }

    string getLatestNews() const { return latestNews; }
    string getCategory() const { return category; }
};

/**
 * Concrete Observer: News Subscriber
 * Receives and displays news
 */
class NewsSubscriber : public Observer {
private:
    string name;
    vector<string> newsHistory;

public:
    NewsSubscriber(const string& n) : name(n) {}

    void update(Subject* subject) override {
        NewsAgency* agency = dynamic_cast<NewsAgency*>(subject);
        if (agency) {
            string news = agency->getLatestNews();
            newsHistory.push_back(news);
            cout << "📧 [" << name << "] received news: \"" << news << "\"\n";
        }
    }

    void showHistory() {
        cout << "\n📜 " << name << "'s news history:\n";
        for (size_t i = 0; i < newsHistory.size(); i++) {
            cout << "   " << (i + 1) << ". " << newsHistory[i] << "\n";
        }
    }
};

/**
 * Concrete Observer: Mobile News App
 * Sends push notifications
 */
class MobileNewsApp : public Observer {
private:
    string deviceId;
    int unreadCount;

public:
    MobileNewsApp(const string& id) : deviceId(id), unreadCount(0) {}

    void update(Subject* subject) override {
        NewsAgency* agency = dynamic_cast<NewsAgency*>(subject);
        if (agency) {
            unreadCount++;
            cout << "📱 [Mobile " << deviceId << "] PUSH NOTIFICATION! "
                 << "(" << unreadCount << " unread)\n";
        }
    }

    void readNews() {
        unreadCount = 0;
        cout << "✅ [Mobile " << deviceId << "] All news read\n";
    }
};

// ============================================
// EXAMPLE 4: EVENT SYSTEM / BUTTON CLICKS
// GUI-style event handling
// ============================================

/**
 * Event Data Class
 * Contains information about the event
 */
class EventData {
public:
    string eventType;
    string message;
    int value;

    EventData(const string& type, const string& msg, int val = 0)
        : eventType(type), message(msg), value(val) {}
};

/**
 * Event Publisher
 * Generic event publishing system
 */
class EventPublisher {
private:
    vector<function<void(const EventData&)>> listeners;

public:
    void subscribe(function<void(const EventData&)> listener) {
        listeners.push_back(listener);
        cout << "✅ Event listener subscribed (Total: " << listeners.size() << ")\n";
    }

    void publish(const EventData& event) {
        cout << "\n🔔 Event published: [" << event.eventType << "] "
             << event.message << "\n";
        for (auto& listener : listeners) {
            listener(event);
        }
    }
};

/**
 * Concrete Button Class
 * Represents a clickable button
 */
class Button {
private:
    string label;
    EventPublisher* eventPublisher;

public:
    Button(const string& lbl, EventPublisher* pub)
        : label(lbl), eventPublisher(pub) {}

    void click() {
        cout << "\n🖱️  Button \"" << label << "\" clicked!\n";
        EventData event("BUTTON_CLICK", label);
        eventPublisher->publish(event);
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateWeatherStation() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: WEATHER STATION        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create subject
    WeatherStation* weatherStation = new WeatherStation();

    // Create observers
    CurrentConditionsDisplay* display1 = new CurrentConditionsDisplay("Living Room");
    StatisticsDisplay* display2 = new StatisticsDisplay("Office");
    ForecastDisplay* display3 = new ForecastDisplay("Kitchen");

    // Register observers
    cout << "--- Registering Observers ---\n";
    weatherStation->attach(display1);
    weatherStation->attach(display2);
    weatherStation->attach(display3);

    // Update weather data
    cout << "\n--- First Measurement ---\n";
    weatherStation->setMeasurements(25.0, 65.0, 1013.0);

    cout << "\n--- Second Measurement ---\n";
    weatherStation->setMeasurements(27.0, 70.0, 1015.0);

    cout << "\n--- Third Measurement ---\n";
    weatherStation->setMeasurements(23.0, 75.0, 1010.0);

    // Remove one observer
    cout << "\n--- Removing Office Display ---\n";
    weatherStation->detach(display2);

    cout << "\n--- Fourth Measurement ---\n";
    weatherStation->setMeasurements(26.0, 68.0, 1012.0);

    // Cleanup
    delete display1;
    delete display2;
    delete display3;
    delete weatherStation;
}

void demonstrateStockMarket() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: STOCK MARKET           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create stock
    Stock* appleStock = new Stock("AAPL", 150.0);

    // Create observers
    Investor* investor1 = new Investor("Alice", 145.0, 160.0);
    Investor* investor2 = new Investor("Bob", 140.0, 155.0);
    StockDisplayBoard* display1 = new StockDisplayBoard("Wall Street");
    StockDisplayBoard* display2 = new StockDisplayBoard("Times Square");

    // Register observers
    cout << "--- Registering Observers ---\n";
    appleStock->attach(investor1);
    appleStock->attach(investor2);
    appleStock->attach(display1);
    appleStock->attach(display2);

    // Stock price changes
    appleStock->setPrice(148.0);
    appleStock->setPrice(144.0);  // Below Alice's buy threshold
    appleStock->setPrice(161.0);  // Above Alice's sell threshold

    // Cleanup
    delete investor1;
    delete investor2;
    delete display1;
    delete display2;
    delete appleStock;
}

void demonstrateNewsAgency() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: NEWS AGENCY            ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create news agency
    NewsAgency* techNews = new NewsAgency("Tech News");

    // Create subscribers
    NewsSubscriber* subscriber1 = new NewsSubscriber("John");
    NewsSubscriber* subscriber2 = new NewsSubscriber("Sarah");
    MobileNewsApp* app1 = new MobileNewsApp("iPhone-123");
    MobileNewsApp* app2 = new MobileNewsApp("Android-456");

    // Register subscribers
    cout << "--- Registering Subscribers ---\n";
    techNews->attach(subscriber1);
    techNews->attach(subscriber2);
    techNews->attach(app1);
    techNews->attach(app2);

    // Publish news
    techNews->publishNews("New smartphone released with amazing features!");
    techNews->publishNews("AI breakthrough announced by research team");

    // Sarah unsubscribes
    cout << "\n--- Sarah Unsubscribing ---\n";
    techNews->detach(subscriber2);

    techNews->publishNews("Electric car sales reach new record");

    // Show history
    subscriber1->showHistory();

    // Cleanup
    delete subscriber1;
    delete subscriber2;
    delete app1;
    delete app2;
    delete techNews;
}

void demonstrateEventSystem() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: EVENT SYSTEM           ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create event publisher
    EventPublisher* eventPub = new EventPublisher();

    // Register event listeners using lambdas
    cout << "--- Registering Event Listeners ---\n";

    // Logger listener
    eventPub->subscribe([](const EventData& event) {
        cout << "📝 [Logger] Event logged: " << event.eventType
             << " - " << event.message << "\n";
    });

    // Analytics listener
    eventPub->subscribe([](const EventData& event) {
        cout << "📊 [Analytics] Tracking event: " << event.eventType << "\n";
    });

    // Notification listener
    eventPub->subscribe([](const EventData& event) {
        cout << "🔔 [Notification] User action detected: " << event.message << "\n";
    });

    // Create buttons
    Button* submitBtn = new Button("Submit", eventPub);
    Button* cancelBtn = new Button("Cancel", eventPub);
    Button* saveBtn = new Button("Save", eventPub);

    // Simulate button clicks
    cout << "\n--- User Interactions ---\n";
    submitBtn->click();
    cancelBtn->click();
    saveBtn->click();

    // Cleanup
    delete submitBtn;
    delete cancelBtn;
    delete saveBtn;
    delete eventPub;
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "OBSERVER PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateWeatherStation();
    demonstrateStockMarket();
    demonstrateNewsAgency();
    demonstrateEventSystem();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF OBSERVER PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Loose Coupling: Subject and observers are loosely coupled\n";
    cout << "✅ Open/Closed: Add new observers without modifying subject\n";
    cout << "✅ Broadcast Communication: One-to-many dependency\n";
    cout << "✅ Dynamic Relationships: Observers can subscribe/unsubscribe\n";
    cout << "✅ Reusable Components: Subjects and observers can be reused\n";
    cout << "✅ Event-Driven: Enables reactive programming patterns\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE OBSERVER\n";
    cout << "========================================\n";
    cout << "1. Change to one object requires changing others\n";
    cout << "2. Don't know how many objects need to be changed\n";
    cout << "3. Object should notify others without assumptions\n";
    cout << "4. Need event handling or publish-subscribe system\n";
    cout << "5. State changes need to trigger multiple updates\n";
    cout << "6. Want to decouple senders from receivers\n";

    cout << "\n========================================\n";
    cout << "OBSERVER vs MEDIATOR\n";
    cout << "========================================\n";
    cout << "Observer Pattern:\n";
    cout << "  • One-way communication (subject → observers)\n";
    cout << "  • Subject doesn't know about observer details\n";
    cout << "  • Observers just receive notifications\n";
    cout << "  • Example: Newsletter notifies subscribers\n";
    cout << "\nMediator Pattern:\n";
    cout << "  • Two-way communication\n";
    cout << "  • Mediator knows all participants\n";
    cout << "  • Coordinates complex interactions\n";
    cout << "  • Example: Chat room coordinates messages\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• GUI event handling (button clicks, mouse events)\n";
    cout << "• News/RSS feed subscriptions\n";
    cout << "• Stock market price monitoring\n";
    cout << "• Weather monitoring stations\n";
    cout << "• Social media notifications (likes, comments)\n";
    cout << "• Email distribution lists\n";
    cout << "• Model-View-Controller (MVC) architecture\n";
    cout << "• Real-time data dashboards\n";
    cout << "• Auction bidding systems\n";
    cout << "• IoT sensor networks\n";
    cout << "• Push notifications in mobile apps\n";
    cout << "• Chat applications\n";

    cout << "\n========================================\n";
    cout << "KEY COMPONENTS\n";
    cout << "========================================\n";
    cout << "1. Subject: Maintains list of observers, notifies them\n";
    cout << "2. Observer: Defines update interface\n";
    cout << "3. Concrete Subject: Stores state, sends notifications\n";
    cout << "4. Concrete Observer: Implements update to stay consistent\n";

    cout << "\n========================================\n";
    cout << "PUSH vs PULL MODEL\n";
    cout << "========================================\n";
    cout << "Push Model:\n";
    cout << "  • Subject sends detailed data to observers\n";
    cout << "  • update(subject, data)\n";
    cout << "  • More efficient if all data is needed\n";
    cout << "  • Less flexible - observers get all data\n";
    cout << "\nPull Model:\n";
    cout << "  • Subject sends minimal notification\n";
    cout << "  • Observers query subject for needed data\n";
    cout << "  • update(subject) then observer calls getters\n";
    cout << "  • More flexible - observers get what they need\n";
    cout << "  • Used in examples above\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "┌──────────────┐\n";
    cout << "│   Subject    │\n";
    cout << "│  (Observable)│\n";
    cout << "│              │\n";
    cout << "│ + attach()   │\n";
    cout << "│ + detach()   │───────┐\n";
    cout << "│ + notify()   │       │ notifies\n";
    cout << "└──────────────┘       │\n";
    cout << "       △                ▼\n";
    cout << "       │         ┌──────────────┐\n";
    cout << "       │         │   Observer   │\n";
    cout << "       │         │              │\n";
    cout << "┌──────────────┐ │ + update()   │\n";
    cout << "│   Concrete   │ └──────────────┘\n";
    cout << "│   Subject    │        △\n";
    cout << "│              │        │\n";
    cout << "│ - state      │ ┌──────────────┐\n";
    cout << "│ + getState() │ │   Concrete   │\n";
    cout << "│ + setState() │ │   Observer   │\n";
    cout << "└──────────────┘ └──────────────┘\n";

    cout << "\n========================================\n";
    cout << "ADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Supports broadcast communication\n";
    cout << "• Loose coupling between subject and observers\n";
    cout << "• Dynamic relationships (runtime subscription)\n";
    cout << "• Subject doesn't need to know observer details\n";
    cout << "• Observers can be added/removed independently\n";
    cout << "• Follows Open/Closed Principle\n";
    cout << "• Reusable subjects and observers\n";

    cout << "\n========================================\n";
    cout << "DISADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Unexpected updates (observers don't know about each other)\n";
    cout << "• Can cause memory leaks if not detached properly\n";
    cout << "• Order of notification is not guaranteed\n";
    cout << "• Performance impact with many observers\n";
    cout << "• Can lead to update cascades (chain reactions)\n";
    cout << "• Difficult to debug (indirect relationships)\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "• Always detach observers in destructor\n";
    cout << "• Use weak pointers to prevent circular references\n";
    cout << "• Consider async notifications for heavy processing\n";
    cout << "• Document update order if it matters\n";
    cout << "• Limit number of observers for performance\n";
    cout << "• Handle exceptions in observer updates\n";
    cout << "• Consider filtering which observers to notify\n";

    cout << "\n========================================\n";
    cout << "IMPLEMENTATION VARIATIONS\n";
    cout << "========================================\n";
    cout << "Simple Subject-Observer:\n";
    cout << "  • Direct references between subject and observers\n";
    cout << "  • Manual attach/detach\n";
    cout << "  • Example: Weather station above\n";
    cout << "\nEvent System:\n";
    cout << "  • Centralized event publisher\n";
    cout << "  • Type-based subscriptions\n";
    cout << "  • Example: GUI events above\n";
    cout << "\nCallback/Delegate Pattern:\n";
    cout << "  • Function pointers or lambdas\n";
    cout << "  • More lightweight\n";
    cout << "  • Example: Modern C++ with std::function\n";
    cout << "\nReactive Extensions (Rx):\n";
    cout << "  • Observable streams\n";
    cout << "  • Functional operations (map, filter)\n";
    cout << "  • Advanced reactive programming\n";

    cout << "\n========================================\n";
    cout << "COMMON ISSUES & SOLUTIONS\n";
    cout << "========================================\n";
    cout << "Memory Leaks:\n";
    cout << "  • Issue: Observers not detached before deletion\n";
    cout << "  • Solution: Use smart pointers, RAII, auto-detach\n";
    cout << "\nUpdate Storms:\n";
    cout << "  • Issue: Too many rapid notifications\n";
    cout << "  • Solution: Batch updates, throttling, debouncing\n";
    cout << "\nCircular Dependencies:\n";
    cout << "  • Issue: Observer modifies subject during update\n";
    cout << "  • Solution: Use flags, defer updates, queue changes\n";
    cout << "\nPerformance:\n";
    cout << "  • Issue: Many observers slow down notifications\n";
    cout << "  • Solution: Async notifications, selective notify\n";

    cout << "\n========================================\n";
    cout << "RELATED PATTERNS\n";
    cout << "========================================\n";
    cout << "Mediator:\n";
    cout << "  • Observer distributes communication\n";
    cout << "  • Mediator centralizes communication\n";
    cout << "\nMVC (Model-View-Controller):\n";
    cout << "  • Model is Subject\n";
    cout << "  • Views are Observers\n";
    cout << "  • Uses Observer for model-view sync\n";
    cout << "\nPublish-Subscribe:\n";
    cout << "  • More decoupled than Observer\n";
    cout << "  • Uses message broker/event bus\n";
    cout << "  • Topic-based subscriptions\n";

    cout << "\n========================================\n";
    cout << "NOTIFICATION STRATEGIES\n";
    cout << "========================================\n";
    cout << "Immediate Notification:\n";
    cout << "  • Notify observers immediately after state change\n";
    cout << "  • Simple and straightforward\n";
    cout << "  • Can cause update storms\n";
    cout << "\nDeferred Notification:\n";
    cout << "  • Collect changes and notify once\n";
    cout << "  • More efficient for multiple changes\n";
    cout << "  • Requires explicit commit/flush\n";
    cout << "\nAsynchronous Notification:\n";
    cout << "  • Notify observers on separate thread\n";
    cout << "  • Non-blocking for subject\n";
    cout << "  • Requires thread synchronization\n";
    cout << "\nSelective Notification:\n";
    cout << "  • Only notify relevant observers\n";
    cout << "  • Filter by event type or criteria\n";
    cout << "  • More efficient for targeted updates\n";

    cout << "\n========================================\n";
    cout << "MODERN C++ IMPLEMENTATIONS\n";
    cout << "========================================\n";
    cout << "Using std::function and lambdas:\n";
    cout << "  • More flexible than interface-based\n";
    cout << "  • No need for Observer base class\n";
    cout << "  • Example: Event system above\n";
    cout << "\nUsing signals/slots (like Qt):\n";
    cout << "  • Type-safe connections\n";
    cout << "  • Automatic disconnection\n";
    cout << "  • Meta-object system\n";
    cout << "\nUsing std::weak_ptr:\n";
    cout << "  • Prevents circular references\n";
    cout << "  • Auto-cleanup of dead observers\n";
    cout << "  • Safer memory management\n";

    return 0;
}
