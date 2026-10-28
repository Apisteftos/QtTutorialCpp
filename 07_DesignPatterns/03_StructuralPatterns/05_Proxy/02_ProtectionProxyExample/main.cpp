// protection_proxy_example.cpp
// Protection Proxy Pattern - Access Control for Sensitive Documents

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <print>

// ===== USER CONTEXT =====
enum class UserRole {
    ADMIN,
    EMPLOYEE,
    GUEST
};

class User {
private:
    std::string name;
    UserRole role;
    std::string department;

public:
    User(const std::string& n, UserRole r, const std::string& dept = "")
        : name(n), role(r), department(dept) {}

    std::string getName() const { return name; }
    UserRole getRole() const { return role; }
    std::string getDepartment() const { return department; }

    std::string getRoleString() const {
        switch(role) {
        case UserRole::ADMIN: return "Admin";
        case UserRole::EMPLOYEE: return "Employee";
        case UserRole::GUEST: return "Guest";
        }
        return "Unknown";
    }
};

// ===== SUBJECT INTERFACE =====
class IDocument {
public:
    virtual ~IDocument() = default;
    virtual void displayContent() = 0;
    virtual void editContent(const std::string& newContent) = 0;
    virtual void deleteDocument() = 0;
    virtual std::string getTitle() = 0;
};

// ===== REAL SUBJECT - Actual Document =====
class SensitiveDocument : public IDocument {
private:
    std::string title;
    std::string content;
    std::string classification;

public:
    SensitiveDocument(const std::string& t, const std::string& c,
                      const std::string& classif)
        : title(t), content(c), classification(classif) {}

    void displayContent() override {
        std::println("=== Document: {} ===", title);
        std::println("Classification: {}", classification);
        std::println("Content: {}", content);
        std::println("========================");
    }

    void editContent(const std::string& newContent) override {
        content = newContent;
        std::println("Document content updated successfully!");
    }

    void deleteDocument() override {
        std::println("Document '{}' has been deleted!", title);
        content = "[DELETED]";
    }

    std::string getTitle() override {
        return title;
    }
};

// ===== PROTECTION PROXY - Controls Access =====
class DocumentProxy : public IDocument {
private:
    std::unique_ptr<SensitiveDocument> document;
    User* currentUser;
    std::string requiredDepartment;

    // Access control logic
    bool canRead() const {
        if (!currentUser) {
            std::println("❌ Access Denied: No user logged in");
            return false;
        }

        // Admins can read everything
        if (currentUser->getRole() == UserRole::ADMIN) {
            return true;
        }

        // Employees can read if in same department
        if (currentUser->getRole() == UserRole::EMPLOYEE) {
            if (requiredDepartment.empty() ||
                currentUser->getDepartment() == requiredDepartment) {
                return true;
            }
            std::println("❌ Access Denied: Wrong department (requires: {})", requiredDepartment);
            return false;
        }

        // Guests cannot read sensitive documents
        std::println("❌ Access Denied: Guests cannot read sensitive documents");
        return false;
    }

    bool canEdit() const {
        if (!currentUser) {
            std::println("❌ Access Denied: No user logged in");
            return false;
        }

        // Only admins and department employees can edit
        if (currentUser->getRole() == UserRole::ADMIN) {
            return true;
        }

        if (currentUser->getRole() == UserRole::EMPLOYEE &&
            currentUser->getDepartment() == requiredDepartment) {
            return true;
        }

        std::println("❌ Access Denied: Insufficient privileges to edit");
        return false;
    }

    bool canDelete() const {
        if (!currentUser) {
            std::println("❌ Access Denied: No user logged in");
            return false;
        }

        // Only admins can delete
        if (currentUser->getRole() != UserRole::ADMIN) {
            std::println("❌ Access Denied: Only admins can delete documents");
            return false;
        }
        return true;
    }

    void logAccess(const std::string& action) const {
        if (currentUser) {
            std::println("📝 LOG: User '{}' ({}) {} document '{}'",
                currentUser->getName(), currentUser->getRoleString(), action, document->getTitle());
        }
    }

public:
    DocumentProxy(const std::string& title, const std::string& content,
                  const std::string& classification, const std::string& dept = "")
        : document(std::make_unique<SensitiveDocument>(title, content, classification)),
        currentUser(nullptr), requiredDepartment(dept) {}

    void setUser(User* user) {
        currentUser = user;
        if (user) {
            std::println("👤 User '{}' logged in as {}", user->getName(), user->getRoleString());
        }
    }

    void displayContent() override {
        if (canRead()) {
            std::println("✅ Access Granted: Reading document");
            logAccess("read");
            document->displayContent();
        }
    }

    void editContent(const std::string& newContent) override {
        if (canEdit()) {
            std::println("✅ Access Granted: Editing document");
            logAccess("edited");
            document->editContent(newContent);
        }
    }

    void deleteDocument() override {
        if (canDelete()) {
            std::println("✅ Access Granted: Deleting document");
            logAccess("deleted");
            document->deleteDocument();
        }
    }

    std::string getTitle() override {
        // Title can be seen by everyone (for listing purposes)
        return document->getTitle();
    }
};

// ===== DOCUMENT MANAGEMENT SYSTEM =====
class DocumentSystem {
private:
    std::vector<std::unique_ptr<DocumentProxy>> documents;
    User* currentUser;

public:
    DocumentSystem() : currentUser(nullptr) {}

    void addDocument(const std::string& title, const std::string& content,
                     const std::string& classification, const std::string& dept = "") {
        documents.push_back(
            std::make_unique<DocumentProxy>(title, content, classification, dept)
            );
        std::println("📄 Document '{}' added to system", title);
    }

    void loginUser(User* user) {
        currentUser = user;
        // Update all document proxies with current user
        for (auto& doc : documents) {
            doc->setUser(user);
        }
    }

    void accessDocument(size_t index, const std::string& action) {
        if (index >= documents.size()) {
            std::println("Document not found!");
            return;
        }

        std::println("");
        std::println("--- Attempting to {} document: {} ---", action, documents[index]->getTitle());

        if (action == "read") {
            documents[index]->displayContent();
        } else if (action == "edit") {
            documents[index]->editContent("Updated content by " +
                                          (currentUser ? currentUser->getName() : "Unknown"));
        } else if (action == "delete") {
            documents[index]->deleteDocument();
        }
    }

    void listDocuments() {
        std::println("");
        std::println("=== Document List ===");
        for (size_t i = 0; i < documents.size(); ++i) {
            std::println("{}. {}", i + 1, documents[i]->getTitle());
        }
    }
};

// ===== DEMONSTRATION =====
int main() {
    std::println("===== Protection Proxy Pattern Demo =====");
    std::println("Document Management System with Access Control");
    std::println("");

    // Create document system
    DocumentSystem system;

    // Add documents
    std::println("Setting up documents:");
    system.addDocument("Company Strategy 2024", "Confidential strategic plans...",
                       "TOP SECRET", "Management");
    system.addDocument("Employee Handbook", "General company policies...",
                       "PUBLIC", "HR");
    system.addDocument("Financial Report Q4", "Revenue: $10M, Profit: $2M...",
                       "CONFIDENTIAL", "Finance");

    // Create users
    User admin("Alice", UserRole::ADMIN, "IT");
    User financeEmployee("Bob", UserRole::EMPLOYEE, "Finance");
    User hrEmployee("Carol", UserRole::EMPLOYEE, "HR");
    User guest("David", UserRole::GUEST);

    system.listDocuments();

    // Test 1: Guest tries to access
    std::println("");
    std::println("===== Test 1: Guest Access =====");
    system.loginUser(&guest);
    system.accessDocument(0, "read");    // Should fail
    system.accessDocument(1, "edit");    // Should fail

    // Test 2: Employee with wrong department
    std::println("");
    std::println("===== Test 2: Employee Wrong Department =====");
    system.loginUser(&hrEmployee);
    system.accessDocument(2, "read");    // Should fail (Finance document)
    system.accessDocument(1, "read");    // Should succeed (HR document)
    system.accessDocument(1, "edit");    // Should succeed

    // Test 3: Employee with correct department
    std::println("");
    std::println("===== Test 3: Employee Correct Department =====");
    system.loginUser(&financeEmployee);
    system.accessDocument(2, "read");    // Should succeed
    system.accessDocument(2, "edit");    // Should succeed
    system.accessDocument(2, "delete");  // Should fail (not admin)

    // Test 4: Admin access
    std::println("");
    std::println("===== Test 4: Admin Access =====");
    system.loginUser(&admin);
    system.accessDocument(0, "read");    // Should succeed
    system.accessDocument(0, "edit");    // Should succeed
    system.accessDocument(0, "delete");  // Should succeed

    return 0;
}
