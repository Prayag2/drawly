#include "propertywidget.h"

PropertyWidget::PropertyWidget(QWidget *parent) : QObject{parent} {}

QWidget *PropertyWidget::widget() const {
    return m_widget;
}
