#include "Document.h"
#include <print>

int main() {
    // Create a document in the default Draft state
    Document doc("Sample content for state pattern demonstration");
    
    // Show initial state
    std::println("Initial state: {}", doc.getStateName());
    doc.render();
    std::println("");
    
    // Try to publish directly from Draft
    std::println("Attempting to publish:");
    doc.publish();
    std::println("Current state: {}", doc.getStateName());
    std::println("");
    
    // Now approve and publish
    std::println("Approving for publication:");
    doc.publish();
    std::println("Current state: {}", doc.getStateName());
    doc.render();
    std::println("");
    
    // Start a revision
    std::println("Starting a revision:");
    doc.review();
    std::println("Current state: {}", doc.getStateName());
    std::println("");
    
    return 0;
}