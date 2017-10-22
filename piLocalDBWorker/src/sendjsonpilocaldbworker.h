#ifndef SENDJSONPILOCALDBWORKER_H
#define SENDJSONPILOCALDBWORKER_H

#include "pilocaldbworkerbasis.h"
#include <QTimer>

class sendJsonPiLocalDBWorker : public QState
{
public:
    sendJsonPiLocalDBWorker(piLocalDBWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    piLocalDBWorkerBasis *basisptr = nullptr;
    QTimer timer;
};

#endif // SENDJSONPILOCALDBWORKER_H
