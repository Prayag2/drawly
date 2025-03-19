#ifndef TOOLERASERSIZE_H
#define TOOLERASERSIZE_H

#include "toolproperty.h"

class ToolEraserSize : public ToolProperty
{
public:
    ToolEraserSize(QWidget *parent = nullptr);
    QString name() const override;
    const QVariant value() const override;
};

#endif // TOOLERASERSIZE_H
