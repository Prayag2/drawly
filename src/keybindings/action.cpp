#include "action.h"

Action::Action(QString name, QString description, std::function<void()> callable, QObject* parent)
    : m_name{name}, m_description{description}, m_callable{std::move(callable)}, QObject{parent} {}

QString Action::name() const {
    return m_name;
}

void Action::run() {
    m_callable();
}
