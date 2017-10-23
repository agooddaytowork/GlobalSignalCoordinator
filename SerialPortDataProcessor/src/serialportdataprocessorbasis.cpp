#include "serialportdataprocessorbasis.h"

SerialPortDataProcessorBasis::SerialPortDataProcessorBasis(bool isAnUHV2, QObject *parent) :
    AbstractStateMachineBasis(parent),
    isUHV2(isAnUHV2)
{
    registerGlobalSignal;
    anIf(SerialPortDataProcessorBasisDbgEn, anAck("SerialPortDataProcessorBasis Constructed"));
}

SerialPortDataProcessorBasis::~SerialPortDataProcessorBasis()
{
    anIf(SerialPortDataProcessorBasisDbgEn, anWarn("Destroy SerialPortDataProcessorBasis"));
    dispose();
}

void SerialPortDataProcessorBasis::initiate()
{
    dispose();
    autoGlobalSignalToUHV.Type = QVariant::fromValue(SerialPortWorkerBasis::requestBytesTransmission);
    autoGlobalSignalToUHV.DstStrs.append(isUHV2?UHV2SerialPortWorkerObjName:UHV4SerialPortWorkerObjName);
    autoGlobalSignalToUHV.DstStrs.append(parent()->objectName());
    if (openLocalDatabaseConnection())
    {
        if (goToNextGlobalID())
        {
            isInitiated = true;
            emit goIdle();
        }
    }
    anIf(SerialPortDataProcessorBasisDbgEn && isInitiated, anWarn("SerialPortDataProcessorBasis Initialized"));
}

void SerialPortDataProcessorBasis::dispose()
{
    anIf(SerialPortDataProcessorBasisDbgEn && isInitiated, anWarn("Clean SerialPortDataProcessorBasis"));
    closeLocalDatabaseConnection();
    clearPrioritizedBuffer();
    previousStateName.clear();
    currentStateName.clear();
    clearCache();
    clearAutoSignallerCache();
    isInitiated = false;
}

void SerialPortDataProcessorBasis::setError(const SerialPortDataProcessorBasis::Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType != NoError)
    {
        anIf(SerialPortDataProcessorBasisDbgEn, anError("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void SerialPortDataProcessorBasis::clearError()
{
    anIf(SerialPortDataProcessorBasisDbgEn && (ErrorType!=NoError), anInfo("Clear Error !"));
    ErrorType = NoError;
    ErrorInfo.clear();
    lastQSqlError = QSqlError();
}

void SerialPortDataProcessorBasis::uninitiatedSerialPortDataProcessorOnEntry()
{
    if (!isInitiated)
    {
        initiate();
    }
}

void SerialPortDataProcessorBasis::idleSerialPortDataProcessorOnEntry()
{
    if (previousStateName == QStringLiteral("uninitiatedSerialPortDataProcessor"))
    {
        GlobalSignal iamReady;
        iamReady.Type = QVariant::fromValue(readyToWork);
        iamReady.Data = QVariant::fromValue(parent()->objectName());
        iamReady.TimeStamp = NOW2String;
        iamReady.DstStrs.append(GlobalSignalCoordinatorObjName);
        iamReady.SignalPriority = 200;
        pushAGlobalSignalIntoPrioritizedBuffer(iamReady);
        emit GlobalSignalExecutionRequested();
    }
}

void SerialPortDataProcessorBasis::autoSignaller()
{
    if (!isPressureMeasured)
    {
        autoGlobalSignalToUHV.Data = QVariant::fromValue(QBAReadP);
    }
    else if (!isVoltageMeasured)
    {
        autoGlobalSignalToUHV.Data = QVariant::fromValue(QBAReadV);
    }
    else if (!isCurrentMeasured)
    {
        autoGlobalSignalToUHV.Data = QVariant::fromValue(QBAReadI);
    }
    emit Out(GlobalSignal(autoGlobalSignalToUHV));
}

bool SerialPortDataProcessorBasis::arePVIDataReady()
{
    return isPressureMeasured && isVoltageMeasured && isCurrentMeasured;
}

void SerialPortDataProcessorBasis::runningSerialPortDataProcessorOnEntry()
{
    clearCache();
    if (prioritizedBuffer.size())
    {
        currentGlobalSignal = prioritizedBuffer.last().takeFirst();
        deleteEmptyListsFromPrioritizedBuffer();
        QString currentGlobalSignalTypeTypeName = currentGlobalSignal.Type.typeName();
        if (currentGlobalSignalTypeTypeName == QStringLiteral("SerialPortDataProcessorBasis::Data"))
        {
            switch (currentGlobalSignal.Type.toInt()) {
            case disableAutoSignaller:
            {
                anIf(SerialPortDataProcessorBasisDbgEn, anWarn("disableAutoSignaller"));
                emitCurrentPVIDataToLocalDatabase();
                isAutoSignallerEnabled = false;
                break;
            }
            case enableAutoSignaller:
            {
                anIf(SerialPortDataProcessorBasisDbgEn, anAck("disableAutoSignaller"));
                if (isPressureMeasured || isVoltageMeasured || isCurrentMeasured)
                {
                    goToNextGlobalID();
                }
                isAutoSignallerEnabled = true;
                break;
            }
            default:
                break;
            }
        }
        else if (currentGlobalSignalTypeTypeName == QStringLiteral("SerialPortDataProcessorBasis::Notification"))
        {
            switch (currentGlobalSignal.Type.toInt()) {
            case readyToWork:
            {
                anIf(SerialPortDataProcessorBasisDbgEn, anAck("readyToWork"));
                emit Out(GlobalSignal(currentGlobalSignal));
                break;
            }
            default:
                break;
            }
        }
        else if (currentGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerBasis::Data"))
        {            
            switch (currentGlobalSignal.Type.toInt()) {
            case SerialPortWorkerBasis::replyBytesWithTimeStamp:
            {
                anIf(SerialPortDataProcessorBasisDbgEn, anAck("SerialPortWorkerBasis::replyBytesWithTimeStamp"));
                if (isUHV2)
                {
                    BinaryProtocol reply = BinaryProtocol::fromQByteArray(currentGlobalSignal.Data.toByteArray());
                    if (reply.GetMessageDirection() == QStringLiteral("From")
                            && reply.GetBPNo() == currentPumpAddress
                            && reply.GetChannel() == currentPumpChannel)
                    {
                        QString replyTopic = reply.GetCommand();
                        if (replyTopic == QStringLiteral("ReadP"))
                        {
                            currentPVIData.Pressure = reply.GetData();
                            isPressureMeasured = true;
                        }
                        else if (replyTopic == QStringLiteral("ReadV"))
                        {
                            currentPVIData.Voltage = reply.GetData();
                            isVoltageMeasured = true;
                        }
                        else if (replyTopic == QStringLiteral("ReadI"))
                        {
                            currentPVIData.Current = reply.GetData();
                            isCurrentMeasured = true;
                        }
                    }
                }
                else
                {
                    WindowProtocol reply = WindowProtocol::fromQByteArray(currentGlobalSignal.Data.toByteArray());
                    if (reply.getWPNo() == currentPumpAddress && !reply.getDATA().isEmpty())
                    {
                        QString replyTopic = reply.getWINMean();
                        if (replyTopic.contains(QStringLiteral("PMeasured"),Qt::CaseInsensitive))
                        {
                            currentPVIData.Pressure = reply.getDATAMean();
                            isPressureMeasured = true;
                        }
                        else if (replyTopic.contains(QStringLiteral("VMeasured"),Qt::CaseInsensitive))
                        {
                            currentPVIData.Voltage = reply.getDATAMean();
                            isVoltageMeasured = true;
                        }
                        else if (replyTopic.contains(QStringLiteral("IMeasured"),Qt::CaseInsensitive))
                        {
                            currentPVIData.Current = reply.getDATAMean();
                            isCurrentMeasured = true;
                        }
                    }
                }
                if (arePVIDataReady())
                {
                    emitCurrentPVIDataToLocalDatabase();
                    if (!goToNextGlobalID())
                    {
                        isAutoSignallerEnabled = false;
                    }
                }
                break;
            }
            default:
                break;
            }
        }
        else if (currentGlobalSignalTypeTypeName == QStringLiteral("SerialPortWorkerBasis::Warning"))
        {            
            switch (currentGlobalSignal.Type.toInt()) {
            case SerialPortWorkerBasis::ReadyReadTimedOut:
            {
                anIf(SerialPortDataProcessorBasisDbgEn, anWarn("SerialPortWorkerBasis::ReadyReadTimedOut"));
                if (isUHV2)
                {
                    BinaryProtocol reply = BinaryProtocol::fromQByteArray(currentGlobalSignal.Data.toByteArray());
                    if (reply.GetMessageDirection() == QStringLiteral("To")
                            && reply.GetBPNo() == currentPumpAddress
                            && reply.GetChannel() == currentPumpChannel)
                    {
                        QString replyTopic = reply.GetCommand();
                        if (replyTopic == QStringLiteral("ReadP"))
                        {
                            currentPVIData.Pressure = QStringLiteral("0");
                            isPressureMeasured = true;
                        }
                        else if (replyTopic == QStringLiteral("ReadV"))
                        {
                            currentPVIData.Voltage = QStringLiteral("0");
                            isVoltageMeasured = true;
                        }
                        else if (replyTopic == QStringLiteral("ReadI"))
                        {
                            currentPVIData.Current = QStringLiteral("0");
                            isCurrentMeasured = true;
                        }
                    }
                }
                else
                {
                    WindowProtocol reply = WindowProtocol::fromQByteArray(currentGlobalSignal.Data.toByteArray());
                    if (reply.getWPNo() == currentPumpAddress && reply.getDATA().isEmpty())
                    {
                        QString replyTopic = reply.getWINMean();
                        if (replyTopic.contains(QStringLiteral("PMeasured"),Qt::CaseInsensitive))
                        {
                            currentPVIData.Pressure = QStringLiteral("0");
                            isPressureMeasured = true;
                        }
                        else if (replyTopic.contains(QStringLiteral("VMeasured"),Qt::CaseInsensitive))
                        {
                            currentPVIData.Voltage = QStringLiteral("0");
                            isVoltageMeasured = true;
                        }
                        else if (replyTopic.contains(QStringLiteral("IMeasured"),Qt::CaseInsensitive))
                        {
                            currentPVIData.Current = QStringLiteral("0");
                            isCurrentMeasured = true;
                        }
                    }
                }
                if (arePVIDataReady())
                {
                    emitCurrentPVIDataToLocalDatabase();
                    if (!goToNextGlobalID())
                    {
                        isAutoSignallerEnabled = false;
                    }
                }
                break;
            }
            default:
                break;
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

void SerialPortDataProcessorBasis::errorSerialPortDataProcessorOnEntry()
{
    anIf(SerialPortDataProcessorBasisDbgEn,
         anError("Emit SerialPortDataProcessorBasis::Error");
         anInfo("ErrorType: " + QString(ErrorMetaEnum.valueToKey(static_cast<int>(ErrorType))));
         anInfo("ErrorInfo: " + ErrorInfo);
    );
    GlobalSignal errorGlobalSignal;
    errorGlobalSignal.Type = QVariant::fromValue(ErrorType);
    errorGlobalSignal.Data = QVariant::fromValue(ErrorInfo);
    errorGlobalSignal.Priority = 200;
    errorGlobalSignal.SignalPriority = 200;
    errorGlobalSignal.DstStrs.append(GlobalSignalCoordinatorObjName);
    emit Out(errorGlobalSignal);
}

void SerialPortDataProcessorBasis::In(const GlobalSignal &aGlobalSignal)
{
    pushAGlobalSignalIntoPrioritizedBuffer(aGlobalSignal);
    if (currentStateName == QStringLiteral("idleSerialPortDataProcessor"))
    {
        emit GlobalSignalExecutionRequested();
    }
}

bool SerialPortDataProcessorBasis::openLocalDatabaseConnection()
{
    localDb = QSqlDatabase::cloneDatabase(localQSqlDatabase, parent()->objectName());
    if (localDb.open())
    {
        anIf(SerialPortDataProcessorBasisDbgEn, anAck("OK Local Database Connected !"));
        return true;
    }
    else
    {
        lastQSqlError = localDb.lastError();
        anIf(SerialPortDataProcessorBasisDbgEn,
             anError("Failed To Connect Local Database !");
             anInfo(lastQSqlError.text()));
        setError(QSqlErrorOccured,lastQSqlError.text());
    }
    return false;
}

void SerialPortDataProcessorBasis::closeLocalDatabaseConnection()
{
    QString currentConnection = localDb.connectionName();
    localDb.close();
    localDb = QSqlDatabase();
    localDb.removeDatabase(currentConnection);
    anIf(SerialPortDataProcessorBasisDbgEn && currentConnection.size(), anWarn("Local Database Connection Closed !"));
}

void SerialPortDataProcessorBasis::clearCache()
{
    currentGlobalSignal = GlobalSignal();
}

void SerialPortDataProcessorBasis::clearAutoSignallerCache()
{
    isPressureMeasured = false;
    isVoltageMeasured = false;
    isCurrentMeasured = false;
    QBAReadP.clear();
    QBAReadV.clear();
    QBAReadI.clear();
    currentPVIData = piLocalDBWorkerBasis::PVIData();
    autoGlobalSignalToUHV.Data = QVariant::fromValue(QByteArray());
}

bool SerialPortDataProcessorBasis::goToNextGlobalID()
{
    clearAutoSignallerCache();
    QSqlQuery scopedQuery(localDb);
    if (!execSQL(&scopedQuery,
                 "SELECT GlobalID,pumpAddr,pumpCH FROM stations WHERE GlobalID>"
                    + QString::number(currentGlobalID) + " AND pumpType="
                    + QString(isUHV2?"0":"1") + " ORDER BY GlobalID ASC LIMIT 1"))
    {
        currentGlobalID = 0;
        if (!execSQL(&scopedQuery,
                     "SELECT GlobalID,pumpAddr,pumpCH FROM stations WHERE GlobalID>"
                        + QString::number(currentGlobalID) + " AND pumpType="
                        + QString(isUHV2?"0":"1") + " ORDER BY GlobalID ASC LIMIT 1"))
        {
            return false;
        }
    }
    currentGlobalID = scopedQuery.value("GlobalID").toInt();
    currentPVIData.GlobalID = currentGlobalID;
    currentPumpAddress = scopedQuery.value("pumpAddr").toInt();
    currentPumpChannel = scopedQuery.value("pumpCH").toInt();
    if (isUHV2)
    {
        BinaryProtocol pumpProtocol;
        QBAReadP = pumpProtocol.SetBPNo(currentPumpAddress).ReadP().ChannelNo(currentPumpChannel).GenMsg();
        QBAReadV = pumpProtocol.SetBPNo(currentPumpAddress).ReadV().ChannelNo(currentPumpChannel).GenMsg();
        QBAReadI = pumpProtocol.SetBPNo(currentPumpAddress).ReadI().ChannelNo(currentPumpChannel).GenMsg();
    }
    else
    {
        WindowProtocol pumpProtocol;
        QBAReadP = pumpProtocol.setWPNo(currentPumpAddress).PMeasured().setChNo(currentPumpChannel).Read().clearDATA().genMSG();
        QBAReadV = pumpProtocol.setWPNo(currentPumpAddress).VMeasured().setChNo(currentPumpChannel).Read().clearDATA().genMSG();
        QBAReadI = pumpProtocol.setWPNo(currentPumpAddress).IMeasured().setChNo(currentPumpChannel).Read().clearDATA().genMSG();
    }
    anIf(SerialPortDataProcessorBasisDbgEn, anAck("Move To GlobalID = " << currentGlobalID));
    return true;
}

void SerialPortDataProcessorBasis::emitCurrentPVIDataToLocalDatabase()
{
    anIf(SerialPortDataProcessorBasisDbgEn,
            anAck("Emit currentPVIData To Local Database !");
            anVar(currentPVIData.GlobalID);
            anVar(currentPVIData.Pressure);
            anVar(currentPVIData.Voltage);
            anVar(currentPVIData.Current));
    GlobalSignal updatePVIData;
    updatePVIData.Type = QVariant::fromValue(piLocalDBWorkerBasis::updatePVIData);
    updatePVIData.Data = QVariant::fromValue(currentPVIData);
    updatePVIData.TimeStamp = NOW2String;
    updatePVIData.DstStrs.append(piLocalDBWorkerObjName);
    emit Out(updatePVIData);
}

bool SerialPortDataProcessorBasis::execSQL(QSqlQuery *aQuery, const QString &aSQL, bool navigateFirstRecordForSELECT)
{
    anIf(SerialPortDataProcessorBasisDbgEn, anInfo("SQL : " << aSQL));
    if (aQuery->exec(aSQL))
    {
        anIf(SerialPortDataProcessorBasisDbgEn, anInfo("OK SQL"));
        if (navigateFirstRecordForSELECT && aSQL.trimmed().left(6).contains(QStringLiteral("SELECT"),Qt::CaseInsensitive))
        {
            bool tmpReturn = aQuery->next();
            anIf(SerialPortDataProcessorBasisDbgEn, anInfo("Is Any Info Selected? " << (tmpReturn?"YES":"NO")));
            return tmpReturn;
        }
        return true;
    }
    lastQSqlError = aQuery->lastError();
    anIf(SerialPortDataProcessorBasisDbgEn, anError("ERROR SQL: " << lastQSqlError.text()));
    setError(QSqlErrorOccured,lastQSqlError.text());
    return false;
}


const QMetaEnum SerialPortDataProcessorBasis::DataMetaEnum = QMetaEnum::fromType<SerialPortDataProcessorBasis::Data>();
const QMetaEnum SerialPortDataProcessorBasis::ErrorMetaEnum = QMetaEnum::fromType<SerialPortDataProcessorBasis::Error>();
const QMetaEnum SerialPortDataProcessorBasis::WarningMetaEnum = QMetaEnum::fromType<SerialPortDataProcessorBasis::Warning>();
const QMetaEnum SerialPortDataProcessorBasis::NotificationMetaEnum = QMetaEnum::fromType<SerialPortDataProcessorBasis::Notification>();
