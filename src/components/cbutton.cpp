#include "cbutton.h"

CButton::CButton(QWidget* parent) : QPushButton(parent) {
    this->setProperties();
}

CButton::CButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent) {
    this->setProperties();
}

CButton::CButton(const QIcon& icon, const QString& text, QWidget* parent)
    : QPushButton(icon, text, parent) {
    this->setProperties();
}

void CButton::setProperties() {
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}
