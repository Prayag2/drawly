#ifndef CTOOLBAR_H
#define CTOOLBAR_H

#include <QWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QEvent>

class CToolBar : public QFrame
{
Q_OBJECT
public:
    CToolBar(QWidget *parent = nullptr);
    void addTool(QWidget *widget);

    Qt::Edge position() const;
    void setPosition(Qt::Edge edge);
    void setPosition();

    qint32 margin() const;
    void setMargin(qint32 margin);

    bool eventFilter(QObject* object, QEvent* event) override;
private:
    QHBoxLayout* m_layout {};
    Qt::Edge m_position {Qt::BottomEdge};
    qint32 m_margin {50};
};

#endif // CTOOLBAR_H
