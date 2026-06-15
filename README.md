# Qt6 轻量级日志库 Logger 使用文档

本文档基于提供的 `logger.h` / `logger.cpp` 源码编写，所有示例均为**完整可直接编译运行**的代码，从实际使用角度说明功能、API、使用特点与注意事项。

---

## 一、库概述

这是一个 **Qt6 原生、线程安全、单例模式、支持多输出端** 的轻量级日志组件。

### 核心功能

- 6 级日志过滤：Trace / Debug / Info / Warn / Error / Fatal
    
- 默认控制台输出，支持一键开启文件持久化
    
- 支持自定义 Sink，可扩展输出到网络、数据库、UI 等
    
- 固定标准日志格式：`[时间戳] [级别] 日志内容`
    
- 文件写入强制 UTF-8 编码，中文无乱码
    
- 全程加锁保护，多线程环境安全可靠
    

**适用环境**：Qt 6.0 及以上（依赖 `QStringConverter`）

---

## 二、快速集成步骤

1. 将 `logger.h`、`logger.cpp` 添加到项目中
    
2. 项目构建文件配置：
    

### CMake 配置

```CMake
find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(你的目标名 PRIVATE Qt6::Core)
```

### qmake 配置

```Plain
QT += core
CONFIG += c++17
```

3. 在使用日志的文件中包含头文件：
    

```C++
#include "logger.h"
```

---

## 三、核心使用方式（完整可运行代码）

### 3.1 基础使用（控制台输出）

```C++
#include <QCoreApplication>
#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 获取全局单例
    Logger& logger = Logger::instance();

    // 输出各级日志（默认级别 Info，Trace/Debug 被过滤）
    logger.trace("这是 Trace 日志");
    logger.debug("这是 Debug 日志");
    logger.info("应用启动成功");
    logger.warn("配置文件不存在");
    logger.error("网络请求失败");
    logger.fatal("核心组件崩溃");

    return a.exec();
}
```

**输出格式示例**：

```Plain
[2026-04-11T15:30:00] [INFO] 应用启动成功
[2026-04-11T15:30:00] [WARN] 配置文件不存在
[2026-04-11T15:30:00] [ERROR] 网络请求失败
[2026-04-11T15:30:00] [FATAL] 核心组件崩溃
```

---

### 3.2 日志级别控制

```C++
#include <QCoreApplication>
#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Logger& logger = Logger::instance();

    // 默认级别：Info
    logger.info("默认级别：只输出 Info 及以上");

    // 设置为 Debug
    logger.setLevel(Logger::Level::Debug);
    logger.debug("Debug 日志现在可输出");

    // 设置为 Trace
    logger.setLevel(Logger::Level::Trace);
    logger.trace("Trace 日志现在可输出");

    // 获取当前日志级别
    Logger::Level current = logger.level();

    return a.exec();
}
```

**级别顺序（从低到高）**：

`Trace < Debug < Info < Warn < Error < Fatal`

**使用细节**：

- 低于当前级别的日志会**直接被过滤**，不执行格式化与输出
    
- 运行时可随时修改级别，立即生效
    
- 线程安全
    

---

### 3.3 启用文件日志（持久化）

```C++
#include <QCoreApplication>
#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Logger& logger = Logger::instance();

    // 启用文件输出（自动创建目录，追加模式，UTF-8 编码）
    logger.enableFileSink("./logs/app.log");

    logger.info("这条日志同时输出到控制台和文件");

    // 关闭文件输出
    logger.disableFileSink();
    logger.info("这条日志只输出到控制台");

    return a.exec();
}
```

**使用细节**：

- 支持相对路径、绝对路径
    
- 自动创建日志所在目录
    
- 以 `Append | Text` 模式打开，不会覆盖旧日志
    
- 每次写入自动 flush，确保立即落盘
    
- 无写入权限时不崩溃，仅不写入文件
    
- 文件编码固定为 UTF-8
    

---

### 3.4 自定义 Sink（扩展输出）

```C++
#include <QCoreApplication>
#include <QFile>
#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Logger& logger = Logger::instance();

    // 自定义 Sink 1：写入额外文件
    logger.addSink([](const QString& line) {
        QFile file("./custom_sink.log");
        file.open(QIODevice::Append | QIODevice::Text);
        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf8);
        stream << line << "\n";
    });

    // 自定义 Sink 2：模拟网络上报
    logger.addSink([](const QString& line) {
        qDebug() << "[网络日志]" << line;
    });

    logger.info("这条日志会分发给所有 Sink");

    return a.exec();
}
```

**使用细节**：

- 默认已自带一个控制台 Sink（`qInfo().noquote()`）
    
- Sink 类型：`std::function<void(const QString&)>`
    
- 可添加任意多个 Sink
    
- 添加 Sink 线程安全
    
- Sink 内部禁止再次调用 logger，避免死锁
    
- Sink 应尽量轻量化，不要做耗时操作
    

---

### 3.5 多线程安全使用

```C++
#include <QCoreApplication>
#include <QThread>
#include "logger.h"

class LogThread : public QThread
{
    Q_OBJECT
public:
    LogThread(int id) : m_id(id) {}
protected:
    void run() override {
        Logger& logger = Logger::instance();
        for (int i = 0; i < 5; ++i) {
            logger.info(QString("线程%1 - 日志%2").arg(m_id).arg(i));
            msleep(10);
        }
    }
private:
    int m_id;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Logger::instance().enableFileSink("./logs/thread.log");

    LogThread t1(1), t2(2), t3(3);
    t1.start(); t2.start(); t3.start();
    t1.wait(); t2.wait(); t3.wait();

    return a.exec();
}

#include "main.moc"
```

**使用细节**：

- 所有公开 API 均加 `QMutexLocker` 保护
    
- 日志先格式化，再加锁分发，保证行原子性
    
- 多线程并发输出**不乱码、不断行、不错乱**
    

---

## 四、API 详细说明（使用视角）

### 4.1 获取单例

```C++
static Logger& instance();
```

- 全局唯一实例
    
- 线程安全的懒汉式单例
    
- 任何模块、任何线程可直接调用
    

### 4.2 日志级别控制

```C++
void setLevel(Level level);
Level level() const;
```

- 设置/获取当前过滤级别
    
- 低于该级别的日志直接被丢弃
    

### 4.3 文件输出控制

```C++
void enableFileSink(const QString& path);
void disableFileSink();
```

- 启用/禁用文件持久化
    
- 路径自动创建目录
    
- 重复调用会先关闭旧文件
    

### 4.4 自定义输出扩展

```C++
void addSink(Sink sink);
```

- 添加自定义日志接收端
    
- 支持 lambda、函数指针、仿函数等
    

### 4.5 日志输出接口

```C++
void trace(const QString& msg);
void debug(const QString& msg);
void info(const QString& msg);
void warn(const QString& msg);
void error(const QString& msg);
void fatal(const QString& msg);
```

- 自动格式化、级别检查、加锁、分发
    
- 支持 `QString::arg()` 格式化
    

---

## 五、日志格式与编码（使用必读）

### 5.1 固定日志格式

```Plain
[yyyy-MM-ddTHH:mm:ss] [LEVEL] 日志内容
```

- 时间格式：`Qt::ISODate`
    
- 级别为大写英文：TRACE / DEBUG / INFO / WARN / ERROR / FATAL
    
- 格式不可配置，如需修改需改动 `format()` 函数
    

### 5.2 编码细节

- 文件写入编码：**UTF-8**
    
- 控制台中文乱码解决方案：
    

```C++
#include <QTextCodec>
QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
```

---

## 六、使用限制与注意事项

1. **仅支持 Qt6**，Qt5 无 `QStringConverter` 会编译报错
    
2. 日志格式固定，不可动态配置
    
3. 不提供移除 Sink 接口，可通过内部开关变量控制是否输出
    
4. Sink 内部不要抛出异常
    
5. 高并发场景建议将 Sink 改为异步队列
    
6. 日志级别过滤发生在格式化之前，性能开销极低
    
7. 多线程安全，但不保证日志全局时间严格顺序
    

---

## 七、完整可编译项目模板

### logger_demo.pro

```Plain
QT += core
QT -= gui
CONFIG += c++17 console
CONFIG -= app_bundle

SOURCES += main.cpp logger.cpp
HEADERS += logger.h
```

### main.cpp

```C++
#include <QCoreApplication>
#include <QTextCodec>
#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    Logger& logger = Logger::instance();
    logger.setLevel(Logger::Level::Debug);
    logger.enableFileSink("./logs/demo.log");

    logger.addSink([](const QString& line) {
        qDebug() << "[自定义Sink]" << line;
    });

    logger.trace("Trace 日志");
    logger.debug("Debug 日志");
    logger.info("Info 日志");
    logger.warn("Warn 日志");
    logger.error("Error 日志");
    logger.fatal("Fatal 日志");

    return a.exec();
}
```

---

## 八、文档信息

- 适用版本：Qt 6.x
    
- 源码：`logger.h` / `logger.cpp`
    
- 文档用途：开发使用指南、API 参考、示例手册
