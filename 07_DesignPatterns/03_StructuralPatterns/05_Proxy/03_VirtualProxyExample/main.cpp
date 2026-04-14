// virtual_proxy_example.cpp
// Virtual Proxy Pattern - Lazy Loading of Large Images

#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <vector>

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
        std::cout << "Loading image from disk: " << filename << std::endl;
        std::cout << "  [Simulating expensive operation...]" << std::endl;

        // Simulate expensive loading operation
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Simulate loading image data
        imageData = "Binary image data for " + filename;
        width = 1920;
        height = 1080;

        std::cout << "  [Image loaded! Size: " << width << "x" << height << "]" << std::endl;
    }

public:
    explicit RealImage(const std::string& fname) : filename(fname) {
        loadFromDisk();
    }

    void display() override {
        std::cout << "Displaying: " << filename << std::endl;
        std::cout << "  Resolution: " << width << "x" << height << std::endl;
        std::cout << "  [Rendering image on screen...]" << std::endl;
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
            std::cout << "\n[Proxy: First access detected, loading real image...]" << std::endl;
            realImage = std::make_unique<RealImage>(filename);
            isLoaded = true;
        }
    }

public:
    explicit ImageProxy(const std::string& fname) : filename(fname) {
        std::cout << "ImageProxy created for: " << filename << " (not loaded yet)" << std::endl;
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
            std::cout << "\n--- Showing image #" << index + 1 << " ---" << std::endl;
            images[index]->display();
        }
    }

    void listImages() {
        std::cout << "\n--- Gallery Contents ---" << std::endl;
        for (size_t i = 0; i < images.size(); ++i) {
            std::cout << i + 1 << ". " << images[i]->getInfo() << std::endl;
        }
    }
};

// ===== DEMONSTRATION =====
int main() {
    std::cout << "===== Virtual Proxy Pattern Demo =====" << std::endl;
    std::cout << "Loading an image gallery...\n" << std::endl;

    // Create gallery
    ImageGallery gallery;

    // Add images (fast - just creates proxies)
    std::cout << "Adding images to gallery:" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    gallery.addImage("vacation_photo_1.jpg");
    gallery.addImage("vacation_photo_2.jpg");
    gallery.addImage("vacation_photo_3.jpg");
    gallery.addImage("huge_panorama.jpg");
    gallery.addImage("family_portrait.jpg");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\nTime to add 5 images: " << duration.count() << "ms" << std::endl;
    std::cout << "(Fast because images aren't actually loaded yet!)\n" << std::endl;

    // List images (still fast - no loading)
    gallery.listImages();

    // User interaction simulation
    std::cout << "\n===== User Interaction =====" << std::endl;

    std::cout << "\nUser clicks on image #1:" << std::endl;
    gallery.showImage(0);  // First access - loads the image

    std::cout << "\nUser clicks on image #1 again:" << std::endl;
    gallery.showImage(0);  // Already loaded - displays immediately

    std::cout << "\nUser clicks on image #3:" << std::endl;
    gallery.showImage(2);  // First access - loads the image

    // Show final status
    std::cout << "\n===== Final Status =====" << std::endl;
    gallery.listImages();

    std::cout << "\n===== Key Benefits =====" << std::endl;
    std::cout << "✓ Gallery opens instantly (no waiting for all images)" << std::endl;
    std::cout << "✓ Images load only when actually viewed" << std::endl;
    std::cout << "✓ Subsequent views are instant (already loaded)" << std::endl;
    std::cout << "✓ Memory saved for unviewed images" << std::endl;
    std::cout << "✓ Client code doesn't know about lazy loading" << std::endl;

    return 0;
}
