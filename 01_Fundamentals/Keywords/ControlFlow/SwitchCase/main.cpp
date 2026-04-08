#include <iostream>
#include <map>
#include <string>

enum Color {
    WHITE = 0,
    RED = 1,
    BLUE = 2
};

enum Clock{
    ZERO,
    ONE,
    TWO
};

typedef enum {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} Days;


enum class Pets {
    DOG,
    CAT,
    SNAKE
};


int main(int argc, char *argv[])
{
    std::cout << "=====================================================\n";
    std::cout << "Example 1: Switch with INT\n";
    std::cout << "=====================================================\n";


    Color color = RED;

    switch (color) {
    case 0:
        std::cout << "WHITE COLOR" << std::endl;
        break;
    case 1:
        std::cout << "RED COLOR" << std::endl;
        break;
    case 2:
        std::cout << "BLUE COLOR" << std::endl;
        break;
    default:
        std::cout << "UNKNOWN" << std::endl;
        break;
    }

    std::cout << "=====================================================\n";
    std::cout << "Example 2: Switch with ENUM\n";
    std::cout << "=====================================================\n";


    Clock clock = TWO;

    switch (clock) {
    case ZERO:
        std::cout << "HOUR IS ZERO\n";
        break;
    case ONE:
        std::cout << "HOUR IS ONE\n";
        break;
    case TWO:
        std::cout << "HOUR IS TWO\n";
        break;
    default:
        std::cout << "UNKNOWN\n";
        break;
    }

    std::cout << "=====================================================\n";
    std::cout << "Example 3: Switch with ENUM CLASS in C++23\n";
    std::cout << "=====================================================\n";

    Pets pet = Pets::DOG;

    switch (pet) {
    case Pets::DOG:
        std::cout << "The pet is DOG\n";
        break;
    case Pets::CAT:
        std::cout << "The pet is CAT\n";
        break;
    case Pets::SNAKE:
        std::cout << "The pet is SNAKE\n";
        break;
    default:
        std::cout << "UNKNOWN\n";
        break;
    }


    std::cout << "=====================================================\n";
    std::cout << "Example 4: Switch with TypeDef ENUM  like in C\n";
    std::cout << "=====================================================\n";

    Days day = MONDAY;

    switch (day) {
    case MONDAY:
        std::cout << "MONDAY\n";
        break;
    case TUESDAY:
        std::cout << "TUESDAY\n";
        break;
    case WEDNESDAY:
        std::cout << "WEDNESDAY\n";
        break;
    case THURSDAY:
        std::cout << "THURSDAY\n";
        break;
    case FRIDAY:
        std::cout << "FRIDAY\n";
        break;
    case SATURDAY:
        std::cout << "SATURDAY\n";
        break;
    case SUNDAY:
        std::cout << "SUNDAY\n";
        break;
    default:
        std::cout << "UNKNOWN\n";
        break;
    }


    std::cout << "=====================================================\n";
    std::cout << "Example 5: Switch by using braces\n";
    std::cout << "=====================================================\n";


    Color myColor = RED;

    switch (myColor) {
    case WHITE: {
        std::string colorName = "White";
        std::cout << "Color: " << colorName << " (RGB: 255, 255, 255)\n";
        break;
    }
    case RED: {
        std::string colorName = "Red";
        std::cout << "Color: " << colorName << " (RGB: 255, 0, 0)\n";
        break;
    }
    case BLUE: {
        std::string colorName = "Blue";
        std::cout << "Color: " << colorName << " (RGB: 0, 0, 255)\n";
        break;
    }
    default: {
        std::cout << "Unknown color\n";
        break;
    }
    }


    std::cout << "=====================================================\n";
    std::cout << "Example 5: Switch with FALL-THROUGH\n";
    std::cout << "=====================================================\n";


    int month = 2;

    switch (month) {
    case 12:
    case 1:
    case 2:
        std::cout << "Winter season\n";
        break;
    case 3:
    case 4:
    case 5:
        std::cout << "Spring season\n";
        break;
    case 6:
    case 7:
    case 8:
        std::cout << "Summer season\n";
        break;
    case 9:
    case 10:
    case 11:
        std::cout << "Fall season\n";
        break;
    default:
        std::cout << "Invalid month\n";
        break;
    }


    std::cout << "=====================================================\n";
    std::cout << "Example 6: C++23 workaround with constexpr map\n";
    std::cout << "=====================================================\n";


    const std::map<std::string, int> colorMap = {
        {"WHITE", 0}, {"RED", 1}, {"BLUE", 2}
    };

    auto it = colorMap.find("RED");
    if (it != colorMap.end()) {
        switch (it->second) {
        case 0: { std::cout << "White\n"; break; }
        case 1: { std::cout << "Red\n";   break; }
        case 2: { std::cout << "Blue\n";  break; }
        }
    }


    return 0;
}
