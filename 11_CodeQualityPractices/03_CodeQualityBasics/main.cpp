 #include <vector>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
#include <print>

// ============================================
// WHAT IS CODE QUALITY?
// ============================================

/*
CODE QUALITY:
    The measure of how well code is written in terms of
    maintainability, readability, reliability, and efficiency!

CHARACTERISTICS OF QUALITY CODE:

1. READABLE:
   - Clear, self-documenting
   - Easy to understand at first glance
   - Meaningful names

2. MAINTAINABLE:
   - Easy to modify and extend
   - Well-organized
   - Modular

3. RELIABLE:
   - Works correctly
   - Handles errors gracefully
   - Tested

4. EFFICIENT:
   - Performs well
   - Uses resources wisely
   - Not prematurely optimized

5. SIMPLE:
   - No unnecessary complexity
   - Clear logic
   - Direct approach

WHY CODE QUALITY MATTERS:

   💰 COST: Poor quality costs 3-10x more to maintain
   🐛 BUGS: Quality code has fewer defects
   ⏱️  TIME: Quality code is faster to modify
   😊 TEAM: Easier for team collaboration
   📈 SCALABILITY: Quality code scales better

CODE SMELLS:
    Warning signs that indicate potential problems!

CATEGORIES OF CODE SMELLS:

1. BLOATERS: Too big, too long
   - Long methods/functions
   - Large classes
   - Long parameter lists

2. OBJECT-ORIENTATION ABUSERS:
   - God objects (do everything)
   - Refused inheritance
   - Poor abstraction

3. CHANGE PREVENTERS:
   - Divergent change
   - Shotgun surgery (one change = many files)
   - Parallel inheritance

4. DISPENSABLES:
   - Dead code
   - Duplicate code
   - Speculative generality

5. COUPLERS:
   - High coupling
   - Feature envy
   - Inappropriate intimacy

METRICS TO MEASURE QUALITY:

   • Cyclomatic Complexity (< 10 is good)
   • Lines of Code per function (< 50 is good)
   • Code Coverage (> 80% is good)
   • Technical Debt Ratio
   • Coupling/Cohesion
   • Code Duplication %
*/

// ============================================
// EXAMPLE 1: MAGIC NUMBERS (Code Smell)
// ============================================

void example1_magic_numbers() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 1: MAGIC NUMBERS (Code Smell)          ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Magic numbers everywhere
    std::println("");
}

// ============================================
// EXAMPLE 2: LONG FUNCTIONS (Bloater Smell)
// ============================================

void example2_long_functions() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 2: LONG FUNCTIONS (Bloater Smell)      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: One giant function
    std::println("");
    std::println("    ");

}

// ============================================
// EXAMPLE 3: UNCLEAR NAMING (Readability Smell)
// ============================================

void example3_unclear_naming() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 3: UNCLEAR NAMING (Readability)        ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Cryptic names
    std::println("");
}

// ============================================
// EXAMPLE 4: GOD OBJECT (OO Abuse Smell)
// ============================================

void example4_god_object() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 4: GOD OBJECT (OO Abuse Smell)         ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: One class does everything
    std::println("");
    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");

}

// ============================================
// EXAMPLE 5: DEEP NESTING (Complexity Smell)
// ============================================

void example5_deep_nesting() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 5: DEEP NESTING (Complexity Smell)     ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Deeply nested conditions
    std::println("");
    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");

}

// ============================================
// EXAMPLE 6: CODE DUPLICATION (Dispensable Smell)
// ============================================

void example6_code_duplication() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 6: CODE DUPLICATION (DRY Violation)    ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Copy-pasted code
    std::println("");
}

// ============================================
// EXAMPLE 7: POOR ERROR HANDLING
// ============================================

void example7_poor_error_handling() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 7: POOR ERROR HANDLING                 ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Ignoring errors
    std::println("");
    std::println("    ");

    std::println("    ");

}

// ============================================
// EXAMPLE 8: COMMENTS AS CRUTCH
// ============================================

void example8_comments_as_crutch() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 8: COMMENTS AS CRUTCH (Code Smell)     ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Need comments to understand
    std::println("");
    std::println("    ");
    std::println("    ");
    std::println("    ");

}

// ============================================
// EXAMPLE 9: PREMATURE OPTIMIZATION
// ============================================

void example9_premature_optimization() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 9: PREMATURE OPTIMIZATION(Anti-Pattern)║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Overly optimized, hard to read
    std::println("");
    std::println("    ");

}

// ============================================
// EXAMPLE 10: GLOBAL STATE & COUPLING
// ============================================

void example10_global_state() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 10: GLOBAL STATE (Coupling Smell)      ║");
    std::println("╚══════════════════════════════════════════════════╝");

    // ❌ BAD CODE: Global variables everywhere
    std::println("");
    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");
    std::println("    ");

}

// ============================================
// EXAMPLE 11: CODE QUALITY METRICS
// ============================================

void example11_quality_metrics() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 11: CODE QUALITY METRICS               ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// EXAMPLE 12: CODE QUALITY CHECKLIST
// ============================================

void example12_quality_checklist() {
    std::println("");
    std::println("╔══════════════════════════════════════════════════╗");
    std::println("║   EXAMPLE 12: CODE QUALITY CHECKLIST             ║");
    std::println("╚══════════════════════════════════════════════════╝");

}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    std::println("╔═══════════════════════════════════════════════════════╗");
    std::println("║                                                       ║");
    std::println("║         CODE QUALITY BASICS - MASTERCLASS             ║");
    std::println("║                                                       ║");
    std::println("║    Understanding Quality Code & Code Smells!          ║");
    std::println("║                                                       ║");
    std::println("╚═══════════════════════════════════════════════════════╝");

    example1_magic_numbers();
    example2_long_functions();
    example3_unclear_naming();
    example4_god_object();
    example5_deep_nesting();
    example6_code_duplication();
    example7_poor_error_handling();
    example8_comments_as_crutch();
    example9_premature_optimization();
    example10_global_state();
    example11_quality_metrics();
    example12_quality_checklist();

    return 0;
}
