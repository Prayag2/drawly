#ifndef UNDOREDOMANAGER_H
#define UNDOREDOMANAGER_H

#include <memory>
#include <deque>
#include "command.h"
class ApplicationContext;

class CommandHistory {
public:
    CommandHistory(ApplicationContext* context);
    ~CommandHistory();

    void undo();
    void redo();
    void insert(std::shared_ptr<Command> command);

    static constexpr int maxCommands{100}; // arbitrary

private:
    std::unique_ptr<std::deque<std::shared_ptr<Command>>> m_undoStack;
    std::unique_ptr<std::deque<std::shared_ptr<Command>>> m_redoStack;

    ApplicationContext* m_context;
};

#endif // UNDOREDOMANAGER_H
