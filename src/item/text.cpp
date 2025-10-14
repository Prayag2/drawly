#include "text.h"

#include "../common/utils.h"
#include <QFontMetricsF>
#include <utility>

/*
 * TODO: The current implementation is not optimal. A single character insertion
 * costs n operations on average, making it O(n) per character. Use a better
 * data structure like Ropes or Gap buffers although it may be overkill for a
 * simple whiteboard app.
 */

Text::Text() {
    m_properties[ItemProperty::StrokeWidth] = ItemProperty(1);
    m_properties[ItemProperty::StrokeColor] = ItemProperty(QColor(Qt::white).rgba());
    m_properties[ItemProperty::Opacity] = ItemProperty(255);
    m_properties[ItemProperty::FontSize] = ItemProperty(18);

    m_selectionStart = INVALID;
    m_selectionEnd = INVALID;
    m_caretIndex = 0;
    m_text = "";
    m_mode = NORMAL;
}

Text::~Text() {
}

void Text::createTextBox(const QPoint position) {
    m_boundingBox.setTopLeft(position);
    m_boundingBox.setWidth(20);
    m_boundingBox.setHeight(50);
}

bool Text::intersects(const QRectF &rect) {
    return m_boundingBox.intersects(rect);
}

bool Text::intersects(const QLineF &line) {
    return Common::intersects(m_boundingBox, line);
}

void Text::draw(QPainter &painter, const QPointF &offset) {
    painter.save();

    QRectF curBox{m_boundingBox.translated(-offset)};

    painter.setFont(getFont());
    painter.setPen(getPen());

    qsizetype cur{caret()};

    if (mode() == EDIT) {
        QPen boundingBoxPen{Qt::blue};
        boundingBoxPen.setWidth(1);
        painter.setPen(boundingBoxPen);

        painter.drawRect(curBox);

        // PERF: There is no need to scan the entire text just to place the caret
        // This can be a lot more efficient, so feel free to open a PR
        auto [start, end] = getLineRange(cur);
        const QString &curLine{m_text.mid(start, cur - start)};

        int lineCount{0};
        for (int pos{0}; pos < cur; pos++) {
            if (m_text[pos] == "\n")
                lineCount++;
        }

        QFontMetrics metrics{getFont()};

        int width{metrics.size(getTextFlags(), curLine).width()};
        int lineHeight{metrics.height()};

        QPointF caretTop{curBox.topLeft().x() + width,
                         curBox.topLeft().y() + lineHeight * lineCount};

        QPointF caretBottom{caretTop.x(), caretTop.y() + lineHeight};

        QLineF line{caretTop, caretBottom};

        painter.setPen(getPen());
        painter.drawLine(caretTop, caretBottom);
    }

    painter.drawText(curBox, getTextFlags(), m_text);
    painter.restore();
}

void Text::erase(QPainter &painter, const QPointF &offset, QColor color) const {
}

void Text::translate(const QPointF &amount) {
}

void Text::m_draw(QPainter &painter, const QPointF &offset) const {
}

Text::Mode Text::mode() const {
    return m_mode;
}

void Text::setMode(Mode mode) {
    m_mode = mode;
}

qsizetype Text::caret() const {
    return m_caretIndex;
}

qsizetype Text::caretPosInLine() const {
    return m_caretPosInLine;
}

void Text::setCaret(qsizetype index, bool updatePosInLine) {
    if (index < 0 || index > m_text.size())
        return;

    m_caretIndex = index;
    if (updatePosInLine) {
        qsizetype firstCharOfCurLine{m_text.lastIndexOf("\n", m_caretIndex - 1)};
        m_caretPosInLine = m_caretIndex - firstCharOfCurLine;
    }
}

int Text::getLineFromY(double yPos) const {
    QFontMetricsF metrics{getFont()};
    double lineHeight{metrics.height()};

    if (lineHeight <= 0)
        return 0;

    const double distFromTop{std::abs(yPos - boundingBox().y())};
    return static_cast<int>(std::ceil(distFromTop / lineHeight));
}

qsizetype Text::getIndexFromX(double xPos, int lineNumber) const {
    QFontMetricsF metrics{getFont()};

    auto [start, end] = getLineRange(lineNumber);
    const QString line{m_text.mid(start, end - start + 1)};

    const double distanceFromLeft{std::abs(xPos - boundingBox().x())};
    const double lineWidth{
        metrics.boundingRect(boundingBox(), getTextFlags(), line).width()};

    if (distanceFromLeft > lineWidth)
        return end;

    qsizetype low{0}, high{m_text.size()}, index{0};
    while (low <= high) {
        double mid{low + (high - low) / 2.0};

        const double prefixWidth{
            metrics.boundingRect(boundingBox(), getTextFlags(), line.left(mid)).width()};

        if (prefixWidth <= distanceFromLeft) {
            index = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (index < line.size() - 1) {
        const double widthBefore{
            metrics.boundingRect(boundingBox(), getTextFlags(), line.left(index)).width()};
        const double widthAfter{
            metrics.boundingRect(boundingBox(), getTextFlags(), line.left(index + 1)).width()};

        const double midPoint{(widthBefore + widthAfter) / 2.0};
        if (distanceFromLeft > midPoint)
            index++;
    }

    return start + index;
}

void Text::setCaret(const QPointF &cursorPos) {
    if (!boundingBox().contains(cursorPos))
        return;

    const int lineNumber{getLineFromY(cursorPos.y())};
    const qsizetype index{getIndexFromX(cursorPos.x(), lineNumber)};

    setCaret(index);
}

int Text::selectionStart() const {
    return m_selectionStart;
}

int Text::selectionEnd() const {
    return m_selectionEnd;
}

void Text::setSelectionStart(int index) {
    if (index < 0 || index >= m_text.size())
        return;

    m_selectionStart = index;
}

void Text::insertText(const QString &text, const QPointF &offset) {
    if (text.isEmpty())
        return;

    qsizetype textSize{text.size()};
    qsizetype cur{caret()};

    m_text.insert(cur, text);
    setCaret(cur + textSize);

    // PERF: Some clever techniques can be used to update the bounding box instead
    //       of using the entire string to calculate it again
    QFontMetricsF metrics{getFont()};
    QSizeF size{metrics.size(getTextFlags(), m_text)};

    m_boundingBox.setWidth(size.width());
    m_boundingBox.setHeight(size.height());
}

void Text::deleteSubStr(int start, int end) {
    if (start < 0 || start >= m_text.size() || end < 0 || end >= m_text.size() || end < start)
        return;

    m_text.erase(m_text.begin() + start, m_text.begin() + end + 1);

    QFontMetricsF metrics{getFont()};
    QSizeF size{metrics.size(getTextFlags(), m_text)};

    m_boundingBox.setWidth(size.width());
    m_boundingBox.setHeight(size.height());
}

QFont Text::getFont() const {
    QFont font{};
    font.setPointSize(getProperty(ItemProperty::FontSize).value().toInt());

    return font;
}

QPen Text::getPen() const {
    QPen pen{};

    QColor color{QColor::fromRgba(getProperty(ItemProperty::StrokeColor).value().toUInt())};
    color.setAlpha(getProperty(ItemProperty::Opacity).value().toInt());
    pen.setWidth(getProperty(ItemProperty::StrokeWidth).value().toInt());
    pen.setColor(color);

    return pen;
}

std::pair<qsizetype, qsizetype> Text::getLineRange(int lineNumber) const {
    qsizetype len{m_text.length()};

    qsizetype startIndex{0};
    for (qsizetype pos{0}; pos < len; pos++) {
        if (lineNumber == 1)
            break;

        if (m_text[pos] == "\n") {
            startIndex = pos + 1;
            lineNumber--;
        }
    }

    qsizetype endIndex{m_text.indexOf("\n", startIndex)};
    if (endIndex == -1)
        endIndex = len - 1;

    return std::make_pair(startIndex, endIndex);
}

std::pair<qsizetype, qsizetype> Text::getLineRange(qsizetype position) const {
    qsizetype cur{caret()};

    qsizetype start{m_text.lastIndexOf("\n", cur - 1)};
    if (start == -1)
        start = 0;

    qsizetype end{m_text.indexOf("\n", cur)};
    if (cur == -1)
        cur = m_text.size() - 1;

    return std::make_pair(start, end);
}

constexpr int Text::getTextFlags() {
    return (Qt::TextExpandTabs);
}

void Text::setSelectionEnd(int index) {
    if (index < 0 || index >= m_text.size())
        return;

    m_selectionEnd = index;
}

const QString &Text::text() const {
    return m_text;
}

Item::Type Text::type() const {
    return Item::Text;
}
