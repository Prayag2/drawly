#include "commandhistory.h"
#include <QDebug>

CommandHistory::CommandHistory(ApplicationContext* context)
    : m_context{context}
{
    m_undoStack = std::make_unique<std::stack<std::shared_ptr<Command>>>();
    m_redoStack = std::make_unique<std::stack<std::shared_ptr<Command>>>();
}

CommandHistory::~CommandHistory() {
    qDebug() << "Object deleted: CommandHistory";
}

void CommandHistory::undo() {
    if (m_undoStack->empty())
        return;

    std::shared_ptr<Command> lastCommand{m_undoStack->top()};
    lastCommand->undo(m_context);

    m_redoStack->push(lastCommand);
    m_undoStack->pop();
}

void CommandHistory::redo() {
    if (m_redoStack->empty())
        return;

    std::shared_ptr<Command> nextCommand{m_redoStack->top()};
    nextCommand->execute(m_context);

    m_undoStack->push(nextCommand);
    m_redoStack->pop();
}

void CommandHistory::insert(std::shared_ptr<Command> command) {
    while (!m_redoStack->empty()) {
        m_redoStack->pop();
    }

    command->execute(m_context);
    m_undoStack->push(command);
}
