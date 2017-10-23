#include "globalsignalcoordinatorbasis.h"

GlobalSignalCoordinatorBasis::GlobalSignalCoordinatorBasis(QObject *parent) : AbstractStateMachineBasis(parent)
{
    registerGlobalSignal;
    anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("GlobalSignalCoordinatorBasis Constructed"));
}

GlobalSignalCoordinatorBasis::~GlobalSignalCoordinatorBasis()
{
    anIf(GlobalSignalCoordinatorBasisDbgEn && isInitiated, anWarn("Destroy GlobalSignalCoordinatorBasis"));
    dispose();
}

void GlobalSignalCoordinatorBasis::uninitiatedGlobalSignalCoordinatorOnEntry()
{
    if (!isInitiated)
    {
        initiate();
    }
}

void GlobalSignalCoordinatorBasis::runningGlobalSignalCoordinatorOnEntry()
{
    clearCache();
    if (prioritizedBuffer.size())
    {
        currentGlobalSignal = popMostPrioritizedGlobalSignalOutOfPrioritizedBuffer();
        if (currentGlobalSignal.DstStrs.size())
        {
            QString currentDestination = currentGlobalSignal.DstStrs.takeFirst();
            if (currentDestination == piLocalDBWorkerObjName)
            {
                emit ToPiLocalDBWorker(currentGlobalSignal);
            }
            else if (currentDestination == UHV2SerialPortWorkerObjName)
            {
                emit ToUHV2SerialPortWorker(currentGlobalSignal);
            }
            else if (currentDestination == UHV4SerialPortWorkerObjName)
            {
                emit ToUHV4SerialPortWorker(currentGlobalSignal);
            }
            else if (currentDestination == UHV2SerialPortDataProcessorObjName)
            {
                emit ToUHV2SerialPortDataProcessor(currentGlobalSignal);
            }
            else if (currentDestination == UHV4SerialPortDataProcessorObjName)
            {
                emit ToUHV4SerialPortDataProcessor(currentGlobalSignal);
            }
            else if (currentDestination == CanBusWorkerObjName)
            {
                emit ToCanBusWorker(currentGlobalSignal);
            }
            else if (currentDestination == GlobalSignalCoordinatorObjName)
            {
                if (currentGlobalSignal.Type.typeName() == QStringLiteral("GlobalSignalCoordinatorBasis::Data"))
                {
                    switch (currentGlobalSignal.Type.toInt()) {
                    case enableAutoSignallerForUHV2SerialPort:
                    {
                        GlobalSignal enableAutoSignaller4UHV2SerialPort;
                        enableAutoSignaller4UHV2SerialPort.Type = QVariant::fromValue(SerialPortDataProcessorBasis::enableAutoSignaller);
                        enableAutoSignaller4UHV2SerialPort.DstStrs.append(UHV2SerialPortDataProcessorObjName);
                        emit ToUHV2SerialPortDataProcessor(enableAutoSignaller4UHV2SerialPort);
                        break;
                    }
                    case enableAutoSignallerForUHV4SerialPort:
                    {
                        GlobalSignal enableAutoSignaller4UHV4SerialPort;
                        enableAutoSignaller4UHV4SerialPort.Type = QVariant::fromValue(SerialPortDataProcessorBasis::enableAutoSignaller);
                        enableAutoSignaller4UHV4SerialPort.DstStrs.append(UHV4SerialPortDataProcessorObjName);
                        emit ToUHV4SerialPortDataProcessor(enableAutoSignaller4UHV4SerialPort);
                        break;
                    }
                    case enableAutoSignallerForCanBus:
                    {
                        GlobalSignal enableAutoSignaller4CanBus;
                        enableAutoSignaller4CanBus.Type = QVariant::fromValue(CanDataProcessorBasis::enableAutoSignaller);
                        enableAutoSignaller4CanBus.DstStrs.append(CanDataProcessorObjName);
                        emit ToCanDataProcessor(enableAutoSignaller4CanBus);
                        break;
                    }
                    case enableAllAutoSignallers:
                    {
                        GlobalSignal queueAutoSignallersEnable;
                        queueAutoSignallersEnable.Type = QVariant::fromValue(enableAutoSignallerForUHV2SerialPort);
                        queueAutoSignallersEnable.DstStrs.append(GlobalSignalCoordinatorObjName);
                        pushAGlobalSignalIntoPrioritizedBuffer(queueAutoSignallersEnable);
                        queueAutoSignallersEnable.Type = QVariant::fromValue(enableAutoSignallerForUHV4SerialPort);
                        pushAGlobalSignalIntoPrioritizedBuffer(queueAutoSignallersEnable);
                        queueAutoSignallersEnable.Type = QVariant::fromValue(enableAutoSignallerForCanBus);
                        pushAGlobalSignalIntoPrioritizedBuffer(queueAutoSignallersEnable);
                        break;
                    }
                    case disableAutoSignallerForUHV2SerialPort:
                    {
                        GlobalSignal disableAutoSignaller4UHV2SerialPort;
                        disableAutoSignaller4UHV2SerialPort.Type = QVariant::fromValue(SerialPortDataProcessorBasis::disableAutoSignaller);
                        disableAutoSignaller4UHV2SerialPort.DstStrs.append(UHV2SerialPortDataProcessorObjName);
                        emit ToUHV2SerialPortDataProcessor(disableAutoSignaller4UHV2SerialPort);
                        break;
                    }
                    case disableAutoSignallerForUHV4SerialPort:
                    {
                        GlobalSignal disableAutoSignaller4UHV4SerialPort;
                        disableAutoSignaller4UHV4SerialPort.Type = QVariant::fromValue(SerialPortDataProcessorBasis::disableAutoSignaller);
                        disableAutoSignaller4UHV4SerialPort.DstStrs.append(UHV4SerialPortDataProcessorObjName);
                        emit ToUHV4SerialPortDataProcessor(disableAutoSignaller4UHV4SerialPort);
                        break;
                    }
                    case disableAutoSignallerForCanBus:
                    {
                        GlobalSignal disableAutoSignaller4CanBus;
                        disableAutoSignaller4CanBus.Type = QVariant::fromValue(CanDataProcessorBasis::disableAutoSignaller);
                        disableAutoSignaller4CanBus.DstStrs.append(CanDataProcessorObjName);
                        emit ToCanDataProcessor(disableAutoSignaller4CanBus);
                        break;
                    }
                    case disableAllAutoSignallers:
                    {
                        GlobalSignal queueAutoSignallersDisable;
                        queueAutoSignallersDisable.Type = QVariant::fromValue(disableAutoSignallerForUHV2SerialPort);
                        queueAutoSignallersDisable.DstStrs.append(GlobalSignalCoordinatorObjName);
                        pushAGlobalSignalIntoPrioritizedBuffer(queueAutoSignallersDisable);
                        queueAutoSignallersDisable.Type = QVariant::fromValue(disableAutoSignallerForUHV4SerialPort);
                        pushAGlobalSignalIntoPrioritizedBuffer(queueAutoSignallersDisable);
                        queueAutoSignallersDisable.Type = QVariant::fromValue(disableAutoSignallerForCanBus);
                        pushAGlobalSignalIntoPrioritizedBuffer(queueAutoSignallersDisable);
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
        }
    }
    if (prioritizedBuffer.isEmpty())
    {
        emit goIdle();
    }
    else
    {
        emit GlobalSignalExecutionRequested();
    }
}

void GlobalSignalCoordinatorBasis::errorGlobalSignalCoordinatorOnEntry()
{
    anIf(GlobalSignalCoordinatorBasisDbgEn,
         anError("Emit GlobalSignalCoordinatorBasis::Error");
         anInfo("ErrorType: " + QString(ErrorMetaEnum.valueToKey(static_cast<int>(ErrorType))));
         anInfo("ErrorInfo: " + ErrorInfo);
    );
    GlobalSignal errorGlobalSignal;
    errorGlobalSignal.Type = QVariant::fromValue(ErrorType);
    errorGlobalSignal.Data = QVariant::fromValue(ErrorInfo);
    errorGlobalSignal.Priority = 200;
    emit Out(errorGlobalSignal);
}

void GlobalSignalCoordinatorBasis::clearError()
{
    anIf(GlobalSignalCoordinatorBasisDbgEn && (ErrorType!=NoError), anWarn("Clear Error !"));
    ErrorType = NoError;
    ErrorInfo.clear();
}

void GlobalSignalCoordinatorBasis::In(const GlobalSignal &aGlobalSignal)
{
    if (!isInitiated)
    {
        QString aGlobalSignalTypeTypeName = aGlobalSignal.Type.typeName();
        int aGlobalSignalTypeToInt = aGlobalSignal.Type.toInt();
        if ((aGlobalSignalTypeTypeName==QStringLiteral("SerialPortWorkerBasis::Data"))
                && (aGlobalSignalTypeToInt==SerialPortWorkerBasis::requestPortName))
        {
            QString SenderName = aGlobalSignal.Data.toString();
            GlobalSignal replyUHVPortName;
            replyUHVPortName.Type = QVariant::fromValue(SerialPortWorkerBasis::replyPortName);
            if (SenderName == UHV2SerialPortWorkerObjName)
            {
                replyUHVPortName.DstStrs.append(UHV2SerialPortWorkerObjName);
#ifdef __anWINOS__
                replyUHVPortName.Data = QVariant::fromValue(QStringLiteral("COM3"));
#else
                replyUHVPortName.Data = QVariant::fromValue(QStringLiteral("ttyUSB0"));
#endif
                emit ToUHV2SerialPortWorker(replyUHVPortName);
            }
            else if (SenderName == UHV4SerialPortWorkerObjName)
            {
                replyUHVPortName.DstStrs.append(UHV4SerialPortWorkerObjName);
#ifdef __anWINOS__
                replyUHVPortName.Data = QVariant::fromValue(QStringLiteral("COM5"));
#else
                replyUHVPortName.Data = QVariant::fromValue(QStringLiteral("ttyUSB1"));
#endif
                emit ToUHV4SerialPortWorker(replyUHVPortName);
            }
        }
        else
        {
            if ((aGlobalSignalTypeTypeName == QStringLiteral("piLocalDBWorkerBasis::Notification"))
                    && (aGlobalSignalTypeToInt == piLocalDBWorkerBasis::readyToWork))
            {
                anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("piLocalDBWorkerBasis::readyToWork"));
                if (aGlobalSignal.Data.toString() == piLocalDBWorkerObjName)
                {
                    isPiLocalDBWorkerReady = true;
                    anIf(GlobalSignalCoordinatorBasisDbgEn, anVar(isPiLocalDBWorkerReady));
                }
            }
            else if ((aGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerBasis::Notification"))
                        && (aGlobalSignalTypeToInt == SerialPortWorkerBasis::readyToWork))
            {
                anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("SerialPortWorkerBasis::readyToWork"));
                QString WhoIsReadyToWork = aGlobalSignal.Data.toString();
                if (WhoIsReadyToWork == UHV2SerialPortWorkerObjName)
                {
                    isUHV2SerialPortWorkerReady = true;
                    anIf(GlobalSignalCoordinatorBasisDbgEn, anVar(isUHV2SerialPortWorkerReady));
                }
                else if (WhoIsReadyToWork == UHV4SerialPortWorkerObjName)
                {
                    isUHV4SerialPortWorkerReady = true;
                    anIf(GlobalSignalCoordinatorBasisDbgEn, anVar(isUHV4SerialPortWorkerReady));
                }
            }
            else if ((aGlobalSignalTypeTypeName == QStringLiteral("SerialPortDataProcessorBasis::Notification"))
                        && (aGlobalSignalTypeToInt == SerialPortDataProcessorBasis::readyToWork))
            {
                anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("SerialPortDataProcessorBasis::readyToWork"));

                QString WhoIsReadyToWork = aGlobalSignal.Data.toString();
                if (WhoIsReadyToWork == UHV2SerialPortDataProcessorObjName)
                {
                    isUHV2SerialPortDataProcessorReady = true;
                    anIf(GlobalSignalCoordinatorBasisDbgEn, anVar(isUHV2SerialPortDataProcessorReady));
                }
                else if (WhoIsReadyToWork == UHV4SerialPortDataProcessorObjName)
                {
                    isUHV4SerialPortDataProcessorReady = true;
                    anIf(GlobalSignalCoordinatorBasisDbgEn, anVar(isUHV4SerialPortDataProcessorReady));
                }
            }
            else if ((aGlobalSignalTypeTypeName == QStringLiteral("CanBusWorkerBasis::Notification"))
                        && (aGlobalSignalTypeToInt == CanBusWorkerBasis::readyToWork))
            {
                anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("CanBusWorkerBasis::Notification"));
                if (aGlobalSignal.Data.toString() == CanBusWorkerObjName)
                {
                    isCanBusWorkerReady = true;
                    anIf(GlobalSignalCoordinatorBasisDbgEn, anVar(isCanBusWorkerReady));
                }
            }
            else if ((aGlobalSignalTypeTypeName == QStringLiteral("CanDataProcessorBasis::Notification"))
                        && (aGlobalSignalTypeToInt == CanDataProcessorBasis::readyToWork))
            {
                anIf(GlobalSignalCoordinatorBasisDbgEn, anAck("CanDataProcessorBasis::Notification"));
                if (aGlobalSignal.Data.toString() == CanDataProcessorObjName)
                {
                    isCanDataProcessorReady = true;
                    anIf(GlobalSignalCoordinatorBasisDbgEn, anVar(isCanDataProcessorReady));
                }
            }
            if (areWorkersReady())
            {
                emit InitiationRequested();
            }
        }        
    }
    else
    {
        pushAGlobalSignalIntoPrioritizedBuffer(aGlobalSignal);
        if (currentStateName == QStringLiteral("idleGlobalSignalCoordinator"))
        {
            emit GlobalSignalExecutionRequested();
        }
    }
}

void GlobalSignalCoordinatorBasis::initiate()
{
    if (areWorkersReady())
    {
        isInitiated = true;
        emit goIdle();
    }
    anIf(GlobalSignalCoordinatorBasisDbgEn && isInitiated, anAck("GlobalSignalCoordinatorBasis Initiated"));
}

void GlobalSignalCoordinatorBasis::dispose()
{
    anIf(GlobalSignalCoordinatorBasisDbgEn && isInitiated, anWarn("Clean GlobalSignalCoordinatorBasis"));
    clearPrioritizedBuffer();
    clearCache();
    clearError();
    currentStateName.clear();
    previousStateName.clear();
    isInitiated = false;
}

void GlobalSignalCoordinatorBasis::setError(const GlobalSignalCoordinatorBasis::Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType!=NoError)
    {
        anIf(GlobalSignalCoordinatorBasisDbgEn, anWarn("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void GlobalSignalCoordinatorBasis::clearCache()
{
    currentGlobalSignal = GlobalSignal();
}

bool GlobalSignalCoordinatorBasis::areWorkersReady()
{
    return isPiLocalDBWorkerReady
            && isUHV2SerialPortWorkerReady
            && isUHV4SerialPortWorkerReady
            && isUHV2SerialPortDataProcessorReady
            && isUHV4SerialPortDataProcessorReady
        #ifndef __anWINOS__
            && isCanBusWorkerReady
            && isCanDataProcessorReady
        #endif
            ;
}

const QMetaEnum GlobalSignalCoordinatorBasis::DataMetaEnum = QMetaEnum::fromType<GlobalSignalCoordinatorBasis::Data>();
const QMetaEnum GlobalSignalCoordinatorBasis::ErrorMetaEnum = QMetaEnum::fromType<GlobalSignalCoordinatorBasis::Error>();
const QMetaEnum GlobalSignalCoordinatorBasis::WarningMetaEnum = QMetaEnum::fromType<GlobalSignalCoordinatorBasis::Warning>();
const QMetaEnum GlobalSignalCoordinatorBasis::NotificationMetaEnum = QMetaEnum::fromType<GlobalSignalCoordinatorBasis::Notification>();
