#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* run_cmd(const char *cmd) {
    FILE *fp = popen(cmd, "r");
    if (!fp) return NULL;
    char *data = NULL;
    size_t len = 0;
    char buf[256];
    while (fgets(buf, sizeof(buf), fp)) {
        size_t blen = strlen(buf);
        data = realloc(data, len + blen + 1);
        memcpy(data + len, buf, blen);
        len += blen;
        data[len] = '\0';
    }
    pclose(fp);
    return data;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "Usage: c_auditor [--devices] [--list-packages] [--third-party] [--permissions PKG] [--net]\n");
        return 1;
    }

    int list_packages = 0, third_party = 0, devices = 0, net = 0;
    char *pkg = NULL;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--list-packages") == 0) {
            list_packages = 1;
        } else if (strcmp(argv[i], "--third-party") == 0) {
            third_party = 1;
        } else if (strcmp(argv[i], "--devices") == 0) {
            devices = 1;
        } else if (strcmp(argv[i], "--net") == 0) {
            net = 1;
        } else if (strcmp(argv[i], "--permissions") == 0 && i + 1 < argc) {
            pkg = argv[++i];
        }
    }

    if (devices) {
        char *out = run_cmd("adb devices -l");
        if (out) {
            printf("%s", out);
            free(out);
        }
    }

    if (list_packages) {
        char cmd[256];
        if (third_party) {
            strcpy(cmd, "adb shell pm list packages -3");
        } else {
            strcpy(cmd, "adb shell pm list packages");
        }
        char *out = run_cmd(cmd);
        if (out) {
            printf("%s", out);
            free(out);
        }
    }

    if (pkg) {
        char cmd[256];
        sprintf(cmd, "adb shell dumpsys package %s", pkg);
        char *out = run_cmd(cmd);
        if (out) {
            // parse permissions
            char *line = strtok(out, "\n");
            int collect = 0;
            while (line) {
                if (strstr(line, "grantedPermissions:")) {
                    collect = 1;
                } else if (collect && !*line) {
                    break;
                } else if (collect) {
                    printf("%s\n", line);
                }
                line = strtok(NULL, "\n");
            }
            free(out);
        }
    }

    if (net) {
        char *out = run_cmd("adb shell netstat");
        if (out) {
            printf("%s", out);
            free(out);
        }
    }

    return 0;
}