#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <print>

// ============================================================
// FILE AND STREAM in C++23
// ============================================================
// Topics covered:
//   1.  Writing to a file        — ofstream
//   2.  Reading from a file      — ifstream
//   3.  Read and Write           — fstream
//   4.  getline — read full line — std::getline
//   5.  getline — char array     — istream::getline
//   6.  String streams           — ostringstream / istringstream
//   7.  Binary file I/O          — ios::binary
//   8.  File existence / info    — std::filesystem
//   9.  Appending to a file      — ios::app
//  10.  Error handling           — failbit, badbit, eof
// ============================================================

namespace fs = std::filesystem;

// Helper — print file contents
void printFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) { std::println("  [cannot open {}]", path); return; }
    std::string line;
    while (std::getline(f, line))
        std::println("  | {}", line);
}


int main()
{
    // -------------------------------------------------------
    std::println("=====================================================");
    std::println("Example 1: Writing to a file — ofstream");
    std::println("=====================================================");

    {
        // ofstream — output file stream (creates or truncates)
        std::ofstream outFile("testout.txt");

        if (!outFile) {
            std::println(stderr, "ERROR: could not open testout.txt for writing");
            return 1;
        }

        // Write with << operator — same as std::cout
        outFile << "Line 1: Hello from C++23\n";
        outFile << "Line 2: Writing to a file\n";
        outFile << "Line 3: ofstream example\n";

        // Write formatted data
        outFile << "Name: Kostas\n";
        outFile << "Port: " << 8080 << "\n";
        outFile << "Pi:   " << 3.14159 << "\n";

        std::println("Written to testout.txt");
    }   // ofstream destructor closes the file automatically

    printFile("testout.txt");


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 2: Reading from a file — ifstream");
    std::println("=====================================================");

    {
        std::ifstream inFile("testout.txt");

        if (!inFile) {
            std::println(stderr, "ERROR: could not open testout.txt for reading");
            return 1;
        }

        std::println("Reading line by line:");
        std::string line;
        int lineNum = 1;
        while (std::getline(inFile, line)) {
            std::println("  [{}] {}", lineNum++, line);
        }

        std::println("");
        std::println("Reading word by word:");
        // Reset to beginning
        inFile.clear();
        inFile.seekg(0);

        std::string word;
        int count = 0;
        while (inFile >> word && count < 6) {
            std::print("  '{}' ", word);
            ++count;
        }
        std::println("...");
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 3: Read and Write — fstream");
    std::println("=====================================================");

    {
        // fstream — both read and write
        std::fstream rwFile("rw_test.txt",
                            std::ios::in | std::ios::out |
                                std::ios::trunc);   // create fresh

        if (!rwFile) {
            std::println(stderr, "ERROR: could not open rw_test.txt");
            return 1;
        }

        // Write
        rwFile << "Alpha\nBeta\nGamma\nDelta\n";

        // Seek back to beginning and read
        rwFile.seekg(0);
        std::println("Read back:");
        std::string line;
        while (std::getline(rwFile, line))
            std::println("  {}", line);

        // Seek to end and append more
        rwFile.clear();
        rwFile.seekp(0, std::ios::end);
        rwFile << "Epsilon\n";

        // Read from beginning again
        rwFile.seekg(0);
        rwFile.clear();
        std::println("After appending Epsilon:");
        while (std::getline(rwFile, line))
            std::println("  {}", line);
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 4: getline — read full line (string)");
    std::println("=====================================================");

    {
        // Write a file with comma-separated values
        {
            std::ofstream f("csv_test.txt");
            f << "Kostas,41,Hannover\n";
            f << "Andre,28,Berlin\n";
            f << "Maria,35,München\n";
        }

        std::ifstream f("csv_test.txt");
        std::string line;

        std::println("Reading CSV with getline:");
        while (std::getline(f, line)) {
            // Parse comma-separated manually
            std::istringstream ss(line);
            std::string name, age, city;
            std::getline(ss, name, ',');
            std::getline(ss, age,  ',');
            std::getline(ss, city, ',');
            std::println("  name={} age={} city={}", name, age, city);
        }

        std::println("");
        std::println("getline with custom delimiter:");
        f.clear(); f.seekg(0);
        std::string token;
        while (std::getline(f, token, ','))
            std::print("  [{}]", token);
        std::println("");
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 5: getline — char array (C-style)");
    std::println("=====================================================");

    {
        std::ifstream f("testout.txt");
        char buffer[64];

        std::println("Reading with char buffer (max 63 chars per line):");
        while (f.getline(buffer, sizeof(buffer))) {
            std::println("  '{}'", buffer);
        }
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 6: String streams — ostringstream / istringstream");
    std::println("=====================================================");

    {
        // ostringstream — build a string like writing to a stream
        std::ostringstream oss;
        oss << "Job: " << "JOB-001";
        oss << " | Port: " << 8080;
        oss << " | Queue: " << "QUEUE-A";
        std::string result = oss.str();
        std::println("ostringstream result: {}", result);

        // Format numbers with precision
        std::ostringstream numStream;
        numStream.precision(4);
        numStream << std::fixed << 3.14159265;
        std::println("formatted double: {}", numStream.str());

        // istringstream — parse a string like reading from a stream
        std::string data = "42 3.14 hello true";
        std::istringstream iss(data);

        int    i; double d; std::string s; bool b;
        iss >> i >> d >> s >> b;
        std::println("parsed: int={} double={} string={} bool={}", i, d, s, b);

        // Parse CSV line
        std::string csv = "Alice,30,Engineer";
        std::istringstream csvStream(csv);
        std::string token;
        std::print("CSV tokens: ");
        while (std::getline(csvStream, token, ','))
            std::print("[{}] ", token);
        std::println("");
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 7: Binary file I/O");
    std::println("=====================================================");

    {
        struct Record {
            int    id;
            double value;
            char   label[16];
        };

        // Write binary
        {
            std::ofstream f("binary.bin", std::ios::binary);
            Record r1 { 1, 3.14,  "alpha" };
            Record r2 { 2, 2.71,  "beta"  };
            Record r3 { 3, 1.414, "gamma" };
            f.write(reinterpret_cast<const char*>(&r1), sizeof(r1));
            f.write(reinterpret_cast<const char*>(&r2), sizeof(r2));
            f.write(reinterpret_cast<const char*>(&r3), sizeof(r3));
            std::println("Written {} binary records", 3);
        }

        // Read binary
        {
            std::ifstream f("binary.bin", std::ios::binary);
            Record r;
            std::println("Binary records read:");
            while (f.read(reinterpret_cast<char*>(&r), sizeof(r)))
                std::println("  id={} value={:.3f} label={}", r.id, r.value, r.label);
        }
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 8: std::filesystem — file info");
    std::println("=====================================================");

    {
        std::string path = "testout.txt";

        std::println("exists:        {}", fs::exists(path));
        std::println("is_regular:    {}", fs::is_regular_file(path));
        std::println("file_size:     {} bytes", fs::file_size(path));
        std::println("absolute path: {}", fs::absolute(path).string());

        // List current directory
        std::println("");
        std::println("Files in current directory:");
        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.is_regular_file())
                std::println("  {} ({} bytes)",
                             entry.path().filename().string(),
                             entry.file_size());
        }
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 9: Appending to a file — ios::app");
    std::println("=====================================================");

    {
        std::string logFile = "log.txt";

        // Write initial content
        {
            std::ofstream f(logFile);
            f << "[INFO]  Application started\n";
        }

        // Append — does NOT truncate existing content
        auto logEntry = [&](const std::string& level, const std::string& msg) {
            std::ofstream f(logFile, std::ios::app);
            f << "[" << level << "] " << msg << '\n';
        };

        logEntry("INFO",  "Job JOB-001 started");
        logEntry("WARN",  "Buffer usage 85%");
        logEntry("ERROR", "Connection timeout");
        logEntry("INFO",  "Reconnecting...");

        std::println("Log file contents:");
        printFile(logFile);
    }


    // -------------------------------------------------------
    std::println("");
    std::println("=====================================================");
    std::println("Example 10: Error handling — stream state flags");
    std::println("=====================================================");

    {
        // Non-existent file
        std::ifstream badFile("nonexistent.txt");
        std::println("bad file good():  {}", badFile.good());
        std::println("bad file fail():  {}", badFile.fail());
        std::println("bad file is_open: {}", badFile.is_open());

        // Reading past EOF
        std::ifstream f("testout.txt");
        std::string line;
        while (std::getline(f, line)) {}  // read all
        std::println("");
        std::println("after all lines read:");
        std::println("  eof():  {}", f.eof());
        std::println("  fail(): {}", f.fail());
        std::println("  good(): {}", f.good());

        // clear() + seekg() to re-read
        f.clear();
        f.seekg(0);
        std::getline(f, line);
        std::println("after clear+seekg, first line: '{}'", line);

        // Exceptions on stream errors
        std::ifstream safeFile("testout.txt");
        safeFile.exceptions(std::ios::badbit);  // throw on bad I/O
        try {
            std::string l;
            while (std::getline(safeFile, l))
                std::println("  ok: '{}'", l);
        } catch (const std::ios_base::failure& e) {
            std::println("stream error: {}", e.what());
        }
    }


    // Cleanup temp files
    for (const auto& f : {"testout.txt","rw_test.txt","csv_test.txt",
                          "binary.bin","log.txt"})
        fs::remove(f);

    std::println("");
    std::println("=== File and Stream complete ===");
    return 0;
}
