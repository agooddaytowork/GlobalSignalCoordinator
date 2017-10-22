#include "uninitiatedpilocaldbworker.h"

uninitiatedPiLocalDBWorker::uninitiatedPiLocalDBWorker(piLocalDBWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("uninitiatedPiLocalDBWorker"));
    anIf(piLocalDBWorkerBasisDbgEn, anAck("uninitiatedPiLocalDBWorker Constructed"));
}

void uninitiatedPiLocalDBWorker::onEntry(QEvent *)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("uninitiatedPiLocalDBWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->uninitiatedPiLocalDBWorkerOnEntry();
}

void uninitiatedPiLocalDBWorker::onExit(QEvent *)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("Leave uninitiatedPiLocalDBWorker"));
    basisptr->previousStateName = objectName();
}
