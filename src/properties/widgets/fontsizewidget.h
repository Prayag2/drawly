#ifndef FONTSIZEWIDGET_H
#define FONTSIZEWIDGET_H

#include "propertywidget.h"

class FontSizeWidget : public PropertyWidget {
public:
    FontSizeWidget(QWidget *parent = nullptr);

    QString name() const override;
    const Property value() const override;
};

#endif  // FONTSIZEWIDGET_H
