#ifndef SELECTIONTOOLSELECTSTATE_H
#define SELECTIONTOOLSELECTSTATE_H

#include "selectiontoolstate.h"
#include <QPointF>

class SelectionToolSelectState : public SelectionToolState {
public:
    bool mousePressed(ApplicationContext *context) override;
    void mouseMoved(ApplicationContext *context) override;
    bool mouseReleased(ApplicationContext *context) override;

private:
    QPointF m_lastPos;
};

#endif  // SELECTIONTOOLSELECTSTATE_H
