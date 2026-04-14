#include <iostream>
#include <memory>
#include <thread>

using namespace std;

// ===============================
// GLOBAL MEMORY (extern concept)
// ===============================
int globalValue = 42;

// ===============================
// RAII CLASS (automatic memory safety)
// ===============================
class Resource {
private:
    int* data;

public:
    Resource(int value) {
        data = new int(value);   // heap allocation
        cout << "Resource acquired: " << *data << endl;
    }

    ~Resource() {
        cout << "Resource released: " << *data << endl;
        delete data; // automatic cleanup (RAII)
    }

    int get() const {
        return *data;
    }
};

// ===============================
// THREAD LOCAL MEMORY
// ===============================
thread_local int threadCounter = 0;

void threadFunction() {
    threadCounter++;
    cout << "Thread ID: " << this_thread::get_id()
         << " Counter: " << threadCounter << endl;
}

// ===============================
// MAIN
// ===============================
int main() {

    cout << "===== STACK MEMORY =====" << endl;
    int a = 10;
    int b = 20;
    cout << "Stack values: " << a << ", " << b << endl;

    cout << "\n===== HEAP MEMORY (new/delete) =====" << endl;
    int* p = new int(100);
    cout << "Heap value: " << *p << endl;
    delete p;
    p = nullptr; // safe pointer reset

    cout << "\n===== HEAP ARRAY =====" << endl;
    int* arr = new int[3]{1, 2, 3};
    for (int i = 0; i < 3; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    delete[] arr;

    cout << "\n===== RAW POINTER DANGER EXAMPLE =====" << endl;
    int* danger = new int(50);
    cout << "Before delete: " << *danger << endl;
    delete danger;
    danger = nullptr;

    cout << "\n===== SMART POINTER (unique_ptr) =====" << endl;
    unique_ptr<int> uptr = make_unique<int>(200);
    cout << "unique_ptr value: " << *uptr << endl;

    cout << "\n===== SMART POINTER (shared_ptr) =====" << endl;
    shared_ptr<int> sp1 = make_shared<int>(300);
    shared_ptr<int> sp2 = sp1;
    cout << "shared_ptr count: " << sp1.use_count() << endl;

    cout << "\n===== RAII CLASS =====" << endl;
    {
        Resource obj(500);
        cout << "Inside scope value: " << obj.get() << endl;
    } // destructor called automatically here

    cout << "\n===== STATIC MEMORY =====" << endl;
    static int staticCounter = 0;
    staticCounter++;
    cout << "Static counter: " << staticCounter << endl;

    cout << "\n===== EXTERN GLOBAL MEMORY =====" << endl;
    cout << "Global value: " << globalValue << endl;

    cout << "\n===== THREAD LOCAL MEMORY =====" << endl;
    thread t1(threadFunction);
    thread t2(threadFunction);

    t1.join();
    t2.join();

    cout << "\n===== nullptr SAFETY =====" << endl;
    int* safePtr = nullptr;
    if (safePtr == nullptr) {
        cout << "Pointer is safe (nullptr)" << endl;
    }

    cout << "\n===== END PROGRAM =====" << endl;

    return 0;
}
