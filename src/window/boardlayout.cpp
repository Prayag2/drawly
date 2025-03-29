#include "boardlayout.h"
#include <QWidget>

BoardLayout::BoardLayout(QWidget *parent) : QLayout(parent) {
    m_widgets[0] = &m_centralWidget;
    m_widgets[1] = &m_leftWidget;
    m_widgets[2] = &m_topWidget;
    m_widgets[3] = &m_rightWidget;
    m_widgets[4] = &m_bottomWidget;
}

BoardLayout::~BoardLayout() {
    if (m_centralWidget != nullptr) {
        delete m_centralWidget;
    }
}

void BoardLayout::addItem(QLayoutItem *item) {
    if (m_centralWidget == nullptr) {
        m_centralWidget = item;
    } else {
        delete item;
    }
}

QLayoutItem* BoardLayout::itemAt(int index) const {
    try {
        return *m_widgets.at(index);
    } catch (std::out_of_range) {
        return nullptr;
    }
}

QLayoutItem* BoardLayout::takeAt(int index) {
    try {
        QLayoutItem *item {*m_widgets.at(index)};
        *m_widgets[index] = nullptr;
        return item;
    } catch (std::out_of_range) {
        return nullptr;
    }
}

void BoardLayout::setLeftWidget(QWidget* const item, bool pinned) {
    if (m_leftWidget != nullptr) {
        delete m_leftWidget;
    }
    m_leftWidget = new QWidgetItem(item);
    m_isLeftPinned = pinned;
    if (item) item->raise();
}

void BoardLayout::setTopWidget(QWidget* const item, bool pinned) {
    if (m_topWidget != nullptr) {
        delete m_topWidget;
    }
    m_topWidget = new QWidgetItem(item);
    m_isTopPinned = pinned;
    if (item) item->raise();
}

void BoardLayout::setRightWidget(QWidget* const item, bool pinned) {
    if (m_rightWidget != nullptr) {
        delete m_rightWidget;
    }
    m_rightWidget = new QWidgetItem(item);
    m_isRightPinned = pinned;
    if (item) item->raise();
}

void BoardLayout::setBottomWidget(QWidget* const item, bool pinned) {
    if (m_bottomWidget != nullptr) {
        delete m_bottomWidget;
    }
    m_bottomWidget = new QWidgetItem(item);
    m_isBottomPinned = pinned;
    if (item) item->raise();
}

void BoardLayout::setCentralWidget(QWidget* const item) {
    if (m_centralWidget != nullptr) {
        delete m_centralWidget;
    }
    m_centralWidget = new QWidgetItem(item);
}

void BoardLayout::setGeometry(const QRect& rect) {
    QLayout::setGeometry(rect);
    if (m_centralWidget != nullptr) {
        m_centralWidget->setGeometry(rect);
        if (QWidget *w = m_centralWidget->widget()) {
            w->lower();
        }
    }

    if (m_leftWidget != nullptr) {
        m_leftWidget->setGeometry(QRect(0, (rect.height()-m_leftWidget->sizeHint().height())/2, m_leftWidget->sizeHint().width(), m_leftWidget->sizeHint().height()));
    }
    if (m_rightWidget != nullptr) {
        m_rightWidget->setGeometry(QRect(rect.width()-m_rightWidget->sizeHint().width(), (rect.height()-m_rightWidget->sizeHint().height())/2, m_rightWidget->sizeHint().width(), m_rightWidget->sizeHint().height()));
    }
    if (m_topWidget != nullptr) {
        m_topWidget->setGeometry(QRect((rect.width()-m_topWidget->sizeHint().width())/2, 0, m_topWidget->sizeHint().width(), m_topWidget->sizeHint().height()));
    }
    if (m_bottomWidget != nullptr) {
        m_bottomWidget->setGeometry(QRect((rect.width()-m_bottomWidget->sizeHint().width())/2, rect.height()-m_bottomWidget->sizeHint().height(), m_bottomWidget->sizeHint().width(), m_bottomWidget->sizeHint().height()));
    }
}

int BoardLayout::count() const {
    // there are only 5 widgets so its o(1)
    int count {0};
    for (const auto item : m_widgets) {
        if (*item != nullptr) count++;
    }
    return count;
}

QSize BoardLayout::sizeHint() const {
    return (m_centralWidget == nullptr ? QSize(0, 0) : m_centralWidget->sizeHint());
}

QSize BoardLayout::minimumSize() const {
    return (m_centralWidget == nullptr ? QSize(10, 10) : m_centralWidget->minimumSize());
}
