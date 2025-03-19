#ifndef TOOLSTROKEWIDTH_H
#define TOOLSTROKEWIDTH_H

#include "toolproperty.h"

class ToolStrokeWidth : public ToolProperty
{
public:
    ToolStrokeWidth(QWidget* parent = nullptr);
    QString name() const override;
    const QVariant value() const override;
};


#endif // TOOLSTROKEWIDTH_H
