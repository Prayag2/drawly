#include "toolerasersize.h"

#include <QSpinBox>

ToolEraserSize::ToolEraserSize(QWidget* parent) {
    QSpinBox* box{new QSpinBox(parent)};
    box->setRange(1, 100);
    box->setValue(30);

    box->hide();
    m_widget = box;
}

QString ToolEraserSize::name() const {
    return "Eraser Size";
};

const QVariant ToolEraserSize::value() const {
    return dynamic_cast<QSpinBox*>(m_widget)->value();
}
