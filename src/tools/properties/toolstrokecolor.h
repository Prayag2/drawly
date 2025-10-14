#ifndef TOOLSTROKECOLOR_H
#define TOOLSTROKECOLOR_H

#include "toolproperty.h"
class QButtonGroup;

class ToolStrokeColor : public ToolProperty {
public:
    ToolStrokeColor(QWidget *parent = nullptr);
    QString name() const override;
    const QVariant value() const override;

private:
    QButtonGroup *m_group{};
};

#endif  // TOOLSTROKECOLOR_H
