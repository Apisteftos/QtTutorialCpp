#include <iostream>
#include <string>
#include <print>

// ============================================================
// Only include the PUBLIC API header — not internal details
// ============================================================
#include "Package/API/package.h"

// ============================================================
// PACKAGE STRUCTURE — Organizing headers into directories
// ============================================================
// Topics covered:
//   1.  Why organize headers into directories
//   2.  Including via path: "Package/API/package.h"
//   3.  Public API vs Internal headers
//   4.  Namespace matches directory structure
//   5.  How real libraries (Qt, Boost) do this
// ============================================================

int main()
{
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Basic PackageClass usage\n";
    std::cout << "=====================================================\n";

    // Your original example — cleaned up
    Package::PackageClass pk1(23, "amazing");
    Package::PackageClass pk2(13, "awaiting");

    pk1.display();
    pk2.display();


    std::cout << "\n=====================================================\n";
    std::cout << "Example 2: PackageUtils — static utility methods\n";
    std::cout << "=====================================================\n";

    std::println("add(10, 5)        = {}", Package::PackageUtils::add(10, 5));
    std::println("isEmpty(\"\")        = {}", Package::PackageUtils::isEmpty(""));
    std::println("isEmpty(\"hello\")   = {}", Package::PackageUtils::isEmpty("hello"));
    std::println("version()         = {}", Package::PackageUtils::version());


    std::cout << "\n=====================================================\n";
    std::cout << "Example 3: Include path shows structure\n";
    std::cout << "=====================================================\n";

    std::cout << "  #include \"Package/API/package.h\"\n\n";
    std::cout << "  The path tells you:\n";
    std::cout << "    Package/   — the package name\n";
    std::cout << "    API/       — public-facing headers\n";
    std::cout << "    package.h  — the header file\n\n";
    std::cout << "  Internal headers (Package/Internal/) are NOT included\n";
    std::cout << "  Users never need to know about implementation details\n";


    std::cout << "\n=====================================================\n";
    std::cout << "Example 4: Namespace mirrors directory structure\n";
    std::cout << "=====================================================\n";

    std::cout << "  Directory:  Package/API/package.h\n";
    std::cout << "  Namespace:  namespace Package { ... }\n\n";
    std::cout << "  Usage:      Package::PackageClass\n";
    std::cout << "              Package::PackageUtils\n\n";
    std::cout << "  Convention: directory name = namespace name\n";
    std::cout << "  Makes code self-documenting — you know\n";
    std::cout << "  where any symbol comes from by its namespace\n";


    std::cout << "\n=====================================================\n";
    std::cout << "Example 5: How Qt does it\n";
    std::cout << "=====================================================\n";

    std::cout << "  Qt uses the same pattern:\n\n";
    std::cout << "  #include <QThread>        // Qt/QThread\n";
    std::cout << "  #include <QObject>        // Qt/QObject\n";
    std::cout << "  #include <QtCore/QString> // QtCore/QString\n\n";
    std::cout << "  Usage: QThread, QObject — no ::  (Qt uses Q prefix)\n\n";
    std::cout << "  Boost uses namespaces:\n";
    std::cout << "  #include <boost/filesystem.hpp>\n";
    std::cout << "  Usage: boost::filesystem::path\n";

    std::cout << "\n=== Package Structure complete ===\n";
    return 0;
}
