#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "../tools/tool.h"
#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>

class ToolBar : public QFrame {
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
    ~ToolBar();

    Tool &curTool() const;
    void addTool(Tool *tool, Tool::Type type);
    QVector<Tool *> tools() const;

    Tool &tool(Tool::Type type) const;
    void changeTool(Tool::Type type);

signals:
    void toolChanged(Tool &);

private:
    QButtonGroup *m_group{};
    QHBoxLayout *m_layout{};
    std::unordered_map<int, Tool *> m_tools{};
    void createButtons() const;

private slots:
    void onToolChanged(int id);
};

#endif  // TOOLBAR_H
