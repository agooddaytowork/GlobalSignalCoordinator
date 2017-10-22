#ifndef PILOCALDBWORKERBASIS_H
#define PILOCALDBWORKERBASIS_H

#define piLocalDBWorkerBasisDbgEn 1

#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUrl>
#include <QtNetwork>
#include <QNetworkReply>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "shared/commonthings.h"
#include "shared/abstractstatemachinebasis.h"
#include "anLogger/src/anlogger.h"

class piLocalDBWorkerBasis : public AbstractStateMachineBasis
{
    Q_OBJECT
public:
    explicit piLocalDBWorkerBasis(QObject *parent = nullptr);
    ~piLocalDBWorkerBasis();

    enum Data
    {
        NoData = 0,
        updatePVIData,
        updateRFIDData,
        syncOfflineData,
        executeSQL,
        clearBuffer,
        ignoreError
    };
    Q_ENUM(Data)

    enum Error
    {
        NoError = 0,
        QSqlErrorOccured
    };
    Q_ENUM(Error)

    enum Warning
    {
        NoWarning = 0
    };
    Q_ENUM(Warning)

    enum Notification
    {
        NoNotification = 0,
        readyToWork
    };
    Q_ENUM(Notification)

    typedef struct {
        quint32 GlobalID;
        QString Pressure;
        QString Voltage;
        QString Current;
    } PVIData;

    typedef struct {
        quint32 GlobalID;
        QString srcRFID;
    } RFIDData;

    static const QMetaEnum DataMetaEnum;
    static const QMetaEnum ErrorMetaEnum;
    static const QMetaEnum WarningMetaEnum;
    static const QMetaEnum NotificationMetaEnum;

    void uninitiatedPiLocalDBWorkerOnEntry();
    void idlePiLocalDBWorkerOnEntry();
    void runningPiLocalDBWorkerOnEntry();
    bool sendJsonPiLocalDBWorkerOnEntry();
    void setIsSentPiLocalDBWorkerOnEntry();
    void errorPiLocalDBWorkerOnEntry();

    void setOnOffLine(bool on1off0);//On = true, Off = false
    void clearError();
signals:
    void sendingPendingJsonDataPackage();
    void jsonPackageTransmitted();
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    QSqlDatabase localDb;
    Error ErrorType = NoError;
    QString ErrorInfo;

    bool isOnline = true;

    //Cache
    QByteArray pendingJsonData;
    QString reservedStr1;
    QString reservedStr2;
    QSqlError lastQSqlError;

    //Temp
    QSqlQuery tmpQuery1;
    QSqlQuery tmpQuery2;
    QString tmpStr;
    QString tmpStr1;
    QString tmpStr2;
    QString tmpStr3;
    QString tmpStr4;
    QString tmpStr5;

    QNetworkAccessManager NAManager;
    QNetworkReply * networkReply = nullptr;

    static const QNetworkRequest onlineDb;
    static const QByteArray constPINGJsonPackage;

    void initiate();
    void dispose();    
    void setError(const Error & anErrorType, const QString & anErrorInfo);
    bool execSQL(QSqlQuery *aQuery,const QString &aSQL,bool navigateFirstRecordForSELECT = true);
    const QJsonArray getPendingJsonDataPackage();
    void sendJsonDataPackage(const QByteArray &data);
    bool openLocalDatabaseConnection();
    void closeLocalDatabaseConnection();
    void clearCache();

    static const QJsonObject createAJsonDataPackage(const QString &GlobalID,
                                                    const QString &HV,
                                                    const QString &Valve,
                                                    const QString &Pressure,
                                                    const QString &Voltage,
                                                    const QString &Current,
                                                    const QString &dstRFID,
                                                    const QString &Time);
};

Q_DECLARE_METATYPE(piLocalDBWorkerBasis::PVIData)
Q_DECLARE_METATYPE(piLocalDBWorkerBasis::RFIDData)

#endif // PILOCALDBWORKERBASIS_H
