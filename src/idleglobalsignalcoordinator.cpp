#include "idleglobalsignalcoordinator.h"

idleGlobalSignalCoordinator::idleGlobalSignalCoordinator(GlobalSignalCoordinatorBasis *parentBasis, QState *paret) :
    QState(parent),
    basisptr(parentBasis)
{
    setObjectName(QStringLiteral("idleGlobalSignalCoordinator"));
    timer.setParent(this);
    timer.setInterval(45000);
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, this, [&](){
        GlobalSignal enableAllAutoSignallers;
        enableAllAutoSignallers.Type = QVariant::fromValue(GlobalSignalCoordinatorBasis::enableAllAutoSignallers);
        enableAllAutoSignallers.DstStrs.append(GlobalSignalCoordinatorObjName);
        basisptr->pushAGlobalSignalIntoPrioritizedBuffer(enableAllAutoSignallers);
        emit basisptr->GlobalSignalExecutionRequested();
    });
    anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("idleGlobalSignalCoordinator Constructed"));
}

void idleGlobalSignalCoordinator::onEntry(QEvent *)
{
    anIf(GlobalSignalCoordinatorBasisDbgEn, anTrk("idleGlobalSignalCoordinator Entered"));
    basisptr->currentStateName = objectName();
    qApp->processEvents();
    timer.start();
}

void idleGlobalSignalCoordinator::onExit(QEvent *)
{
    anIf(GlobalSignalCoordinatorBasisDbgEn, anTrk("Leave idleGlobalSignalCoordinator"));
    timer.stop();
    basisptr->previousStateName = objectName();
}
