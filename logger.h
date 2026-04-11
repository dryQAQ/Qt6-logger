#pragma once

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QList>
#include <QTextStream>
#include <memory>
#include <functional>


// Lightweight logger that wraps QLoggingCategory and optional file persistence.
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
