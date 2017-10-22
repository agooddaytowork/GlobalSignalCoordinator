#ifndef UNINITIATEDCANDATAPROCESSOR_H
#define UNINITIATEDCANDATAPROCESSOR_H

#include "candataprocessorbasis.h"

class uninitiatedCanDataProcessor : public QState
{
public:
    uninitiatedCanDataProcessor(CanDataProcessorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanDataProcessorBasis *basisptr = nullptr;
};

#endif // UNINITIATEDCANDATAPROCESSOR_H
