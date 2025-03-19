#include "toolstrokewidth.h"
#include <QSpinBox>
#include "../../item/properties/itemproperty.h"

ToolStrokeWidth::ToolStrokeWidth(QWidget* parent) {
    QSpinBox* box {new QSpinBox(parent)};
    box->setRange(1, 10);
    m_widget = box;
}

QString ToolStrokeWidth::name() const {
    return "Thickness";
};

const QVariant ToolStrokeWidth::value() const {
    return dynamic_cast<QSpinBox*>(m_widget)->value();
}
