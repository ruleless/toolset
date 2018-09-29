#include "toolset.h"

typedef std::vector<Command *> CommandList;

static char PROGRAM_NAME[64] = {0};
static char CMDNAME[64] = {0};

const char *programName()
{
    return PROGRAM_NAME;
}

int main(int argc, char *argv[])
{
    CommandFactoryRegistry *registry = CommandFactoryRegistry::getRegistry();
    CommandFactory *factory;

    // get program name
    if (argc > 0) {
        const char *ptr = argv[0] + strlen(argv[0]);
        while (ptr > argv[0] && *ptr != '/')
            ptr--;
        if ('/' == *ptr)
            ptr++;
        snprintf(PROGRAM_NAME, sizeof(PROGRAM_NAME), "%s", ptr);
    }

    if (argc <= 1) {
        goto error;
    }

    snprintf(CMDNAME, sizeof(CMDNAME), "%s", argv[1]);
    factory = registry->getCommandFactory(CMDNAME);
    if (factory) {
        Command *cmd = factory->getCommand();
        assert(cmd && "create command failed");
        cmd->run(argc - 1, argv + 1);
    } else {
        goto error;
    }

    exit(0);

error:
    // 命令补全
    CommandList cmdlist;
    for (auto it : *registry) {
        CommandFactory *fac = it;
        Command *cmd = fac->getCommand();
        cmdlist.push_back(cmd);
    }

    if (*CMDNAME) {
        printf("\"%s\" is invalid, Possible commands are:\n\n", CMDNAME);
    } else {
        printf("Supported commands\n\n");
    }

    for (auto i : cmdlist) {
        if (!*CMDNAME || i->shouldHint(CMDNAME)) {
            printf("  %-20s %s\n", i->getName().c_str(), i->desc());
        }
    }
    puts("");

    exit(1);
}
