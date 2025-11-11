#ifndef PROPERTYBAR_H
#define PROPERTYBAR_H

#include <QFrame>
#include <QStackedWidget>
#include <QVBoxLayout>
class Tool;
class PropertyManager;
enum class ToolID;

class PropertyBar : public QFrame {
    Q_OBJECT
public:
    PropertyBar(QWidget *parent = nullptr);

    void setPropertyManager(PropertyManager *manager);
    void updateToolProperties();

public slots:
    void updateProperties(Tool &);

private:
    QVBoxLayout *m_layout{};
    PropertyManager *m_propertyManager{};
};

#endif  // PROPERTYBAR_H
