// #include "core/configmanager.h"
#include "core/systemmonitor.h"
#include "thread/threadmanager.h"
#include "viewmodel/processlistviewmodel.h"
#include "viewmodel/systemstatsviewmodel.h"
// #include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include <QtCharts/QChartView>
#include <QtPlugin>

int main(int argc, char *argv[])
{
    // QGuiApplication app(argc, argv);
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Tạo các thành phần backend
    SystemMonitor* systemMonitor = new SystemMonitor;
    ConfigManager* configManager = new ConfigManager;
    ThreadManager* threadMgr = new ThreadManager();
    SystemStatsViewModel* systemStatsViewModel = new SystemStatsViewModel;
    ProcessListViewModel* processListViewModel = new ProcessListViewModel;

    threadMgr->setup(systemMonitor, configManager);
    // Kết nối: khi Monitor có dữ liệu mới → ViewModel cập nhật
    systemStatsViewModel->bindToMonitor(systemMonitor);
    processListViewModel->bindToMonitor(systemMonitor);

    //======CPU, MEM LineChart=====
    engine.rootContext()->setContextProperty("SystemStatsVM", systemStatsViewModel);
    engine.rootContext()->setContextProperty("ProcessListVM", processListViewModel);

    //=========================
    engine.rootContext()->setContextProperty("SystemMonitor", systemMonitor);
    engine.rootContext()->setContextProperty("ConfigManager", configManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("MyWindownApp", "Main");

    // systemMonitor->startMonitoring();

    // Dùng QTimer để truyền fake data mỗi 1 giây
    QTimer* timer = new QTimer;
    QObject::connect(timer, &QTimer::timeout, systemMonitor, [systemMonitor](){
        QByteArray fakeData = systemMonitor->generateFakeData();
        systemMonitor->feedFakeData(fakeData);
    });
    timer->start(1000);

    return app.exec();
}
