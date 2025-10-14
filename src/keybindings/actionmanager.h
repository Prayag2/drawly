#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>
class ApplicationContext;

class ActionManager : public QObject {
    Q_OBJECT
public:
    ActionManager(ApplicationContext *context);

    void zoomIn();
    void zoomOut();
    void undo();
    void redo();
    void increaseThickness();
    void decreaseThickness();
    void switchToFreeformTool();
    void switchToEraserTool();
    void switchToSelectionTool();
    void switchToRectangleTool();
    void switchToEllipseTool();
    void switchToLineTool();
    void switchToArrowTool();
    void switchToMoveTool();

private:
    ApplicationContext *m_context;
};

#endif  // ACTIONMANAGER_H
