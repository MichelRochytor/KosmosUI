#include "kosmos.h"
#include "resource.h"


KosmosWindow(ProcessarDialogo) {
    KWINDOW texto = GetDlgItem(hDlg, TEXTO);            
    switch (msg) {
        case WINDOW_INIT: {
        }
        return TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case OK:
                    SetWindowText(texto, L"HELLO KOSMOS!");
                    break;
                case IDCANCEL:
                    EndKosmos(hDlg);
                    break;
            }
        return TRUE;
        case WM_CLOSE:
            EndKosmos(hDlg);
            break;
    }
    return FALSE;
}

KosmosMain(){
    KWINDOW hDialog = KCreateWindow(hInst, IDD_DIALOG1, ProcessarDialogo);
    ShowWindow(hDialog, 1);
    LoopMsg(hDialog);
    return 0;
}
