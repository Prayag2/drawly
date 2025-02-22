#include "toolbar.h"

Toolbar::Toolbar(QWidget *parent)
    : QFrame{parent}
{
    m_group = new QButtonGroup(this);
    m_layout = new QHBoxLayout(this);
    this->setLayout(m_layout);

    QObject::connect(m_group, &QButtonGroup::idClicked, this, &Toolbar::onToolChanged);
}

Toolbar::~Toolbar() {
    for (Tool *tool : m_tools) {
        delete tool;
    }
}

Tool& Toolbar::getCurTool() const {
    return *m_tools[m_group->checkedId()];
}

void Toolbar::addTool(Tool *tool) {
    if (tool == nullptr) return;

    QPushButton* btn {new QPushButton(tool->iconAlt(), this)};
    btn->setCheckable(true);

    int id {static_cast<int>(m_tools.size())};

    m_tools.push_back(tool);
    m_group->addButton(btn, id);
    m_layout->addWidget(btn);
    if (m_tools.size() == 1) {
        m_group->button(id)->setChecked(true);
        emit toolChanged(*tool);
    }
};

// PRIVATE SLOTS
void Toolbar::onToolChanged(int id) {
    emit toolChanged(getCurTool());
}
