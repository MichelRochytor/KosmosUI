#include "kosmos.h"
#include "janelaprincipal.h"
#include "console.h"
#include "criar.h"
#include "resource.h"

KosmosMain(){
    KWINDOW janelaprincipal = KCreateWindow(hInst, JANELAPRINCIPAL, JanelaPrincipal);
    telaConsole = KCreateWindow(hInst, CONSOLE, Console);
    janelaCriar = KCreateWindow(hInst, JANELACRIAR, Criar);
    ShowWindow(janelaprincipal, 1);
    ShowWindow(telaConsole, 0);
    ShowWindow(janelaCriar, 0);
    LoopMsg(janelaprincipal);
    return 0;
}
