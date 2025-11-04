#ifndef TEXT_H
#define TEXT_H

#include "item.h"
#include <QPainter>
#include <QRect>
#include <set>

class Text : public Item {
public:
    Text();
    ~Text();

    bool intersects(const QRectF &rect) override;
    bool intersects(const QLineF &rect) override;

    void draw(QPainter &painter, const QPointF &offset) override;
    void erase(QPainter &painter,
               const QPointF &offset,
               QColor color = Qt::transparent) const override;

    void translate(const QPointF &amount) override;

    void createTextBox(const QPoint position);

    enum Mode { EDIT, NORMAL };

    Mode mode() const;
    void setMode(Mode mode);

    int getLineFromY(double yPos) const;
    qsizetype getIndexFromX(double xPos, int lineNumber) const;

    qsizetype caret() const;
    void setCaret(qsizetype index, bool updatePosInLine = true);
    void setCaret(const QPointF &cursorPos);
    qsizetype caretPosInLine() const;

    qsizetype selectionStart() const;
    qsizetype selectionEnd() const;
    void setSelectionStart(qsizetype index);
    void setSelectionEnd(qsizetype index);
    const QString selectedText() const;

    const QString &text() const;
    void insertText(const QString &text);
    void deleteSubStr(int start, int end);
    void deleteSelection();

    bool hasSelection() const;

    std::pair<qsizetype, qsizetype> getLineRange(int lineNumber) const;
    std::pair<qsizetype, qsizetype> getLineRange(qsizetype position) const;

    qsizetype getPrevBreak(qsizetype pos) const;
    qsizetype getNextBreak(qsizetype pos) const;

    Item::Type type() const override;

    constexpr static int INVALID{-1};

protected:
    void m_draw(QPainter &painter, const QPointF &offset) const override;

private:
    QFont getFont() const;
    QPen getPen() const;

    constexpr static int getTextFlags();

    QString m_text;
    std::set<int> m_newlinePositions;  // stores positions of "\n" characters

    void renderBoundingBox(QPainter& painter) const;

    qsizetype m_caretIndex;
    qsizetype m_selectionStart;
    qsizetype m_selectionEnd;
    qsizetype m_caretPosInLine;
    Mode m_mode;
};

#endif  // TEXT_H
