#include "uninitiatedglobalsignalcoordinator.h"

uninitiatedGlobalSignalCoordinator::uninitiatedGlobalSignalCoordinator(GlobalSignalCoordinatorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("uninitiatedGlobalSignalCoordinator"));
    anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("uninitiatedGlobalSignalCoordinator Constructed"));
}

void uninitiatedGlobalSignalCoordinator::onEntry(QEvent *)
{
    anIf(GlobalSignalCoordinatorBasisDbgEn, anTrk("uninitiatedGlobalSignalCoordinator Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->uninitiatedGlobalSignalCoordinatorOnEntry();
}

void uninitiatedGlobalSignalCoordinator::onExit(QEvent *)
{
    anIf(GlobalSignalCoordinatorBasisDbgEn, anTrk("Leave uninitiatedGlobalSignalCoordinator"));
    basisptr->previousStateName = objectName();
}
