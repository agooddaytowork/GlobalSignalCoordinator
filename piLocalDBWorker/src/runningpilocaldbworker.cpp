#include "runningpilocaldbworker.h"

runningPiLocalDBWorker::runningPiLocalDBWorker(piLocalDBWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("runningPiLocalDBWorker"));
    anIf(piLocalDBWorkerBasisDbgEn, anAck("runningPiLocalDBWorker Constructed"));
}

void runningPiLocalDBWorker::onEntry(QEvent *)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("runningPiLocalDBWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->runningPiLocalDBWorkerOnEntry();
}

void runningPiLocalDBWorker::onExit(QEvent *)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("Leave runningPiLocalDBWorker"));
    basisptr->previousStateName = objectName();
}
