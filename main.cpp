#include <QCoreApplication>
#include <QThread>
#include "piLocalDBWorker/src/pilocaldbworker.h"
#include "SerialPortWorker/src/serialportworker.h"
#include "SerialPortDataProcessor/src/serialportdataprocessor.h"
#include "CanBusWorker/src/canbusworker.h"
#include "CanDataProcessor/src/candataprocessor.h"
#include "src/globalsignalcoordinator.h"

int main(int argc, char *argv[])
{
    registerGlobalSignal;
    connectLocalQSqlDatabase;

    QCoreApplication a(argc, argv);

    SerialPortWorker * uhv2worker = new SerialPortWorker();
    uhv2worker->setObjectName(UHV2SerialPortWorkerObjName);
    SerialPortWorker * uhv4worker = new SerialPortWorker();
    uhv4worker->setObjectName(UHV4SerialPortWorkerObjName);
    SerialPortDataProcessor * uhv2dataprocessor = new SerialPortDataProcessor(true);
    uhv2dataprocessor->setObjectName(UHV2SerialPortDataProcessorObjName);
    SerialPortDataProcessor * uhv4dataprocessor = new SerialPortDataProcessor(false);
    uhv4dataprocessor->setObjectName(UHV4SerialPortDataProcessorObjName);
    piLocalDBWorker * piLocalDatabase = new piLocalDBWorker();
    piLocalDatabase->setObjectName(piLocalDBWorkerObjName);
    CanBusWorker * canbusworker = new CanBusWorker();
    canbusworker->setObjectName(CanBusWorkerObjName);
    CanDataProcessor * candataprocessor = new CanDataProcessor();
    candataprocessor->setObjectName(CanDataProcessorObjName);
    GlobalSignalCoordinator * coordinator = new GlobalSignalCoordinator();
    coordinator->setObjectName(GlobalSignalCoordinatorObjName);

    QObject::connect(uhv2worker, &SerialPortWorker::Out, coordinator, &GlobalSignalCoordinator::In);
    QObject::connect(uhv4worker, &SerialPortWorker::Out, coordinator, &GlobalSignalCoordinator::In);
    QObject::connect(uhv2dataprocessor, &SerialPortDataProcessor::Out, coordinator, &GlobalSignalCoordinator::In);
    QObject::connect(uhv4dataprocessor, &SerialPortDataProcessor::Out, coordinator, &GlobalSignalCoordinator::In);
    QObject::connect(piLocalDatabase, &piLocalDBWorker::Out, coordinator, &GlobalSignalCoordinator::In);
    QObject::connect(canbusworker, &CanBusWorker::Out, coordinator, &GlobalSignalCoordinator::In);
    QObject::connect(candataprocessor, &CanDataProcessor::Out, coordinator, &GlobalSignalCoordinator::In);

    QObject::connect(coordinator, &GlobalSignalCoordinator::ToPiLocalDBWorker, piLocalDatabase, &piLocalDBWorker::In);
    QObject::connect(coordinator, &GlobalSignalCoordinator::ToUHV2SerialPortWorker, uhv2worker, &SerialPortWorker::In);
    QObject::connect(coordinator, &GlobalSignalCoordinator::ToUHV4SerialPortWorker, uhv4worker, &SerialPortWorker::In);
    QObject::connect(coordinator, &GlobalSignalCoordinator::ToUHV2SerialPortDataProcessor, uhv2dataprocessor, &SerialPortDataProcessor::In);
    QObject::connect(coordinator, &GlobalSignalCoordinator::ToUHV4SerialPortDataProcessor, uhv4dataprocessor, &SerialPortDataProcessor::In);
    QObject::connect(coordinator, &GlobalSignalCoordinator::ToCanBusWorker, canbusworker, &CanBusWorker::In);
    QObject::connect(coordinator, &GlobalSignalCoordinator::ToCanDataProcessor, candataprocessor, &CanDataProcessor::In);

    QThread * uhv2workerThread = new QThread();
    QThread * uhv4workerThread = new QThread();
    QThread * uhv2dataprocessorThread = new QThread();
    QThread * uhv4dataprocessorThread = new QThread();
    QThread * piLocalDatabaseThread = new QThread();
    QThread * canbusworkerThread = new QThread();
    QThread * candataprocessorThread = new QThread();
    QThread * coordinatorThread = new QThread();

    uhv2worker->moveToThread(uhv2workerThread);
    uhv4worker->moveToThread(uhv4workerThread);
    uhv2dataprocessor->moveToThread(uhv2dataprocessorThread);
    uhv4dataprocessor->moveToThread(uhv4dataprocessorThread);
    piLocalDatabase->moveToThread(piLocalDatabaseThread);
    canbusworker->moveToThread(canbusworkerThread);
    candataprocessor->moveToThread(candataprocessorThread);
    coordinator->moveToThread(coordinatorThread);

    QObject::connect(uhv2workerThread, &QThread::started, uhv2worker, &SerialPortWorker::start);
    QObject::connect(uhv4workerThread, &QThread::started, uhv4worker, &SerialPortWorker::start);
    QObject::connect(uhv2dataprocessorThread, &QThread::started, uhv2dataprocessor, &SerialPortDataProcessor::start);
    QObject::connect(uhv4dataprocessorThread, &QThread::started, uhv4dataprocessor, &SerialPortDataProcessor::start);
    QObject::connect(piLocalDatabaseThread, &QThread::started, piLocalDatabase, &piLocalDBWorker::start);
    QObject::connect(canbusworkerThread, &QThread::started, canbusworker, &CanBusWorker::start);
    QObject::connect(candataprocessorThread, &QThread::started, candataprocessor, &CanDataProcessor::start);
    QObject::connect(coordinatorThread, &QThread::started, coordinator, &GlobalSignalCoordinator::start);

    QObject::connect(coordinator, &GlobalSignalCoordinator::startWorkers, [&](){
        piLocalDatabaseThread->start();
        uhv2workerThread->start();
        uhv4workerThread->start();
        uhv2dataprocessorThread->start();
        uhv4dataprocessorThread->start();
        canbusworkerThread->start();
        candataprocessorThread->start();
    });

    coordinatorThread->start();

    return a.exec();
}
