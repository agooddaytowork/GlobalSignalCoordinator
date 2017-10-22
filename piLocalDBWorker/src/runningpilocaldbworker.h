#ifndef RUNNINGPILOCALDBWORKER_H
#define RUNNINGPILOCALDBWORKER_H

#include "pilocaldbworkerbasis.h"

class runningPiLocalDBWorker : public QState
{
public:
    runningPiLocalDBWorker(piLocalDBWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    piLocalDBWorkerBasis *basisptr = nullptr;
};

#endif // RUNNINGPILOCALDBWORKER_H
