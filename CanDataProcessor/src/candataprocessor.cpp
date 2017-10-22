#include "candataprocessor.h"

CanDataProcessor::CanDataProcessor(QObject *parent) : QStateMachine(parent)
{
    currentBasis = new CanDataProcessorBasis(this);
    QObject::connect(currentBasis, &CanDataProcessorBasis::Out, this, &CanDataProcessor::Out);

    QState *main = new QState();
    main->setObjectName(QStringLiteral("main"));

    uninitiatedCanDataProcessor *state0 = new uninitiatedCanDataProcessor(currentBasis,main);
    idleCanDataProcessor *state1 = new idleCanDataProcessor(currentBasis,main);
    runningCanDataProcessor *state2 = new runningCanDataProcessor(currentBasis,main);

    state0->addTransition(currentBasis, &CanDataProcessorBasis::goIdle, state1);
    state1->addTransition(currentBasis, &CanDataProcessorBasis::GlobalSignalExecutionRequested, state2);
    state2->addTransition(currentBasis, &CanDataProcessorBasis::goIdle, state1);
    state2->addTransition(currentBasis, &CanDataProcessorBasis::GlobalSignalExecutionRequested, state2);

    main->setInitialState(state0);

    errorCanDataProcessor *state7 = new errorCanDataProcessor(currentBasis);

    main->addTransition(currentBasis, &CanDataProcessorBasis::ErrorOccurred, state7);

    addState(main);
    addState(state7);
    setInitialState(main);

    anIf(CanDataProcessorBasisDbgEn, anAck("CanDataProcessor Constructed"));
}

void CanDataProcessor::In(const GlobalSignal &aGlobalSignal)
{
    if (isRunning())
    {
        currentBasis->In(aGlobalSignal);
    }
}
