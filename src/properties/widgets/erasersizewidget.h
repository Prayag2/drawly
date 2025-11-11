#ifndef ERASERSIZEWIDGET_H
#define ERASERSIZEWIDGET_H

#include "propertywidget.h"

class EraserSizeWidget : public PropertyWidget {
public:
    EraserSizeWidget(QWidget *parent = nullptr);

    QString name() const override;
    const Property value() const override;
};

#endif  // TOOLERASERSIZE_H
