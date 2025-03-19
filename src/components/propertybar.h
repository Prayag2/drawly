#ifndef PROPERTYBAR_H
#define PROPERTYBAR_H

#include <QFrame>
#include <QVBoxLayout>
class Tool;

class PropertyBar : public QFrame
{
    Q_OBJECT
public:
    PropertyBar(QWidget* parent = nullptr);

public slots:
    void toolChanged(Tool&);

private:
    QVBoxLayout* m_layout {};
};

#endif // PROPERTYBAR_H
