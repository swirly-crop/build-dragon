#include "bridge.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Objective-C wrapper exposed as C for convenience. Uses popen so it works on macOS/Linux.
void objc_run_idevice_info(const char *udid) {
    if (!udid) { puts("udid is null"); return; }
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "ideviceinfo -u %s 2>/dev/null", udid);
    FILE *f = popen(cmd, "r");
    if (!f) { puts("failed to run ideviceinfo"); return; }
    char buf[256];
    while (fgets(buf, sizeof(buf), f) != NULL) fputs(buf, stdout);
    pclose(f);
}
