#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <print>

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
    std::vector<Observer*> observers;  // List of observers

public:
    virtual ~Subject() {}

    // Attach an observer
    void attach(Observer* observer) {
        observers.push_back(observer);
        std::println("✅ Observer attached (Total: {})", observers.size());
    }

    // Detach an observer
    void detach(Observer* observer) {
        auto it = find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
            std::println("❌ Observer detached (Total: {})", observers.size());
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
    std::println("📢 Notifying {} observers...", observers.size());
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
        std::println("");
        std::println("🌡️  Weather Station: New measurements received");
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
    std::string name;

public:
    CurrentConditionsDisplay(const std::string& n) : name(n), temperature(0), humidity(0) {}

    void update(Subject* subject) override {
        WeatherStation* station = dynamic_cast<WeatherStation*>(subject);
        if (station) {
            temperature = station->getTemperature();
            humidity = station->getHumidity();
            display();
        }
    }

    void display() {
        std::println("📺 [{}] Current conditions: {}°C and {}% humidity", name, temperature, humidity);
    }
};

/**
 * Concrete Observer: Statistics Display
 * Shows statistical data (avg, min, max)
 */
class StatisticsDisplay : public Observer {
private:
    std::vector<float> temperatureHistory;
    std::string name;

public:
    StatisticsDisplay(const std::string& n) : name(n) {}

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
        std::println("📊 [{}] Avg: {}°C, Min: {}°C, Max: {}°C", name, avg, minTemp, maxTemp);
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
    std::string name;

public:
    ForecastDisplay(const std::string& n) : name(n), lastPressure(0), currentPressure(0) {}

    void update(Subject* subject) override {
        WeatherStation* station = dynamic_cast<WeatherStation*>(subject);
        if (station) {
            lastPressure = currentPressure;
            currentPressure = station->getPressure();
            display();
        }
    }

    void display() {
        std::print("🔮 [{}] Forecast: ", name);
        if (currentPressure > lastPressure) {
            std::println("Improving weather on the way!");
        } else if (currentPressure < lastPressure) {
            std::println("Watch out for cooler, rainy weather");
        } else {
            std::println("More of the same");
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
    std::string symbol;
    double price;

public:
    Stock(const std::string& sym, double initialPrice)
        : symbol(sym), price(initialPrice) {}

    void setPrice(double newPrice) {
        if (newPrice != price) {
            std::println("");
            std::println("📈 Stock {} price changed: ${} → ${}", symbol, price, newPrice);
            price = newPrice;
            notify();
        }
    }

    std::string getSymbol() const { return symbol; }
    double getPrice() const { return price; }
};

/**
 * Concrete Observer: Investor
 * Monitors stock prices
 */
class Investor : public Observer {
private:
    std::string name;
    double buyThreshold;
    double sellThreshold;

public:
    Investor(const std::string& n, double buyAt, double sellAt)
        : name(n), buyThreshold(buyAt), sellThreshold(sellAt) {}

    void update(Subject* subject) override {
        Stock* stock = dynamic_cast<Stock*>(subject);
        if (stock) {
            double price = stock->getPrice();
            std::print("👤 [{}] notified: {} is now ${}", name, stock->getSymbol(), price);

            if (price <= buyThreshold) {
                std::println(" - 💰 BUY SIGNAL!");
            } else if (price >= sellThreshold) {
                std::println(" - 💸 SELL SIGNAL!");
            } else {
                std::println(" - ⏸️  HOLD");
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
    std::string location;

public:
    StockDisplayBoard(const std::string& loc) : location(loc) {}

    void update(Subject* subject) override {
        Stock* stock = dynamic_cast<Stock*>(subject);
        if (stock) {
            std::println("🖥️  [Display @ {}] {}: ${}", location, stock->getSymbol(), stock->getPrice());
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
    std::string latestNews;
    std::string category;

public:
    NewsAgency(const std::string& cat) : category(cat) {}

    void publishNews(const std::string& news) {
        std::println("");
        std::println("📰 [{}] Publishing: \"{}\"", category, news);
        latestNews = news;
        notify();
    }

    std::string getLatestNews() const { return latestNews; }
    std::string getCategory() const { return category; }
};

/**
 * Concrete Observer: News Subscriber
 * Receives and displays news
 */
class NewsSubscriber : public Observer {
private:
    std::string name;
    std::vector<std::string> newsHistory;

public:
    NewsSubscriber(const std::string& n) : name(n) {}

    void update(Subject* subject) override {
        NewsAgency* agency = dynamic_cast<NewsAgency*>(subject);
        if (agency) {
            std::string news = agency->getLatestNews();
            newsHistory.push_back(news);
            std::println("📧 [{}] received news: \"{}\"", name, news);
        }
    }

    void showHistory() {
        std::println("");
        std::println("📜 {}'s news history:", name);
        for (size_t i = 0; i < newsHistory.size(); i++) {
            std::println("   {}. {}", (i + 1), newsHistory[i]);
        }
    }
};

/**
 * Concrete Observer: Mobile News App
 * Sends push notifications
 */
class MobileNewsApp : public Observer {
private:
    std::string deviceId;
    int unreadCount;

public:
    MobileNewsApp(const std::string& id) : deviceId(id), unreadCount(0) {}

    void update(Subject* subject) override {
        NewsAgency* agency = dynamic_cast<NewsAgency*>(subject);
        if (agency) {
            unreadCount++;
            std::println("📱 [Mobile {}] PUSH NOTIFICATION! ({} unread)", deviceId, unreadCount);
        }
    }

    void readNews() {
        unreadCount = 0;
        std::println("✅ [Mobile {}] All news read", deviceId);
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
    std::string eventType;
    std::string message;
    int value;

    EventData(const std::string& type, const std::string& msg, int val = 0)
        : eventType(type), message(msg), value(val) {}
};

/**
 * Event Publisher
 * Generic event publishing system
 */
class EventPublisher {
private:
    std::vector<std::function<void(const EventData&)>> listeners;

public:
    void subscribe(std::function<void(const EventData&)> listener) {
        listeners.push_back(listener);
        std::println("✅ Event listener subscribed (Total: {})", listeners.size());
    }

    void publish(const EventData& event) {
        std::println("");
        std::println("🔔 Event published: [{}] {}", event.eventType, event.message);
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
    std::string label;
    EventPublisher* eventPublisher;

public:
    Button(const std::string& lbl, EventPublisher* pub)
        : label(lbl), eventPublisher(pub) {}

    void click() {
        std::println("");
        std::println("🖱️  Button \"{}\" clicked!", label);
        EventData event("BUTTON_CLICK", label);
        eventPublisher->publish(event);
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateWeatherStation() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: WEATHER STATION        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create subject
    WeatherStation* weatherStation = new WeatherStation();

    // Create observers
    CurrentConditionsDisplay* display1 = new CurrentConditionsDisplay("Living Room");
    StatisticsDisplay* display2 = new StatisticsDisplay("Office");
    ForecastDisplay* display3 = new ForecastDisplay("Kitchen");

    // Register observers
    std::println("--- Registering Observers ---");
    weatherStation->attach(display1);
    weatherStation->attach(display2);
    weatherStation->attach(display3);

    // Update weather data
    std::println("");
    std::println("--- First Measurement ---");
    weatherStation->setMeasurements(25.0, 65.0, 1013.0);

    std::println("");
    std::println("--- Second Measurement ---");
    weatherStation->setMeasurements(27.0, 70.0, 1015.0);

    std::println("");
    std::println("--- Third Measurement ---");
    weatherStation->setMeasurements(23.0, 75.0, 1010.0);

    // Remove one observer
    std::println("");
    std::println("--- Removing Office Display ---");
    weatherStation->detach(display2);

    std::println("");
    std::println("--- Fourth Measurement ---");
    weatherStation->setMeasurements(26.0, 68.0, 1012.0);

    // Cleanup
    delete display1;
    delete display2;
    delete display3;
    delete weatherStation;
}

void demonstrateStockMarket() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: STOCK MARKET           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create stock
    Stock* appleStock = new Stock("AAPL", 150.0);

    // Create observers
    Investor* investor1 = new Investor("Alice", 145.0, 160.0);
    Investor* investor2 = new Investor("Bob", 140.0, 155.0);
    StockDisplayBoard* display1 = new StockDisplayBoard("Wall Street");
    StockDisplayBoard* display2 = new StockDisplayBoard("Times Square");

    // Register observers
    std::println("--- Registering Observers ---");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: NEWS AGENCY            ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create news agency
    NewsAgency* techNews = new NewsAgency("Tech News");

    // Create subscribers
    NewsSubscriber* subscriber1 = new NewsSubscriber("John");
    NewsSubscriber* subscriber2 = new NewsSubscriber("Sarah");
    MobileNewsApp* app1 = new MobileNewsApp("iPhone-123");
    MobileNewsApp* app2 = new MobileNewsApp("Android-456");

    // Register subscribers
    std::println("--- Registering Subscribers ---");
    techNews->attach(subscriber1);
    techNews->attach(subscriber2);
    techNews->attach(app1);
    techNews->attach(app2);

    // Publish news
    techNews->publishNews("New smartphone released with amazing features!");
    techNews->publishNews("AI breakthrough announced by research team");

    // Sarah unsubscribes
    std::println("");
    std::println("--- Sarah Unsubscribing ---");
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: EVENT SYSTEM           ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create event publisher
    EventPublisher* eventPub = new EventPublisher();

    // Register event listeners using lambdas
    std::println("--- Registering Event Listeners ---");

    // Logger listener
    eventPub->subscribe([](const EventData& event) {
        std::println("📝 [Logger] Event logged: {} - {}", event.eventType, event.message);
    });

    // Analytics listener
    eventPub->subscribe([](const EventData& event) {
        std::println("📊 [Analytics] Tracking event: {}", event.eventType);
    });

    // Notification listener
    eventPub->subscribe([](const EventData& event) {
        std::println("🔔 [Notification] User action detected: {}", event.message);
    });

    // Create buttons
    Button* submitBtn = new Button("Submit", eventPub);
    Button* cancelBtn = new Button("Cancel", eventPub);
    Button* saveBtn = new Button("Save", eventPub);

    // Simulate button clicks
    std::println("");
    std::println("--- User Interactions ---");
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
    std::println("========================================");
    std::println("OBSERVER PATTERN DEMO");
    std::println("========================================");

    demonstrateWeatherStation();
    demonstrateStockMarket();
    demonstrateNewsAgency();
    demonstrateEventSystem();

    return 0;
}
