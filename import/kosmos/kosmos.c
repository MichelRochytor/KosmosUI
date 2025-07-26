#include "kosmos.h"
#include <string.h>

//EstiloVisual g_estilo;

void ConfigurarDPI() {
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
}

KFONT CriarFontePersonalizada(const wchar_t* nomeFonte, int tamanho, int peso) {
    return CreateFontW(
        -MulDiv(tamanho, GetDpiForSystem(), 96), 0, 0, 0, peso,
        FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, nomeFonte
    );
}

HWND KCreateWindow(HINSTANCE instancia, int idDialogo, KosmosWindowProc procedimento) {
    ConfigurarDPI();
    return CreateDialogParam(instancia, MAKEINTRESOURCE(idDialogo), NULL, procedimento, 0);
}

void LoopMsg(HWND dialogo) {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsWindow(dialogo) || !IsDialogMessage(dialogo, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void EndKosmos(HWND dialogo) {
    if (IsWindow(dialogo)) {
        EndDialog(dialogo, 0);
        PostQuitMessage(0);
    }
}

void DefinirFonteDialogo(HWND dialogo, HFONT fonte) {
    SendMessage(dialogo, WM_SETFONT, (WPARAM)fonte, TRUE);
}

void ConfigurarMenuOwnerDraw(HWND hwnd, HMENU menu, const EstiloVisual* estilo) {
    MENUINFO mi = { sizeof(MENUINFO) };
    mi.fMask = MIM_BACKGROUND | MIM_APPLYTOSUBMENUS;
    mi.hbrBack = CreateSolidBrush(estilo->corFundo);
    SetMenuInfo(menu, &mi);

    int count = GetMenuItemCount(menu);
    for (int i = 0; i < count; i++) {
        MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
        wchar_t text[256];
        mii.fMask = MIIM_STRING | MIIM_SUBMENU;
        mii.dwTypeData = text;
        mii.cch = ARRAYSIZE(text);
        GetMenuItemInfo(menu, i, TRUE, &mii);

        DadosItemMenu* pData = (DadosItemMenu*)malloc(sizeof(DadosItemMenu));
        wcscpy_s(pData->texto, ARRAYSIZE(pData->texto), text);
        pData->icone = NULL;

        mii.fMask = MIIM_FTYPE | MIIM_DATA;
        mii.fType = MFT_OWNERDRAW;
        mii.dwItemData = (ULONG_PTR)pData;
        SetMenuItemInfo(menu, i, TRUE, &mii);

        if (mii.hSubMenu) {
            ConfigurarMenuOwnerDraw(hwnd, mii.hSubMenu, estilo);
        }
    }
}

INT_PTR TratarMensagemMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, const EstiloVisual* estilo) {
    switch (msg) {
    case WM_MEASUREITEM: {
        MEASUREITEMSTRUCT* mis = (MEASUREITEMSTRUCT*)lParam;
        if (mis->CtlType == ODT_MENU) {
            DadosItemMenu* pData = (DadosItemMenu*)mis->itemData;
            if (pData) {
                HDC hdc = GetDC(hwnd);
                HFONT hFont = CriarFontePersonalizada(estilo->fonteNome, estilo->fonteTamanho, estilo->fontePeso);
                HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

                SIZE size;
                GetTextExtentPoint32(hdc, pData->texto, (int)wcslen(pData->texto), &size);
                mis->itemWidth = size.cx + 40;
                mis->itemHeight = size.cy + 10;

                SelectObject(hdc, hOldFont);
                DeleteObject(hFont);
                ReleaseDC(hwnd, hdc);
            }
        }
        return TRUE;
    }

    case WM_DRAWITEM: {
        DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;
        if (dis->CtlType == ODT_MENU) {
            DadosItemMenu* pData = (DadosItemMenu*)dis->itemData;
            if (pData) {
                COLORREF bgColor = (dis->itemState & ODS_SELECTED) ? estilo->corDestaque :
                    ((dis->itemState & ODS_DISABLED) ? estilo->corFundo : estilo->corFundo);
                COLORREF textColor = (dis->itemState & ODS_DISABLED) ? estilo->corDesativado : estilo->corTexto;

                HBRUSH hBrush = CreateSolidBrush(bgColor);
                FillRect(dis->hDC, &dis->rcItem, hBrush);
                DeleteObject(hBrush);

                HFONT hFont = CriarFontePersonalizada(estilo->fonteNome, estilo->fonteTamanho, estilo->fontePeso);
                HFONT hOldFont = (HFONT)SelectObject(dis->hDC, hFont);

                SetBkMode(dis->hDC, TRANSPARENT);
                SetTextColor(dis->hDC, textColor);

                RECT rcText = dis->rcItem;
                rcText.left += 20;
                DrawText(dis->hDC, pData->texto, -1, &rcText, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

                SelectObject(dis->hDC, hOldFont);
                DeleteObject(hFont);
            }
        }
        return TRUE;
    }

    case WM_MENUCHAR:
        return MNC_CLOSE << 16;

    case WM_DESTROY: {
        HMENU hMenu = GetMenu(hwnd);
        if (hMenu) {
            int count = GetMenuItemCount(hMenu);
            for (int i = 0; i < count; i++) {
                MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
                mii.fMask = MIIM_DATA;
                if (GetMenuItemInfo(hMenu, i, TRUE, &mii)) {
                    free((void*)mii.dwItemData);
                }
            }
        }
        break;
    }
    }
    return FALSE;
}