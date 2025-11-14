#include "iconmanager.h"

#include <QIcon>

IconManager::IconManager(QObject *parent) : QObject{parent} {
    m_iconName = {{TOOL_SELECTION, "tool-selection"},
                  {TOOL_FREEFORM, "tool-freeform"},
                  {TOOL_RECTANGLE, "tool-rectangle"},
                  {TOOL_ELLIPSE, "tool-ellipse"},
                  {TOOL_ARROW, "tool-arrow"},
                  {TOOL_LINE, "tool-line"},
                  {TOOL_ERASER, "tool-eraser"},
                  {TOOL_TEXT, "tool-text"},
                  {TOOL_MOVE, "tool-move"},
                  {ACTION_SAVE, "action-save"},
                  {ACTION_OPEN_FILE, "action-open_file"},
                  {ACTION_ZOOM_IN, "action-zoom_in"},
                  {ACTION_ZOOM_OUT, "action-zoom_out"},
                  {ACTION_UNDO, "action-undo"},
                  {ACTION_REDO, "action-redo"},
                  {ACTION_DARK_MODE, "action-dark_mode"},
                  {ACTION_LIGHT_MODE, "action-light_mode"},
                  {UI_ANGLE_DOWN, "ui-angle_up"},
                  {UI_ANGLE_UP, "ui-angle_down"}};
}

QIcon IconManager::icon(Icon icon) const {
    QString path{":/icons/" + m_curTheme + "/" + m_iconName.at(icon) + ".svg"};
    return QIcon(path);
}
