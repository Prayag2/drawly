#include "ctoolbar.h"

CToolBar::CToolBar(QWidget *parent) : QFrame(parent) {
    m_layout = new QHBoxLayout(this);
    this->setLayout(m_layout);
    this->adjustSize();
    parent->installEventFilter(this);
}

void CToolBar::addTool(QWidget *widget) {
    m_layout->addWidget(widget);
    this->adjustSize();
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
    if (edge == Qt::LeftEdge || edge == Qt::BottomEdge) {
        qWarning() << "Can't set toolbar position to left or right edge. Defaulting to bottom edge.";
        edge = Qt::BottomEdge;
    }
    this->m_position = edge;
    this->updatePosition();
}

void CToolBar::updatePosition() {
    const qint32 windowWidth {this->parentWidget()->width()};
    const qint32 windowHeight {this->parentWidget()->height()};

    switch (this->m_position) {
    case Qt::BottomEdge:
        this->setGeometry(
            (windowWidth-this->width())/2,
            (windowHeight-this->height()-m_margin),
            this->width(),
            this->height()
        );
        break;
    case Qt::TopEdge:
        this->setGeometry(
            (windowWidth-this->width())/2,
            m_margin,
            this->width(),
            this->height()
        );
        break;
    default:
        break;
    }
}

bool CToolBar::eventFilter(QObject* object, QEvent* event) {
    if (object == this->parentWidget() && event->type() == QEvent::Resize) {
        this->updatePosition();
    }
    return QObject::eventFilter(object, event);
}
