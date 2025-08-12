#ifndef UNDOREDOMANAGER_H
#define UNDOREDOMANAGER_H

#include <memory>
#include <stack>
#include "command.h"
class ApplicationContext;

class CommandHistory {
public:
    CommandHistory(ApplicationContext* context);
    ~CommandHistory();

    void undo();
    void redo();
    void insert(std::shared_ptr<Command> command);

private:
    std::unique_ptr<std::stack<std::shared_ptr<Command>>> m_undoStack;
    std::unique_ptr<std::stack<std::shared_ptr<Command>>> m_redoStack;

    ApplicationContext* m_context;
};

#endif // UNDOREDOMANAGER_H
