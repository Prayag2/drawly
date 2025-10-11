#ifndef KEYBINDMANAGER_H
#define KEYBINDMANAGER_H

#include <unordered_map>
#include <QKeySequence>
#include <QShortcut>
#include "action.h"

class KeybindManager : public QObject {
public:
    KeybindManager(QObject* parent);

    void addKeybinding(Action* action, const QString& sequence);
    void removeKeybinding(QKeySequence sequence);
    void disable();
    void enable();

private:
    std::unordered_map<QString, QShortcut*> m_keyToShortcut;
    std::unordered_map<QString, Action*> m_keyToAction;
};

#endif  // KEYBINDMANAGER_H
