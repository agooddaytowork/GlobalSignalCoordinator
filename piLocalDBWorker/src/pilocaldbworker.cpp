#include "pilocaldbworker.h"

piLocalDBWorker::piLocalDBWorker(QObject *parent) : QStateMachine(parent)
{
    currentBasis = new piLocalDBWorkerBasis(this);
    QObject::connect(currentBasis, &piLocalDBWorkerBasis::Out, this, &piLocalDBWorker::Out);

    QState *main = new QState();
    main->setObjectName(QStringLiteral("main"));

    uninitiatedPiLocalDBWorker *state0 = new uninitiatedPiLocalDBWorker(currentBasis,main);
    idlePiLocalDBWorker *state1 = new idlePiLocalDBWorker(currentBasis,main);
    runningPiLocalDBWorker *state2 = new runningPiLocalDBWorker(currentBasis,main);
    sendJsonPiLocalDBWorker *state3 = new sendJsonPiLocalDBWorker(currentBasis,main);
    setIsSentPiLocalDBWorker *state4 = new setIsSentPiLocalDBWorker(currentBasis,main);

    state0->addTransition(currentBasis, &piLocalDBWorkerBasis::goIdle, state1);
    state1->addTransition(currentBasis, &piLocalDBWorkerBasis::GlobalSignalExecutionRequested, state2);
    state2->addTransition(currentBasis, &piLocalDBWorkerBasis::goIdle, state1);
    state2->addTransition(currentBasis, &piLocalDBWorkerBasis::GlobalSignalExecutionRequested, state2);
    state2->addTransition(currentBasis, &piLocalDBWorkerBasis::sendingPendingJsonDataPackage, state3);
    state3->addTransition(currentBasis, &piLocalDBWorkerBasis::jsonPackageTransmitted, state4);
    state3->addTransition(currentBasis, &piLocalDBWorkerBasis::GlobalSignalExecutionRequested, state2);
    state4->addTransition(currentBasis, &piLocalDBWorkerBasis::GlobalSignalExecutionRequested, state2);

    main->setInitialState(state0);

    errorPiLocalDBWorker *state7 = new errorPiLocalDBWorker(currentBasis);

    state7->addTransition(currentBasis, &piLocalDBWorkerBasis::GlobalSignalExecutionRequested, state2);

    main->addTransition(currentBasis, &piLocalDBWorkerBasis::ErrorOccurred, state7);

    addState(main);
    addState(state7);
    setInitialState(main);
    anIf(piLocalDBWorkerDbgEn, anAck("piLocalDBWorker Constructed"));
}

void piLocalDBWorker::In(const GlobalSignal &aGlobalSignal)
{
    if (isRunning())
    {
        currentBasis->In(aGlobalSignal);
    }
}
