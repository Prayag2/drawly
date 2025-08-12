#ifndef COMMAND_H
#define COMMAND_H

class ApplicationContext;

class Command {
public:
    virtual void execute(ApplicationContext* context) = 0;
    virtual void undo(ApplicationContext* context) = 0;
};

#endif // COMMAND_H
