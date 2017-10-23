#ifndef GLOBALSIGNALCOORDINATOR_H
#define GLOBALSIGNALCOORDINATOR_H

#define GlobalSignalCoordinatorDbgEn 1

#include "globalsignalcoordinatorbasis.h"
#include "shared/directtransition.h"
#include "errorglobalsignalcoordinator.h"
#include "idleglobalsignalcoordinator.h"
#include "runningglobalsignalcoordinator.h"
#include "uninitiatedglobalsignalcoordinator.h"

class GlobalSignalCoordinator : public QStateMachine
{
    Q_OBJECT
public:
    explicit GlobalSignalCoordinator(QObject *parent = nullptr);

signals:
    void ToPiLocalDBWorker(const GlobalSignal &);
    void ToUHV2SerialPortWorker(const GlobalSignal &);
    void ToUHV4SerialPortWorker(const GlobalSignal &);
    void ToUHV2SerialPortDataProcessor(const GlobalSignal &);
    void ToUHV4SerialPortDataProcessor(const GlobalSignal &);
    void ToCanBusWorker(const GlobalSignal &);
    void ToCanDataProcessor(const GlobalSignal &);
    void Out(const GlobalSignal &);
    void startWorkers();
public slots:
    void In(const GlobalSignal &aGlobalSignal);
private:
    GlobalSignalCoordinatorBasis * currentBasis = nullptr;
};

#endif // GLOBALSIGNALCOORDINATOR_H
