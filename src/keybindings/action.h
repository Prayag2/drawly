#ifndef ACTION_H
#define ACTION_H

#include <QDebug>
#include <QObject>

class Action : public QObject {
    Q_OBJECT
private:
    QString m_name;
    QString m_description;
    std::function<void()> m_callable;

public slots:
    void run();

public:
    Action(QString name, QString description, std::function<void()> callable, QObject *parent);
    QString name() const;
};

namespace std {
template <>
struct hash<Action> {
    size_t operator()(Action &action) const noexcept { return std::hash<QString>{}(action.name()); }
};
};  // namespace std

#endif
