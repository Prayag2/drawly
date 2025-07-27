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
    explicit ToolBar(QWidget* parent = nullptr);
    ~ToolBar();

    Tool& curTool() const;
    void addTool(Tool* tool);
    QVector<Tool*> tools() const;

signals:
    void toolChanged(Tool&);

private:
    QButtonGroup* m_group{};
    QHBoxLayout* m_layout{};
    QVector<Tool*> m_tools{};
    void createButtons() const;

private slots:
    void onToolChanged(int id);
};

#endif  // TOOLBAR_H
