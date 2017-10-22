#include "runningglobalsignalcoordinator.h"

runningGlobalSignalCoordinator::runningGlobalSignalCoordinator(GlobalSignalCoordinatorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("runningGlobalSignalCoordinator"));
    anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("runningGlobalSignalCoordinator Constructed"));
}

void runningGlobalSignalCoordinator::onEntry(QEvent *)
{
    anIf(GlobalSignalCoordinatorBasisDbgEn, anTrk("runningGlobalSignalCoordinator Entered"));
    basisptr->currentStateName = objectName();
}

void runningGlobalSignalCoordinator::onExit(QEvent *)
{
    anIf(GlobalSignalCoordinatorBasisDbgEn, anTrk("Leave runningGlobalSignalCoordinator"));
    basisptr->previousStateName = objectName();
}
