#include "keybindmanager.h"

KeybindManager::KeybindManager(QObject* parent)
    : QObject{parent} {}

void KeybindManager::addKeybinding(Action* action, const QString& sequence) {
    if (m_keyToAction.find(sequence) != m_keyToAction.end())
        return;

    QShortcut* shortcut;
    if (m_keyToShortcut.find(sequence) == m_keyToShortcut.end()) {
        shortcut = new QShortcut{QKeySequence::fromString(sequence), parent()};
    } else {
        shortcut = m_keyToShortcut[sequence];
    }

    QObject::connect(shortcut, &QShortcut::activated, action, &Action::run);
}

void removeKeybinding(QKeySequence sequence);
