#ifndef ACTIONBAR_H
#define ACTIONBAR_H

#include <QFrame>
#include <QHBoxLayout>
#include <QHash>
#include <QPushButton>

class ActionBar : public QFrame {
    Q_OBJECT
public:
    ActionBar(QWidget* parent = nullptr);

    void addButton(const QString& text, int id);
    QPushButton& button(int id);

private:
    QHash<int, QPushButton*> m_map{};
    QHBoxLayout* m_layout{};
};

#endif  // ACTIONBAR_H
