#include "toolfontsize.h"

#include <QSpinBox>

ToolFontSize::ToolFontSize(QWidget *parent) {
    // TODO: Remove magic numbers
    QSpinBox *box{new QSpinBox(parent)};
    box->setMinimum(1);
    box->setValue(24);

    box->hide();
    m_widget = box;
}

QString ToolFontSize::name() const {
    return "Font Size";
};

const QVariant ToolFontSize::value() const {
    return dynamic_cast<QSpinBox *>(m_widget)->value();
}
