#include "kosmos.h"
#include "mainprincipal.h"
#include "resource.h"

KosmosMain(){
    KCustomFont(L"Resource\\Audiowide.ttf");
    KWidget janelaprincipal = KCreateWindow(hInst, JANELA, mainPrincipal);
    ShowWindow(janelaprincipal, 1);
    LoopMsg(janelaprincipal);
    return 0;
}
