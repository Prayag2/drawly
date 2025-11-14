#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QObject>
#include <unordered_map>

class IconManager : public QObject {
public:
    IconManager(QObject *parent = nullptr);

    enum Icon {
        TOOL_SELECTION,
        TOOL_FREEFORM,
        TOOL_RECTANGLE,
        TOOL_ELLIPSE,
        TOOL_ARROW,
        TOOL_LINE,
        TOOL_ERASER,
        TOOL_TEXT,
        TOOL_MOVE,
        ACTION_SAVE,
        ACTION_OPEN_FILE,
        ACTION_ZOOM_IN,
        ACTION_ZOOM_OUT,
        ACTION_UNDO,
        ACTION_REDO,
        ACTION_DARK_MODE,
        ACTION_LIGHT_MODE,
        UI_ANGLE_UP,
        UI_ANGLE_DOWN,
    };

    QIcon icon(Icon icon) const;

private:
    std::unordered_map<Icon, QString> m_iconName;

    QString m_curTheme{"fa-dark"}; // default
};

#endif // ICONMANAGER_H
