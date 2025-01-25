#ifndef EVENT_H
#define EVENT_H

#include <QtTypes>
#include <QPoint>

class Event
{
public:
    Event();
    const QPoint& pos() const;
    Qt::MouseButton button() const;

    void setPos(const QPoint& point);
    void setButton(Qt::MouseButton btn);

private:
    Qt::MouseButton m_button;
    QPoint m_pos;
    qreal m_pressure;
};

#endif // EVENT_H
