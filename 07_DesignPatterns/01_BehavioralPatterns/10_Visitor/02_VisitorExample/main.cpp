#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <print>

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
    virtual std::string getName() const = 0;
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

    std::string getName() const override {
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

    std::string getName() const override {
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

    std::string getName() const override {
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
        std::println("📐 Circle area: {} sq units", area);
        totalArea += area;
    }

    void visitRectangle(Rectangle* rect) override {
        double area = rect->getWidth() * rect->getHeight();
        std::println("📐 Rectangle area: {} sq units", area);
        totalArea += area;
    }

    void visitTriangle(Triangle* tri) override {
        double area = 0.5 * tri->getBase() * tri->getHeight();
        std::println("📐 Triangle area: {} sq units", area);
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
        std::println("📏 Circle perimeter: {} units", perimeter);
        totalPerimeter += perimeter;
    }

    void visitRectangle(Rectangle* rect) override {
        double perimeter = 2 * (rect->getWidth() + rect->getHeight());
        std::println("📏 Rectangle perimeter: {} units", perimeter);
        totalPerimeter += perimeter;
    }

    void visitTriangle(Triangle* tri) override {
        // For simplicity, assuming equilateral triangle
        double side = tri->getBase();
        double perimeter = 3 * side;
        std::println("📏 Triangle perimeter: {} units", perimeter);
        totalPerimeter += perimeter;
    }

    double getTotalPerimeter() const { return totalPerimeter; }
};

/**
 * Concrete Visitor: XML Exporter
 */
class XMLExporter : public ShapeVisitor {
private:
    std::string xml;

public:
    XMLExporter() : xml("<shapes>\n") {}

    void visitCircle(Circle* circle) override {
        xml += "  <circle radius=\"" + std::to_string(circle->getRadius()) + "\"/>\n";
    }

    void visitRectangle(Rectangle* rect) override {
        xml += "  <rectangle width=\"" + std::to_string(rect->getWidth()) +
               "\" height=\"" + std::to_string(rect->getHeight()) + "\"/>\n";
    }

    void visitTriangle(Triangle* tri) override {
        xml += "  <triangle base=\"" + std::to_string(tri->getBase()) +
               "\" height=\"" + std::to_string(tri->getHeight()) + "\"/>\n";
    }

    std::string getXML() {
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
    std::string name;
    double baseSalary;

public:
    Employee(const std::string& n, double salary) : name(n), baseSalary(salary) {}
    virtual ~Employee() {}

    std::string getName() const { return name; }
    double getBaseSalary() const { return baseSalary; }

    virtual void accept(EmployeeVisitor* visitor) = 0;
    virtual std::string getRole() const = 0;
};

/**
 * Concrete Element: Manager
 */
class Manager : public Employee {
private:
    int teamSize;

public:
    Manager(const std::string& n, double salary, int team)
        : Employee(n, salary), teamSize(team) {}

    int getTeamSize() const { return teamSize; }

    void accept(EmployeeVisitor* visitor) override {
        visitor->visitManager(this);
    }

    std::string getRole() const override {
        return "Manager";
    }
};

/**
 * Concrete Element: Developer
 */
class Developer : public Employee {
private:
    std::string programmingLanguage;

public:
    Developer(const std::string& n, double salary, const std::string& lang)
        : Employee(n, salary), programmingLanguage(lang) {}

    std::string getProgrammingLanguage() const { return programmingLanguage; }

    void accept(EmployeeVisitor* visitor) override {
        visitor->visitDeveloper(this);
    }

    std::string getRole() const override {
        return "Developer";
    }
};

/**
 * Concrete Element: Designer
 */
class Designer : public Employee {
private:
    std::string designTool;

public:
    Designer(const std::string& n, double salary, const std::string& tool)
        : Employee(n, salary), designTool(tool) {}

    std::string getDesignTool() const { return designTool; }

    void accept(EmployeeVisitor* visitor) override {
        visitor->visitDesigner(this);
    }

    std::string getRole() const override {
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
        std::println("💰 [{}] Manager salary: ${} (base: ${} + team bonus: ${})",
            manager->getName(), salary, manager->getBaseSalary(), bonus);
        totalSalary += salary;
    }

    void visitDeveloper(Developer* dev) override {
        // Developers get skill bonus
        double bonus = 5000;
        double salary = dev->getBaseSalary() + bonus;
        std::println("💰 [{}] Developer salary: ${} (base: ${} + skill bonus: ${})",
            dev->getName(), salary, dev->getBaseSalary(), bonus);
        totalSalary += salary;
    }

    void visitDesigner(Designer* designer) override {
        // Designers get tool proficiency bonus
        double bonus = 3000;
        double salary = designer->getBaseSalary() + bonus;
        std::println("💰 [{}] Designer salary: ${} (base: ${} + tool bonus: ${})",
            designer->getName(), salary, designer->getBaseSalary(), bonus);
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
        std::println("📊 [{}] Manager Performance:", manager->getName());
        std::println("   • Leadership: Excellent");
        std::println("   • Team Management: Managing {} members", manager->getTeamSize());
        std::println("   • Rating: ⭐⭐⭐⭐⭐");
    }

    void visitDeveloper(Developer* dev) override {
        std::println("📊 [{}] Developer Performance:", dev->getName());
        std::println("   • Technical Skills: {}", dev->getProgrammingLanguage());
        std::println("   • Code Quality: High");
        std::println("   • Rating: ⭐⭐⭐⭐");
    }

    void visitDesigner(Designer* designer) override {
        std::println("📊 [{}] Designer Performance:", designer->getName());
        std::println("   • Design Tool: {}", designer->getDesignTool());
        std::println("   • Creativity: Outstanding");
        std::println("   • Rating: ⭐⭐⭐⭐⭐");
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
    std::string text;

public:
    Paragraph(const std::string& t) : text(t) {}

    std::string getText() const { return text; }

    void accept(DocumentVisitor* visitor) override {
        visitor->visitParagraph(this);
    }
};

/**
 * Concrete Element: Heading
 */
class Heading : public DocumentElement {
private:
    std::string text;
    int level;

public:
    Heading(const std::string& t, int lvl) : text(t), level(lvl) {}

    std::string getText() const { return text; }
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
    std::string url;
    std::string altText;

public:
    Image(const std::string& u, const std::string& alt) : url(u), altText(alt) {}

    std::string getUrl() const { return url; }
    std::string getAltText() const { return altText; }

    void accept(DocumentVisitor* visitor) override {
        visitor->visitImage(this);
    }
};

/**
 * Concrete Visitor: HTML Exporter
 */
class HTMLExporter : public DocumentVisitor {
private:
    std::string html;

public:
    void visitParagraph(Paragraph* para) override {
        html += "<p>" + para->getText() + "</p>\n";
    }

    void visitHeading(Heading* heading) override {
        std::string tag = "h" + std::to_string(heading->getLevel());
        html += "<" + tag + ">" + heading->getText() + "</" + tag + ">\n";
    }

    void visitImage(Image* image) override {
        html += "<img src=\"" + image->getUrl() + "\" alt=\"" +
                image->getAltText() + "\" />\n";
    }

    std::string getHTML() const { return html; }
};

/**
 * Concrete Visitor: Markdown Exporter
 */
class MarkdownExporter : public DocumentVisitor {
private:
    std::string markdown;

public:
    void visitParagraph(Paragraph* para) override {
        markdown += para->getText() + "\n\n";
    }

    void visitHeading(Heading* heading) override {
        std::string hashes(heading->getLevel(), '#');
        markdown += hashes + " " + heading->getText() + "\n\n";
    }

    void visitImage(Image* image) override {
        markdown += "![" + image->getAltText() + "](" + image->getUrl() + ")\n\n";
    }

    std::string getMarkdown() const { return markdown; }
};

/**
 * Concrete Visitor: Plain Text Exporter
 */
class PlainTextExporter : public DocumentVisitor {
private:
    std::string text;

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

    std::string getText() const { return text; }
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
    std::string name;
    double price;

public:
    Product(const std::string& n, double p) : name(n), price(p) {}
    virtual ~Product() {}

    std::string getName() const { return name; }
    double getPrice() const { return price; }

    virtual void accept(ProductVisitor* visitor) = 0;
};

/**
 * Concrete Element: Book
 */
class Book : public Product {
private:
    std::string author;

public:
    Book(const std::string& n, double p, const std::string& a)
        : Product(n, p), author(a) {}

    std::string getAuthor() const { return author; }

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
    Electronics(const std::string& n, double p, int warranty)
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
    std::string size;

public:
    Clothing(const std::string& n, double p, const std::string& s)
        : Product(n, p), size(s) {}

    std::string getSize() const { return size; }

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
        std::println("📚 [{}] Tax: ${} (tax-exempt)", book->getName(), tax);
        totalTax += tax;
    }

    void visitElectronics(Electronics* electronics) override {
        double tax = electronics->getPrice() * 0.15;  // 15% tax
        std::println("💻 [{}] Tax: ${} (15%)", electronics->getName(), tax);
        totalTax += tax;
    }

    void visitClothing(Clothing* clothing) override {
        double tax = clothing->getPrice() * 0.08;  // 8% tax
        std::println("👕 [{}] Tax: ${} (8%)", clothing->getName(), tax);
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
        std::println("📚 [{}] Discount: ${} (10% off)", book->getName(), discount);
        totalDiscount += discount;
    }

    void visitElectronics(Electronics* electronics) override {
        double discount = electronics->getPrice() * 0.05;  // 5% discount
        std::println("💻 [{}] Discount: ${} (5% off)", electronics->getName(), discount);
        totalDiscount += discount;
    }

    void visitClothing(Clothing* clothing) override {
        double discount = clothing->getPrice() * 0.20;  // 20% discount
        std::println("👕 [{}] Discount: ${} (20% off)", clothing->getName(), discount);
        totalDiscount += discount;
    }

    double getTotalDiscount() const { return totalDiscount; }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateShapeVisitor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: SHAPE OPERATIONS       ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create shapes
    std::vector<Shape*> shapes;
    shapes.push_back(new Circle(5.0));
    shapes.push_back(new Rectangle(4.0, 6.0));
    shapes.push_back(new Triangle(3.0, 4.0));

    // Calculate areas
    std::println("--- Calculating Areas ---");
    AreaCalculator areaCalc;
    for (Shape* shape : shapes) {
        shape->accept(&areaCalc);
    }
    std::println("Total Area: {} sq units", areaCalc.getTotalArea());

    // Calculate perimeters
    std::println("");
    std::println("--- Calculating Perimeters ---");
    PerimeterCalculator perimCalc;
    for (Shape* shape : shapes) {
        shape->accept(&perimCalc);
    }
    std::println("Total Perimeter: {} units", perimCalc.getTotalPerimeter());

    // Export to XML
    std::println("");
    std::println("--- Exporting to XML ---");
    XMLExporter xmlExporter;
    for (Shape* shape : shapes) {
        shape->accept(&xmlExporter);
    }
    std::println("{}", xmlExporter.getXML());

    // Cleanup
    for (Shape* shape : shapes) {
        delete shape;
    }
}

void demonstrateEmployeeVisitor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: EMPLOYEE SYSTEM        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create employees
    std::vector<Employee*> employees;
    employees.push_back(new Manager("Alice", 80000, 5));
    employees.push_back(new Developer("Bob", 70000, "C++"));
    employees.push_back(new Developer("Charlie", 75000, "Python"));
    employees.push_back(new Designer("Diana", 65000, "Figma"));

    // Calculate salaries
    std::println("--- Salary Calculation ---");
    SalaryCalculator salaryCalc;
    for (Employee* emp : employees) {
        emp->accept(&salaryCalc);
    }
    std::println("");
    std::println("Total Payroll: ${}", salaryCalc.getTotalSalary());

    // Evaluate performance
    std::println("");
    std::println("--- Performance Evaluation ---");
    PerformanceEvaluator perfEval;
    for (Employee* emp : employees) {
        emp->accept(&perfEval);
        std::println("");
    }

    // Cleanup
    for (Employee* emp : employees) {
        delete emp;
    }
}

void demonstrateDocumentVisitor() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 3: DOCUMENT EXPORT        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create document elements
    std::vector<DocumentElement*> document;
    document.push_back(new Heading("Welcome to Visitor Pattern", 1));
    document.push_back(new Paragraph("The Visitor pattern allows adding new operations."));
    document.push_back(new Heading("Benefits", 2));
    document.push_back(new Paragraph("Easy to add new operations without modifying classes."));
    document.push_back(new Image("diagram.png", "Visitor Pattern Diagram"));

    // Export to HTML
    std::println("--- Exporting to HTML ---");
    HTMLExporter htmlExp;
    for (DocumentElement* elem : document) {
        elem->accept(&htmlExp);
    }
    std::println("{}", htmlExp.getHTML());

    // Export to Markdown
    std::println("--- Exporting to Markdown ---");
    MarkdownExporter mdExp;
    for (DocumentElement* elem : document) {
        elem->accept(&mdExp);
    }
    std::println("{}", mdExp.getMarkdown());

    // Export to Plain Text
    std::println("--- Exporting to Plain Text ---");
    PlainTextExporter txtExp;
    for (DocumentElement* elem : document) {
        elem->accept(&txtExp);
    }
    std::println("{}", txtExp.getText());

    // Cleanup
    for (DocumentElement* elem : document) {
        delete elem;
    }
}

void demonstrateShoppingCart() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 4: SHOPPING CART          ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create products
    std::vector<Product*> cart;
    cart.push_back(new Book("Design Patterns", 45.00, "Gang of Four"));
    cart.push_back(new Electronics("Laptop", 1200.00, 2));
    cart.push_back(new Clothing("T-Shirt", 25.00, "L"));
    cart.push_back(new Book("Clean Code", 40.00, "Robert Martin"));

    // Calculate taxes
    std::println("--- Tax Calculation ---");
    TaxCalculator taxCalc;
    for (Product* product : cart) {
        product->accept(&taxCalc);
    }
    std::println("");
    std::println("Total Tax: ${}", taxCalc.getTotalTax());

    // Calculate discounts
    std::println("");
    std::println("--- Discount Calculation ---");
    DiscountCalculator discCalc;
    for (Product* product : cart) {
        product->accept(&discCalc);
    }
    std::println("");
    std::println("Total Discount: ${}", discCalc.getTotalDiscount());

    // Calculate final price
    double subtotal = 0;
    for (Product* product : cart) {
        subtotal += product->getPrice();
    }
    double finalPrice = subtotal + taxCalc.getTotalTax() - discCalc.getTotalDiscount();

    std::println("");
    std::println("--- Order Summary ---");
    std::println("Subtotal:  ${}", subtotal);
    std::println("Tax:      +${}", taxCalc.getTotalTax());
    std::println("Discount: -${}", discCalc.getTotalDiscount());
    std::println("─────────────────");
    std::println("Total:     ${}", finalPrice);

    // Cleanup
    for (Product* product : cart) {
        delete product;
    }
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    std::println("========================================");
    std::println("VISITOR PATTERN DEMO");
    std::println("========================================");

    demonstrateShapeVisitor();
    demonstrateEmployeeVisitor();
    demonstrateDocumentVisitor();
    demonstrateShoppingCart();

    return 0;
}
