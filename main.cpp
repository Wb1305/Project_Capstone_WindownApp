
#include "BE/Processes/updateprocessesui.h"
// #include "core/configmanager.h"
#include "core/systemmonitor.h"
#include "viewmodel/systemstatsviewmodel.h"
// #include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include <QtCharts/QChartView>
#include <QtPlugin>

// Q_IMPORT_PLUGIN()
// Q_IMPORT_PLUGIN(QtChartsQml2Plugin)

int main(int argc, char *argv[])
{
    // QGuiApplication app(argc, argv);
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Tạo các thành phần backend
    SystemMonitor* systemMonitor = new SystemMonitor;
    DataProcessor* processor = new DataProcessor;
    systemMonitor->setDataProcessor(processor);

    SystemStatsViewModel* systemStatsViewModel = new SystemStatsViewModel;

    // Kết nối: khi Monitor có dữ liệu mới → ViewModel cập nhật
    // QObject::connect(systemMonitor, &SystemMonitor::systemUpdated,
    //                  systemStatsViewModel, [=](const SystemStats& systemStats, const QVector<ProcessInfo>&){
    //                      systemStatsViewModel->updateFromStats(systemStats);
    //                  });
    systemStatsViewModel->bindToMonitor(systemMonitor);

    //======CPU, MEM LineChart=====



    // UpdateProcessesUI processModel;
    UpdateProcessesUI processModel;
    processModel.addProcess({"Qt Creator", "1123", "2%", "503MB", "0MB/s", "0Mbps"});
    processModel.addProcess({"Google Chrome", "2001", "3%", "248MB", "0MB/s", "0Mbps"});
    processModel.addProcess({"Microsoft Edge", "4423", "0%", "87MB", "0MB/s", "0Mbps"});

    engine.rootContext()->setContextProperty("ProcessModel", &processModel);
    engine.rootContext()->setContextProperty("SystemStatsVM", systemStatsViewModel);

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
    timer->start(2000);

    /*test configmanager*/
    // ConfigManager config;
    // QString ip = config.getValue("server/ip").toString();
    // int port = config.getValue("server/port").toInt();
    // qDebug()<<"server on: "<<ip<<", "<<port;
    return app.exec();
}
