#ifndef SELECTIONTOOLSTATE_H
#define SELECTIONTOOLSTATE_H

class ApplicationContext;

class SelectionToolState {
public:
    // The bool return value indicates whether to lock the state or not
    virtual bool mousePressed(ApplicationContext *context) = 0;
    virtual void mouseMoved(ApplicationContext *context) = 0;
    virtual bool mouseReleased(ApplicationContext *context) = 0;

protected:
    bool m_isActive{false};
};

#endif  // SELECTIONTOOLSTATE_H
