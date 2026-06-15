#include <QCoreApplication>
#include <QThread>
#include "logger.h"

class LogWorker : public QThread
{
    Q_OBJECT
public:
    LogWorker(int id) : m_id(id) {}

protected:
    void run() override {
        for (int i = 0; i < 10; ++i) {
            LOG_INFO_FMT("Thread %d - Message %d", m_id, i);
            msleep(50);
        }
    }

private:
    int m_id;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LOG_ENABLE_FILE("./logs/thread.log");
    LOG_INFO("Multi-thread logging test started");

    LogWorker w1(1), w2(2), w3(3);
    w1.start();
    w2.start();
    w3.start();

    w1.wait();
    w2.wait();
    w3.wait();

    LOG_INFO("Multi-thread logging test completed");

    return a.exec();
}

#include "main.moc"
