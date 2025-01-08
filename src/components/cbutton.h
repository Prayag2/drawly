#ifndef CBUTTON_H
#define CBUTTON_H

#include <QPushButton>

class CButton : public QPushButton
{
Q_OBJECT
public:
    CButton(QWidget* = nullptr);
    CButton(const QString&, QWidget* = nullptr);
    CButton(const QIcon &, const QString&, QWidget* = nullptr);
private:
    void setProperties();
};

#endif // CBUTTON_H
