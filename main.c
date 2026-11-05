#include "log.h"

int main(void)
{
    restart_log();

    LOG("PRIVET");

    close_log();

    return 0;
}
