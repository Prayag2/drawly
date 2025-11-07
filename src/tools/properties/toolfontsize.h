#ifndef TOOLFONTSIZE_H
#define TOOLFONTSIZE_H

#include "toolproperty.h"

class ToolFontSize : public ToolProperty {
public:
    ToolFontSize(QWidget *parent = nullptr);
    QString name() const override;
    const QVariant value() const override;
};

#endif  // TOOLFONTSIZE_H
