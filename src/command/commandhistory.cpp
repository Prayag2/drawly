#include "commandhistory.h"

#include <QDebug>

CommandHistory::CommandHistory(ApplicationContext *context) : m_context{context} {
    m_undoStack = std::make_unique<std::deque<std::shared_ptr<Command>>>();
    m_redoStack = std::make_unique<std::deque<std::shared_ptr<Command>>>();
}

CommandHistory::~CommandHistory() {
    qDebug() << "Object deleted: CommandHistory";
}

void CommandHistory::undo() {
    if (m_undoStack->empty())
        return;

    std::shared_ptr<Command> lastCommand{m_undoStack->front()};
    lastCommand->undo(m_context);

    m_redoStack->push_front(lastCommand);

    if (m_redoStack->size() == maxCommands)
        m_redoStack->pop_back();

    m_undoStack->pop_front();
}

void CommandHistory::redo() {
    if (m_redoStack->empty())
        return;

    std::shared_ptr<Command> nextCommand{m_redoStack->front()};
    nextCommand->execute(m_context);

    m_undoStack->push_front(nextCommand);
    if (m_undoStack->size() == maxCommands)
        m_undoStack->pop_back();

    m_redoStack->pop_front();
}

void CommandHistory::insert(std::shared_ptr<Command> command) {
    while (!m_redoStack->empty()) {
        m_redoStack->pop_front();
    }

    command->execute(m_context);

    m_undoStack->push_front(command);
    if (m_undoStack->size() == maxCommands)
        m_undoStack->pop_back();
}
