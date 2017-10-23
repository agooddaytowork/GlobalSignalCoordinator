#include "globalsignalcoordinator.h"

GlobalSignalCoordinator::GlobalSignalCoordinator(QObject *parent) : QStateMachine(parent)
{
    currentBasis = new GlobalSignalCoordinatorBasis(this);
    QObject::connect(currentBasis, &GlobalSignalCoordinatorBasis::ToPiLocalDBWorker,
                     this, &GlobalSignalCoordinator::ToPiLocalDBWorker, uniqueQtConnectionType);
    QObject::connect(currentBasis, &GlobalSignalCoordinatorBasis::ToUHV2SerialPortWorker,
                     this, &GlobalSignalCoordinator::ToUHV2SerialPortWorker, uniqueQtConnectionType);
    QObject::connect(currentBasis, &GlobalSignalCoordinatorBasis::ToUHV4SerialPortWorker,
                     this, &GlobalSignalCoordinator::ToUHV4SerialPortWorker, uniqueQtConnectionType);
    QObject::connect(currentBasis, &GlobalSignalCoordinatorBasis::ToUHV2SerialPortDataProcessor,
                     this, &GlobalSignalCoordinator::ToUHV2SerialPortDataProcessor, uniqueQtConnectionType);
    QObject::connect(currentBasis, &GlobalSignalCoordinatorBasis::ToUHV4SerialPortDataProcessor,
                     this, &GlobalSignalCoordinator::ToUHV4SerialPortDataProcessor, uniqueQtConnectionType);
    QObject::connect(currentBasis, &GlobalSignalCoordinatorBasis::ToCanBusWorker,
                     this, &GlobalSignalCoordinator::ToCanBusWorker, uniqueQtConnectionType);
    QObject::connect(currentBasis, &GlobalSignalCoordinatorBasis::ToCanDataProcessor,
                     this, &GlobalSignalCoordinator::ToCanDataProcessor, uniqueQtConnectionType);
    QObject::connect(currentBasis, &GlobalSignalCoordinatorBasis::Out,
                     this, &GlobalSignalCoordinator::Out, uniqueQtConnectionType);

    QState *main = new QState();
    main->setObjectName(QStringLiteral("main"));

    uninitiatedGlobalSignalCoordinator *state0 = new uninitiatedGlobalSignalCoordinator(currentBasis,main);
    idleGlobalSignalCoordinator *state1 = new idleGlobalSignalCoordinator(currentBasis,main);
    runningGlobalSignalCoordinator *state2 = new runningGlobalSignalCoordinator(currentBasis,main);

    state0->addTransition(currentBasis, &SerialPortDataProcessorBasis::goIdle, state1);
    state1->addTransition(currentBasis, &SerialPortDataProcessorBasis::GlobalSignalExecutionRequested, state2);
    state2->addTransition(currentBasis, &SerialPortDataProcessorBasis::goIdle, state1);
    state2->addTransition(currentBasis, &SerialPortDataProcessorBasis::GlobalSignalExecutionRequested, state2);

    main->addTransition(currentBasis, &SerialPortDataProcessorBasis::InitiationRequested, state0);
    main->setInitialState(state0);

    errorGlobalSignalCoordinator *state7 = new errorGlobalSignalCoordinator(currentBasis);

    main->addTransition(currentBasis, &SerialPortDataProcessorBasis::ErrorOccurred, state7);

    addState(main);
    addState(state7);
    setInitialState(main);

    QObject::connect(this, &GlobalSignalCoordinator::started, this, &GlobalSignalCoordinator::startWorkers);

    anIf(GlobalSignalCoordinatorDbgEn, anAck("GlobalSignalCoordinator Constructed"));
}

void GlobalSignalCoordinator::In(const GlobalSignal &aGlobalSignal)
{
    if (isRunning())
    {
        currentBasis->In(aGlobalSignal);
    }
}
