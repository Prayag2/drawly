#ifndef FILL_H
#define FILL_H

#include <QColor>
#include "./itemproperty.h"

class Fill : public ItemProperty {
  public:
    Fill();
    const QColor& color() const;
    void setColor(const QColor& color);
  private:
    QColor m_color {};
};

#endif // FILL_H
