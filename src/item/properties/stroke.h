#ifndef STROKE_H
#define STROKE_H

#include <QColor>
#include "./itemproperty.h"

class Stroke : public ItemProperty {
  public:
    Stroke();
    int width() const;
    void setWidth(int width);

    const QColor& color() const;
    void setColor(const QColor& color);

  private:
    int m_width {};
    QColor m_color {};
};

#endif // STROKE_H
