#ifndef SELECTIONTOOLMOVESTATE_H
#define SELECTIONTOOLMOVESTATE_H

#include "selectiontoolstate.h"
#include <QPointF>

class SelectionToolMoveState : public SelectionToolState {
public:
    void mouseMoved(ApplicationContext* context) override;
    void mousePressed(ApplicationContext* context) override;
    void mouseReleased(ApplicationContext* context) override;

private:
    QPointF m_lastPos;
};

#endif // SELECTIONTOOLMOVESTATE_H
