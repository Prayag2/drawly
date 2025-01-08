#include "ctoolbar.h"

CToolBar::CToolBar(QWidget *parent) : QFrame(parent) {
    m_layout = new QHBoxLayout(this);
    this->setStyleSheet("CToolBar {border: 2px solid red;}");
    this->setLayout(m_layout);
    parent->installEventFilter(this);
}

void CToolBar::addTool(QWidget *widget) {
    m_layout->addWidget(widget);
}

Qt::Edge CToolBar::position() const {
    return m_position;
}

void CToolBar::setMargin(qint32 margin) {
    m_margin = margin;
}

qint32 CToolBar::margin() const {
    return m_margin;
}

void CToolBar::setPosition(Qt::Edge edge) {
    QSize windowSize = this->parentWidget()->size();
    this->adjustSize();

    switch(edge) {
    case Qt::LeftEdge:
    case Qt::RightEdge:
        qWarning() << "Can't set toolbar position to left or right edge. Defaulting to bottom edge.";
        [[fallthrough]];
    case Qt::BottomEdge:
        this->setGeometry(
            (windowSize.width()-this->width())/2,
            (windowSize.height()-this->height()-m_margin),
            this->width(),
            this->height()
        );
        this->m_position = Qt::BottomEdge;
        break;
    case Qt::TopEdge:
        this->setGeometry(
            (windowSize.width()-this->width())/2,
            m_margin,
            this->width(),
            this->height()
        );
        this->m_position = Qt::TopEdge;
        break;
    }
}

void CToolBar::setPosition() {
    this->setPosition(m_position);
}

bool CToolBar::eventFilter(QObject* object, QEvent* event) {
    if (object == this->parentWidget() && event->type() == QEvent::Resize) {
        this->setPosition();
    }
    return QFrame::eventFilter(object, event);
}
