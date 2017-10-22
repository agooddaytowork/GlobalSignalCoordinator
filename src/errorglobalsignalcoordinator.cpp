#include "errorglobalsignalcoordinator.h"

errorGlobalSignalCoordinator::errorGlobalSignalCoordinator(GlobalSignalCoordinatorBasis *parentBasis, QState *parent) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("errorGlobalSignalCoordinator"));
    anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("errorGlobalSignalCoordinator Constructed"));
}

void errorGlobalSignalCoordinator::onEntry(QEvent *)
{
    anIf(GlobalSignalCoordinatorBasisDbgEn, anTrk("errorGlobalSignalCoordinator Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->errorGlobalSignalCoordinatorOnEntry();
}

void errorGlobalSignalCoordinator::onExit(QEvent *)
{
    anIf(GlobalSignalCoordinatorBasisDbgEn, anTrk("Leave errorGlobalSignalCoordinator"));
    basisptr->clearError();
    basisptr->previousStateName = objectName();
}
