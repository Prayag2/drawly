#ifndef FREEFORM_H
#define FREEFORM_H

#include "item.h"

class Freeform : public Item {
  public:
    Freeform();
    ~Freeform() = default;

    void draw(QPainter& painter, const QPoint& offset) override;
    void erase(QPainter& painter, const QPoint& offset) const override;
    void quickDraw(QPainter& painter, const QPoint& offset) const;
    bool intersects(const QRect& rect) override;

    virtual void addPoint(const QPoint& point);

  protected:
    void m_draw(QPainter& painter, const QPoint& offset) const override;
    QVector<QPoint> m_points {};
    QVector<QPoint> m_buffer {};
    int m_bufferSize {15};

    std::unique_ptr<QImage> m_cache {};
    bool m_cacheDirty {true};

  private:
    QPoint optimizePoint(QVector<QPoint>& points, int bufferSize, const QPoint& newPoint) const;
};

#endif // FREEFORM_H
