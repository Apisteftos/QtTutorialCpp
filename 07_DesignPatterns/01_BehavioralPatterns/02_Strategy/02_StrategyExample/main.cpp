#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

// ============================================
// EXAMPLE 1: PAYMENT PROCESSING
// Classic strategy pattern example
// ============================================

/**
 * Strategy Interface
 * Defines the interface for all payment algorithms
 */
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() {}
    virtual void pay(double amount) = 0;
    virtual string getPaymentType() const = 0;
};

/**
 * Concrete Strategy: Credit Card Payment
 */
class CreditCardPayment : public PaymentStrategy {
private:
    string cardNumber;
    string cardHolder;
    string cvv;
    string expiryDate;

public:
    CreditCardPayment(const string& number, const string& holder,
                      const string& cv, const string& expiry)
        : cardNumber(number), cardHolder(holder), cvv(cv), expiryDate(expiry) {}

    void pay(double amount) override {
        cout << "💳 Processing Credit Card payment of $" << amount << "\n";
        cout << "   Card: ****" << cardNumber.substr(cardNumber.length() - 4) << "\n";
        cout << "   Holder: " << cardHolder << "\n";
        cout << "✅ Payment successful!\n";
    }

    string getPaymentType() const override {
        return "Credit Card";
    }
};

/**
 * Concrete Strategy: PayPal Payment
 */
class PayPalPayment : public PaymentStrategy {
private:
    string email;
    string password;

public:
    PayPalPayment(const string& mail, const string& pass)
        : email(mail), password(pass) {}

    void pay(double amount) override {
        cout << "💰 Processing PayPal payment of $" << amount << "\n";
        cout << "   Account: " << email << "\n";
        cout << "   Authenticating...\n";
        cout << "✅ Payment successful!\n";
    }

    string getPaymentType() const override {
        return "PayPal";
    }
};

/**
 * Concrete Strategy: Cryptocurrency Payment
 */
class CryptoPayment : public PaymentStrategy {
private:
    string walletAddress;
    string cryptoType;

public:
    CryptoPayment(const string& wallet, const string& crypto)
        : walletAddress(wallet), cryptoType(crypto) {}

    void pay(double amount) override {
        cout << "₿ Processing " << cryptoType << " payment of $" << amount << "\n";
        cout << "   Wallet: " << walletAddress.substr(0, 10) << "...\n";
        cout << "   Broadcasting to blockchain...\n";
        cout << "✅ Payment successful!\n";
    }

    string getPaymentType() const override {
        return cryptoType + " Cryptocurrency";
    }
};

/**
 * Context: Shopping Cart
 * Uses a payment strategy
 */
class ShoppingCart {
private:
    shared_ptr<PaymentStrategy> paymentStrategy;
    vector<pair<string, double>> items;

public:
    void addItem(const string& item, double price) {
        items.push_back({item, price});
        cout << "🛒 Added: " << item << " - $" << price << "\n";
    }

    double calculateTotal() const {
        double total = 0;
        for (const auto& item : items) {
            total += item.second;
        }
        return total;
    }

    void setPaymentStrategy(shared_ptr<PaymentStrategy> strategy) {
        paymentStrategy = strategy;
        cout << "💳 Payment method set to: " << strategy->getPaymentType() << "\n";
    }

    void checkout() {
        if (!paymentStrategy) {
            cout << "❌ Please select a payment method\n";
            return;
        }

        double total = calculateTotal();
        cout << "\n📋 Checkout Summary:\n";
        for (const auto& item : items) {
            cout << "   • " << item.first << ": $" << item.second << "\n";
        }
        cout << "   Total: $" << total << "\n\n";

        paymentStrategy->pay(total);
    }
};

// ============================================
// EXAMPLE 2: SORTING STRATEGIES
// Different sorting algorithms
// ============================================

/**
 * Sorting Strategy Interface
 */
class SortStrategy {
public:
    virtual ~SortStrategy() {}
    virtual void sort(vector<int>& data) = 0;
    virtual string getAlgorithmName() const = 0;
};

/**
 * Concrete Strategy: Bubble Sort
 */
class BubbleSort : public SortStrategy {
public:
    void sort(vector<int>& data) override {
        cout << "🔢 Using Bubble Sort...\n";
        int n = data.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (data[j] > data[j + 1]) {
                    swap(data[j], data[j + 1]);
                }
            }
        }
        cout << "✅ Bubble Sort complete\n";
    }

    string getAlgorithmName() const override {
        return "Bubble Sort (O(n²))";
    }
};

/**
 * Concrete Strategy: Quick Sort
 */
class QuickSort : public SortStrategy {
private:
    void quickSortHelper(vector<int>& data, int low, int high) {
        if (low < high) {
            int pivot = partition(data, low, high);
            quickSortHelper(data, low, pivot - 1);
            quickSortHelper(data, pivot + 1, high);
        }
    }

    int partition(vector<int>& data, int low, int high) {
        int pivot = data[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (data[j] < pivot) {
                i++;
                swap(data[i], data[j]);
            }
        }
        swap(data[i + 1], data[high]);
        return i + 1;
    }

public:
    void sort(vector<int>& data) override {
        cout << "⚡ Using Quick Sort...\n";
        quickSortHelper(data, 0, data.size() - 1);
        cout << "✅ Quick Sort complete\n";
    }

    string getAlgorithmName() const override {
        return "Quick Sort (O(n log n))";
    }
};

/**
 * Concrete Strategy: Merge Sort
 */
class MergeSort : public SortStrategy {
private:
    void merge(vector<int>& data, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        vector<int> L(n1), R(n2);
        for (int i = 0; i < n1; i++) L[i] = data[left + i];
        for (int j = 0; j < n2; j++) R[j] = data[mid + 1 + j];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                data[k++] = L[i++];
            } else {
                data[k++] = R[j++];
            }
        }
        while (i < n1) data[k++] = L[i++];
        while (j < n2) data[k++] = R[j++];
    }

    void mergeSortHelper(vector<int>& data, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSortHelper(data, left, mid);
            mergeSortHelper(data, mid + 1, right);
            merge(data, left, mid, right);
        }
    }

public:
    void sort(vector<int>& data) override {
        cout << "🔀 Using Merge Sort...\n";
        mergeSortHelper(data, 0, data.size() - 1);
        cout << "✅ Merge Sort complete\n";
    }

    string getAlgorithmName() const override {
        return "Merge Sort (O(n log n))";
    }
};

/**
 * Context: Data Sorter
 */
class DataSorter {
private:
    shared_ptr<SortStrategy> strategy;

public:
    void setSortStrategy(shared_ptr<SortStrategy> strat) {
        strategy = strat;
        cout << "📊 Sort algorithm set to: " << strategy->getAlgorithmName() << "\n";
    }

    void sortData(vector<int>& data) {
        if (!strategy) {
            cout << "❌ No sorting strategy set\n";
            return;
        }

        cout << "Unsorted: ";
        printData(data);

        strategy->sort(data);

        cout << "Sorted:   ";
        printData(data);
    }

private:
    void printData(const vector<int>& data) {
        for (int val : data) {
            cout << val << " ";
        }
        cout << "\n";
    }
};

// ============================================
// EXAMPLE 3: COMPRESSION STRATEGIES
// Different compression algorithms
// ============================================

/**
 * Compression Strategy Interface
 */
class CompressionStrategy {
public:
    virtual ~CompressionStrategy() {}
    virtual string compress(const string& data) = 0;
    virtual string decompress(const string& data) = 0;
    virtual string getCompressionType() const = 0;
};

/**
 * Concrete Strategy: ZIP Compression
 */
class ZipCompression : public CompressionStrategy {
public:
    string compress(const string& data) override {
        cout << "🗜️  [ZIP] Compressing " << data.length() << " bytes...\n";
        // Simulated compression
        string compressed = "[ZIP:" + data + "]";
        cout << "   Compressed to " << compressed.length() << " bytes\n";
        return compressed;
    }

    string decompress(const string& data) override {
        cout << "📦 [ZIP] Decompressing...\n";
        // Simulated decompression
        return data.substr(5, data.length() - 6);
    }

    string getCompressionType() const override {
        return "ZIP";
    }
};

/**
 * Concrete Strategy: RAR Compression
 */
class RarCompression : public CompressionStrategy {
public:
    string compress(const string& data) override {
        cout << "🗜️  [RAR] Compressing " << data.length() << " bytes...\n";
        // Simulated compression with better ratio
        string compressed = "[RAR:" + data + "]";
        cout << "   Compressed to " << compressed.length() << " bytes (better ratio)\n";
        return compressed;
    }

    string decompress(const string& data) override {
        cout << "📦 [RAR] Decompressing...\n";
        return data.substr(5, data.length() - 6);
    }

    string getCompressionType() const override {
        return "RAR";
    }
};

/**
 * Concrete Strategy: 7z Compression
 */
class SevenZipCompression : public CompressionStrategy {
public:
    string compress(const string& data) override {
        cout << "🗜️  [7z] Compressing " << data.length() << " bytes...\n";
        // Simulated compression with best ratio
        string compressed = "[7z:" + data + "]";
        cout << "   Compressed to " << compressed.length() << " bytes (best ratio)\n";
        return compressed;
    }

    string decompress(const string& data) override {
        cout << "📦 [7z] Decompressing...\n";
        return data.substr(4, data.length() - 5);
    }

    string getCompressionType() const override {
        return "7-Zip";
    }
};

/**
 * Context: File Compressor
 */
class FileCompressor {
private:
    shared_ptr<CompressionStrategy> strategy;

public:
    void setCompressionStrategy(shared_ptr<CompressionStrategy> strat) {
        strategy = strat;
        cout << "📁 Compression method set to: " << strategy->getCompressionType() << "\n";
    }

    void compressFile(const string& filename, const string& data) {
        if (!strategy) {
            cout << "❌ No compression strategy set\n";
            return;
        }

        cout << "\n📄 Compressing file: " << filename << "\n";
        string compressed = strategy->compress(data);
        cout << "✅ File compressed successfully\n";
    }

    void decompressFile(const string& filename, const string& data) {
        if (!strategy) {
            cout << "❌ No compression strategy set\n";
            return;
        }

        cout << "\n📄 Decompressing file: " << filename << "\n";
        string decompressed = strategy->decompress(data);
        cout << "✅ File decompressed successfully\n";
    }
};

// ============================================
// EXAMPLE 4: NAVIGATION STRATEGIES
// Different route calculation methods
// ============================================

/**
 * Navigation Strategy Interface
 */
class NavigationStrategy {
public:
    virtual ~NavigationStrategy() {}
    virtual void buildRoute(const string& from, const string& to) = 0;
    virtual string getRouteType() const = 0;
};

/**
 * Concrete Strategy: Car Navigation
 */
class CarNavigation : public NavigationStrategy {
public:
    void buildRoute(const string& from, const string& to) override {
        cout << "🚗 Building car route from " << from << " to " << to << "\n";
        cout << "   • Taking highways\n";
        cout << "   • Avoiding toll roads\n";
        cout << "   • ETA: 45 minutes\n";
        cout << "   • Distance: 30 miles\n";
    }

    string getRouteType() const override {
        return "Car (Fastest Route)";
    }
};

/**
 * Concrete Strategy: Walking Navigation
 */
class WalkingNavigation : public NavigationStrategy {
public:
    void buildRoute(const string& from, const string& to) override {
        cout << "🚶 Building walking route from " << from << " to " << to << "\n";
        cout << "   • Using pedestrian paths\n";
        cout << "   • Avoiding busy streets\n";
        cout << "   • ETA: 2 hours\n";
        cout << "   • Distance: 5 miles\n";
    }

    string getRouteType() const override {
        return "Walking (Pedestrian Paths)";
    }
};

/**
 * Concrete Strategy: Public Transport Navigation
 */
class PublicTransportNavigation : public NavigationStrategy {
public:
    void buildRoute(const string& from, const string& to) override {
        cout << "🚌 Building public transport route from " << from << " to " << to << "\n";
        cout << "   • Bus #45 to Central Station\n";
        cout << "   • Transfer to Metro Line 2\n";
        cout << "   • ETA: 1 hour 15 minutes\n";
        cout << "   • Cost: $3.50\n";
    }

    string getRouteType() const override {
        return "Public Transport (Cheapest)";
    }
};

/**
 * Concrete Strategy: Bicycle Navigation
 */
class BicycleNavigation : public NavigationStrategy {
public:
    void buildRoute(const string& from, const string& to) override {
        cout << "🚴 Building bicycle route from " << from << " to " << to << "\n";
        cout << "   • Using bike lanes\n";
        cout << "   • Avoiding steep hills\n";
        cout << "   • ETA: 1 hour\n";
        cout << "   • Distance: 8 miles\n";
    }

    string getRouteType() const override {
        return "Bicycle (Bike Lanes)";
    }
};

/**
 * Context: Navigation App
 */
class Navigator {
private:
    shared_ptr<NavigationStrategy> strategy;

public:
    void setNavigationStrategy(shared_ptr<NavigationStrategy> strat) {
        strategy = strat;
        cout << "🗺️  Navigation mode set to: " << strategy->getRouteType() << "\n";
    }

    void navigate(const string& from, const string& to) {
        if (!strategy) {
            cout << "❌ Please select navigation mode\n";
            return;
        }

        cout << "\n📍 Navigation Request\n";
        strategy->buildRoute(from, to);
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstratePaymentStrategies() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: PAYMENT PROCESSING     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    ShoppingCart cart;

    // Add items
    cout << "--- Adding Items to Cart ---\n";
    cart.addItem("Laptop", 999.99);
    cart.addItem("Mouse", 29.99);
    cart.addItem("Keyboard", 79.99);

    // Pay with Credit Card
    cout << "\n--- Paying with Credit Card ---\n";
    auto creditCard = make_shared<CreditCardPayment>(
        "1234567890123456", "John Doe", "123", "12/25");
    cart.setPaymentStrategy(creditCard);
    cart.checkout();

    // Change to PayPal
    cout << "\n--- Changing to PayPal ---\n";
    auto paypal = make_shared<PayPalPayment>("john@example.com", "password");
    cart.setPaymentStrategy(paypal);

    // Change to Crypto
    cout << "\n--- Changing to Cryptocurrency ---\n";
    auto crypto = make_shared<CryptoPayment>("0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb", "Bitcoin");
    cart.setPaymentStrategy(crypto);
}

void demonstrateSortingStrategies() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: SORTING ALGORITHMS     ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    DataSorter sorter;

    // Test with different algorithms
    vector<int> data1 = {64, 34, 25, 12, 22, 11, 90};

    cout << "--- Using Bubble Sort ---\n";
    sorter.setSortStrategy(make_shared<BubbleSort>());
    sorter.sortData(data1);

    vector<int> data2 = {64, 34, 25, 12, 22, 11, 90};
    cout << "\n--- Using Quick Sort ---\n";
    sorter.setSortStrategy(make_shared<QuickSort>());
    sorter.sortData(data2);

    vector<int> data3 = {64, 34, 25, 12, 22, 11, 90};
    cout << "\n--- Using Merge Sort ---\n";
    sorter.setSortStrategy(make_shared<MergeSort>());
    sorter.sortData(data3);
}

void demonstrateCompressionStrategies() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: FILE COMPRESSION       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    FileCompressor compressor;
    string fileData = "This is sample file data that needs compression";

    // Try ZIP
    cout << "--- Using ZIP Compression ---\n";
    compressor.setCompressionStrategy(make_shared<ZipCompression>());
    compressor.compressFile("document.txt", fileData);

    // Try RAR
    cout << "\n--- Using RAR Compression ---\n";
    compressor.setCompressionStrategy(make_shared<RarCompression>());
    compressor.compressFile("document.txt", fileData);

    // Try 7-Zip
    cout << "\n--- Using 7-Zip Compression ---\n";
    compressor.setCompressionStrategy(make_shared<SevenZipCompression>());
    compressor.compressFile("document.txt", fileData);
}

void demonstrateNavigationStrategies() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: NAVIGATION ROUTES      ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Navigator nav;
    string from = "Home";
    string to = "Office";

    // Car navigation
    cout << "--- Car Navigation ---\n";
    nav.setNavigationStrategy(make_shared<CarNavigation>());
    nav.navigate(from, to);

    // Walking navigation
    cout << "\n--- Walking Navigation ---\n";
    nav.setNavigationStrategy(make_shared<WalkingNavigation>());
    nav.navigate(from, to);

    // Public transport
    cout << "\n--- Public Transport ---\n";
    nav.setNavigationStrategy(make_shared<PublicTransportNavigation>());
    nav.navigate(from, to);

    // Bicycle
    cout << "\n--- Bicycle Navigation ---\n";
    nav.setNavigationStrategy(make_shared<BicycleNavigation>());
    nav.navigate(from, to);
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "STRATEGY PATTERN DEMO\n";
    cout << "========================================\n";

    demonstratePaymentStrategies();
    demonstrateSortingStrategies();
    demonstrateCompressionStrategies();
    demonstrateNavigationStrategies();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF STRATEGY PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Open/Closed: Add new strategies without modifying context\n";
    cout << "✅ Single Responsibility: Isolates algorithm implementation\n";
    cout << "✅ Runtime flexibility: Switch algorithms at runtime\n";
    cout << "✅ Eliminates conditionals: No large if/switch statements\n";
    cout << "✅ Testability: Easy to test strategies independently\n";
    cout << "✅ Reusability: Strategies can be reused across contexts\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE STRATEGY\n";
    cout << "========================================\n";
    cout << "1. Need different variants of an algorithm\n";
    cout << "2. Want to switch algorithms at runtime\n";
    cout << "3. Have many related classes differing only in behavior\n";
    cout << "4. Need to isolate algorithm implementation details\n";
    cout << "5. Class has massive conditional statements\n";
    cout << "6. Want to provide multiple implementations of same behavior\n";

    cout << "\n========================================\n";
    cout << "STRATEGY vs STATE\n";
    cout << "========================================\n";
    cout << "Strategy Pattern:\n";
    cout << "  • Encapsulates interchangeable algorithms\n";
    cout << "  • Client chooses strategy explicitly\n";
    cout << "  • Strategies don't know about each other\n";
    cout << "  • Example: Different payment methods\n";
    cout << "\nState Pattern:\n";
    cout << "  • Encapsulates state-dependent behavior\n";
    cout << "  • States can transition to other states\n";
    cout << "  • Context behavior changes with state\n";
    cout << "  • Example: TCP connection states\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Payment gateways (credit card, PayPal, crypto)\n";
    cout << "• Sorting algorithms (bubble, quick, merge)\n";
    cout << "• Compression algorithms (ZIP, RAR, 7z)\n";
    cout << "• Navigation routes (car, walk, public transport)\n";
    cout << "• Validation strategies (email, phone, format)\n";
    cout << "• Pricing strategies (regular, discount, seasonal)\n";
    cout << "• Authentication methods (password, OAuth, biometric)\n";
    cout << "• Rendering engines (SVG, Canvas, WebGL)\n";
    cout << "• Search algorithms (linear, binary, hash-based)\n";
    cout << "• Notification delivery (email, SMS, push)\n";
    cout << "• Export formats (PDF, Excel, CSV)\n";
    cout << "• Encryption algorithms (AES, RSA, DES)\n";

    cout << "\n========================================\n";
    cout << "KEY COMPONENTS\n";
    cout << "========================================\n";
    cout << "1. Strategy Interface: Defines common interface\n";
    cout << "2. Concrete Strategies: Implement specific algorithms\n";
    cout << "3. Context: Uses a strategy, can switch at runtime\n";
    cout << "4. Client: Chooses appropriate strategy\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "┌─────────────────┐\n";
    cout << "│     Client      │\n";
    cout << "└────────┬────────┘\n";
    cout << "         │ configures\n";
    cout << "         ▼\n";
    cout << "┌─────────────────┐\n";
    cout << "│    Context      │\n";
    cout << "│  (ShoppingCart) │\n";
    cout << "│                 │\n";
    cout << "│ - strategy ─────┼─────┐ uses\n";
    cout << "│ + setStrategy() │     │\n";
    cout << "│ + executeAlg()  │     │\n";
    cout << "└─────────────────┘     │\n";
    cout << "                        ▼\n";
    cout << "                 ┌─────────────┐\n";
    cout << "                 │  Strategy   │\n";
    cout << "                 │ (Interface) │\n";
    cout << "                 │             │\n";
    cout << "                 │ + execute() │\n";
    cout << "                 └─────────────┘\n";
    cout << "                        △\n";
    cout << "        ┌───────────────┼───────────────┐\n";
    cout << "        │               │               │\n";
    cout << "┌─────────────┐ ┌─────────────┐ ┌─────────────┐\n";
    cout << "│ StrategyA   │ │ StrategyB   │ │ StrategyC   │\n";
    cout << "│ (CreditCard)│ │ (PayPal)    │ │ (Crypto)    │\n";
    cout << "│             │ │             │ │             │\n";
    cout << "│ + execute() │ │ + execute() │ │ + execute() │\n";
    cout << "└─────────────┘ └─────────────┘ └─────────────┘\n";

    cout << "\n========================================\n";
    cout << "ADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Algorithms are interchangeable\n";
    cout << "• Easy to add new strategies\n";
    cout << "• Eliminates conditional statements\n";
    cout << "• Runtime algorithm selection\n";
    cout << "• Isolates algorithm implementation\n";
    cout << "• Promotes composition over inheritance\n";
    cout << "• Follows Open/Closed Principle\n";
    cout << "• Each strategy can be tested independently\n";

    cout << "\n========================================\n";
    cout << "DISADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Increases number of objects/classes\n";
    cout << "• Clients must be aware of different strategies\n";
    cout << "• Communication overhead between context and strategy\n";
    cout << "• May be overkill for simple variations\n";
    cout << "• Context may pass data strategy doesn't need\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "• Keep strategy interface focused and cohesive\n";
    cout << "• Use dependency injection for strategies\n";
    cout << "• Consider default strategy in context\n";
    cout << "• Document when each strategy should be used\n";
    cout << "• Make strategies stateless when possible\n";
    cout << "• Use factory pattern to create strategies\n";
    cout << "• Consider strategy combination/composition\n";
    cout << "• Profile performance of different strategies\n";

    cout << "\n========================================\n";
    cout << "IMPLEMENTATION VARIATIONS\n";
    cout << "========================================\n";
    cout << "Basic Strategy:\n";
    cout << "  • Context has reference to strategy\n";
    cout << "  • Strategy set via setter method\n";
    cout << "  • Example: All examples above\n";
    cout << "\nFunction Pointers/Lambdas:\n";
    cout << "  • Use std::function instead of interface\n";
    cout << "  • More lightweight, less boilerplate\n";
    cout << "  • Good for simple strategies\n";
    cout << "\nTemplate-Based:\n";
    cout << "  • Strategy as template parameter\n";
    cout << "  • Compile-time binding\n";
    cout << "  • Better performance, no virtual calls\n";
    cout << "\nStrategy with Parameters:\n";
    cout << "  • Pass context data to strategy\n";
    cout << "  • Strategy accesses context methods\n";
    cout << "  • More flexible data exchange\n";

    cout << "\n========================================\n";
    cout << "STRATEGY SELECTION APPROACHES\n";
    cout << "========================================\n";
    cout << "Client Selection:\n";
    cout << "  • Client explicitly chooses strategy\n";
    cout << "  • Most common approach\n";
    cout << "  • Example: User selects payment method\n";
    cout << "\nAutomatic Selection:\n";
    cout << "  • Context chooses based on conditions\n";
    cout << "  • Rules-based selection\n";
    cout << "  • Example: Choose sort based on data size\n";
    cout << "\nFactory Pattern:\n";
    cout << "  • Factory creates appropriate strategy\n";
    cout << "  • Centralized strategy creation\n";
    cout << "  • Example: Strategy factory based on config\n";
    cout << "\nConfiguration-Based:\n";
    cout << "  • Strategy loaded from config file\n";
    cout << "  • Runtime configuration\n";
    cout << "  • Example: Plugin-based architectures\n";

    cout << "\n========================================\n";
    cout << "COMMON ISSUES & SOLUTIONS\n";
    cout << "========================================\n";
    cout << "Too Many Strategies:\n";
    cout << "  • Issue: Overwhelming number of strategy classes\n";
    cout << "  • Solution: Group related strategies, use composition\n";
    cout << "\nStrategy Coupling:\n";
    cout << "  • Issue: Strategies depend on each other\n";
    cout << "  • Solution: Keep strategies independent\n";
    cout << "\nData Passing:\n";
    cout << "  • Issue: Context passes unnecessary data\n";
    cout << "  • Solution: Use specific interfaces, callbacks\n";
    cout << "\nPerformance:\n";
    cout << "  • Issue: Virtual function overhead\n";
    cout << "  • Solution: Use template-based strategies\n";

    cout << "\n========================================\n";
    cout << "RELATED PATTERNS\n";
    cout << "========================================\n";
    cout << "State:\n";
    cout << "  • Strategy focuses on algorithms\n";
    cout << "  • State focuses on state-dependent behavior\n";
    cout << "\nTemplate Method:\n";
    cout << "  • Strategy uses composition\n";
    cout << "  • Template Method uses inheritance\n";
    cout << "  • Strategy is more flexible\n";
    cout << "\nFactory:\n";
    cout << "  • Often used to create strategies\n";
    cout << "  • Centralizes strategy instantiation\n";
    cout << "\nDecorator:\n";
    cout << "  • Decorator adds responsibilities\n";
    cout << "  • Strategy replaces entire algorithm\n";

    cout << "\n========================================\n";
    cout << "MODERN C++ IMPLEMENTATIONS\n";
    cout << "========================================\n";
    cout << "Using std::function:\n";
    cout << "  • No need for interface hierarchy\n";
    cout << "  • Can use lambdas or function pointers\n";
    cout << "  • Example: std::function<void(int)> strategy\n";
    cout << "\nUsing Templates:\n";
    cout << "  • Compile-time polymorphism\n";
    cout << "  • Better performance\n";
    cout << "  • Example: template<typename Strategy> class Context\n";
    cout << "\nUsing Variants:\n";
    cout << "  • std::variant for type-safe strategies\n";
    cout << "  • No virtual calls\n";
    cout << "  • Example: std::variant<StrategyA, StrategyB>\n";

    cout << "\n========================================\n";
    cout << "TESTING STRATEGIES\n";
    cout << "========================================\n";
    cout << "Unit Tests:\n";
    cout << "  • Test each strategy independently\n";
    cout << "  • Mock context if needed\n";
    cout << "  • Verify algorithm correctness\n";
    cout << "\nIntegration Tests:\n";
    cout << "  • Test context with different strategies\n";
    cout << "  • Verify strategy switching\n";
    cout << "  • Test with real data\n";
    cout << "\nPerformance Tests:\n";
    cout << "  • Benchmark different strategies\n";
    cout << "  • Compare execution times\n";
    cout << "  • Profile memory usage\n";

    cout << "\n========================================\n";
    cout << "USE CASES BY DOMAIN\n";
    cout << "========================================\n";
    cout << "E-commerce:\n";
    cout << "  • Payment methods\n";
    cout << "  • Shipping calculators\n";
    cout << "  • Discount strategies\n";
    cout << "  • Tax calculation\n";
    cout << "\nData Processing:\n";
    cout << "  • Sorting algorithms\n";
    cout << "  • Search strategies\n";
    cout << "  • Validation rules\n";
    cout << "  • Export formats\n";
    cout << "\nGaming:\n";
    cout << "  • AI behaviors\n";
    cout << "  • Movement strategies\n";
    cout << "  • Combat tactics\n";
    cout << "  • Pathfinding algorithms\n";
    cout << "\nUI/UX:\n";
    cout << "  • Layout strategies\n";
    cout << "  • Rendering methods\n";
    cout << "  • Animation timing\n";
    cout << "  • Theme variations\n";

    return 0;
}
