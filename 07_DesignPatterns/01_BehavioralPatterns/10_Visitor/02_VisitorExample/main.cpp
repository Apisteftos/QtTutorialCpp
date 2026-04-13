#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
using namespace std;

// ============================================
// EXAMPLE 1: SHAPES AND OPERATIONS
// Classic visitor pattern example
// ============================================

// Forward declarations
class Circle;
class Rectangle;
class Triangle;

/**
 * Visitor Interface
 * Declares visit methods for each concrete element type
 */
class ShapeVisitor {
public:
    virtual ~ShapeVisitor() {}

    virtual void visitCircle(Circle* circle) = 0;
    virtual void visitRectangle(Rectangle* rectangle) = 0;
    virtual void visitTriangle(Triangle* triangle) = 0;
};

/**
 * Element Interface
 * Declares accept method that takes a visitor
 */
class Shape {
public:
    virtual ~Shape() {}

    // Accept method - the key to visitor pattern
    virtual void accept(ShapeVisitor* visitor) = 0;
    virtual string getName() const = 0;
};

/**
 * Concrete Element: Circle
 */
class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double getRadius() const { return radius; }

    void accept(ShapeVisitor* visitor) override {
        visitor->visitCircle(this);
    }

    string getName() const override {
        return "Circle";
    }
};

/**
 * Concrete Element: Rectangle
 */
class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double getWidth() const { return width; }
    double getHeight() const { return height; }

    void accept(ShapeVisitor* visitor) override {
        visitor->visitRectangle(this);
    }

    string getName() const override {
        return "Rectangle";
    }
};

/**
 * Concrete Element: Triangle
 */
class Triangle : public Shape {
private:
    double base;
    double height;

public:
    Triangle(double b, double h) : base(b), height(h) {}

    double getBase() const { return base; }
    double getHeight() const { return height; }

    void accept(ShapeVisitor* visitor) override {
        visitor->visitTriangle(this);
    }

    string getName() const override {
        return "Triangle";
    }
};

/**
 * Concrete Visitor: Area Calculator
 */
class AreaCalculator : public ShapeVisitor {
private:
    double totalArea;

public:
    AreaCalculator() : totalArea(0) {}

    void visitCircle(Circle* circle) override {
        double area = 3.14159 * circle->getRadius() * circle->getRadius();
        cout << "📐 Circle area: " << area << " sq units\n";
        totalArea += area;
    }

    void visitRectangle(Rectangle* rect) override {
        double area = rect->getWidth() * rect->getHeight();
        cout << "📐 Rectangle area: " << area << " sq units\n";
        totalArea += area;
    }

    void visitTriangle(Triangle* tri) override {
        double area = 0.5 * tri->getBase() * tri->getHeight();
        cout << "📐 Triangle area: " << area << " sq units\n";
        totalArea += area;
    }

    double getTotalArea() const { return totalArea; }
};

/**
 * Concrete Visitor: Perimeter Calculator
 */
class PerimeterCalculator : public ShapeVisitor {
private:
    double totalPerimeter;

public:
    PerimeterCalculator() : totalPerimeter(0) {}

    void visitCircle(Circle* circle) override {
        double perimeter = 2 * 3.14159 * circle->getRadius();
        cout << "📏 Circle perimeter: " << perimeter << " units\n";
        totalPerimeter += perimeter;
    }

    void visitRectangle(Rectangle* rect) override {
        double perimeter = 2 * (rect->getWidth() + rect->getHeight());
        cout << "📏 Rectangle perimeter: " << perimeter << " units\n";
        totalPerimeter += perimeter;
    }

    void visitTriangle(Triangle* tri) override {
        // For simplicity, assuming equilateral triangle
        double side = tri->getBase();
        double perimeter = 3 * side;
        cout << "📏 Triangle perimeter: " << perimeter << " units\n";
        totalPerimeter += perimeter;
    }

    double getTotalPerimeter() const { return totalPerimeter; }
};

/**
 * Concrete Visitor: XML Exporter
 */
class XMLExporter : public ShapeVisitor {
private:
    string xml;

public:
    XMLExporter() : xml("<shapes>\n") {}

    void visitCircle(Circle* circle) override {
        xml += "  <circle radius=\"" + to_string(circle->getRadius()) + "\"/>\n";
    }

    void visitRectangle(Rectangle* rect) override {
        xml += "  <rectangle width=\"" + to_string(rect->getWidth()) +
               "\" height=\"" + to_string(rect->getHeight()) + "\"/>\n";
    }

    void visitTriangle(Triangle* tri) override {
        xml += "  <triangle base=\"" + to_string(tri->getBase()) +
               "\" height=\"" + to_string(tri->getHeight()) + "\"/>\n";
    }

    string getXML() {
        return xml + "</shapes>";
    }
};

// ============================================
// EXAMPLE 2: EMPLOYEE HIERARCHY
// Visitor for organization structure
// ============================================

// Forward declarations
class Manager;
class Developer;
class Designer;

/**
 * Employee Visitor Interface
 */
class EmployeeVisitor {
public:
    virtual ~EmployeeVisitor() {}

    virtual void visitManager(Manager* manager) = 0;
    virtual void visitDeveloper(Developer* developer) = 0;
    virtual void visitDesigner(Designer* designer) = 0;
};

/**
 * Employee Element Interface
 */
class Employee {
protected:
    string name;
    double baseSalary;

public:
    Employee(const string& n, double salary) : name(n), baseSalary(salary) {}
    virtual ~Employee() {}

    string getName() const { return name; }
    double getBaseSalary() const { return baseSalary; }

    virtual void accept(EmployeeVisitor* visitor) = 0;
    virtual string getRole() const = 0;
};

/**
 * Concrete Element: Manager
 */
class Manager : public Employee {
private:
    int teamSize;

public:
    Manager(const string& n, double salary, int team)
        : Employee(n, salary), teamSize(team) {}

    int getTeamSize() const { return teamSize; }

    void accept(EmployeeVisitor* visitor) override {
        visitor->visitManager(this);
    }

    string getRole() const override {
        return "Manager";
    }
};

/**
 * Concrete Element: Developer
 */
class Developer : public Employee {
private:
    string programmingLanguage;

public:
    Developer(const string& n, double salary, const string& lang)
        : Employee(n, salary), programmingLanguage(lang) {}

    string getProgrammingLanguage() const { return programmingLanguage; }

    void accept(EmployeeVisitor* visitor) override {
        visitor->visitDeveloper(this);
    }

    string getRole() const override {
        return "Developer";
    }
};

/**
 * Concrete Element: Designer
 */
class Designer : public Employee {
private:
    string designTool;

public:
    Designer(const string& n, double salary, const string& tool)
        : Employee(n, salary), designTool(tool) {}

    string getDesignTool() const { return designTool; }

    void accept(EmployeeVisitor* visitor) override {
        visitor->visitDesigner(this);
    }

    string getRole() const override {
        return "Designer";
    }
};

/**
 * Concrete Visitor: Salary Calculator
 */
class SalaryCalculator : public EmployeeVisitor {
private:
    double totalSalary;

public:
    SalaryCalculator() : totalSalary(0) {}

    void visitManager(Manager* manager) override {
        // Managers get bonus based on team size
        double bonus = manager->getTeamSize() * 1000;
        double salary = manager->getBaseSalary() + bonus;
        cout << "💰 [" << manager->getName() << "] Manager salary: $"
             << salary << " (base: $" << manager->getBaseSalary()
             << " + team bonus: $" << bonus << ")\n";
        totalSalary += salary;
    }

    void visitDeveloper(Developer* dev) override {
        // Developers get skill bonus
        double bonus = 5000;
        double salary = dev->getBaseSalary() + bonus;
        cout << "💰 [" << dev->getName() << "] Developer salary: $"
             << salary << " (base: $" << dev->getBaseSalary()
             << " + skill bonus: $" << bonus << ")\n";
        totalSalary += salary;
    }

    void visitDesigner(Designer* designer) override {
        // Designers get tool proficiency bonus
        double bonus = 3000;
        double salary = designer->getBaseSalary() + bonus;
        cout << "💰 [" << designer->getName() << "] Designer salary: $"
             << salary << " (base: $" << designer->getBaseSalary()
             << " + tool bonus: $" << bonus << ")\n";
        totalSalary += salary;
    }

    double getTotalSalary() const { return totalSalary; }
};

/**
 * Concrete Visitor: Performance Evaluator
 */
class PerformanceEvaluator : public EmployeeVisitor {
public:
    void visitManager(Manager* manager) override {
        cout << "📊 [" << manager->getName() << "] Manager Performance:\n";
        cout << "   • Leadership: Excellent\n";
        cout << "   • Team Management: Managing " << manager->getTeamSize() << " members\n";
        cout << "   • Rating: ⭐⭐⭐⭐⭐\n";
    }

    void visitDeveloper(Developer* dev) override {
        cout << "📊 [" << dev->getName() << "] Developer Performance:\n";
        cout << "   • Technical Skills: " << dev->getProgrammingLanguage() << "\n";
        cout << "   • Code Quality: High\n";
        cout << "   • Rating: ⭐⭐⭐⭐\n";
    }

    void visitDesigner(Designer* designer) override {
        cout << "📊 [" << designer->getName() << "] Designer Performance:\n";
        cout << "   • Design Tool: " << designer->getDesignTool() << "\n";
        cout << "   • Creativity: Outstanding\n";
        cout << "   • Rating: ⭐⭐⭐⭐⭐\n";
    }
};

// ============================================
// EXAMPLE 3: DOCUMENT STRUCTURE
// Export documents to different formats
// ============================================

// Forward declarations
class Paragraph;
class Heading;
class Image;

/**
 * Document Element Visitor
 */
class DocumentVisitor {
public:
    virtual ~DocumentVisitor() {}

    virtual void visitParagraph(Paragraph* para) = 0;
    virtual void visitHeading(Heading* heading) = 0;
    virtual void visitImage(Image* image) = 0;
};

/**
 * Document Element Interface
 */
class DocumentElement {
public:
    virtual ~DocumentElement() {}
    virtual void accept(DocumentVisitor* visitor) = 0;
};

/**
 * Concrete Element: Paragraph
 */
class Paragraph : public DocumentElement {
private:
    string text;

public:
    Paragraph(const string& t) : text(t) {}

    string getText() const { return text; }

    void accept(DocumentVisitor* visitor) override {
        visitor->visitParagraph(this);
    }
};

/**
 * Concrete Element: Heading
 */
class Heading : public DocumentElement {
private:
    string text;
    int level;

public:
    Heading(const string& t, int lvl) : text(t), level(lvl) {}

    string getText() const { return text; }
    int getLevel() const { return level; }

    void accept(DocumentVisitor* visitor) override {
        visitor->visitHeading(this);
    }
};

/**
 * Concrete Element: Image
 */
class Image : public DocumentElement {
private:
    string url;
    string altText;

public:
    Image(const string& u, const string& alt) : url(u), altText(alt) {}

    string getUrl() const { return url; }
    string getAltText() const { return altText; }

    void accept(DocumentVisitor* visitor) override {
        visitor->visitImage(this);
    }
};

/**
 * Concrete Visitor: HTML Exporter
 */
class HTMLExporter : public DocumentVisitor {
private:
    string html;

public:
    void visitParagraph(Paragraph* para) override {
        html += "<p>" + para->getText() + "</p>\n";
    }

    void visitHeading(Heading* heading) override {
        string tag = "h" + to_string(heading->getLevel());
        html += "<" + tag + ">" + heading->getText() + "</" + tag + ">\n";
    }

    void visitImage(Image* image) override {
        html += "<img src=\"" + image->getUrl() + "\" alt=\"" +
                image->getAltText() + "\" />\n";
    }

    string getHTML() const { return html; }
};

/**
 * Concrete Visitor: Markdown Exporter
 */
class MarkdownExporter : public DocumentVisitor {
private:
    string markdown;

public:
    void visitParagraph(Paragraph* para) override {
        markdown += para->getText() + "\n\n";
    }

    void visitHeading(Heading* heading) override {
        string hashes(heading->getLevel(), '#');
        markdown += hashes + " " + heading->getText() + "\n\n";
    }

    void visitImage(Image* image) override {
        markdown += "![" + image->getAltText() + "](" + image->getUrl() + ")\n\n";
    }

    string getMarkdown() const { return markdown; }
};

/**
 * Concrete Visitor: Plain Text Exporter
 */
class PlainTextExporter : public DocumentVisitor {
private:
    string text;

public:
    void visitParagraph(Paragraph* para) override {
        text += para->getText() + "\n\n";
    }

    void visitHeading(Heading* heading) override {
        text += "=== " + heading->getText() + " ===\n\n";
    }

    void visitImage(Image* image) override {
        text += "[Image: " + image->getAltText() + "]\n\n";
    }

    string getText() const { return text; }
};

// ============================================
// EXAMPLE 4: SHOPPING CART
// Different pricing strategies using visitor
// ============================================

// Forward declarations
class Book;
class Electronics;
class Clothing;

/**
 * Product Visitor
 */
class ProductVisitor {
public:
    virtual ~ProductVisitor() {}

    virtual void visitBook(Book* book) = 0;
    virtual void visitElectronics(Electronics* electronics) = 0;
    virtual void visitClothing(Clothing* clothing) = 0;
};

/**
 * Product Element Interface
 */
class Product {
protected:
    string name;
    double price;

public:
    Product(const string& n, double p) : name(n), price(p) {}
    virtual ~Product() {}

    string getName() const { return name; }
    double getPrice() const { return price; }

    virtual void accept(ProductVisitor* visitor) = 0;
};

/**
 * Concrete Element: Book
 */
class Book : public Product {
private:
    string author;

public:
    Book(const string& n, double p, const string& a)
        : Product(n, p), author(a) {}

    string getAuthor() const { return author; }

    void accept(ProductVisitor* visitor) override {
        visitor->visitBook(this);
    }
};

/**
 * Concrete Element: Electronics
 */
class Electronics : public Product {
private:
    int warrantyYears;

public:
    Electronics(const string& n, double p, int warranty)
        : Product(n, p), warrantyYears(warranty) {}

    int getWarrantyYears() const { return warrantyYears; }

    void accept(ProductVisitor* visitor) override {
        visitor->visitElectronics(this);
    }
};

/**
 * Concrete Element: Clothing
 */
class Clothing : public Product {
private:
    string size;

public:
    Clothing(const string& n, double p, const string& s)
        : Product(n, p), size(s) {}

    string getSize() const { return size; }

    void accept(ProductVisitor* visitor) override {
        visitor->visitClothing(this);
    }
};

/**
 * Concrete Visitor: Tax Calculator
 */
class TaxCalculator : public ProductVisitor {
private:
    double totalTax;

public:
    TaxCalculator() : totalTax(0) {}

    void visitBook(Book* book) override {
        double tax = 0;  // Books are tax-exempt
        cout << "📚 [" << book->getName() << "] Tax: $" << tax
             << " (tax-exempt)\n";
        totalTax += tax;
    }

    void visitElectronics(Electronics* electronics) override {
        double tax = electronics->getPrice() * 0.15;  // 15% tax
        cout << "💻 [" << electronics->getName() << "] Tax: $" << tax
             << " (15%)\n";
        totalTax += tax;
    }

    void visitClothing(Clothing* clothing) override {
        double tax = clothing->getPrice() * 0.08;  // 8% tax
        cout << "👕 [" << clothing->getName() << "] Tax: $" << tax
             << " (8%)\n";
        totalTax += tax;
    }

    double getTotalTax() const { return totalTax; }
};

/**
 * Concrete Visitor: Discount Calculator
 */
class DiscountCalculator : public ProductVisitor {
private:
    double totalDiscount;

public:
    DiscountCalculator() : totalDiscount(0) {}

    void visitBook(Book* book) override {
        double discount = book->getPrice() * 0.10;  // 10% discount
        cout << "📚 [" << book->getName() << "] Discount: $" << discount
             << " (10% off)\n";
        totalDiscount += discount;
    }

    void visitElectronics(Electronics* electronics) override {
        double discount = electronics->getPrice() * 0.05;  // 5% discount
        cout << "💻 [" << electronics->getName() << "] Discount: $" << discount
             << " (5% off)\n";
        totalDiscount += discount;
    }

    void visitClothing(Clothing* clothing) override {
        double discount = clothing->getPrice() * 0.20;  // 20% discount
        cout << "👕 [" << clothing->getName() << "] Discount: $" << discount
             << " (20% off)\n";
        totalDiscount += discount;
    }

    double getTotalDiscount() const { return totalDiscount; }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateShapeVisitor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: SHAPE OPERATIONS       ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create shapes
    vector<Shape*> shapes;
    shapes.push_back(new Circle(5.0));
    shapes.push_back(new Rectangle(4.0, 6.0));
    shapes.push_back(new Triangle(3.0, 4.0));

    // Calculate areas
    cout << "--- Calculating Areas ---\n";
    AreaCalculator areaCalc;
    for (Shape* shape : shapes) {
        shape->accept(&areaCalc);
    }
    cout << "Total Area: " << areaCalc.getTotalArea() << " sq units\n";

    // Calculate perimeters
    cout << "\n--- Calculating Perimeters ---\n";
    PerimeterCalculator perimCalc;
    for (Shape* shape : shapes) {
        shape->accept(&perimCalc);
    }
    cout << "Total Perimeter: " << perimCalc.getTotalPerimeter() << " units\n";

    // Export to XML
    cout << "\n--- Exporting to XML ---\n";
    XMLExporter xmlExporter;
    for (Shape* shape : shapes) {
        shape->accept(&xmlExporter);
    }
    cout << xmlExporter.getXML() << "\n";

    // Cleanup
    for (Shape* shape : shapes) {
        delete shape;
    }
}

void demonstrateEmployeeVisitor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: EMPLOYEE SYSTEM        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create employees
    vector<Employee*> employees;
    employees.push_back(new Manager("Alice", 80000, 5));
    employees.push_back(new Developer("Bob", 70000, "C++"));
    employees.push_back(new Developer("Charlie", 75000, "Python"));
    employees.push_back(new Designer("Diana", 65000, "Figma"));

    // Calculate salaries
    cout << "--- Salary Calculation ---\n";
    SalaryCalculator salaryCalc;
    for (Employee* emp : employees) {
        emp->accept(&salaryCalc);
    }
    cout << "\nTotal Payroll: $" << salaryCalc.getTotalSalary() << "\n";

    // Evaluate performance
    cout << "\n--- Performance Evaluation ---\n";
    PerformanceEvaluator perfEval;
    for (Employee* emp : employees) {
        emp->accept(&perfEval);
        cout << "\n";
    }

    // Cleanup
    for (Employee* emp : employees) {
        delete emp;
    }
}

void demonstrateDocumentVisitor() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 3: DOCUMENT EXPORT        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create document elements
    vector<DocumentElement*> document;
    document.push_back(new Heading("Welcome to Visitor Pattern", 1));
    document.push_back(new Paragraph("The Visitor pattern allows adding new operations."));
    document.push_back(new Heading("Benefits", 2));
    document.push_back(new Paragraph("Easy to add new operations without modifying classes."));
    document.push_back(new Image("diagram.png", "Visitor Pattern Diagram"));

    // Export to HTML
    cout << "--- Exporting to HTML ---\n";
    HTMLExporter htmlExp;
    for (DocumentElement* elem : document) {
        elem->accept(&htmlExp);
    }
    cout << htmlExp.getHTML() << "\n";

    // Export to Markdown
    cout << "--- Exporting to Markdown ---\n";
    MarkdownExporter mdExp;
    for (DocumentElement* elem : document) {
        elem->accept(&mdExp);
    }
    cout << mdExp.getMarkdown() << "\n";

    // Export to Plain Text
    cout << "--- Exporting to Plain Text ---\n";
    PlainTextExporter txtExp;
    for (DocumentElement* elem : document) {
        elem->accept(&txtExp);
    }
    cout << txtExp.getText() << "\n";

    // Cleanup
    for (DocumentElement* elem : document) {
        delete elem;
    }
}

void demonstrateShoppingCart() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 4: SHOPPING CART          ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create products
    vector<Product*> cart;
    cart.push_back(new Book("Design Patterns", 45.00, "Gang of Four"));
    cart.push_back(new Electronics("Laptop", 1200.00, 2));
    cart.push_back(new Clothing("T-Shirt", 25.00, "L"));
    cart.push_back(new Book("Clean Code", 40.00, "Robert Martin"));

    // Calculate taxes
    cout << "--- Tax Calculation ---\n";
    TaxCalculator taxCalc;
    for (Product* product : cart) {
        product->accept(&taxCalc);
    }
    cout << "\nTotal Tax: $" << taxCalc.getTotalTax() << "\n";

    // Calculate discounts
    cout << "\n--- Discount Calculation ---\n";
    DiscountCalculator discCalc;
    for (Product* product : cart) {
        product->accept(&discCalc);
    }
    cout << "\nTotal Discount: $" << discCalc.getTotalDiscount() << "\n";

    // Calculate final price
    double subtotal = 0;
    for (Product* product : cart) {
        subtotal += product->getPrice();
    }
    double finalPrice = subtotal + taxCalc.getTotalTax() - discCalc.getTotalDiscount();

    cout << "\n--- Order Summary ---\n";
    cout << "Subtotal:  $" << subtotal << "\n";
    cout << "Tax:      +$" << taxCalc.getTotalTax() << "\n";
    cout << "Discount: -$" << discCalc.getTotalDiscount() << "\n";
    cout << "─────────────────\n";
    cout << "Total:     $" << finalPrice << "\n";

    // Cleanup
    for (Product* product : cart) {
        delete product;
    }
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "VISITOR PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateShapeVisitor();
    demonstrateEmployeeVisitor();
    demonstrateDocumentVisitor();
    demonstrateShoppingCart();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF VISITOR PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Open/Closed: Add operations without modifying elements\n";
    cout << "✅ Single Responsibility: Operations grouped in visitor\n";
    cout << "✅ Easy to add new operations: Just create new visitor\n";
    cout << "✅ Accumulation: Visitor can accumulate state during traversal\n";
    cout << "✅ Related operations: Groups related operations together\n";
    cout << "✅ Visiting across hierarchies: Works with different class types\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE VISITOR\n";
    cout << "========================================\n";
    cout << "1. Need to perform operations across unrelated classes\n";
    cout << "2. Object structure is stable but operations change often\n";
    cout << "3. Want to avoid polluting classes with many operations\n";
    cout << "4. Need to accumulate state during traversal\n";
    cout << "5. Want to group related operations in one place\n";
    cout << "6. Classes have different interfaces but need common operations\n";

    cout << "\n========================================\n";
    cout << "VISITOR vs STRATEGY\n";
    cout << "========================================\n";
    cout << "Visitor Pattern:\n";
    cout << "  • Operations distributed across class hierarchy\n";
    cout << "  • Double dispatch (runtime polymorphism twice)\n";
    cout << "  • Used for operations on object structures\n";
    cout << "  • Example: Export shapes to different formats\n";
    cout << "\nStrategy Pattern:\n";
    cout << "  • Single algorithm encapsulated\n";
    cout << "  • Single dispatch (runtime polymorphism once)\n";
    cout << "  • Used for algorithm selection\n";
    cout << "  • Example: Different payment methods\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• Compiler AST traversal (syntax analysis, optimization)\n";
    cout << "• Document export (HTML, PDF, Markdown)\n";
    cout << "• Shopping cart calculations (tax, discount, shipping)\n";
    cout << "• File system operations (size calc, search, backup)\n";
    cout << "• Graphics rendering (draw, bounds calc, hit test)\n";
    cout << "• Report generation from data structures\n";
    cout << "• Code analysis and refactoring tools\n";
    cout << "• XML/JSON processing and transformation\n";
    cout << "• Game entity systems (rendering, physics, AI)\n";
    cout << "• UI component traversal (layout, rendering)\n";
    cout << "• Database query optimization\n";
    cout << "• 3D scene graph operations\n";

    cout << "\n========================================\n";
    cout << "KEY COMPONENTS\n";
    cout << "========================================\n";
    cout << "1. Visitor Interface: Declares visit methods for each element\n";
    cout << "2. Concrete Visitors: Implement operations on elements\n";
    cout << "3. Element Interface: Declares accept(visitor) method\n";
    cout << "4. Concrete Elements: Implement accept by calling visitor\n";
    cout << "5. Object Structure: Collection of elements to visit\n";

    cout << "\n========================================\n";
    cout << "DOUBLE DISPATCH\n";
    cout << "========================================\n";
    cout << "What is Double Dispatch:\n";
    cout << "  • Operation depends on both:\n";
    cout << "    1. Type of element (Circle, Rectangle)\n";
    cout << "    2. Type of visitor (AreaCalculator, XMLExporter)\n";
    cout << "  • Achieved through two virtual calls\n";
    cout << "\nHow it works:\n";
    cout << "  1. Client: shape->accept(visitor)\n";
    cout << "  2. Element: visitor->visitCircle(this)\n";
    cout << "  3. Visitor: Execute operation on Circle\n";
    cout << "\nWhy needed:\n";
    cout << "  • C++ has single dispatch (only receiver type matters)\n";
    cout << "  • Visitor simulates double dispatch\n";
    cout << "  • Both element and visitor types determine behavior\n";

    cout << "\n========================================\n";
    cout << "STRUCTURE\n";
    cout << "========================================\n";
    cout << "┌─────────────────┐\n";
    cout << "│     Client      │\n";
    cout << "└────────┬────────┘\n";
    cout << "         │ uses\n";
    cout << "         ▼\n";
    cout << "┌─────────────────┐       ┌─────────────────┐\n";
    cout << "│    Visitor      │       │    Element      │\n";
    cout << "│  (Interface)    │       │  (Interface)    │\n";
    cout << "│                 │       │                 │\n";
    cout << "│ + visitA()      │◄──────│ + accept(v)     │\n";
    cout << "│ + visitB()      │       └─────────────────┘\n";
    cout << "└─────────────────┘              △\n";
    cout << "         △                       │\n";
    cout << "         │              ┌────────┴────────┐\n";
    cout << "┌────────┴────────┐    │                 │\n";
    cout << "│                 │    │                 │\n";
    cout << "│  ConcreteV1     │    │  ConcreteA      │\n";
    cout << "│                 │    │                 │\n";
    cout << "│ + visitA()      │    │ + accept(v) {   │\n";
    cout << "│ + visitB()      │    │     v.visitA()  │\n";
    cout << "└─────────────────┘    │   }             │\n";
    cout << "                       └─────────────────┘\n";

    cout << "\n========================================\n";
    cout << "ADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Easy to add new operations (new visitor)\n";
    cout << "• Groups related operations in one class\n";
    cout << "• Can accumulate state during traversal\n";
    cout << "• Separates algorithm from object structure\n";
    cout << "• Follows Single Responsibility Principle\n";
    cout << "• Follows Open/Closed Principle (for operations)\n";
    cout << "• Clean separation of concerns\n";

    cout << "\n========================================\n";
    cout << "DISADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Hard to add new element types\n";
    cout << "• Breaks encapsulation (visitor needs element internals)\n";
    cout << "• Complex pattern to understand\n";
    cout << "• Requires updating all visitors when element changes\n";
    cout << "• Can lead to circular dependencies\n";
    cout << "• May violate Law of Demeter\n";

    cout << "\n========================================\n";
    cout << "BEST PRACTICES\n";
    cout << "========================================\n";
    cout << "• Use when element hierarchy is stable\n";
    cout << "• Group related operations in same visitor\n";
    cout << "• Consider providing default implementations\n";
    cout << "• Document visitor responsibilities clearly\n";
    cout << "• Use const correctness appropriately\n";
    cout << "• Consider acyclic visitor for flexibility\n";
    cout << "• Keep visitor interface focused\n";
    cout << "• Test each visitor independently\n";

    cout << "\n========================================\n";
    cout << "IMPLEMENTATION VARIATIONS\n";
    cout << "========================================\n";
    cout << "Classic Visitor:\n";
    cout << "  • Visit method for each concrete element\n";
    cout << "  • Type-safe but rigid\n";
    cout << "  • Example: All examples above\n";
    cout << "\nReflective Visitor:\n";
    cout << "  • Single visit method using RTTI\n";
    cout << "  • More flexible but less type-safe\n";
    cout << "  • Uses dynamic_cast or typeid\n";
    cout << "\nAcyclic Visitor:\n";
    cout << "  • Visitor and elements don't know each other\n";
    cout << "  • Uses multiple inheritance\n";
    cout << "  • More flexible, can add elements\n";
    cout << "\nExternal Visitor:\n";
    cout << "  • Visitor doesn't need accept method\n";
    cout << "  • Works with existing classes\n";
    cout << "  • Less intrusive\n";

    cout << "\n========================================\n";
    cout << "WHEN NOT TO USE VISITOR\n";
    cout << "========================================\n";
    cout << "• Element hierarchy changes frequently\n";
    cout << "• Only one or two operations needed\n";
    cout << "• Elements can't expose internal state\n";
    cout << "• Operations are tightly coupled to elements\n";
    cout << "• Simple switch/if statements would suffice\n";
    cout << "• Performance is critical (virtual call overhead)\n";

    cout << "\n========================================\n";
    cout << "COMMON ISSUES & SOLUTIONS\n";
    cout << "========================================\n";
    cout << "Circular Dependencies:\n";
    cout << "  • Issue: Visitor and elements include each other\n";
    cout << "  • Solution: Forward declarations, separate headers\n";
    cout << "\nAdding New Elements:\n";
    cout << "  • Issue: Must update all visitors\n";
    cout << "  • Solution: Consider acyclic visitor, default handlers\n";
    cout << "\nBreaking Encapsulation:\n";
    cout << "  • Issue: Visitor needs element internals\n";
    cout << "  • Solution: Provide getter methods, friend classes\n";
    cout << "\nComplex Hierarchies:\n";
    cout << "  • Issue: Too many visit methods\n";
    cout << "  • Solution: Group similar elements, use composite\n";

    cout << "\n========================================\n";
    cout << "VISITOR WITH OTHER PATTERNS\n";
    cout << "========================================\n";
    cout << "Visitor + Composite:\n";
    cout << "  • Visit composite structures\n";
    cout << "  • Traverse tree/graph structures\n";
    cout << "  • Example: File system traversal\n";
    cout << "\nVisitor + Iterator:\n";
    cout << "  • Iterate through collection\n";
    cout << "  • Apply visitor to each element\n";
    cout << "  • Clean separation of concerns\n";
    cout << "\nVisitor + Interpreter:\n";
    cout << "  • Visitor for AST traversal\n";
    cout << "  • Interpreter evaluates expressions\n";
    cout << "  • Example: Compiler design\n";

    cout << "\n========================================\n";
    cout << "RELATED PATTERNS\n";
    cout << "========================================\n";
    cout << "Composite:\n";
    cout << "  • Often used together\n";
    cout << "  • Visitor traverses composite structure\n";
    cout << "  • Composite handles hierarchy, Visitor handles operations\n";
    cout << "\nIterator:\n";
    cout << "  • Both traverse structures\n";
    cout << "  • Iterator focuses on access\n";
    cout << "  • Visitor focuses on operations\n";
    cout << "\nStrategy:\n";
    cout << "  • Both encapsulate algorithms\n";
    cout << "  • Strategy for single object\n";
    cout << "  • Visitor for object structure\n";

    cout << "\n========================================\n";
    cout << "TESTING VISITORS\n";
    cout << "========================================\n";
    cout << "Unit Tests:\n";
    cout << "  • Test each visitor independently\n";
    cout << "  • Mock element objects\n";
    cout << "  • Verify correct visit methods called\n";
    cout << "\nIntegration Tests:\n";
    cout << "  • Test visitor with real elements\n";
    cout << "  • Verify traversal order\n";
    cout << "  • Test accumulated state\n";
    cout << "\nEdge Cases:\n";
    cout << "  • Empty structures\n";
    cout << "  • Single element\n";
    cout << "  • Deep nesting\n";
    cout << "  • Circular references\n";

    cout << "\n========================================\n";
    cout << "PERFORMANCE CONSIDERATIONS\n";
    cout << "========================================\n";
    cout << "Virtual Call Overhead:\n";
    cout << "  • Two virtual calls per operation\n";
    cout << "  • May impact hot paths\n";
    cout << "  • Profile before optimizing\n";
    cout << "\nCache Locality:\n";
    cout << "  • Visitor may jump between classes\n";
    cout << "  • Poor cache performance possible\n";
    cout << "  • Consider data-oriented design\n";
    cout << "\nAlternatives:\n";
    cout << "  • Function pointers for simple cases\n";
    cout << "  • std::variant + std::visit (C++17)\n";
    cout << "  • Virtual methods in elements (if stable operations)\n";

    cout << "\n========================================\n";
    cout << "MODERN C++ ALTERNATIVES\n";
    cout << "========================================\n";
    cout << "std::variant + std::visit:\n";
    cout << "  • Type-safe union\n";
    cout << "  • Compile-time visitor\n";
    cout << "  • No virtual calls\n";
    cout << "  • Example: std::variant<Circle, Rectangle>\n";
    cout << "\nLambdas:\n";
    cout << "  • Inline visitor operations\n";
    cout << "  • Less boilerplate\n";
    cout << "  • Good for one-off operations\n";
    cout << "\nConcepts (C++20):\n";
    cout << "  • Constrain visitor types\n";
    cout << "  • Better error messages\n";
    cout << "  • Compile-time checking\n";

    cout << "\n========================================\n";
    cout << "VISITOR IN REAL SYSTEMS\n";
    cout << "========================================\n";
    cout << "Compilers:\n";
    cout << "  • AST traversal\n";
    cout << "  • Type checking\n";
    cout << "  • Code generation\n";
    cout << "  • Optimization passes\n";
    cout << "\nGraphics Systems:\n";
    cout << "  • Scene graph traversal\n";
    cout << "  • Rendering\n";
    cout << "  • Collision detection\n";
    cout << "  • Bounding box calculation\n";
    cout << "\nDocument Processing:\n";
    cout << "  • Format conversion\n";
    cout << "  • Validation\n";
    cout << "  • Search and replace\n";
    cout << "  • Statistics gathering\n";

    cout << "\n========================================\n";
    cout << "SUMMARY\n";
    cout << "========================================\n";
    cout << "Use Visitor when:\n";
    cout << "  ✓ Need to add many unrelated operations\n";
    cout << "  ✓ Element hierarchy is stable\n";
    cout << "  ✓ Operations change more than structure\n";
    cout << "  ✓ Need to accumulate state across elements\n";
    cout << "\nAvoid Visitor when:\n";
    cout << "  ✗ Element hierarchy changes frequently\n";
    cout << "  ✗ Operations are tightly coupled to elements\n";
    cout << "  ✗ Simple solutions would work\n";
    cout << "  ✗ Performance is critical\n";

    return 0;
}
