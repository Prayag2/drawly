#ifndef EVENT_H
#define EVENT_H
#include <qglobal.h>

#include <QPoint>
#include <QString>

class Event {
public:
    Event();
    const QPoint &pos() const;
    Qt::MouseButton button() const;
    qreal pressure() const;
    QString text() const;
    int key() const;
    Qt::KeyboardModifiers modifiers() const;

    void setPos(const QPoint &point, qreal const scale = 1.0);
    void setButton(Qt::MouseButton btn);
    void setPressure(qreal pressure);
    void setKey(int key);
    void setText(const QString &text);
    void setModifiers(Qt::KeyboardModifiers modifiers);

private:
    Qt::MouseButton m_button;
    QPoint m_pos;
    qreal m_pressure{1.0};
    QString m_text;
    int m_key;
    Qt::KeyboardModifiers m_modifiers;
};

#endif  // EVENT_H
