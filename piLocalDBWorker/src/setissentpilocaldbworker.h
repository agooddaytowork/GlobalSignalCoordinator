#ifndef SETISSENTPILOCALDBWORKER_H
#define SETISSENTPILOCALDBWORKER_H

#include "pilocaldbworkerbasis.h"

class setIsSentPiLocalDBWorker : public QState
{
public:
    setIsSentPiLocalDBWorker(piLocalDBWorkerBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    piLocalDBWorkerBasis *basisptr = nullptr;
};

#endif // SETISSENTPILOCALDBWORKER_H
