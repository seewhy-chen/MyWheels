#include <stdio.h>
#include <string.h>

static void usage(const char *prog);

int main(int argc, const char *argv[]) {
    // 1. parse options
    // 2. start searcher
    // 3. join the search
    const char *prog = strrchr(argv[0], '\\') + 1;
    usage(prog);
    return 0;
}

static void usage(const char *prog) {
    fprintf(stdout, "Usage:\n");
    fprintf(stdout, "\n%s [options] PATTERN\n", prog);
    fprintf(stdout, "  search for files that match PATTERN\n");
    fprintf(stdout, "\nExample:\n");
    fprintf(stdout, "  %s *.sh\n", prog);
    fprintf(stdout, "\nOptions:\n");
}
