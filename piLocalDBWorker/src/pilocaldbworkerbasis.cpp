#include "pilocaldbworkerbasis.h"

piLocalDBWorkerBasis::piLocalDBWorkerBasis(QObject *parent) : AbstractStateMachineBasis(parent)
{
    registerGlobalSignal;
    qRegisterMetaType<piLocalDBWorkerBasis::PVIData>("piLocalDBWorkerBasis::PVIData");
    qRegisterMetaType<piLocalDBWorkerBasis::RFIDData>("piLocalDBWorkerBasis::RFIDData");
    NAManager.setParent(this);
    anIf(piLocalDBWorkerBasisDbgEn, anAck("piLocalDBWorkerBasis Constructed"));
}

piLocalDBWorkerBasis::~piLocalDBWorkerBasis()
{    
    anIf(piLocalDBWorkerBasisDbgEn, anWarn("Destroy piLocalDBWorkerBasis"));
    dispose();
}

void piLocalDBWorkerBasis::uninitiatedPiLocalDBWorkerOnEntry()
{
    if (!isInitiated)
    {
        initiate();
    }
}

void piLocalDBWorkerBasis::idlePiLocalDBWorkerOnEntry()
{
    if (previousStateName == QStringLiteral("uninitiatedPiLocalDBWorker"))
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

void piLocalDBWorkerBasis::runningPiLocalDBWorkerOnEntry()
{
    clearCache();
    if (prioritizedBuffer.size())
    {
        currentGlobalSignal = popMostPrioritizedGlobalSignalOutOfPrioritizedBuffer();
        QString currentGlobalSignalTypeTypeName = currentGlobalSignal.Type.typeName();
        if (currentGlobalSignalTypeTypeName == QStringLiteral("piLocalDBWorkerBasis::Data"))
        {
            switch (currentGlobalSignal.Type.toInt()) {
            case updatePVIData:
            {
                anIf(piLocalDBWorkerBasisDbgEn, anAck("updatePVIData"));
                PVIData currentPVIData = currentGlobalSignal.Data.value<PVIData>();
                tmpStr1 = QString::number(currentPVIData.GlobalID);
                tmpStr2 = currentPVIData.Pressure;
                tmpStr3 = currentPVIData.Voltage;
                tmpStr4 = currentPVIData.Current;
                tmpStr5 = NOW2String;
                //tmpStr5 = currentGlobalSignal.TimeStamp;
                if (execSQL(&tmpQuery1,
                               "SELECT setHVOn,setRoughValveOn,RFID "
                               "FROM stations "
                               "WHERE GlobalID=" + tmpStr1 + " LIMIT 1"))
                {
                    tmpStr = tmpQuery1.value("RFID").toString();
                    if (execSQL(&tmpQuery2,
                                   "INSERT INTO " + tmpStr.toLower() + " (Time,Pressure,Voltage,Current) "
                                   "VALUES ('" + tmpStr5 + "'," + tmpStr2 + "," + tmpStr3 + "," + tmpStr4 + ")"))
                    {
                        if (isOnline)
                        {
                            pendingJsonData = QJsonDocument(createAJsonDataPackage(tmpStr1,
                                                                                   tmpQuery1.value("setHVOn").toString(),
                                                                                   tmpQuery1.value("setRoughValveOn").toString(),
                                                                                   tmpStr2,
                                                                                   tmpStr3,
                                                                                   tmpStr4,
                                                                                   tmpStr,
                                                                                   tmpStr5)).toJson(QJsonDocument::Compact);
                            reservedStr1 = tmpStr.toLower();
                            reservedStr2 = tmpStr5;
                        }
                        else
                        {
                            pendingJsonData = constPINGJsonPackage;
                        }
                        emit sendingPendingJsonDataPackage();
                        return;
                    }
                    else
                    {
                        anIf(piLocalDBWorkerBasisDbgEn, anWarn("Not Found Table " + tmpStr.toLower()));
                    }
                }
                break;
            }
            case updateRFIDData:
            {
                anIf(piLocalDBWorkerBasisDbgEn, anAck("updateRFIDData"));
                RFIDData currentRFIDData = currentGlobalSignal.Data.value<RFIDData>();
                tmpStr = QString::number(currentRFIDData.GlobalID);
                tmpStr1 = QStringLiteral("R") + currentRFIDData.srcRFID;
                if (execSQL(&tmpQuery1,
                               "SELECT setHVOn,setRoughValveOn,RFID "
                               "FROM stations "
                               "WHERE GlobalID=" + tmpStr))
                {
                    if (!(tmpQuery1.value("RFID").toString().contains(tmpStr1,Qt::CaseInsensitive)))
                    {
                        anIf(piLocalDBWorkerBasisDbgEn, anInfo("RFID Changed At GlobalID=" + tmpStr));
                        if (execSQL(&tmpQuery2,
                                       "UPDATE stations "
                                       "SET RFID='" + tmpStr1 + "',time=NOW() "
                                       "WHERE GlobalID=" + tmpStr))
                        {
                            if (!execSQL(&tmpQuery2,
                                            "SELECT * FROM rfids "
                                            "WHERE RFID='" + tmpStr1 + "'"))
                            {
                                anIf(piLocalDBWorkerBasisDbgEn, anInfo(tmpStr1 + " Does Not Exist In rfids"));
                                if (execSQL(&tmpQuery2,
                                               "INSERT INTO rfids (RFID) VALUES ('" + tmpStr1 + "')"))
                                {
                                    anIf(piLocalDBWorkerBasisDbgEn, anInfo(tmpStr1 + " Inserted Into rfids"));
                                    execSQL(&tmpQuery2,
                                               "CREATE TABLE IF NOT EXISTS " + tmpStr1.toLower() + " ("
                                               "Time DATETIME NOT NULL,"
                                               "Pressure DOUBLE NULL,"
                                               "Voltage DOUBLE NULL,"
                                               "Current DOUBLE NULL,"
                                               "`isSent` tinyint(1) NOT NULL DEFAULT '0',"
                                               "PRIMARY KEY (Time),"
                                               "UNIQUE INDEX Time_UNIQUE (Time ASC)) ENGINE = InnoDB");
                                    if (isOnline)
                                    {
                                        if (execSQL(&tmpQuery2,
                                                       "SELECT * FROM " + tmpStr1.toLower() + " WHERE isSent=0 ORDER BY Time DESC LIMIT 1"))
                                        {
                                            tmpStr2 = tmpQuery2.value("Pressure").toString();
                                            tmpStr3 = tmpQuery2.value("Voltage").toString();
                                            tmpStr4 = tmpQuery2.value("Current").toString();
                                            tmpStr5 = tmpQuery2.value("Time").toString();
                                        }
                                        else
                                        {
                                            tmpStr2.clear();
                                            tmpStr3.clear();
                                            tmpStr4.clear();
                                            tmpStr5.clear();
                                        }
                                        pendingJsonData = QJsonDocument(createAJsonDataPackage(tmpStr,
                                                                                               tmpQuery1.value("setHVOn").toString(),
                                                                                               tmpQuery1.value("setRoughValveOn").toString(),
                                                                                               tmpStr2,
                                                                                               tmpStr3,
                                                                                               tmpStr4,
                                                                                               tmpStr1,
                                                                                               tmpStr5)).toJson(QJsonDocument::Compact);
                                        reservedStr1 = tmpStr1.toLower();
                                        reservedStr2 = tmpStr5;
                                    }
                                    else
                                    {
                                        pendingJsonData = constPINGJsonPackage;
                                    }
                                    emit sendingPendingJsonDataPackage();
                                    return;
                                }
                            }
                        }

                    }
                }
                break;
            }
            case syncOfflineData:
            {
                anIf(piLocalDBWorkerBasisDbgEn, anAck("syncOfflineData"));
                if (isOnline)
                    pendingJsonData = QJsonDocument(getPendingJsonDataPackage()).toJson(QJsonDocument::Compact);
                else
                {
                    anIf(piLocalDBWorkerBasisDbgEn, anWarn("Offline Again !"));
                    pendingJsonData = constPINGJsonPackage;
                }
                emit sendingPendingJsonDataPackage();
                return;
            }
            case clearBuffer:
            {
                anIf(piLocalDBWorkerBasisDbgEn, anWarn("clearBuffer"));
                clearPrioritizedBuffer();
                break;
            }
            case executeSQL:
            {
                anIf(piLocalDBWorkerBasisDbgEn, anAck("executeSQL"));
                execSQL(&tmpQuery1,currentGlobalSignal.Data.toString());
                break;
            }
            default:
                break;
            }
        }
        else if (currentGlobalSignalTypeTypeName == QStringLiteral("piLocalDBWorkerBasis::Notification"))
        {
            switch (currentGlobalSignal.Type.toInt()) {
            case readyToWork:
            {
                anIf(piLocalDBWorkerBasisDbgEn, anAck("readyToWork"));
                emit Out(GlobalSignal(currentGlobalSignal));
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

bool piLocalDBWorkerBasis::sendJsonPiLocalDBWorkerOnEntry()
{
    if (pendingJsonData.isEmpty())
    {
       return false;
    }
    sendJsonDataPackage(pendingJsonData);
    return true;
}

void piLocalDBWorkerBasis::setIsSentPiLocalDBWorkerOnEntry()
{
    if (isOnline && (pendingJsonData != constPINGJsonPackage))
    {
        switch (currentGlobalSignal.Type.toInt()) {
        case updatePVIData:
        {
            if (reservedStr1.size() && reservedStr2.size())
                execSQL(&tmpQuery1,"UPDATE " + reservedStr1 + " SET isSent=1 WHERE Time='" + reservedStr2 + "'");
            break;
        }
        case updateRFIDData:
        {
            if (reservedStr1.size() && reservedStr2.size())
                execSQL(&tmpQuery1,"UPDATE " + reservedStr1 + " SET isSent=1 WHERE Time='" + reservedStr2 + "'");
            break;
        }
        case syncOfflineData:
        {
            if (execSQL(&tmpQuery1,QStringLiteral("SELECT RFID FROM stations"),false))
            {
                while (tmpQuery1.next())
                {
                    execSQL(&tmpQuery2,"UPDATE " + tmpQuery1.value("RFID").toString().toLower() + " SET isSent=1");
                }
            }
            anIf(piLocalDBWorkerBasisDbgEn, anAck("Offline Data Synchronized !"));
            break;
        }
        default:
            break;
        }
    }
    emit GlobalSignalExecutionRequested();
}

void piLocalDBWorkerBasis::errorPiLocalDBWorkerOnEntry()
{
    anIf(piLocalDBWorkerBasisDbgEn,
         anError("Emit piLocalDBWorkerBasis::Error");
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

void piLocalDBWorkerBasis::initiate()
{
    dispose();
    if (openLocalDatabaseConnection())
    {
        isInitiated = true;
        emit goIdle();
    }
    anIf(piLocalDBWorkerBasisDbgEn && isInitiated, anAck("piLocalDBWorkerBasis Initialized"));
}

void piLocalDBWorkerBasis::dispose()
{
    anIf(piLocalDBWorkerBasisDbgEn && isInitiated, anWarn("Clean piLocalDBWorkerBasis"));
    closeLocalDatabaseConnection();
    clearPrioritizedBuffer();
    previousStateName.clear();
    currentStateName.clear();
    clearCache();
    tmpQuery1.clear();
    tmpQuery2.clear();
    tmpStr.clear();
    tmpStr1.clear();
    tmpStr2.clear();
    tmpStr3.clear();
    tmpStr4.clear();
    tmpStr5.clear();
    isInitiated = false;
}

void piLocalDBWorkerBasis::setOnOffLine(bool on1off0)
{
    if (isOnline>on1off0)
    {
        anIf(piLocalDBWorkerBasisDbgEn, anWarn("Go Offline !"))
    }
    else if (isOnline<on1off0)
    {
        anIf(piLocalDBWorkerBasisDbgEn, anAck("Go Online !"))
        GlobalSignal requestSyncOfflineData;
        requestSyncOfflineData.Type = QVariant::fromValue(syncOfflineData);
        requestSyncOfflineData.Priority = 100;
        pushAGlobalSignalIntoPrioritizedBuffer(requestSyncOfflineData);
        if (currentStateName == QStringLiteral("idlePiLocalDBWorker"))
        {
            emit GlobalSignalExecutionRequested();
        }
        anIf(piLocalDBWorkerBasisDbgEn, anAck("Offline Data Sync Requested !"))
    }
    isOnline = on1off0;
}

void piLocalDBWorkerBasis::setError(const piLocalDBWorkerBasis::Error &anErrorType, const QString &anErrorInfo)
{
    if (anErrorType!=NoError)
    {
        anIf(piLocalDBWorkerBasisDbgEn, anError("Error Occurred !"));
        ErrorType = anErrorType;
        ErrorInfo = anErrorInfo;
        emit ErrorOccurred();
    }
}

void piLocalDBWorkerBasis::clearError()
{
    anIf(piLocalDBWorkerBasisDbgEn && (ErrorType!=NoError), anInfo("Clear Error !"));
    ErrorType = NoError;
    ErrorInfo.clear();
    lastQSqlError = QSqlError();
}

void piLocalDBWorkerBasis::clearCache()
{
    currentGlobalSignal = GlobalSignal();
    pendingJsonData.clear();
    reservedStr1.clear();
    reservedStr2.clear();
}

bool piLocalDBWorkerBasis::execSQL(QSqlQuery *aQuery, const QString &aSQL, bool navigateFirstRecordForSELECT)
{
    anIf(piLocalDBWorkerBasisDbgEn, anInfo("SQL : " << aSQL));
    if (aQuery->exec(aSQL))
    {
        anIf(piLocalDBWorkerBasisDbgEn, anInfo("OK SQL"));
        if (navigateFirstRecordForSELECT && aSQL.trimmed().left(6).contains(QStringLiteral("SELECT"),Qt::CaseInsensitive))
        {
            bool tmpReturn = aQuery->next();
            anIf(piLocalDBWorkerBasisDbgEn, anInfo("Is Any Info Selected? " << (tmpReturn?"YES":"NO")));
            return tmpReturn;
        }
        return true;
    }
    lastQSqlError = aQuery->lastError();
    anIf(piLocalDBWorkerBasisDbgEn, anError("ERROR SQL: " << lastQSqlError.text()));
    setError(QSqlErrorOccured,lastQSqlError.text());
    return false;
}

const QJsonObject piLocalDBWorkerBasis::createAJsonDataPackage(const QString &GlobalID, const QString &HV, const QString &Valve, const QString &Pressure, const QString &Voltage, const QString &Current, const QString &dstRFID, const QString &Time)
{
    QJsonObject value
    {
        {"hv",HV,},
        {"valve",Valve,},
        {"pressure",Pressure,},
        {"current",Current,},
        {"voltage",Voltage,},
        {"rfid",dstRFID,},
        {"time",Time,}
    };
    QJsonObject pump
    {
        {GlobalID,value},
    };
    return pump;
}

const QJsonArray piLocalDBWorkerBasis::getPendingJsonDataPackage()
{
    QSqlQuery tmpScopedSourceQuerry(localDb);
    QSqlQuery tmpScopedQuerry(localDb);
    QJsonArray tmpReturn;
    QString tmpScopedString;
    if (execSQL(&tmpScopedSourceQuerry,QStringLiteral(
                   "SELECT RFID, setRoughValveOn, setHVOn, GlobalID FROM stations"),
                   false))
    {
        while (tmpScopedSourceQuerry.next())
        {
            tmpScopedString = tmpScopedSourceQuerry.value("RFID").toString();
            if (execSQL(&tmpScopedQuerry,
                           "SELECT * FROM " + tmpScopedString.toLower() + "  WHERE isSent<>1 ORDER BY Time ASC",
                           false))
            {
                while (tmpScopedQuerry.next())
                {
                    tmpReturn << createAJsonDataPackage(tmpScopedSourceQuerry.value("GlobalID").toString(),
                                                  tmpScopedSourceQuerry.value("setHVOn").toString(),
                                                  tmpScopedSourceQuerry.value("setRoughValveOn").toString(),
                                                  tmpScopedQuerry.value("Pressure").toString(),
                                                  tmpScopedQuerry.value("Current").toString(),
                                                  tmpScopedQuerry.value("Voltage").toString(),
                                                  tmpScopedString,
                                                  tmpScopedQuerry.value("Time").toString());
                }
            }
        }
    }
    return tmpReturn;
}

void piLocalDBWorkerBasis::sendJsonDataPackage(const QByteArray &data)
{
    if (networkReply)
        delete networkReply;
    networkReply = NAManager.post(onlineDb, data);
    QObject::connect(networkReply, &QNetworkReply::finished, this, [&](){
        setOnOffLine(true);
        anIf(piLocalDBWorkerBasisDbgEn, anAck("Json Package Transmitted !"));
        emit jsonPackageTransmitted();
    });
    anIf(piLocalDBWorkerBasisDbgEn,anAck("Try Sending Json Package ...");anInfo(data););
}

void piLocalDBWorkerBasis::In(const GlobalSignal &aGlobalSignal)
{
    if (currentStateName == QStringLiteral("errorPiLocalDBWorker")
            && aGlobalSignal.Type.typeName() == QStringLiteral("piLocalDBWorkerBasis::Data")
            && aGlobalSignal.Type.toInt() == ignoreError)
    {
        anIf(piLocalDBWorkerBasisDbgEn, anWarn("ignoreError"));
        emit GlobalSignalExecutionRequested();
    }
    else
    {
        pushAGlobalSignalIntoPrioritizedBuffer(aGlobalSignal);
        if (currentStateName == QStringLiteral("idlePiLocalDBWorker"))
        {
            emit GlobalSignalExecutionRequested();
        }
    }
}

bool piLocalDBWorkerBasis::openLocalDatabaseConnection()
{
    localDb = QSqlDatabase::cloneDatabase(localQSqlDatabase, parent()->objectName());
    if (localDb.open())
    {
        anIf(piLocalDBWorkerBasisDbgEn, anAck("OK Local Database Connected !"));
        tmpQuery1 = QSqlQuery(localDb);
        tmpQuery2 = QSqlQuery(localDb);
        return true;
    }
    else
    {
        lastQSqlError = localDb.lastError();
        anIf(piLocalDBWorkerBasisDbgEn,
             anError("Failed To Connect Local Database !");
             anInfo(lastQSqlError.text()));
        setError(QSqlErrorOccured,lastQSqlError.text());
    }
    return false;
}

void piLocalDBWorkerBasis::closeLocalDatabaseConnection()
{
    QString currentConnection = localDb.connectionName();
    localDb.close();
    localDb = QSqlDatabase();
    localDb.removeDatabase(currentConnection);
    anIf(piLocalDBWorkerBasisDbgEn && currentConnection.size(), anAck("Local Database Connection Closed !"));
}

const QMetaEnum piLocalDBWorkerBasis::DataMetaEnum = QMetaEnum::fromType<piLocalDBWorkerBasis::Data>();
const QMetaEnum piLocalDBWorkerBasis::ErrorMetaEnum = QMetaEnum::fromType<piLocalDBWorkerBasis::Error>();
const QMetaEnum piLocalDBWorkerBasis::WarningMetaEnum = QMetaEnum::fromType<piLocalDBWorkerBasis::Warning>();
const QMetaEnum piLocalDBWorkerBasis::NotificationMetaEnum = QMetaEnum::fromType<piLocalDBWorkerBasis::Notification>();

const QNetworkRequest piLocalDBWorkerBasis::onlineDb = [](){
    QNetworkRequest tmp(QUrl("http://essdepots.com:82/KTproj/update.php"));
    tmp.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    return tmp;
}();

const QByteArray piLocalDBWorkerBasis::constPINGJsonPackage = QByteArray("PING");
