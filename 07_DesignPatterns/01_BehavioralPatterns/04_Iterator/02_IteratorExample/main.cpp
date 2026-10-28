#include <vector>
#include <string>
#include <memory>
#include <print>

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
    std::string title;
    std::string author;
    int year;

    Book(const std::string& t, const std::string& a, int y)
        : title(t), author(a), year(y) {}

    // Display book information
    void display() const {
        std::println("📖 \"{}\" by {} ({})", title, author, year);
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
    virtual std::shared_ptr<Iterator<T>> createIterator() = 0;
};

// Forward declaration
class BookIterator;

/**
 * Concrete Collection: BookCollection
 * Stores books and provides iteration capability
 */
class BookCollection : public Aggregate<Book> {
private:
    std::vector<Book> books;  // Internal storage

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
    std::shared_ptr<Iterator<Book>> createIterator() override;

    // Create different types of iterators
    std::shared_ptr<Iterator<Book>> createReverseIterator();
    std::shared_ptr<Iterator<Book>> createFilteredIterator(int minYear);
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
            throw std::out_of_range("No more elements");
        }
        return collection->books[currentIndex++];
    }

    // Get current element without advancing
    Book current() const override {
        if (currentIndex >= collection->books.size()) {
            throw std::out_of_range("Invalid position");
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
            throw std::out_of_range("No more elements");
        }
        return collection->books[currentIndex--];
    }

    // Get current element
    Book current() const override {
        if (currentIndex < 0 || currentIndex >= (int)collection->books.size()) {
            throw std::out_of_range("Invalid position");
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
            throw std::out_of_range("No more elements");
        }
        Book book = collection->books[currentIndex++];
        findNext();  // Move to next valid element
        return book;
    }

    // Get current filtered element
    Book current() const override {
        if (!hasNext()) {
            throw std::out_of_range("Invalid position");
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
std::shared_ptr<Iterator<Book>> BookCollection::createIterator() {
    return std::make_shared<BookIterator>(this);
}

std::shared_ptr<Iterator<Book>> BookCollection::createReverseIterator() {
    return std::make_shared<ReverseBookIterator>(this);
}

std::shared_ptr<Iterator<Book>> BookCollection::createFilteredIterator(int minYear) {
    return std::make_shared<FilteredBookIterator>(this, minYear);
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
    std::string title;
    std::string artist;
    int duration;  // in seconds

    Song(const std::string& t, const std::string& a, int d)
        : title(t), artist(a), duration(d) {}

    void play() const {
        std::println("🎵 Playing: \"{}\" by {} [{}s]", title, artist, duration);
    }
};

/**
 * Playlist Collection
 */
class Playlist {
private:
    std::vector<Song> songs;
    std::string name;

public:
    Playlist(const std::string& n) : name(n) {}

    void addSong(const Song& song) {
        songs.push_back(song);
    }

    std::string getName() const { return name; }
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
    std::vector<size_t> shuffledIndices;
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
            std::swap(shuffledIndices[i], shuffledIndices[j]);
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
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 1: BOOK COLLECTION        ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    // Create a book collection
    BookCollection library;

    // Add books to the collection
    library.addBook(Book("1984", "George Orwell", 1949));
    library.addBook(Book("To Kill a Mockingbird", "Harper Lee", 1960));
    library.addBook(Book("The Great Gatsby", "F. Scott Fitzgerald", 1925));
    library.addBook(Book("Pride and Prejudice", "Jane Austen", 1813));
    library.addBook(Book("The Catcher in the Rye", "J.D. Salinger", 1951));

    // Forward iteration
    std::println("📚 Forward Iteration:");
    std::println("---------------------");
    auto forwardIterator = library.createIterator();
    while (forwardIterator->hasNext()) {
        Book book = forwardIterator->next();
        book.display();
    }

    // Reverse iteration
    std::println("");
    std::println("📚 Reverse Iteration:");
    std::println("---------------------");
    auto reverseIterator = library.createReverseIterator();
    while (reverseIterator->hasNext()) {
        Book book = reverseIterator->next();
        book.display();
    }

    // Filtered iteration (books after 1940)
    std::println("");
    std::println("📚 Filtered Iteration (Year >= 1940):");
    std::println("--------------------------------------");
    auto filteredIterator = library.createFilteredIterator(1940);
    while (filteredIterator->hasNext()) {
        Book book = filteredIterator->next();
        book.display();
    }
}

void demonstratePlaylistIterator() {
    std::println("");
    std::println("╔════════════════════════════════════╗");
    std::println("║  EXAMPLE 2: MUSIC PLAYLIST         ║");
    std::println("╚════════════════════════════════════╝");
    std::println("");

    Playlist myPlaylist("My Favorites");
    myPlaylist.addSong(Song("Bohemian Rhapsody", "Queen", 354));
    myPlaylist.addSong(Song("Hotel California", "Eagles", 391));
    myPlaylist.addSong(Song("Stairway to Heaven", "Led Zeppelin", 482));
    myPlaylist.addSong(Song("Imagine", "John Lennon", 183));

    // Sequential playback
    std::println("🎼 Sequential Playback:");
    std::println("-----------------------");
    SequentialIterator sequential(&myPlaylist);
    while (sequential.hasNext()) {
        Song song = sequential.next();
        song.play();
    }

    // Shuffle playback
    std::println("");
    std::println("🔀 Shuffle Playback:");
    std::println("--------------------");
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
    std::println("========================================");
    std::println("ITERATOR PATTERN DEMO");
    std::println("========================================");

    demonstrateBookIterator();
    demonstratePlaylistIterator();

    return 0;
}
