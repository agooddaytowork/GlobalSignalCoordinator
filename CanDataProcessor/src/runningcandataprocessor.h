#ifndef RUNNINGCANDATAPROCESSOR_H
#define RUNNINGCANDATAPROCESSOR_H

#include "candataprocessorbasis.h"

class runningCanDataProcessor : public QState
{
public:
    runningCanDataProcessor(CanDataProcessorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanDataProcessorBasis *basisptr = nullptr;
};

#endif // RUNNINGCANDATAPROCESSOR_H
