#include "toolset.h"

RUN(fake, "fake command test")
{
    CONSOLE(Debug, "this is a fake command test");
    CONSOLE(Info, "this is a fake command test");
    CONSOLE(Warning, "this is a fake command test");
    CONSOLE(Error, "this is a fake command test");
}
