#include "actionbar.h"

#include <stdexcept>

ActionBar::ActionBar(QWidget *parent) : QFrame{parent} {
    m_layout = new QHBoxLayout{this};
    m_layout->setSpacing(10);
    this->setLayout(m_layout);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void ActionBar::addButton(const QString &text, int id) {
    if (m_map.contains(id)) {
        throw std::logic_error("Button with same id exists in the ActionBar.");
    }

    m_map[id] = new QPushButton{text, this};
    m_layout->addWidget(m_map[id]);
}

QPushButton &ActionBar::button(int id) {
    if (!m_map.contains(id)) {
        throw std::logic_error(
            QString::asprintf("Button with id %d was not found.", id).toStdString());
    }
    return *m_map[id];
}
