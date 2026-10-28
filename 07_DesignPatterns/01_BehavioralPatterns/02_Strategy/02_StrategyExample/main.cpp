#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <print>

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
    virtual std::string getPaymentType() const = 0;
};

/**
 * Concrete Strategy: Credit Card Payment
 */
class CreditCardPayment : public PaymentStrategy {
private:
    std::string cardNumber;
    std::string cardHolder;
    std::string cvv;
    std::string expiryDate;

public:
    CreditCardPayment(const std::string& number, const std::string& holder,
                      const std::string& cv, const std::string& expiry)
        : cardNumber(number), cardHolder(holder), cvv(cv), expiryDate(expiry) {}

    void pay(double amount) override {
        std::println("💳 Processing Credit Card payment of ${}", amount);
        std::println("   Card: ****{}", cardNumber.substr(cardNumber.length() - 4));
        std::println("   Holder: {}", cardHolder);
        std::println("✅ Payment successful!");
    }

    std::string getPaymentType() const override {
        return "Credit Card";
    }
};

/**
 * Concrete Strategy: PayPal Payment
 */
class PayPalPayment : public PaymentStrategy {
private:
    std::string email;
    std::string password;

public:
    PayPalPayment(const std::string& mail, const std::string& pass)
        : email(mail), password(pass) {}

    void pay(double amount) override {
        std::println("💰 Processing PayPal payment of ${}", amount);
        std::println("   Account: {}", email);
        std::println("   Authenticating...");
        std::println("✅ Payment successful!");
    }

    std::string getPaymentType() const override {
        return "PayPal";
    }
};

/**
 * Concrete Strategy: Cryptocurrency Payment
 */
class CryptoPayment : public PaymentStrategy {
private:
    std::string walletAddress;
    std::string cryptoType;

public:
    CryptoPayment(const std::string& wallet, const std::string& crypto)
        : walletAddress(wallet), cryptoType(crypto) {}

    void pay(double amount) override {
        std::println("₿ Processing {} payment of ${}", cryptoType, amount);
        std::println("   Wallet: {}...", walletAddress.substr(0, 10));
        std::println("   Broadcasting to blockchain...");
        std::println("✅ Payment successful!");
    }

    std::string getPaymentType() const override {
        return cryptoType + " Cryptocurrency";
    }
};

/**
 * Context: Shopping Cart
 * Uses a payment strategy
 */
class ShoppingCart {
private:
    std::shared_ptr<PaymentStrategy> paymentStrategy;
    std::vector<std::pair<std::string, double>> items;

public:
    void addItem(const std::string& item, double price) {
        items.push_back({item, price});
        std::println("🛒 Added: {} - ${}", item, price);
    }

    double calculateTotal() const {
        double total = 0;
        for (const auto& item : items) {
            total += item.second;
        }
        return total;
    }

    void setPaymentStrategy(std::shared_ptr<PaymentStrategy> strategy) {
        paymentStrategy = strategy;
        std::println("💳 Payment method set to: {}", strategy->getPaymentType());
    }

    void checkout() {
        if (!paymentStrategy) {
            std::println("❌ Please select a payment method");
            return;
        }

        double total = calculateTotal();
        std::println("");
        std::println("📋 Checkout Summary:");
        for (const auto& item : items) {
            std::println("   • {}: ${}", item.first, item.second);
        }
        std::println("   Total: ${}", total);
        std::println("");

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
    virtual void sort(std::vector<int>& data) = 0;
    virtual std::string getAlgorithmName() const = 0;
};

/**
 * Concrete Strategy: Bubble Sort
 */
class BubbleSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) override {
        std::println("🔢 Using Bubble Sort...");
        int n = data.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
        std::println("✅ Bubble Sort complete");
    }

    std::string getAlgorithmName() const override {
        return "Bubble Sort (O(n²))";
    }
};

/**
 * Concrete Strategy: Quick Sort
 */
class QuickSort : public SortStrategy {
private:
    void quickSortHelper(std::vector<int>& data, int low, int high) {
        if (low < high) {
            int pivot = partition(data, low, high);
            quickSortHelper(data, low, pivot - 1);
            quickSortHelper(data, pivot + 1, high);
        }
    }

    int partition(std::vector<int>& data, int low, int high) {
        int pivot = data[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (data[j] < pivot) {
                i++;
                std::swap(data[i], data[j]);
            }
        }
        std::swap(data[i + 1], data[high]);
        return i + 1;
    }

public:
    void sort(std::vector<int>& data) override {
        std::println("⚡ Using Quick Sort...");
        quickSortHelper(data, 0, data.size() - 1);
        std::println("✅ Quick Sort complete");
    }

    std::string getAlgorithmName() const override {
        return "Quick Sort (O(n log n))";
    }
};

/**
 * Concrete Strategy: Merge Sort
 */
class MergeSort : public SortStrategy {
private:
    void merge(std::vector<int>& data, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<int> L(n1), R(n2);
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

    void mergeSortHelper(std::vector<int>& data, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSortHelper(data, left, mid);
            mergeSortHelper(data, mid + 1, right);
            merge(data, left, mid, right);
        }
    }

public:
    void sort(std::vector<int>& data) override {
        std::println("🔀 Using Merge Sort...");
        mergeSortHelper(data, 0, data.size() - 1);
        std::println("✅ Merge Sort complete");
    }

    std::string getAlgorithmName() const override {
        return "Merge Sort (O(n log n))";
    }
};

/**
 * Context: Data Sorter
 */
class DataSorter {
private:
    std::shared_ptr<SortStrategy> strategy;

public:
    void setSortStrategy(std::shared_ptr<SortStrategy> strat) {
        strategy = strat;
        std::println("📊 Sort algorithm set to: {}", strategy->getAlgorithmName());
    }

    void sortData(std::vector<int>& data) {
        if (!strategy) {
            std::println("❌ No sorting strategy set");
            return;
        }

        std::print("Unsorted: ");
        printData(data);

        strategy->sort(data);

        std::print("Sorted:   ");
        printData(data);
    }

private:
    void printData(const std::vector<int>& data) {
        for (int val : data) {
            std::print("{} ", val);
        }
        std::println("");
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
    virtual std::string compress(const std::string& data) = 0;
    virtual std::string decompress(const std::string& data) = 0;
    virtual std::string getCompressionType() const = 0;
};

/**
 * Concrete Strategy: ZIP Compression
 */
class ZipCompression : public CompressionStrategy {
public:
    std::string compress(const std::string& data) override {
        std::println("🗜️  [ZIP] Compressing {} bytes...", data.length());
        // Simulated compression
        std::string compressed = "[ZIP:" + data + "]";
        std::println("   Compressed to {} bytes", compressed.length());
        return compressed;
    }

    std::string decompress(const std::string& data) override {
        std::println("📦 [ZIP] Decompressing...");
        // Simulated decompression
        return data.substr(5, data.length() - 6);
    }

    std::string getCompressionType() const override {
        return "ZIP";
    }
};

/**
 * Concrete Strategy: RAR Compression
 */
class RarCompression : public CompressionStrategy {
public:
    std::string compress(const std::string& data) override {
        std::println("🗜️  [RAR] Compressing {} bytes...", data.length());
        // Simulated compression with better ratio
        std::string compressed = "[RAR:" + data + "]";
        std::println("   Compressed to {} bytes (better ratio)", compressed.length());
        return compressed;
    }

    std::string decompress(const std::string& data) override {
        std::println("📦 [RAR] Decompressing...");
        return data.substr(5, data.length() - 6);
    }

    std::string getCompressionType() const override {
        return "RAR";
    }
};

/**
 * Concrete Strategy: 7z Compression
 */
class SevenZipCompression : public CompressionStrategy {
public:
    std::string compress(const std::string& data) override {
        std::println("🗜️  [7z] Compressing {} bytes...", data.length());
        // Simulated compression with best ratio
        std::string compressed = "[7z:" + data + "]";
        std::println("   Compressed to {} bytes (best ratio)", compressed.length());
        return compressed;
    }

    std::string decompress(const std::string& data) override {
        std::println("📦 [7z] Decompressing...");
        return data.substr(4, data.length() - 5);
    }

    std::string getCompressionType() const override {
        return "7-Zip";
    }
};

/**
 * Context: File Compressor
 */
class FileCompressor {
private:
    std::shared_ptr<CompressionStrategy> strategy;

public:
    void setCompressionStrategy(std::shared_ptr<CompressionStrategy> strat) {
        strategy = strat;
        std::println("📁 Compression method set to: {}", strategy->getCompressionType());
    }

    void compressFile(const std::string& filename, const std::string& data) {
        if (!strategy) {
            std::println("❌ No compression strategy set");
            return;
        }

        std::println("");
        std::println("📄 Compressing file: {}", filename);
        std::string compressed = strategy->compress(data);
        std::println("✅ File compressed successfully");
    }

    void decompressFile(const std::string& filename, const std::string& data) {
        if (!strategy) {
            std::println("❌ No compression strategy set");
            return;
        }

        std::println("");
        std::println("📄 Decompressing file: {}", filename);
        std::string decompressed = strategy->decompress(data);
        std::println("✅ File decompressed successfully");
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
    virtual void buildRoute(const std::string& from, const std::string& to) = 0;
    virtual std::string getRouteType() const = 0;
};

/**
 * Concrete Strategy: Car Navigation
 */
class CarNavigation : public NavigationStrategy {
public:
    void buildRoute(const std::string& from, const std::string& to) override {
        std::println("🚗 Building car route from {} to {}", from, to);
    }

    std::string getRouteType() const override {
        return "Car (Fastest Route)";
    }
};

/**
 * Concrete Strategy: Walking Navigation
 */
class WalkingNavigation : public NavigationStrategy {
public:
    void buildRoute(const std::string& from, const std::string& to) override {
        std::println("🚶 Building walking route from {} to {}", from, to);
    }

    std::string getRouteType() const override {
        return "Walking (Pedestrian Paths)";
    }
};

/**
 * Concrete Strategy: Public Transport Navigation
 */
class PublicTransportNavigation : public NavigationStrategy {
public:
    void buildRoute(const std::string& from, const std::string& to) override {
        std::println("🚌 Building public transport route from {} to {}", from, to);
    }

    std::string getRouteType() const override {
        return "Public Transport (Cheapest)";
    }
};

/**
 * Concrete Strategy: Bicycle Navigation
 */
class BicycleNavigation : public NavigationStrategy {
public:
    void buildRoute(const std::string& from, const std::string& to) override {
        std::println("🚴 Building bicycle route from {} to {}", from, to);
    }

    std::string getRouteType() const override {
        return "Bicycle (Bike Lanes)";
    }
};

/**
 * Context: Navigation App
 */
class Navigator {
private:
    std::shared_ptr<NavigationStrategy> strategy;

public:
    void setNavigationStrategy(std::shared_ptr<NavigationStrategy> strat) {
        strategy = strat;
        std::println("🗺️  Navigation mode set to: {}", strategy->getRouteType());
    }

    void navigate(const std::string& from, const std::string& to) {
        if (!strategy) {
            std::println("❌ Please select navigation mode");
            return;
        }

        std::println("");
        std::println("📍 Navigation Request");
        strategy->buildRoute(from, to);
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstratePaymentStrategies() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: PAYMENT PROCESSING     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    ShoppingCart cart;

    // Add items
    std::println("--- Adding Items to Cart ---");
    cart.addItem("Laptop", 999.99);
    cart.addItem("Mouse", 29.99);
    cart.addItem("Keyboard", 79.99);

    // Pay with Credit Card
    std::println("");
    std::println("--- Paying with Credit Card ---");
    auto creditCard = std::make_shared<CreditCardPayment>(
        "1234567890123456", "John Doe", "123", "12/25");
    cart.setPaymentStrategy(creditCard);
    cart.checkout();

    // Change to PayPal
    std::println("");
    std::println("--- Changing to PayPal ---");
    auto paypal = std::make_shared<PayPalPayment>("john@example.com", "password");
    cart.setPaymentStrategy(paypal);

    // Change to Crypto
    std::println("");
    std::println("--- Changing to Cryptocurrency ---");
    auto crypto = std::make_shared<CryptoPayment>("0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb", "Bitcoin");
    cart.setPaymentStrategy(crypto);
}

void demonstrateSortingStrategies() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: SORTING ALGORITHMS     ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    DataSorter sorter;

    // Test with different algorithms
    std::vector<int> data1 = {64, 34, 25, 12, 22, 11, 90};

    std::println("--- Using Bubble Sort ---");
    sorter.setSortStrategy(std::make_shared<BubbleSort>());
    sorter.sortData(data1);

    std::vector<int> data2 = {64, 34, 25, 12, 22, 11, 90};
    std::println("");
    std::println("--- Using Quick Sort ---");
    sorter.setSortStrategy(std::make_shared<QuickSort>());
    sorter.sortData(data2);

    std::vector<int> data3 = {64, 34, 25, 12, 22, 11, 90};
    std::println("");
    std::println("--- Using Merge Sort ---");
    sorter.setSortStrategy(std::make_shared<MergeSort>());
    sorter.sortData(data3);
}

void demonstrateCompressionStrategies() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: FILE COMPRESSION       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    FileCompressor compressor;
    std::string fileData = "This is sample file data that needs compression";

    // Try ZIP
    std::println("--- Using ZIP Compression ---");
    compressor.setCompressionStrategy(std::make_shared<ZipCompression>());
    compressor.compressFile("document.txt", fileData);

    // Try RAR
    std::println("");
    std::println("--- Using RAR Compression ---");
    compressor.setCompressionStrategy(std::make_shared<RarCompression>());
    compressor.compressFile("document.txt", fileData);

    // Try 7-Zip
    std::println("");
    std::println("--- Using 7-Zip Compression ---");
    compressor.setCompressionStrategy(std::make_shared<SevenZipCompression>());
    compressor.compressFile("document.txt", fileData);
}

void demonstrateNavigationStrategies() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: NAVIGATION ROUTES      ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Navigator nav;
    std::string from = "Home";
    std::string to = "Office";

    // Car navigation
    std::println("--- Car Navigation ---");
    nav.setNavigationStrategy(std::make_shared<CarNavigation>());
    nav.navigate(from, to);

    // Walking navigation
    std::println("");
    std::println("--- Walking Navigation ---");
    nav.setNavigationStrategy(std::make_shared<WalkingNavigation>());
    nav.navigate(from, to);

    // Public transport
    std::println("");
    std::println("--- Public Transport ---");
    nav.setNavigationStrategy(std::make_shared<PublicTransportNavigation>());
    nav.navigate(from, to);

    // Bicycle
    std::println("");
    std::println("--- Bicycle Navigation ---");
    nav.setNavigationStrategy(std::make_shared<BicycleNavigation>());
    nav.navigate(from, to);
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("STRATEGY PATTERN DEMO");
    std::println("========================================");

    demonstratePaymentStrategies();
    demonstrateSortingStrategies();
    demonstrateCompressionStrategies();
    demonstrateNavigationStrategies();

    return 0;
}
