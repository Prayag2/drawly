#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QHBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QButtonGroup>
#include "../tools/tool.h"

class Toolbar : public QFrame
{
    Q_OBJECT
public:
    explicit Toolbar(QWidget *parent = nullptr);
    ~Toolbar();

    Tool& getCurTool() const;
    void addTool(Tool *tool);

signals:
    void toolChanged(Tool&);

private:
    QButtonGroup *m_group {};
    QHBoxLayout *m_layout {};
    QVector<Tool*> m_tools {};
    void createButtons() const;

private slots:
    void onToolChanged(int id);
};

#endif // TOOLBAR_H
