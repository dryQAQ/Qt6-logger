#pragma once

#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QList>
#include <QTextStream>
#include <memory>
#include <functional>

class Logger
{
public:
    enum class Level
    {
        Trace = 0,
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };

    using Sink = std::function<void(const QString&)>;

    static Logger& instance();

    void setLevel(Level level);
    Level level() const;

    void enableFileSink(const QString& path);
    void disableFileSink();
    void addSink(Sink sink);

    void trace(const QString& msg);
    void debug(const QString& msg);
    void info(const QString& msg);
    void warn(const QString& msg);
    void error(const QString& msg);
    void fatal(const QString& msg);

private:
    Logger();
    QString format(Level level, const QString& msg) const;
    void emitLine(Level level, const QString& msg);
    void writeToFile(const QString& line);

    Level m_level;
    QFile m_file;
    QTextStream m_stream;
    QList<Sink> m_sinks;
    QMutex m_mutex;
};

#define LOG_TRACE(msg) Logger::instance().trace(msg)
#define LOG_DEBUG(msg) Logger::instance().debug(msg)
#define LOG_INFO(msg) Logger::instance().info(msg)
#define LOG_WARN(msg) Logger::instance().warn(msg)
#define LOG_ERROR(msg) Logger::instance().error(msg)
#define LOG_FATAL(msg) Logger::instance().fatal(msg)

#define LOG_TRACE_FMT(...) Logger::instance().trace(QString(__VA_ARGS__))
#define LOG_DEBUG_FMT(...) Logger::instance().debug(QString(__VA_ARGS__))
#define LOG_INFO_FMT(...) Logger::instance().info(QString(__VA_ARGS__))
#define LOG_WARN_FMT(...) Logger::instance().warn(QString(__VA_ARGS__))
#define LOG_ERROR_FMT(...) Logger::instance().error(QString(__VA_ARGS__))
#define LOG_FATAL_FMT(...) Logger::instance().fatal(QString(__VA_ARGS__))

#define LOG_SET_LEVEL(lvl) Logger::instance().setLevel(lvl)
#define LOG_ENABLE_FILE(path) Logger::instance().enableFileSink(path)
#define LOG_DISABLE_FILE() Logger::instance().disableFileSink()
#define LOG_ADD_SINK(sink) Logger::instance().addSink(sink)
