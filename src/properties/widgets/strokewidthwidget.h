#ifndef STROKEWIDTHWIDGET_H
#define STROKEWIDTHWIDGET_H

#include "propertywidget.h"

class StrokeWidthWidget : public PropertyWidget {
public:
    StrokeWidthWidget(QWidget *parent = nullptr);

    QString name() const override;
    const Property value() const override;
};

#endif  // STROKEWIDTHWIDGET_H
