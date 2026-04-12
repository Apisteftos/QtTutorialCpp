#include <QCoreApplication>
#include <QThread>
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QRunnable>
#include <QThreadPool>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QDebug>
#include <QElapsedTimer>

#include <atomic>
#include <vector>

// ============================================================
// QTHREAD in C++23 / Qt6
// ============================================================
// Topics covered:
//   1.  QThread subclass (old style)
//   2.  Worker object + moveToThread (preferred Qt style)
//   3.  Signals/slots across threads (queued connection)
//   4.  QMutex + QMutexLocker
//   5.  QWaitCondition (Qt condition variable)
//   6.  QRunnable + QThreadPool
//   7.  QtConcurrent::run
//   8.  QFuture + QFutureWatcher
//   9.  QThread::sleep / msleep / usleep
//  10.  std::thread vs QThread comparison
// ============================================================


// ============================================================
// Example 1 — QThread subclass (old style)
// Override run() to define thread work
// ============================================================
class WorkerThread : public QThread {
    Q_OBJECT

public:
    explicit WorkerThread(int id, QObject* parent = nullptr)
        : QThread(parent), m_id(id) {}

protected:
    void run() override {
        qDebug() << "[WorkerThread" << m_id << "] started, thread:"
                 << QThread::currentThread();

        for (int i = 0; i < 3; ++i) {
            qDebug() << "[WorkerThread" << m_id << "] step" << i;
            QThread::msleep(50);
        }

        qDebug() << "[WorkerThread" << m_id << "] finished";
        emit workDone(m_id);
    }

signals:
    void workDone(int id);

private:
    int m_id;
};


// ============================================================
// Example 2 — Worker object + moveToThread (PREFERRED)
// Keep QObject in its own thread without subclassing QThread
// ============================================================
class WorkerObject : public QObject {
    Q_OBJECT

public:
    explicit WorkerObject(QObject* parent = nullptr) : QObject(parent) {}

public slots:
    void doWork(int taskId) {
        qDebug() << "[WorkerObject] task" << taskId << "on thread:"
                 << QThread::currentThread();

        // Simulate work
        QThread::msleep(30);

        int result = taskId * taskId;
        qDebug() << "[WorkerObject] task" << taskId << "result:" << result;
        emit resultReady(taskId, result);
    }

    void processAll() {
        for (int i = 1; i <= 4; ++i) {
            qDebug() << "[WorkerObject] processing" << i;
            QThread::msleep(20);
        }
        emit allDone();
    }

signals:
    void resultReady(int taskId, int result);
    void allDone();
};


// ============================================================
// Example 3 — Controller that uses worker via signals/slots
// Demonstrates queued connection across threads
// ============================================================
class Controller : public QObject {
    Q_OBJECT

public:
    explicit Controller(QObject* parent = nullptr) : QObject(parent) {
        // Create worker and its thread
        m_workerThread = new QThread(this);
        m_worker       = new WorkerObject();   // no parent — will be moved
        m_worker->moveToThread(m_workerThread);

        // Connect signals
        connect(m_workerThread, &QThread::started,
                m_worker, &WorkerObject::processAll);
        connect(m_worker, &WorkerObject::allDone,
                m_workerThread, &QThread::quit);
        connect(m_worker, &WorkerObject::allDone,
                this, &Controller::onAllDone);
        connect(m_workerThread, &QThread::finished,
                m_worker, &QObject::deleteLater);
        connect(m_worker, &WorkerObject::resultReady,
                this, &Controller::onResult);
    }

    void start() {
        qDebug() << "[Controller] starting worker thread";
        m_workerThread->start();
    }

public slots:
    void onResult(int taskId, int result) {
        qDebug() << "[Controller] received result for task" << taskId
                 << "=" << result << "(on main thread:"
                 << (QThread::currentThread() == qApp->thread()) << ")";
    }

    void onAllDone() {
        qDebug() << "[Controller] all work done";
        emit finished();
    }

signals:
    void finished();

private:
    QThread*      m_workerThread;
    WorkerObject* m_worker;
};


// ============================================================
// Example 4 — QMutex + QMutexLocker
// ============================================================
class SharedCounter : public QObject {
    Q_OBJECT

public:
    void increment() {
        QMutexLocker locker(&m_mutex);   // RAII — like std::lock_guard
        ++m_value;
    }

    int value() const {
        QMutexLocker locker(&m_mutex);
        return m_value;
    }

private:
    mutable QMutex m_mutex;
    int            m_value{0};
};


// ============================================================
// Example 5 — QWaitCondition (Qt condition variable)
// ============================================================
class ProducerConsumerQt : public QObject {
    Q_OBJECT

public:
    void produce(int value) {
        QMutexLocker locker(&m_mutex);
        m_queue.enqueue(value);
        qDebug() << "[Qt producer] produced:" << value;
        m_condition.wakeOne();   // wake one consumer
    }

    int consume() {
        QMutexLocker locker(&m_mutex);
        while (m_queue.isEmpty()) {
            m_condition.wait(&m_mutex);   // release mutex, sleep, re-acquire
        }
        int val = m_queue.dequeue();
        qDebug() << "[Qt consumer] consumed:" << val;
        return val;
    }

private:
    QMutex         m_mutex;
    QWaitCondition m_condition;
    QQueue<int>    m_queue;
};


// ============================================================
// Example 6 — QRunnable + QThreadPool
// ============================================================
class ComputeTask : public QRunnable {
public:
    explicit ComputeTask(int id) : m_id(id) {
        setAutoDelete(true);   // QThreadPool deletes after run()
    }

    void run() override {
        qDebug() << "[QRunnable task" << m_id << "] on thread:"
                 << QThread::currentThread();
        QThread::msleep(20);
        qDebug() << "[QRunnable task" << m_id << "] done, result:"
                 << (m_id * m_id);
    }

private:
    int m_id;
};


// ============================================================
// MAIN
// ============================================================
int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "Main thread:" << QThread::currentThread();
    qDebug() << "Ideal thread count:" << QThreadPool::globalInstance()->maxThreadCount();


    // -------------------------------------------------------
    qDebug() << "\n=====================================================";
    qDebug() << "Example 1: QThread subclass";
    qDebug() << "=====================================================";

    {
        WorkerThread wt1(1);
        WorkerThread wt2(2);

        QObject::connect(&wt1, &WorkerThread::workDone, [](int id) {
            qDebug() << "[main] WorkerThread" << id << "done";
        });

        wt1.start();
        wt2.start();
        wt1.wait();   // wait for thread to finish (like join())
        wt2.wait();
        qDebug() << "Both QThread subclasses finished";
    }


    // -------------------------------------------------------
    qDebug() << "\n=====================================================";
    qDebug() << "Example 2 & 3: Worker object + moveToThread";
    qDebug() << "=====================================================";

    {
        Controller controller;
        QObject::connect(&controller, &Controller::finished, &app, [&]() {
            qDebug() << "Controller finished — exiting event loop";
            app.quit();
        });
        controller.start();

        // Run event loop briefly for this example
        QTimer::singleShot(500, &app, &QCoreApplication::quit);
        app.exec();
    }


    // -------------------------------------------------------
    qDebug() << "\n=====================================================";
    qDebug() << "Example 4: QMutex + QMutexLocker";
    qDebug() << "=====================================================";

    {
        SharedCounter counter;
        std::vector<QThread*> threads;

        for (int i = 0; i < 4; ++i) {
            auto* t = QThread::create([&counter]() {
                for (int j = 0; j < 10000; ++j)
                    counter.increment();
            });
            threads.push_back(t);
            t->start();
        }

        for (auto* t : threads) { t->wait(); delete t; }
        qDebug() << "Expected: 40000 | Actual:" << counter.value();
    }


    // -------------------------------------------------------
    qDebug() << "\n=====================================================";
    qDebug() << "Example 5: QWaitCondition";
    qDebug() << "=====================================================";

    {
        ProducerConsumerQt pc;

        auto* producer = QThread::create([&pc]() {
            for (int i = 1; i <= 4; ++i) {
                QThread::msleep(20);
                pc.produce(i);
            }
        });

        auto* consumer = QThread::create([&pc]() {
            for (int i = 0; i < 4; ++i)
                pc.consume();
        });

        producer->start();
        consumer->start();
        producer->wait();
        consumer->wait();
        delete producer;
        delete consumer;
    }


    // -------------------------------------------------------
    qDebug() << "\n=====================================================";
    qDebug() << "Example 6: QRunnable + QThreadPool";
    qDebug() << "=====================================================";

    {
        QThreadPool* pool = QThreadPool::globalInstance();
        qDebug() << "Thread pool max threads:" << pool->maxThreadCount();

        for (int i = 1; i <= 6; ++i)
            pool->start(new ComputeTask(i));   // pool takes ownership (autoDelete)

        pool->waitForDone();   // wait for all tasks
        qDebug() << "All QRunnable tasks done";
    }


    // -------------------------------------------------------
    qDebug() << "\n=====================================================";
    qDebug() << "Example 7: QtConcurrent::run";
    qDebug() << "=====================================================";

    {
        // Run function in thread pool — returns QFuture
        QFuture<int> fut = QtConcurrent::run([]() -> int {
            QThread::msleep(30);
            qDebug() << "[QtConcurrent] running on:" << QThread::currentThread();
            return 42;
        });

        qDebug() << "[main] doing other work while concurrent runs...";
        fut.waitForFinished();
        qDebug() << "[main] QtConcurrent result:" << fut.result();
    }


    // -------------------------------------------------------
    qDebug() << "\n=====================================================";
    qDebug() << "Example 8: QFuture + QFutureWatcher";
    qDebug() << "=====================================================";

    {
        // QFutureWatcher monitors a QFuture and emits signals
        QFutureWatcher<int> watcher;

        QObject::connect(&watcher, &QFutureWatcher<int>::finished, [&watcher]() {
            qDebug() << "[watcher] future finished, result:" << watcher.result();
        });

        QObject::connect(&watcher, &QFutureWatcher<int>::progressValueChanged,
                         [](int value) {
                             qDebug() << "[watcher] progress:" << value;
                         });

        QFuture<int> future = QtConcurrent::run([]() -> int {
            QThread::msleep(50);
            return 99;
        });

        watcher.setFuture(future);

        QTimer::singleShot(200, &app, &QCoreApplication::quit);
        app.exec();
    }


    // -------------------------------------------------------
    qDebug() << "\n=====================================================";
    qDebug() << "Example 9: QThread sleep functions";
    qDebug() << "=====================================================";

    {
        QElapsedTimer timer;

        timer.start();
        QThread::msleep(100);
        qDebug() << "msleep(100):" << timer.elapsed() << "ms";

        timer.restart();
        QThread::usleep(50000);   // 50ms in microseconds
        qDebug() << "usleep(50000):" << timer.elapsed() << "ms";

        timer.restart();
        QThread::sleep(0);   // yield — sleep for 0 whole seconds
        qDebug() << "sleep(0) yield:" << timer.elapsed() << "ms";
    }


    // -------------------------------------------------------
    qDebug() << "\n=====================================================";
    qDebug() << "Example 10: std::thread vs QThread";
    qDebug() << "=====================================================";

    qDebug() << "std::thread:";
    qDebug() << "  + Portable, standard C++23";
    qDebug() << "  + Works with std::mutex, std::future, std::atomic";
    qDebug() << "  - No signals/slots";
    qDebug() << "  - No Qt event loop";
    qDebug() << "  - Cannot use QObject in thread without event loop";

    qDebug() << "QThread:";
    qDebug() << "  + Signals/slots (queued connections across threads)";
    qDebug() << "  + Qt event loop (QTimer, network, etc. in thread)";
    qDebug() << "  + moveToThread pattern (clean ownership)";
    qDebug() << "  + QThreadPool / QtConcurrent integration";
    qDebug() << "  - Qt dependency";
    qDebug() << "  - Slightly more boilerplate";

    qDebug() << "\nRule: use QThread when you need Qt features in the thread.";
    qDebug() << "      use std::thread for pure C++ computation tasks.";


    qDebug() << "\n=== QThread complete ===";
    return 0;
}

#include "main.moc"
