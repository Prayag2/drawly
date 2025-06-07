#include "toolstrokewidth.h"

#include <QSpinBox>

ToolStrokeWidth::ToolStrokeWidth(QWidget* parent) {
    // TODO: Remove magic numbers
    QSpinBox* box{new QSpinBox(parent)};
    box->setRange(1, 10);
    box->setValue(6);
    m_widget = box;
}

QString ToolStrokeWidth::name() const {
    return "Thickness";
};

const QVariant ToolStrokeWidth::value() const {
    return dynamic_cast<QSpinBox*>(m_widget)->value();
}
