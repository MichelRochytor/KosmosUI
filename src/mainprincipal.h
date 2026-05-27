#include "kosmos.h"
#include "resource.h"

KWidget texto, digtar, confirmar, combobox;
KWidget commandlink;

kcontroller mainPrincipal(KWidget janela, KEvent evento, Kid id, KData data){
    switch (evento){
        case KInit: 
            texto = kGetWidget(janela, TEXTO);
            digtar = kGetWidget(janela, DIGTAR);
            confirmar = kGetWidget(janela, CONFIRMAR);
            combobox = kGetWidget(janela, COMBOFELIZ);
            commandlink = kGetWidget(janela, COMMAND1);

            kComboClear(combobox);
            kComboAdd(combobox, L"Opção 1");
            kComboAdd(combobox, L"Opção 2");

            kCommandLinkSetTitle(commandlink, L"Título do Command Link");
            kCommandLinkSetNote(commandlink, L"Nota do Command Link");

            kSetWidgetFont(texto, L"Audiowide", 30, TRUE, FALSE, FALSE);
            return 1;
        break;
        
        case KCommand:
            int widget = kGetId(id);
            switch (widget) {
                case CONFIRMAR:
                    WCHAR buffer[256];
                    kGetText(digtar, buffer, 256);
                    kSetText(texto, buffer);
                break;
            }
        return 1;
        break;
        case KClose:
            EndKosmos(janela);
        return 1;
        break;
   }
   return 0;
}
