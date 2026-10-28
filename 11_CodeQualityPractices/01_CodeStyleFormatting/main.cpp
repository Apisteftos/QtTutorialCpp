#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <print>

// ============================================
// WHAT IS CODE STYLE & FORMATTING?
// ============================================

/*
CODE STYLE & FORMATTING:
    Consistent, readable way of writing code that makes it
    easy for humans to read and maintain!

WHY IT MATTERS:

1. READABILITY:
   - Code is read 10x more than written
   - Consistent style = easier to understand
   - Reduces cognitive load

2. MAINTAINABILITY:
   - Easier to spot errors
   - Faster to modify
   - Less context switching

3. COLLABORATION:
   - Team works together smoothly
   - Code reviews are faster
   - Onboarding is easier

4. PROFESSIONALISM:
   - Shows attention to detail
   - Demonstrates discipline
   - Industry standard

KEY ASPECTS:

1. NAMING CONVENTIONS:
   - Variables: camelCase or snake_case
   - Functions: camelCase or snake_case
   - Classes: PascalCase
   - Constants: UPPER_SNAKE_CASE
   - Private members: trailing underscore

2. INDENTATION:
   - Spaces (2 or 4) or tabs
   - Consistency is key!
   - Align code blocks clearly

3. BRACING STYLES:
   - K&R (same line)
   - Allman (next line)
   - Choose one, stick to it

4. SPACING:
   - Around operators
   - After commas
   - Between logical blocks

5. LINE LENGTH:
   - 80-120 characters max
   - Break long lines logically

6. FILE ORGANIZATION:
   - Header order
   - Function order
   - Logical grouping

GOLDEN RULE:
   "Be consistent! Pick a style and use it everywhere!"

TOOLS:
   • clang-format: Auto-format C++ code
   • Style guides: Google, LLVM, Mozilla
   • EditorConfig: Maintain consistency across editors
*/

// ============================================
// EXAMPLE 1: NAMING CONVENTIONS
// ============================================

void example1_naming_conventions() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: NAMING CONVENTIONS                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Inconsistent naming
    std::println("");
}

// ============================================
// EXAMPLE 2: BRACE STYLES
// ============================================

void example2_brace_styles() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: BRACE STYLES                        ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Inconsistent bracing
    std::println("");
}

// ============================================
// EXAMPLE 3: INDENTATION
// ============================================

void example3_indentation() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: INDENTATION                         ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Inconsistent indentation
    std::println("");
}

// ============================================
// EXAMPLE 4: SPACING & WHITESPACE
// ============================================

void example4_spacing() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: SPACING & WHITESPACE                ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Poor spacing
    std::println("");
    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");

}

// ============================================
// EXAMPLE 5: LINE LENGTH
// ============================================

void example5_line_length() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: LINE LENGTH                         ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Lines too long
    std::println("");
}

// ============================================
// EXAMPLE 6: FUNCTION & CLASS ORGANIZATION
// ============================================

void example6_organization() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: FUNCTION & CLASS ORGANIZATION       ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Poor organization
    std::println("");
    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");

}

// ============================================
// EXAMPLE 7: FILE ORGANIZATION & HEADERS
// ============================================

void example7_file_organization() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: FILE ORGANIZATION & HEADERS         ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Random header order
    std::println("");
    std::println("   ");
}

// ============================================
// EXAMPLE 8: COMMENT STYLE
// ============================================

void example8_comment_style() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: COMMENT STYLE                       ║");
    std::println("╚══════════════════════════════════════════════════╝");
    // ❌ BAD CODE: Poor comments
    std::println("");
    std::println("    ");
    std::println("    ");



}

// ============================================
// EXAMPLE 9: CONSISTENCY IS KEY
// ============================================

void example9_consistency() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: CONSISTENCY IS KEY                  ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 10: MODERN C++ STYLE PREFERENCES
// ============================================

void example10_modern_cpp_style() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 10: MODERN C++ STYLE PREFERENCES       ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 11: CLANG-FORMAT CONFIGURATION
// ============================================

void example11_clang_format() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 11: CLANG-FORMAT CONFIGURATION         ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 12: SUMMARY & BEST PRACTICES
// ============================================

void example12_summary() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 12: SUMMARY & BEST PRACTICES           ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║       CODE STYLE & FORMATTING - MASTERCLASS           ║");
    std::println("║                                                       ║");
    std::println("║    Professional Code Formatting Explained!            ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_naming_conventions();
    example2_brace_styles();
    example3_indentation();
    example4_spacing();
    example5_line_length();
    example6_organization();
    example7_file_organization();
    example8_comment_style();
    example9_consistency();
    example10_modern_cpp_style();
    example11_clang_format();
    example12_summary();

    return 0;
}
