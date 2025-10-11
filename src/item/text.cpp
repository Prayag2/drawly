#include "text.h"
#include "../common/utils.h"

#include <QFontMetricsF>

Text::Text() {
    m_properties[ItemPropertyType::StrokeWidth] = ItemProperty(1);
    m_properties[ItemPropertyType::StrokeColor] = ItemProperty(QColor(Qt::white).rgba());
    m_properties[ItemPropertyType::Opacity] = ItemProperty(255);
    m_properties[ItemPropertyType::FontSize] = ItemProperty(18);

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

    QFont textFont{};
    textFont.setPointSize(getProperty(ItemPropertyType::FontSize).value().toInt());
    painter.setFont(textFont);

    QPen textPen{};
    QColor color{QColor::fromRgba(getProperty(ItemPropertyType::StrokeColor).value().toUInt())};
    color.setAlpha(getProperty(ItemPropertyType::Opacity).value().toInt());
    textPen.setWidth(getProperty(ItemPropertyType::StrokeWidth).value().toInt());
    textPen.setColor(color);

    if (mode() == EDIT) {
        QPen boundingBoxPen{Qt::blue};
        boundingBoxPen.setWidth(1);
        painter.setPen(boundingBoxPen);

        painter.drawRect(curBox);

        QString curLine{};
        for (int pos{m_cursorIndex - 1}; pos >= 0; pos--) {
            if (m_text[pos] == '\n')
                break;

            curLine.push_back(m_text[pos]);
        }

        std::reverse(curLine.begin(), curLine.end());
        QString prefix{m_text.left(cursor())};

        QFontMetrics metrics{textFont};

        QSizeF prefixSize{metrics.size(Qt::TextExpandTabs, prefix)}, curLineSize{metrics.size(Qt::TextExpandTabs, curLine)};

        QPointF carrotBottom{
            curBox.topLeft().x() + curLineSize.width(),
            curBox.topLeft().y() + prefixSize.height()
        };

        QPointF carrotTop{carrotBottom.x(), carrotBottom.y() - metrics.height()};
        QLineF line{carrotTop, carrotBottom};

        painter.setPen(textPen);
        painter.drawLine(carrotTop, carrotBottom);
    }

    painter.drawText(curBox, Qt::TextExpandTabs, m_text);
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
    m_text.insert(cursor(), text);
    setCursor(cursor() + text.size());
    qDebug() << "Text: " << m_text;
    qDebug() << "Cursor: " << m_cursorIndex;

    QFont font{}; font.setPointSize(getProperty(ItemPropertyType::FontSize).value().toInt());
    QFontMetricsF metrics{font};

    QSizeF size {metrics.size(Qt::TextExpandTabs, m_text)};

    m_boundingBox.setWidth(size.width());
    m_boundingBox.setHeight(size.height());
}

void Text::deleteSubStr(int start, int end) {
    if (start < 0 || start >= m_text.size() || end < 0 || end >= m_text.size() || end < start)
        return;

    m_text.erase(m_text.begin()+start, m_text.begin()+end+1);
}

void Text::setSelectionEnd(int index) {
    if (index < 0 || index >= m_text.size())
        return;

    m_selectionEnd = index;
}

const QString& Text::text() const {
    return m_text;
}
