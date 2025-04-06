#include "core/ivisocketserver.h"
#include <QTest>

class TestIviSocketServer  : public QObject{
    Q_OBJECT
private slots:
    void testStartListening();
};


void TestIviSocketServer ::testStartListening(){
    IviSocketServer myServer;
    myServer.startListening(8899);
    QVERIFY(myServer.isListening());
};

QTEST_MAIN(TestIviSocketServer )
#include "test_IviSocketServer.moc"
