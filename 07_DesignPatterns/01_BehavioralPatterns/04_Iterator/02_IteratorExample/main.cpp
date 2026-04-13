#include <iostream>
#include <vector>
#include <string>
#include <memory>
using namespace std;

// ============================================
// EXAMPLE 1: BOOK COLLECTION ITERATOR
// Classic iterator pattern example
// ============================================

/**
 * Book Class
 * Represents a book in our collection
 */
class Book {
public:
    string title;
    string author;
    int year;

    Book(const string& t, const string& a, int y)
        : title(t), author(a), year(y) {}

    // Display book information
    void display() const {
        cout << "📖 \"" << title << "\" by " << author
             << " (" << year << ")\n";
    }
};

/**
 * Iterator Interface
 * Defines standard operations for iterating through a collection
 */
template<typename T>
class Iterator {
public:
    virtual ~Iterator() {}

    // Check if there are more elements
    virtual bool hasNext() const = 0;

    // Get the next element and move forward
    virtual T next() = 0;

    // Get current element without moving
    virtual T current() const = 0;

    // Reset iterator to beginning
    virtual void reset() = 0;
};

/**
 * Aggregate Interface
 * Collection that can create iterators
 */
template<typename T>
class Aggregate {
public:
    virtual ~Aggregate() {}

    // Factory method to create an iterator
    virtual shared_ptr<Iterator<T>> createIterator() = 0;
};

// Forward declaration
class BookIterator;

/**
 * Concrete Collection: BookCollection
 * Stores books and provides iteration capability
 */
class BookCollection : public Aggregate<Book> {
private:
    vector<Book> books;  // Internal storage

    // Allow BookIterator to access private members
    friend class BookIterator;
    friend class ReverseBookIterator;
    friend class FilteredBookIterator;

public:
    // Add a book to the collection
    void addBook(const Book& book) {
        books.push_back(book);
    }

    // Get the number of books
    size_t size() const {
        return books.size();
    }

    // Create an iterator for this collection
    shared_ptr<Iterator<Book>> createIterator() override;

    // Create different types of iterators
    shared_ptr<Iterator<Book>> createReverseIterator();
    shared_ptr<Iterator<Book>> createFilteredIterator(int minYear);
};

/**
 * Concrete Iterator: BookIterator
 * Forward iteration through BookCollection
 */
class BookIterator : public Iterator<Book> {
private:
    const BookCollection* collection;  // Reference to collection
    size_t currentIndex;               // Current position in iteration

public:
    // Constructor takes the collection to iterate
    BookIterator(const BookCollection* col)
        : collection(col), currentIndex(0) {}

    // Check if there are more elements to iterate
    bool hasNext() const override {
        return currentIndex < collection->books.size();
    }

    // Get next element and advance position
    Book next() override {
        if (!hasNext()) {
            throw out_of_range("No more elements");
        }
        return collection->books[currentIndex++];
    }

    // Get current element without advancing
    Book current() const override {
        if (currentIndex >= collection->books.size()) {
            throw out_of_range("Invalid position");
        }
        return collection->books[currentIndex];
    }

    // Reset iterator to the beginning
    void reset() override {
        currentIndex = 0;
    }
};

/**
 * Reverse Iterator
 * Iterates through collection in reverse order
 */
class ReverseBookIterator : public Iterator<Book> {
private:
    const BookCollection* collection;  // Reference to collection
    int currentIndex;                  // Current position (using int for negative check)

public:
    // Start from the end of collection
    ReverseBookIterator(const BookCollection* col)
        : collection(col), currentIndex(col->size() - 1) {}

    // Check if there are more elements (going backwards)
    bool hasNext() const override {
        return currentIndex >= 0;
    }

    // Get next element (previous in forward order) and move backward
    Book next() override {
        if (!hasNext()) {
            throw out_of_range("No more elements");
        }
        return collection->books[currentIndex--];
    }

    // Get current element
    Book current() const override {
        if (currentIndex < 0 || currentIndex >= (int)collection->books.size()) {
            throw out_of_range("Invalid position");
        }
        return collection->books[currentIndex];
    }

    // Reset to end of collection
    void reset() override {
        currentIndex = collection->size() - 1;
    }
};

/**
 * Filtered Iterator
 * Only iterates through books that match a condition
 */
class FilteredBookIterator : public Iterator<Book> {
private:
    const BookCollection* collection;  // Reference to collection
    size_t currentIndex;               // Current position
    int minYear;                       // Filter: minimum publication year

    // Helper: Find next book that matches filter
    void findNext() {
        while (currentIndex < collection->books.size() &&
               collection->books[currentIndex].year < minYear) {
            currentIndex++;
        }
    }

public:
    // Constructor with filter parameter
    FilteredBookIterator(const BookCollection* col, int year)
        : collection(col), currentIndex(0), minYear(year) {
        findNext();  // Position at first valid element
    }

    // Check if there are more filtered elements
    bool hasNext() const override {
        return currentIndex < collection->books.size();
    }

    // Get next filtered element
    Book next() override {
        if (!hasNext()) {
            throw out_of_range("No more elements");
        }
        Book book = collection->books[currentIndex++];
        findNext();  // Move to next valid element
        return book;
    }

    // Get current filtered element
    Book current() const override {
        if (!hasNext()) {
            throw out_of_range("Invalid position");
        }
        return collection->books[currentIndex];
    }

    // Reset to beginning with filter applied
    void reset() override {
        currentIndex = 0;
        findNext();
    }
};

// Implementation of createIterator methods
shared_ptr<Iterator<Book>> BookCollection::createIterator() {
    return make_shared<BookIterator>(this);
}

shared_ptr<Iterator<Book>> BookCollection::createReverseIterator() {
    return make_shared<ReverseBookIterator>(this);
}

shared_ptr<Iterator<Book>> BookCollection::createFilteredIterator(int minYear) {
    return make_shared<FilteredBookIterator>(this, minYear);
}

// ============================================
// EXAMPLE 2: PLAYLIST ITERATOR
// Music player with different playback modes
// ============================================

/**
 * Song Class
 */
class Song {
public:
    string title;
    string artist;
    int duration;  // in seconds

    Song(const string& t, const string& a, int d)
        : title(t), artist(a), duration(d) {}

    void play() const {
        cout << "🎵 Playing: \"" << title << "\" by " << artist
             << " [" << duration << "s]\n";
    }
};

/**
 * Playlist Collection
 */
class Playlist {
private:
    vector<Song> songs;
    string name;

public:
    Playlist(const string& n) : name(n) {}

    void addSong(const Song& song) {
        songs.push_back(song);
    }

    string getName() const { return name; }
    size_t size() const { return songs.size(); }
    const Song& getSong(size_t index) const { return songs[index]; }
};

/**
 * Sequential Iterator
 * Plays songs in order
 */
class SequentialIterator : public Iterator<Song> {
private:
    const Playlist* playlist;
    size_t currentIndex;

public:
    SequentialIterator(const Playlist* p)
        : playlist(p), currentIndex(0) {}

    bool hasNext() const override {
        return currentIndex < playlist->size();
    }

    Song next() override {
        return playlist->getSong(currentIndex++);
    }

    Song current() const override {
        return playlist->getSong(currentIndex);
    }

    void reset() override {
        currentIndex = 0;
    }
};

/**
 * Shuffle Iterator
 * Plays songs in random order
 */
class ShuffleIterator : public Iterator<Song> {
private:
    const Playlist* playlist;
    vector<size_t> shuffledIndices;
    size_t currentPosition;

    void shuffle() {
        // Create indices 0, 1, 2, ... n-1
        shuffledIndices.clear();
        for (size_t i = 0; i < playlist->size(); i++) {
            shuffledIndices.push_back(i);
        }
        // Simple shuffle (not cryptographically secure)
        for (size_t i = shuffledIndices.size() - 1; i > 0; i--) {
            size_t j = rand() % (i + 1);
            swap(shuffledIndices[i], shuffledIndices[j]);
        }
    }

public:
    ShuffleIterator(const Playlist* p)
        : playlist(p), currentPosition(0) {
        shuffle();
    }

    bool hasNext() const override {
        return currentPosition < shuffledIndices.size();
    }

    Song next() override {
        size_t index = shuffledIndices[currentPosition++];
        return playlist->getSong(index);
    }

    Song current() const override {
        size_t index = shuffledIndices[currentPosition];
        return playlist->getSong(index);
    }

    void reset() override {
        currentPosition = 0;
        shuffle();  // Re-shuffle on reset
    }
};

// ============================================
// DEMONSTRATION FUNCTIONS
// ============================================

void demonstrateBookIterator() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 1: BOOK COLLECTION        ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    // Create a book collection
    BookCollection library;

    // Add books to the collection
    library.addBook(Book("1984", "George Orwell", 1949));
    library.addBook(Book("To Kill a Mockingbird", "Harper Lee", 1960));
    library.addBook(Book("The Great Gatsby", "F. Scott Fitzgerald", 1925));
    library.addBook(Book("Pride and Prejudice", "Jane Austen", 1813));
    library.addBook(Book("The Catcher in the Rye", "J.D. Salinger", 1951));

    // Forward iteration
    cout << "📚 Forward Iteration:\n";
    cout << "---------------------\n";
    auto forwardIterator = library.createIterator();
    while (forwardIterator->hasNext()) {
        Book book = forwardIterator->next();
        book.display();
    }

    // Reverse iteration
    cout << "\n📚 Reverse Iteration:\n";
    cout << "---------------------\n";
    auto reverseIterator = library.createReverseIterator();
    while (reverseIterator->hasNext()) {
        Book book = reverseIterator->next();
        book.display();
    }

    // Filtered iteration (books after 1940)
    cout << "\n📚 Filtered Iteration (Year >= 1940):\n";
    cout << "--------------------------------------\n";
    auto filteredIterator = library.createFilteredIterator(1940);
    while (filteredIterator->hasNext()) {
        Book book = filteredIterator->next();
        book.display();
    }
}

void demonstratePlaylistIterator() {
    cout << "\n╔════════════════════════════════════╗\n";
    cout << "║  EXAMPLE 2: MUSIC PLAYLIST         ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    Playlist myPlaylist("My Favorites");
    myPlaylist.addSong(Song("Bohemian Rhapsody", "Queen", 354));
    myPlaylist.addSong(Song("Hotel California", "Eagles", 391));
    myPlaylist.addSong(Song("Stairway to Heaven", "Led Zeppelin", 482));
    myPlaylist.addSong(Song("Imagine", "John Lennon", 183));

    // Sequential playback
    cout << "🎼 Sequential Playback:\n";
    cout << "-----------------------\n";
    SequentialIterator sequential(&myPlaylist);
    while (sequential.hasNext()) {
        Song song = sequential.next();
        song.play();
    }

    // Shuffle playback
    cout << "\n🔀 Shuffle Playback:\n";
    cout << "--------------------\n";
    ShuffleIterator shuffle(&myPlaylist);
    while (shuffle.hasNext()) {
        Song song = shuffle.next();
        song.play();
    }
}

// ============================================
// MAIN - DEMONSTRATING THE PATTERN
// ============================================

int main() {
    cout << "========================================\n";
    cout << "ITERATOR PATTERN DEMO\n";
    cout << "========================================\n";

    demonstrateBookIterator();
    demonstratePlaylistIterator();

    // ============================================
    // Benefits Summary
    // ============================================
    cout << "\n========================================\n";
    cout << "BENEFITS OF ITERATOR PATTERN\n";
    cout << "========================================\n";
    cout << "✅ Single Responsibility: Iteration logic separated from collection\n";
    cout << "✅ Open/Closed: Add new iterators without changing collection\n";
    cout << "✅ Uniform Access: Same interface for different collections\n";
    cout << "✅ Multiple Iterators: Can iterate same collection multiple ways\n";
    cout << "✅ Encapsulation: Collection internals remain hidden\n";
    cout << "✅ Flexibility: Easy to add new traversal algorithms\n";

    cout << "\n========================================\n";
    cout << "WHEN TO USE ITERATOR\n";
    cout << "========================================\n";
    cout << "1. Access collection elements without exposing internal structure\n";
    cout << "2. Support multiple simultaneous traversals of collection\n";
    cout << "3. Provide uniform interface for different collection types\n";
    cout << "4. Need different ways to traverse same collection\n";
    cout << "5. Simplify collection interface by moving traversal to iterator\n";

    cout << "\n========================================\n";
    cout << "ITERATOR TYPES\n";
    cout << "========================================\n";
    cout << "Forward Iterator:\n";
    cout << "  • Traverses from beginning to end\n";
    cout << "  • Most common type\n";
    cout << "  • Example: BookIterator\n";
    cout << "\nReverse Iterator:\n";
    cout << "  • Traverses from end to beginning\n";
    cout << "  • Useful for backward navigation\n";
    cout << "  • Example: ReverseBookIterator\n";
    cout << "\nFiltered Iterator:\n";
    cout << "  • Only returns elements matching criteria\n";
    cout << "  • Combines iteration with filtering\n";
    cout << "  • Example: FilteredBookIterator (year >= 1940)\n";
    cout << "\nRandom/Shuffle Iterator:\n";
    cout << "  • Traverses in random order\n";
    cout << "  • Useful for playlists, games\n";
    cout << "  • Example: ShuffleIterator\n";
    cout << "\nBidirectional Iterator:\n";
    cout << "  • Can move both forward and backward\n";
    cout << "  • Supports next() and previous()\n";
    cout << "  • Used in doubly-linked lists\n";

    cout << "\n========================================\n";
    cout << "INTERNAL vs EXTERNAL ITERATOR\n";
    cout << "========================================\n";
    cout << "External Iterator (Used in examples above):\n";
    cout << "  • Client controls iteration\n";
    cout << "  • More flexible and powerful\n";
    cout << "  • Can have multiple active iterators\n";
    cout << "  • Example: while(it.hasNext()) { it.next(); }\n";
    cout << "\nInternal Iterator:\n";
    cout << "  • Collection controls iteration\n";
    cout << "  • Client passes operation to execute\n";
    cout << "  • Simpler but less flexible\n";
    cout << "  • Example: collection.forEach(lambda)\n";

    cout << "\n========================================\n";
    cout << "REAL-WORLD EXAMPLES\n";
    cout << "========================================\n";
    cout << "• C++ STL iterators (vector, list, map)\n";
    cout << "• Java Iterator and Iterable interfaces\n";
    cout << "• Python __iter__ and __next__ methods\n";
    cout << "• Database result sets (JDBC ResultSet)\n";
    cout << "• File system directory traversal\n";
    cout << "• Tree/Graph traversal algorithms (DFS, BFS)\n";
    cout << "• Menu navigation systems\n";
    cout << "• Playlist management (shuffle, repeat)\n";
    cout << "• Pagination in web applications\n";
    cout << "• Game inventory systems\n";

    cout << "\n========================================\n";
    cout << "ITERATOR vs FOR-EACH LOOP\n";
    cout << "========================================\n";
    cout << "Iterator Pattern:\n";
    cout << "  • Explicit iteration control\n";
    cout << "  • Can pause/resume iteration\n";
    cout << "  • Multiple iterators on same collection\n";
    cout << "  • Works with any traversal algorithm\n";
    cout << "\nFor-Each Loop:\n";
    cout << "  • Syntactic sugar over iterators\n";
    cout << "  • Simpler syntax\n";
    cout << "  • Limited control (can't pause)\n";
    cout << "  • Built-in language feature\n";

    cout << "\n========================================\n";
    cout << "KEY COMPONENTS\n";
    cout << "========================================\n";
    cout << "1. Iterator Interface: hasNext(), next(), current(), reset()\n";
    cout << "2. Concrete Iterator: Implements traversal algorithm\n";
    cout << "3. Aggregate Interface: createIterator()\n";
    cout << "4. Concrete Aggregate: Returns concrete iterator\n";

    cout << "\n========================================\n";
    cout << "ADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Separates traversal from collection structure\n";
    cout << "• Supports multiple simultaneous iterations\n";
    cout << "• Uniform interface for different collections\n";
    cout << "• Easy to add new traversal methods\n";
    cout << "• Encapsulates internal collection structure\n";
    cout << "• Simplifies collection interface\n";

    cout << "\n========================================\n";
    cout << "DISADVANTAGES\n";
    cout << "========================================\n";
    cout << "• Can be overkill for simple collections\n";
    cout << "• Slightly more complex than direct access\n";
    cout << "• May have performance overhead\n";
    cout << "• Need to maintain iterator state\n";

    return 0;
}
