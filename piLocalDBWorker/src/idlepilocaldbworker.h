#ifndef IDLEPILOCALDBWORKER_H
#define IDLEPILOCALDBWORKER_H

#include "pilocaldbworkerbasis.h"

class idlePiLocalDBWorker : public QState
{
public:
    idlePiLocalDBWorker(piLocalDBWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    piLocalDBWorkerBasis *basisptr = nullptr;
};

#endif // IDLEPILOCALDBWORKER_H
