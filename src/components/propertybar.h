#ifndef PROPERTYBAR_H
#define PROPERTYBAR_H

#include <QFrame>
#include <QStackedWidget>
#include <QVBoxLayout>
class Tool;
enum class ToolID;

class PropertyBar : public QFrame {
    Q_OBJECT
public:
    PropertyBar(QWidget *parent = nullptr);

public slots:
    void updateProperties(Tool &);

private:
    QVBoxLayout *m_layout{};
};

#endif  // PROPERTYBAR_H
