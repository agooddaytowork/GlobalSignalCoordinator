#include "idlepilocaldbworker.h"

idlePiLocalDBWorker::idlePiLocalDBWorker(piLocalDBWorkerBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("idlePiLocalDBWorker"));
    anIf(piLocalDBWorkerBasisDbgEn, anAck("idlePiLocalDBWorker Constructed"));
}

void idlePiLocalDBWorker::onEntry(QEvent *)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("idlePiLocalDBWorker Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->idlePiLocalDBWorkerOnEntry();
}

void idlePiLocalDBWorker::onExit(QEvent *)
{
    anIf(piLocalDBWorkerBasisDbgEn, anTrk("Leave idlePiLocalDBWorker"));
    basisptr->previousStateName = objectName();
}
