#ifndef ERRORCANDATAPROCESSOR_H
#define ERRORCANDATAPROCESSOR_H

#include "candataprocessorbasis.h"

class errorCanDataProcessor : public QState
{
public:
    errorCanDataProcessor(CanDataProcessorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanDataProcessorBasis *basisptr = nullptr;
};

#endif // ERRORCANDATAPROCESSOR_H
