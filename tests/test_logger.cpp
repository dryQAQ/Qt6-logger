#include <QtTest>
#include "logger.h"

class TestLogger : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testLevels();
    void testFileSink();
    void testCustomSink();
};

void TestLogger::initTestCase()
{
    LOG_SET_LEVEL(Logger::Level::Trace);
}

void TestLogger::cleanupTestCase()
{
    LOG_DISABLE_FILE();
}

void TestLogger::testLevels()
{
    Logger& logger = Logger::instance();
    
    logger.setLevel(Logger::Level::Debug);
    QCOMPARE(logger.level(), Logger::Level::Debug);
    
    logger.setLevel(Logger::Level::Info);
    QCOMPARE(logger.level(), Logger::Level::Info);
    
    logger.setLevel(Logger::Level::Error);
    QCOMPARE(logger.level(), Logger::Level::Error);
}

void TestLogger::testFileSink()
{
    QString path = "./test_log.log";
    QFile::remove(path);
    
    LOG_ENABLE_FILE(path);
    LOG_INFO("Test message");
    LOG_DISABLE_FILE();
    
    QFile file(path);
    QVERIFY(file.exists());
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    
    QString content = QString::fromUtf8(file.readAll());
    QVERIFY(content.contains("[INFO] Test message"));
    
    file.close();
    QFile::remove(path);
}

void TestLogger::testCustomSink()
{
    QStringList receivedMessages;
    
    LOG_ADD_SINK([&receivedMessages](const QString& line) {
        receivedMessages.append(line);
    });
    
    LOG_INFO("Custom sink test");
    
    QVERIFY(receivedMessages.size() > 0);
    QVERIFY(receivedMessages.last().contains("[INFO] Custom sink test"));
}

QTEST_APPLESS_MAIN(TestLogger)
#include "test_logger.moc"
