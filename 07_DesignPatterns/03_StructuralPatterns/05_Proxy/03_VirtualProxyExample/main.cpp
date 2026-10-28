// virtual_proxy_example.cpp
// Virtual Proxy Pattern - Lazy Loading of Large Images

#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <print>

// ===== SUBJECT INTERFACE =====
class IImage {
public:
    virtual ~IImage() = default;
    virtual void display() = 0;
    virtual std::string getInfo() = 0;
};

// ===== REAL SUBJECT - Expensive to Create =====
class RealImage : public IImage {
private:
    std::string filename;
    std::string imageData;
    int width;
    int height;

    void loadFromDisk() {
        std::println("Loading image from disk: {}", filename);
        std::println("  [Simulating expensive operation...]");

        // Simulate expensive loading operation
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Simulate loading image data
        imageData = "Binary image data for " + filename;
        width = 1920;
        height = 1080;

        std::println("  [Image loaded! Size: {}x{}]", width, height);
    }

public:
    explicit RealImage(const std::string& fname) : filename(fname) {
        loadFromDisk();
    }

    void display() override {
        std::println("Displaying: {}", filename);
        std::println("  Resolution: {}x{}", width, height);
        std::println("  [Rendering image on screen...]");
    }

    std::string getInfo() override {
        return "Image: " + filename + " (" + std::to_string(width) + "x" +
               std::to_string(height) + ")";
    }
};

// ===== PROXY - Controls Access and Delays Loading =====
class ImageProxy : public IImage {
private:
    std::string filename;
    mutable std::unique_ptr<RealImage> realImage;  // Lazy loaded
    mutable bool isLoaded = false;

    // Lazy loading helper
    void ensureLoaded() const {
        if (!isLoaded) {
            std::println("");
            std::println("[Proxy: First access detected, loading real image...]");
            realImage = std::make_unique<RealImage>(filename);
            isLoaded = true;
        }
    }

public:
    explicit ImageProxy(const std::string& fname) : filename(fname) {
        std::println("ImageProxy created for: {} (not loaded yet)", filename);
    }

    void display() override {
        ensureLoaded();
        realImage->display();
    }

    std::string getInfo() override {
        // Can provide basic info without loading
        if (!isLoaded) {
            return "Image: " + filename + " (not loaded)";
        }
        return realImage->getInfo();
    }

    // Additional proxy-specific method
    bool isImageLoaded() const {
        return isLoaded;
    }
};

// ===== IMAGE GALLERY - Client Code =====
class ImageGallery {
private:
    std::vector<std::unique_ptr<IImage>> images;

public:
    void addImage(const std::string& filename) {
        // Use proxy instead of real image
        images.push_back(std::make_unique<ImageProxy>(filename));
    }

    void showImage(size_t index) {
        if (index < images.size()) {
            std::println("");
            std::println("--- Showing image #{} ---", index + 1);
            images[index]->display();
        }
    }

    void listImages() {
        std::println("");
        std::println("--- Gallery Contents ---");
        for (size_t i = 0; i < images.size(); ++i) {
            std::println("{}. {}", i + 1, images[i]->getInfo());
        }
    }
};

// ===== DEMONSTRATION =====
int main() {
    std::println("===== Virtual Proxy Pattern Demo =====");
    std::println("Loading an image gallery...");
    std::println("");

    // Create gallery
    ImageGallery gallery;

    // Add images (fast - just creates proxies)
    std::println("Adding images to gallery:");
    auto start = std::chrono::high_resolution_clock::now();

    gallery.addImage("vacation_photo_1.jpg");
    gallery.addImage("vacation_photo_2.jpg");
    gallery.addImage("vacation_photo_3.jpg");
    gallery.addImage("huge_panorama.jpg");
    gallery.addImage("family_portrait.jpg");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::println("");
    std::println("Time to add 5 images: {}ms", duration.count());
    std::println("(Fast because images aren't actually loaded yet!)");
    std::println("");

    // List images (still fast - no loading)
    gallery.listImages();

    // User interaction simulation
    std::println("");
    std::println("===== User Interaction =====");

    std::println("");
    std::println("User clicks on image #1:");
    gallery.showImage(0);  // First access - loads the image

    std::println("");
    std::println("User clicks on image #1 again:");
    gallery.showImage(0);  // Already loaded - displays immediately

    std::println("");
    std::println("User clicks on image #3:");
    gallery.showImage(2);  // First access - loads the image

    // Show final status
    std::println("");
    std::println("===== Final Status =====");
    gallery.listImages();

    return 0;
}
