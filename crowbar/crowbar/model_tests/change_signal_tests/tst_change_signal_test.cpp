#include <QString>
#include <QtTest>
#include "mapdocument.h"
#include "mapbrush.h"
#include "mapvertex.h"
#include "mapedge.h"
#include "mapface.h"
#include <QList>
#include <QtDebug>

using namespace MODEL_NAMESPACE;

class SignalTester : public QObject
{
    Q_OBJECT
public:
    SignalTester(QObject* parent = 0) : QObject(parent),
        m_iSignalsReceived(0), m_Signals()
    {
    }

    int signalCount() const
    {
        return m_iSignalsReceived;
    }

    const QList<ChangeId_t>& signalsReceived() const
    {
        return m_Signals;
    }

public slots:
    void receiveChangeSignal(ChangeId_t id)
    {
        m_Signals.append(id);
        m_iSignalsReceived++;
    }

    void clearStoredSignals()
    {
        m_Signals.clear();
        m_iSignalsReceived = 0;
    }

private:
    int                 m_iSignalsReceived;
    QList<ChangeId_t>   m_Signals;
};

class Change_Signal_Test : public QObject
{
    Q_OBJECT

public:
    Change_Signal_Test();

private Q_SLOTS:
    void testSignalPropagation();
};

Change_Signal_Test::Change_Signal_Test()
{
}

void Change_Signal_Test::testSignalPropagation()
{
    MapDocument doc;
    MapBrush* b = new MapBrush(NULL);

    // Add vertices.
    MapVertex* v1 = new MapVertex(QVector3D(0,0,0));
    b->vertices().append(v1);

    MapVertex* v2 = new MapVertex(QVector3D(1,0,0));
    b->vertices().append(v2);

    MapVertex* v3 = new MapVertex(QVector3D(0,1,0));
    b->vertices().append(v3);

    // Add edges.
    MapEdge* e1 = new MapEdge(v1, v2);
    b->edges().append(e1);

    MapEdge* e2 = new MapEdge(v2, v3);
    b->edges().append(e2);

    MapEdge* e3 = new MapEdge(v3, v1);
    b->edges().append(e3);

    // Add faces.
    MapFace* f1 = new MapFace();
    f1->vertices().append(v1);
    f1->vertices().append(v2);
    f1->vertices().append(v3);
    b->faces().append(f1);

    // Add brush to document.
    b->setParentComponent(doc.world());

    // Monitor brush signals.
    SignalTester rcv;
    connect(b, &MapBrush::renderDataChanged, &rcv, &SignalTester::receiveChangeSignal);

    // Change a vertex.
    v1->setPosition(QVector3D(0,0,1));

    // Ensure that only one signal was received.
    QVERIFY2(rcv.signalCount() == 1, "Incorrect number of change signals received.");

    // Output the signals received, for info.
    foreach ( ChangeId_t id, rcv.signalsReceived() )
    {
        qDebug() << "Signal ID:" << id;
    }
}

QTEST_APPLESS_MAIN(Change_Signal_Test)

#include "tst_change_signal_test.moc"
