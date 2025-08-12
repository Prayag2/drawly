#ifndef SELECTIONTOOLSTATE_H
#define SELECTIONTOOLSTATE_H

class ApplicationContext;

class SelectionToolState {
public:
    virtual void mousePressed(ApplicationContext* context) = 0;
    virtual void mouseMoved(ApplicationContext* context) = 0;
    virtual void mouseReleased(ApplicationContext* context) = 0;

protected:
    bool m_isActive{false};
};

#endif // SELECTIONTOOLSTATE_H
