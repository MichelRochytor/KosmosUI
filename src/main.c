#include "kosmos.h"
#include "mainprincipal.h"
#include "resource.h"

int KosmosMain(){
    KCustomFont("Resource\\Audiowide.ttf");
    KWidget janelaprincipal = KCreateWindow(JANELA, mainPrincipal);
    ShowWindow(janelaprincipal, 1);
    LoopMsg();
    return 0;
}
