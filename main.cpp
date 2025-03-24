
#include "BE/Processes/updateprocessesui.h"
// #include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QtCharts/QChartView>
#include <QtPlugin>

// Q_IMPORT_PLUGIN()
// Q_IMPORT_PLUGIN(QtChartsQml2Plugin)

int main(int argc, char *argv[])
{
    // QGuiApplication app(argc, argv);
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    UpdateProcessesUI processModel;

    processModel.addProcess({"Qt Creator", "1123", "2%", "503MB", "0MB/s", "0Mbps"});
    processModel.addProcess({"Google Chrome", "2001", "3%", "248MB", "0MB/s", "0Mbps"});
    processModel.addProcess({"Microsoft Edge", "4423", "0%", "87MB", "0MB/s", "0Mbps"});


    engine.rootContext()->setContextProperty("processModel", &processModel);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("MyWindownApp", "Main");

    return app.exec();
}
