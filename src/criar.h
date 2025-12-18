#include "kosmos.h"
#include "resource.h"

KosmosWindow(Criar){
    switch (msg_param) {
        case WM_INITDIALOG:{
            break;
        }
        return TRUE;
        case WM_CTLCOLORDLG:{
            hdc = (HDC)wParam;
            SetBkColor(hdc, RGB(24, 24, 24));
            return (INT_PTR)CreateSolidBrush(RGB(24, 24, 24));
            break;
        }
        return TRUE;
        case WM_CTLCOLOREDIT:{
            hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkColor(hdc, RGB(31, 31, 31));
            return (INT_PTR)CreateSolidBrush(RGB(31, 31, 31));
            break;
        }
        return TRUE;
        case WM_CLOSE:{
            EndDialog(hwnd, 0);

            break;
        }
    }
    return 0;
}