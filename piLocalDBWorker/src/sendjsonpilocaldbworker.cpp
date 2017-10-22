#include "sendjsonpilocaldbworker.h"

sendJsonPiLocalDBWorker::sendJsonPiLocalDBWorker(piLocalDBWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("sendJsonPiLocalDBWorker"));
    timer.setParent(this);
    timer.setInterval(7000);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [&](){
        basisptr->setOnOffLine(false);
        emit basisptr->GlobalSignalExecutionRequested();
    });
    anIf(piLocalDBWorkerBasisDbgEn, anAck("sendJsonPiLocalDBWorker Constructed"));
}

void sendJsonPiLocalDBWorker::onEntry(QEvent *)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("sendJsonPiLocalDBWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    if (basisptr->sendJsonPiLocalDBWorkerOnEntry())
    {        
        timer.start();
    }
}

void sendJsonPiLocalDBWorker::onExit(QEvent *)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("Leave sendJsonPiLocalDBWorker"));
    timer.stop();
    basisptr->previousStateName = objectName();
}
