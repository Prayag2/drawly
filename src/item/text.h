#ifndef TEXT_H
#define TEXT_H

#include "item.h"
#include <set>
#include <QPainter>
#include <QRect>

class Text : public Item {
public:
    Text();
    ~Text();

    bool intersects(const QRectF& rect) override;
    bool intersects(const QLineF& rect) override;

    void draw(QPainter& painter, const QPointF& offset) override;
    void erase(QPainter& painter, const QPointF& offset,
               QColor color = Qt::transparent) const override;

    void translate(const QPointF& amount) override;

    void createTextBox(const QPoint position);

    enum Mode {
        EDIT,
        NORMAL
    };

    Mode mode() const;
    void setMode(Mode mode);

    int cursor() const;
    void setCursor(int index);

    int selectionStart() const;
    int selectionEnd() const;
    void setSelectionStart(int index);
    void setSelectionEnd(int index);

    const QString& text() const;
    void insertText(const QString& text, const QPointF& offset);
    void deleteSubStr(int start, int end);

    std::pair<int, int> getLineRange(int position) const;

    constexpr static int INVALID {-1};

protected:
    void m_draw(QPainter& painter, const QPointF& offset) const override;

private:
    QFont getFont() const;
    QPen getPen() const;

    constexpr static int getTextFlags();

    QString m_text;
    std::set<int> m_newlinePositions; // stores positions of "\n" characters

    int m_cursorIndex;
    int m_selectionStart;
    int m_selectionEnd;
    Mode m_mode;
};

#endif  // TEXT_H
