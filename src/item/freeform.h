#ifndef FREEFORM_H
#define FREEFORM_H

#include "item.h"

class Freeform : public Item {
  public:
    Freeform();
    ~Freeform() = default;

    static int minPointDistance();
    void draw(QPainter& painter, const QPointF& offset) override;
    void erase(QPainter& painter, const QPointF& offset) const override;
    void quickDraw(QPainter& painter, const QPointF& offset) const;
    bool intersects(const QRectF& rect) override;

    virtual void addPoint(const QPointF& point);

  protected:
    void m_draw(QPainter& painter, const QPointF& offset) const override;
    QVector<QPointF> m_points {};
    QVector<QPointF> m_optimizedPoints {};
    int m_bufferSize {15};

    std::unique_ptr<QImage> m_cache {};
    bool m_cacheDirty {true};

  private:
    QPointF optimizePoint(QVector<QPointF>& points, int bufferSize, const QPointF& newPoint) const;
};

#endif // FREEFORM_H
