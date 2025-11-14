#ifndef SELECTIONTOOLMOVESTATE_H
#define SELECTIONTOOLMOVESTATE_H

#include "selectiontoolstate.h"
#include <QPointF>

class SelectionToolMoveState : public SelectionToolState {
public:
    bool mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    bool mouseReleased(ApplicationContext *context) override;

private:
    QPointF m_lastPos{};
    QPointF m_initialPos{};
};

#endif  // SELECTIONTOOLMOVESTATE_H
