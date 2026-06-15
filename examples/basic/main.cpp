#include <QCoreApplication>
#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LOG_INFO("Application started");
    LOG_DEBUG("Debug mode enabled");

    LOG_INFO_FMT("Qt Version: %s", qVersion());

    LOG_TRACE("This is a trace message");
    LOG_DEBUG("This is a debug message");
    LOG_INFO("This is an info message");
    LOG_WARN("This is a warning message");
    LOG_ERROR("This is an error message");
    LOG_FATAL("This is a fatal message");

    LOG_SET_LEVEL(Logger::Level::Trace);
    LOG_TRACE("Now trace messages are visible");

    LOG_ENABLE_FILE("./logs/basic.log");
    LOG_INFO("This message goes to both console and file");

    return a.exec();
}
