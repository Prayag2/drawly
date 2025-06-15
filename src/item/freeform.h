#ifndef FREEFORM_H
#define FREEFORM_H

#include "item.h"
#include <deque>
#include <memory>

class Freeform : public Item, public std::enable_shared_from_this<Freeform> {
public:
    Freeform();
    ~Freeform() = default;
    Freeform(const Freeform& freeform);

    static int minPointDistance();

    void draw(QPainter& painter, const QPointF& offset) override;
    void erase(QPainter& painter, const QPointF& offset,
               QColor color = Qt::transparent) const override;
    void quickDraw(QPainter& painter, const QPointF& offset) const;

    bool intersects(const QRectF& rect) override;
    bool intersects(const QLineF& rect) override;

    void translate(const QPointF& amount) override;

    QVector<std::shared_ptr<Item>> split() const;
    int size() const;
    int maxSize() const;

    virtual void addPoint(const QPointF& point, const qreal pressure, bool optimize = true);

protected:
    void m_draw(QPainter& painter, const QPointF& offset) const override;
    QVector<QPointF> m_points{};
    QVector<qreal> m_pressures{};

private:
    QPointF optimizePoint(const QPointF& newPoint);
    std::deque<QPointF> m_currentWindow;
    QPointF m_currentWindowSum{0, 0};
    int m_bufferSize{7};
};

#endif  // FREEFORM_H
