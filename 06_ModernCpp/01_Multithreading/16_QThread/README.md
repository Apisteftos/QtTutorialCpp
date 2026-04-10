# QThread — Qt Threading in C++23

Qt provides its own threading infrastructure built on top of the OS thread API.
The key advantage over `std::thread` is full integration with Qt's
signal/slot mechanism and the event loop.

---

## QThread vs std::thread

| | `std::thread` | `QThread` |
|--|--------------|----------|
| Standard | ✅ C++23 | Qt only |
| Signals/slots | ❌ | ✅ queued connections |
| Event loop | ❌ | ✅ `exec()` |
| QTimer in thread | ❌ | ✅ |
| QObject in thread | ❌ without event loop | ✅ |
| Portable | ✅ | Qt projects only |
| Use for | Pure C++ tasks | Qt-integrated workers |

**Rule:** use `QThread` when the worker needs Qt features (signals, timers, network).
Use `std::thread` for pure computation with no Qt dependency.

---

## Example 1 — QThread subclass (old style)

Override `run()` to define what the thread does:

```cpp
class WorkerThread : public QThread {
    Q_OBJECT
protected:
    void run() override {
        // runs in the new thread
        doWork();
        emit workDone();
    }
signals:
    void workDone();
};

WorkerThread t;
t.start();   // starts run() in new thread
t.wait();    // like std::thread::join()
```

**Limitation:** the `WorkerThread` object itself lives in the creating thread,
not the worker thread. Calling slots on it from outside does not cross threads.

---

## Example 2 — Worker object + moveToThread (PREFERRED)

Create a plain `QObject` worker, then move it to a `QThread`:

```cpp
class Worker : public QObject {
    Q_OBJECT
public slots:
    void doWork() {
        // runs in workerThread
        emit resultReady(42);
    }
signals:
    void resultReady(int result);
};

QThread*  workerThread = new QThread;
Worker*   worker       = new Worker;   // no parent!
worker->moveToThread(workerThread);

connect(workerThread, &QThread::started,  worker, &Worker::doWork);
connect(worker, &Worker::resultReady,     this,   &MyClass::handleResult);
connect(worker, &Worker::resultReady,     workerThread, &QThread::quit);
connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);

workerThread->start();
```

### Why this is preferred
- Worker's slots run in `workerThread` — true thread separation
- Signal/slot connections across threads are automatically **queued** — thread-safe
- Clean ownership — `deleteLater` ensures safe cleanup

---

## Example 3 — Queued connections (cross-thread signals)

When you connect a signal in one thread to a slot in another, Qt automatically
uses a **queued connection** — the slot call is posted to the receiver's event loop:

```cpp
// Automatic detection:
connect(sender, &Sender::signal, receiver, &Receiver::slot);
// If sender and receiver are in different threads → queued connection

// Explicit:
connect(sender, &Sender::signal, receiver, &Receiver::slot,
        Qt::QueuedConnection);
```

Queued connections are **thread-safe** — no mutex needed for the signal/slot call.
The slot runs in the receiver's thread, not the emitter's thread.

---

## Example 4 — QMutex + QMutexLocker

Qt's mutex API — maps directly to `std::mutex` + `std::lock_guard`:

```cpp
QMutex mutex;

void increment() {
    QMutexLocker locker(&mutex);   // RAII — like std::lock_guard
    ++counter;
}   // locker destructor unlocks

// QRecursiveMutex = std::recursive_mutex
QRecursiveMutex recursiveMutex;
```

---

## Example 5 — QWaitCondition

Qt's condition variable — equivalent to `std::condition_variable`:

```cpp
QMutex         mutex;
QWaitCondition condition;
QQueue<int>    queue;

// Producer
{
    QMutexLocker locker(&mutex);
    queue.enqueue(value);
    condition.wakeOne();   // like notify_one()
}

// Consumer
{
    QMutexLocker locker(&mutex);
    while (queue.isEmpty())
        condition.wait(&mutex);   // releases mutex, sleeps, re-acquires
    int val = queue.dequeue();
}
```

---

## Example 6 — QRunnable + QThreadPool

Lightweight task interface — no signals/slots overhead:

```cpp
class MyTask : public QRunnable {
    void run() override {
        doWork();   // runs in thread pool
    }
};

QThreadPool::globalInstance()->start(new MyTask());
QThreadPool::globalInstance()->waitForDone();
```

`setAutoDelete(true)` (default) — pool deletes the task after `run()`.

Configure the pool:
```cpp
QThreadPool* pool = QThreadPool::globalInstance();
pool->setMaxThreadCount(8);
pool->setExpiryTimeout(5000);   // idle threads expire after 5s
```

---

## Example 7 — QtConcurrent::run

Simplest way to run a function in a thread pool:

```cpp
#include <QtConcurrent>

QFuture<int> fut = QtConcurrent::run([]() -> int {
    return heavyComputation();
});

// Main thread continues
fut.waitForFinished();
int result = fut.result();
```

No subclassing, no `moveToThread` — just pass a callable.

---

## Example 8 — QFuture + QFutureWatcher

`QFutureWatcher` monitors a `QFuture` and emits Qt signals:

```cpp
QFutureWatcher<int> watcher;

connect(&watcher, &QFutureWatcher<int>::finished, [&]() {
    qDebug() << "Result:" << watcher.result();
});

connect(&watcher, &QFutureWatcher<int>::progressValueChanged,
        [](int v) { progressBar->setValue(v); });

QFuture<int> future = QtConcurrent::run(compute);
watcher.setFuture(future);
```

---

## Example 9 — QThread sleep functions

```cpp
QThread::sleep(1);        // sleep 1 second
QThread::msleep(500);     // sleep 500 milliseconds
QThread::usleep(10000);   // sleep 10000 microseconds (= 10ms)

// Equivalent std:: versions:
std::this_thread::sleep_for(std::chrono::milliseconds(500));
```

---

## Common QThread mistakes

```cpp
// MISTAKE 1: Creating QObject with parent before moveToThread
Worker* w = new Worker(this);   // parent set — can't move!
w->moveToThread(thread);        // ERROR: QObject with parent can't be moved

// FIX: no parent when using moveToThread
Worker* w = new Worker();   // no parent
w->moveToThread(thread);    // OK

// MISTAKE 2: Calling slots directly across threads
worker->doWork();           // runs in CALLER's thread, not worker's thread!

// FIX: emit a signal or use QMetaObject::invokeMethod
emit startWork();           // queued — runs in worker's thread
QMetaObject::invokeMethod(worker, "doWork", Qt::QueuedConnection);

// MISTAKE 3: Subclassing QThread and adding slots
class BadThread : public QThread {
    void mySlot();   // runs in CREATING thread, not worker thread!
};
// Use moveToThread pattern instead
```

---

## Qt threading cheat sheet

| Scenario | Tool |
|----------|------|
| Simple background task, no Qt features | `std::thread` or `std::async` |
| Background task with Qt signals | Worker + `moveToThread` |
| Many short tasks | `QThreadPool` + `QRunnable` |
| Simple one-off async with result | `QtConcurrent::run` + `QFuture` |
| Monitor async progress in UI | `QFutureWatcher` |
| Synchronize threads | `QMutex` + `QMutexLocker` |
| Wait for condition | `QWaitCondition` |
