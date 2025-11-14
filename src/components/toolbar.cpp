#include "toolbar.h"
#include "../context/uicontext.h"
#include "../context/applicationcontext.h"
#include "../iconmanager/iconmanager.h"

ToolBar::ToolBar(QWidget *parent) : QFrame{parent} {
    m_group = new QButtonGroup(this);
    m_layout = new QHBoxLayout(this);
    this->setLayout(m_layout);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setFrameShape(QFrame::StyledPanel);
    this->setFrameShadow(QFrame::Raised);
    this->setAutoFillBackground(true);
    this->setProperty("class", "drawlyFrame drawlyToolBar");

    QObject::connect(m_group, &QButtonGroup::idClicked, this, &ToolBar::onToolChanged);
}

ToolBar::~ToolBar() {
    for (auto &idToolPair : m_tools) {
        delete idToolPair.second;
    }
}

Tool &ToolBar::curTool() const {
    int curID{m_group->checkedId()};

    if (m_tools.find(curID) == m_tools.end())
        throw std::logic_error("Trying to access non existent tool");

    return *m_tools.at(curID);
}

QVector<Tool *> ToolBar::tools() const {
    QVector<Tool *> result;
    for (auto &idToolPair : m_tools) {
        result.push_back(idToolPair.second);
    }

    return result;
}

void ToolBar::addTool(Tool *tool, Tool::Type type) {
    if (tool == nullptr)
        return;

    ApplicationContext *context{ApplicationContext::instance()};
    QPushButton *btn{new QPushButton("", this)};
    btn->setIcon(context->uiContext().iconManager().icon(tool->icon()));

    btn->setCheckable(true);
    btn->setProperty("class", "drawlyToolButton");
    btn->setCursor(Qt::PointingHandCursor);

    int id{static_cast<int>(type)};

    m_tools[id] = tool;
    m_group->addButton(btn, id);
    m_layout->addWidget(btn);
    if (m_tools.size() == 1) {
        m_group->button(id)->setChecked(true);
        emit toolChanged(*tool);
    }
};

void ToolBar::changeTool(Tool::Type type) {
    int id{static_cast<int>(type)};

    m_group->button(id)->setChecked(true);
    emit toolChanged(curTool());
}

Tool& ToolBar::tool(Tool::Type type) const {
    return *m_tools.at(type);
}

// PRIVATE SLOTS
void ToolBar::onToolChanged(int id) {
    emit toolChanged(curTool());
}
