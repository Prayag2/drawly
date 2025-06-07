#ifndef EVENT_H
#define EVENT_H
#include <qglobal.h>

#include <QPoint>

class Event {
public:
    Event();
    const QPoint& pos() const;
    Qt::MouseButton button() const;
    qreal pressure() const;

    void setPos(const QPoint& point, qreal const scale = 1.0);
    void setButton(Qt::MouseButton btn);
    void setPressure(qreal pressure);

private:
    Qt::MouseButton m_button;
    QPoint m_pos;
    qreal m_pressure{1.0};
};

#endif  // EVENT_H
