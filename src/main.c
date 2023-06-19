#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"

void prt_help() {
    printf("Usage:\nsl <file>\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    if(argc < 2) prt_help();
    
    lxr_t *lxr = lxr_init(
        read_file(argv[1])
    );

    prs_t *prs = prs_init(lxr);
    ast_t *root = prs_parse(prs, prs->scp);
    vst_t *vst = vst_init();
    vst_visit(vst, root);

    return 0;
}