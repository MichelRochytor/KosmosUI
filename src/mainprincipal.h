#include "kosmos.h"
#include "resource.h"

KWidget texto;
KWidget digtar;

kcontroller mainPrincipal(KWidget janela, KEvent evento, Kid id, KData data){
    switch (evento){
        case KInit: 
            texto = kGetWidget(janela, TEXTO);
            digtar = kGetWidget(janela, DIGTAR);
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
