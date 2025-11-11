#ifndef UICONTEXT_H
#define UICONTEXT_H

#include <QWidget>
class ToolBar;
class PropertyBar;
class ActionBar;
class Event;
class PropertyManager;
class Tool;
class ApplicationContext;
class KeybindManager;
class ActionManager;

class UIContext : public QObject {
public:
    UIContext(ApplicationContext *context);
    ~UIContext();

    void setUIContext();

    ToolBar &toolBar() const;
    PropertyBar &propertyBar() const;
    ActionBar &actionBar() const;
    Event &event() const;
    KeybindManager &keybindManager() const;
    ActionManager &actionManager() const;
    PropertyManager& propertyManager() const;

public slots:
    void toolChanged(Tool &);

private:
    ToolBar *m_toolBar{nullptr};
    PropertyBar *m_propertyBar{};
    ActionBar *m_actionBar{};
    PropertyManager *m_propertyManager{};
    KeybindManager *m_keybindManager{};
    ActionManager *m_actionManager{};
    Event *m_event{nullptr};

    ApplicationContext *m_applicationContext;
};

#endif  // UICONTEXT_H
