#include "serialportdataprocessor.h"

SerialPortDataProcessor::SerialPortDataProcessor(bool isUHV2, QObject *parent) : QStateMachine(parent)
{
    currentBasis = new SerialPortDataProcessorBasis(isUHV2,this);
    QObject::connect(currentBasis, &SerialPortDataProcessorBasis::Out, this, &SerialPortDataProcessor::Out);

    QState *main = new QState();
    main->setObjectName(QStringLiteral("main"));

    uninitiatedSerialPortDataProcessor *state0 = new uninitiatedSerialPortDataProcessor(currentBasis,main);
    idleSerialPortDataProcessor *state1 = new idleSerialPortDataProcessor(currentBasis,main);
    runningSerialPortDataProcessor *state2 = new runningSerialPortDataProcessor(currentBasis,main);

    state0->addTransition(currentBasis, &SerialPortDataProcessorBasis::goIdle, state1);
    state1->addTransition(currentBasis, &SerialPortDataProcessorBasis::GlobalSignalExecutionRequested, state2);
    state2->addTransition(currentBasis, &SerialPortDataProcessorBasis::goIdle, state1);
    state2->addTransition(currentBasis, &SerialPortDataProcessorBasis::GlobalSignalExecutionRequested, state2);

    main->setInitialState(state0);

    errorSerialPortDataProcessor *state7 = new errorSerialPortDataProcessor(currentBasis);

    main->addTransition(currentBasis, &SerialPortDataProcessorBasis::ErrorOccurred, state7);

    addState(main);
    addState(state7);
    setInitialState(main);
    anIf(SerialPortDataProcessorBasisDbgEn, anAck("SerialPortDataProcessor Constructed"));

}

void SerialPortDataProcessor::In(const GlobalSignal &aGlobalSignal)
{
    if (isRunning())
    {
        currentBasis->In(aGlobalSignal);
    }
}
