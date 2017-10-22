#ifndef IDLECANDATAPROCESSOR_H
#define IDLECANDATAPROCESSOR_H

#include "candataprocessorbasis.h"
#include <QTimer>

class idleCanDataProcessor : public QState
{
public:
    idleCanDataProcessor(CanDataProcessorBasis *parentBasis, QState *parent = nullptr);
protected:
    void onEntry(QEvent *) override;
    void onExit(QEvent *) override;
private:
    CanDataProcessorBasis *basisptr = nullptr;
    QTimer autoSignaller;
};

#endif // IDLECANDATAPROCESSOR_H
