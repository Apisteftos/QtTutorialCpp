#include "Document.h"
#include <iostream>

int main() {
    // Create a document in the default Draft state
    Document doc("Sample content for state pattern demonstration");
    
    // Show initial state
    std::cout << "Initial state: " << doc.getStateName() << std::endl;
    doc.render();
    std::cout << std::endl;
    
    // Try to publish directly from Draft
    std::cout << "Attempting to publish:" << std::endl;
    doc.publish();
    std::cout << "Current state: " << doc.getStateName() << std::endl;
    std::cout << std::endl;
    
    // Now approve and publish
    std::cout << "Approving for publication:" << std::endl;
    doc.publish();
    std::cout << "Current state: " << doc.getStateName() << std::endl;
    doc.render();
    std::cout << std::endl;
    
    // Start a revision
    std::cout << "Starting a revision:" << std::endl;
    doc.review();
    std::cout << "Current state: " << doc.getStateName() << std::endl;
    std::cout << std::endl;
    
    return 0;
}