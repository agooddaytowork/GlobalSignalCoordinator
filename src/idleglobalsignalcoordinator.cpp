#include "idleglobalsignalcoordinator.h"

idleGlobalSignalCoordinator::idleGlobalSignalCoordinator(GlobalSignalCoordinatorBasis *parentBasis, QState *paret) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("idleGlobalSignalCoordinator"));
    timer.setParent(this);
    timer.setInterval(30000);
    timer.setSingleShot(true);
    anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("idleGlobalSignalCoordinator Constructed"));
}

void idleGlobalSignalCoordinator::onEntry(QEvent *)
{
    anIf(GlobalSignalCoordinatorBasisDbgEn, anTrk("idleGlobalSignalCoordinator Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    basisptr->idleGlobalSignalCoordinatorOnEntry();
}

void idleGlobalSignalCoordinator::onExit(QEvent *)
{
    anIf(GlobalSignalCoordinatorBasisDbgEn, anTrk("Leave idleGlobalSignalCoordinator"));
    basisptr->previousStateName = objectName();
}
