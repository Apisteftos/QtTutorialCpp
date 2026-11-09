/*
 * std::filesystem (<filesystem>, C++17)
 *
 * Works inside a temporary directory that the program creates and removes.
 *
 * Compile: g++ -std=c++17 -Wall -Wextra main.cpp -o example
 */

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main() {
    // === EXAMPLE 1: path manipulation ===
    std::cout << "=== Example 1: path ===\n";
    fs::path file = "/data/reports/summary.final.txt";
    std::cout << "filename:    " << file.filename().string() << "\n";
    std::cout << "stem:        " << file.stem().string() << "\n";
    std::cout << "extension:   " << file.extension().string() << "\n";
    std::cout << "parent:      " << file.parent_path().string() << "\n";
    std::cout << "replaced:    " << fs::path(file).replace_extension(".csv").string() << "\n";
    std::cout << "joined:      " << (fs::path("/data") / "reports" / "x.txt").string() << "\n";

    // === EXAMPLE 2: create, write, inspect ===
    std::cout << "\n=== Example 2: files and directories ===\n";
    fs::path root = fs::temp_directory_path() / "cpp17_filesystem_example";
    fs::remove_all(root);
    fs::create_directories(root / "sub");
    {
        std::ofstream(root / "a.txt") << "hello";
        std::ofstream(root / "sub" / "b.txt") << "0123456789";
    }
    std::cout << "a.txt exists: " << std::boolalpha << fs::exists(root / "a.txt") << "\n";
    std::cout << "a.txt size:   " << fs::file_size(root / "a.txt") << " bytes\n";
    std::cout << "sub is dir:   " << fs::is_directory(root / "sub") << "\n";

    // === EXAMPLE 3: iterate ===
    std::cout << "\n=== Example 3: recursive iteration ===\n";
    std::vector<std::string> names;
    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (entry.is_regular_file()) {
            names.push_back(fs::relative(entry.path(), root).generic_string());
        }
    }
    std::sort(names.begin(), names.end());   // iteration order is not specified
    for (const auto& name : names) {
        std::cout << name << "\n";
    }

    // === EXAMPLE 4: errors without exceptions, copy and remove ===
    std::cout << "\n=== Example 4: error_code, copy, remove ===\n";
    std::error_code ec;
    [[maybe_unused]] auto size = fs::file_size(root / "missing.txt", ec);   // no exception: the error is stored in ec
    std::cout << "missing file: error code set: " << std::boolalpha << static_cast<bool>(ec) << "\n";
    fs::copy_file(root / "a.txt", root / "copy.txt");
    std::cout << "copy exists: " << fs::exists(root / "copy.txt") << "\n";
    std::cout << "removed " << fs::remove_all(root) << " entries\n";
    return 0;
}
