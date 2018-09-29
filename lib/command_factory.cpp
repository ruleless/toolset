#include "command_factory.h"

bool CommandFactoryRegistry::valid_ = false;

bool Command::shouldHint(const std::string &cmdname) const
{
    std::string source(cmdname);
    std::string target(getName());

    return target.find(source) != std::string::npos;
}


CommandFactoryRegistry::CommandFactoryRegistry()
{
    valid_ = true;
}

CommandFactoryRegistry::~CommandFactoryRegistry()
{
    valid_ = false;
}

CommandFactoryRegistry *CommandFactoryRegistry::getRegistry()
{
    static CommandFactoryRegistry s_registry;
    return &s_registry;
}

bool CommandFactoryRegistry::isValid()
{
    return valid_;
}

void CommandFactoryRegistry::registerFactory(CommandFactory *factory)
{
    factories_.push_back(factory);
}

void CommandFactoryRegistry::unregisterFactory(CommandFactory *factory)
{
    auto it = factories_.begin();
    while (it != factories_.end()) {
        if ((*it) == factory) {
            it = factories_.erase(it);
        } else {
            ++it;
        }
    }
}

CommandFactory *CommandFactoryRegistry::getCommandFactory(const std::string &cmdName)
{
    for (auto i : factories_) {
        if (i->getName() == cmdName) {
            return i;
        }
    }
    return NULL;
}
