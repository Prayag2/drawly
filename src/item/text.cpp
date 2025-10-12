#include "text.h"
#include "../common/utils.h"

#include <QFontMetricsF>

/*
 * TODO: The current implementation is not optimal. A single character insertion costs 
 *       n operations on average, making it O(n) per character.
 *       Use a better data structure like Ropes or Gap buffers although it may be overkill
 *       for a simple whiteboard app.
 */

Text::Text() {
    m_properties[ItemProperty::StrokeWidth] = ItemProperty(1);
    m_properties[ItemProperty::StrokeColor] = ItemProperty(QColor(Qt::white).rgba());
    m_properties[ItemProperty::Opacity] = ItemProperty(255);
    m_properties[ItemProperty::FontSize] = ItemProperty(18);

    m_selectionStart = INVALID;
    m_selectionEnd = INVALID;
    m_cursorIndex = 0;
    m_text = "";
    m_mode = NORMAL;
}

Text::~Text() {}

void Text::createTextBox(const QPoint position) {
    m_boundingBox.setTopLeft(position);
    m_boundingBox.setWidth(20);
    m_boundingBox.setHeight(50);
}

bool Text::intersects(const QRectF& rect) {
    return m_boundingBox.intersects(rect);
}

bool Text::intersects(const QLineF& line) {
    return Common::intersects(m_boundingBox, line);
}

void Text::draw(QPainter& painter, const QPointF& offset) {
    painter.save();

    QRectF curBox{m_boundingBox.translated(-offset)};

    painter.setFont(getFont());
    painter.setPen(getPen());

    int cur{cursor()};

    if (mode() == EDIT) {
        QPen boundingBoxPen{Qt::blue};
        boundingBoxPen.setWidth(1);
        painter.setPen(boundingBoxPen);

        painter.drawRect(curBox);

        auto [start, end] = getLineRange(cur);
        const QString& curLine{m_text.mid(start, cur - start)};

        int lineCount{0};
        for (int pos{0}; pos < cur; pos++) {
            if (m_text[pos] == "\n")
                lineCount++;
        }

        QFontMetrics metrics{getFont()};

        int width {metrics.size(getTextFlags(), curLine).width()};
        int lineHeight {metrics.height()};

        QPointF carrotTop{
            curBox.topLeft().x() + width,
            curBox.topLeft().y() + lineHeight * lineCount
        };

        QPointF carrotBottom{carrotTop.x(), carrotTop.y() + lineHeight};

        QLineF line{carrotTop, carrotBottom};

        painter.setPen(getPen());
        painter.drawLine(carrotTop, carrotBottom);
    }

    painter.drawText(curBox, getTextFlags(), m_text);
    painter.restore();
}

void Text::erase(QPainter& painter, const QPointF& offset, QColor color) const {

}

void Text::translate(const QPointF& amount) {

}

void Text::m_draw(QPainter& painter, const QPointF& offset) const {

}

Text::Mode Text::mode() const {
    return m_mode;
}

void Text::setMode(Mode mode) {
    m_mode = mode;
}

int Text::cursor() const {
    return m_cursorIndex;
}

void Text::setCursor(int index) {
    if (index < 0 || index > m_text.size())
        return;

    m_cursorIndex = index;
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

void Text::insertText(const QString& text, const QPointF& offset) {
    int textSize(text.size());
    int cur{cursor()};

    m_text.insert(cur, text);
    setCursor(cur + textSize);

    QFontMetricsF metrics{getFont()};
    QSizeF size {metrics.size(getTextFlags(), m_text)};

    m_boundingBox.setWidth(size.width());
    m_boundingBox.setHeight(size.height());

    qDebug() << "New Bounding Box: " << m_boundingBox;
    qDebug() << "Text Size: " << m_text.size();
}

void Text::deleteSubStr(int start, int end) {
    if (start < 0 || start >= m_text.size() || end < 0 || end >= m_text.size() || end < start)
        return;

    auto rangeStart = m_newlinePositions.lower_bound(start);
    auto rangeEnd = m_newlinePositions.upper_bound(end);
    m_newlinePositions.erase(rangeStart, rangeEnd);

    m_text.erase(m_text.begin()+start, m_text.begin()+end+1);
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

std::pair<int, int> Text::getLineRange(int position) const {
    int cur{cursor()};
    
    int start = m_text.lastIndexOf("\n", cur-1);
    int end = m_text.indexOf("\n", cur);

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

const QString& Text::text() const {
    return m_text;
}
