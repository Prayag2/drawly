#ifndef STROKECOLORWIDGET_H
#define STROKECOLORWIDGET_H

#include "propertywidget.h"
class QButtonGroup;

class StrokeColorWidget : public PropertyWidget {
public:
    StrokeColorWidget(QWidget *parent = nullptr);

    QString name() const override;
    const Property value() const override;

private:
    QButtonGroup *m_group{};
};

#endif  // STROKECOLORWIDGET_H
