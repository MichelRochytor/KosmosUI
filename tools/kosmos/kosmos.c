#include "kosmos.h"

typedef enum KOSMOS_DPI_AWARENESS {
    KOSMOS_DPI_UNAWARE = 0,
    KOSMOS_SYSTEM_DPI_AWARE = 1,
    KOSMOS_PER_MONITOR_DPI_AWARE = 2
} KOSMOS_DPI_AWARENESS;

// Define um "Tipo de Função" (Ponteiro) que aceita um argumento do tipo enum acima
// e retorna um HRESULT. Isso serve para o compilador saber como chamar a função.
typedef HRESULT (STDAPICALLTYPE *SetProcessDpiAwarenessProc)(KOSMOS_DPI_AWARENESS);

WINBOOL kSetText(HWND widget, LPCWSTR texto) {
    return SetWindowTextW(widget, texto);
}

int kGetText(HWND widget, LPWSTR buffer, int tamanho) {
    return GetWindowTextW(widget, buffer, tamanho);
}

void KosmosListFiles(LPCWSTR pasta, HWND list_arquivos, wchar_t path[260]) {  // Mudado para LPCWSTR
    WIN32_FIND_DATAW FindFileData;  // Usar WIN32_FIND_DATAW
    HANDLE hfind;

    WCHAR formatar_arq[60] = L"   >  ";  // Mudado para WCHAR
    wcscpy(path, pasta);
    wcscat(path, L"\\*.*");
    hfind = FindFirstFileW(path, &FindFileData);  // Usar FindFirstFileW

    if (hfind != INVALID_HANDLE_VALUE) {
        do {
            const WCHAR* name = FindFileData.cFileName;  // Mudado para WCHAR
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                const WCHAR* ext = wcsrchr(FindFileData.cFileName, L'.');  // Usar wcsrchr
                if (ext != NULL && wcscmp(ext, L".s") == 0) {  // Usar wcscmp
                    wcscat(formatar_arq, name);
                    SendMessageW(list_arquivos, LB_ADDSTRING, 0, (LPARAM)formatar_arq);  // Usar SendMessageW
                    wcscpy(formatar_arq, L"   >  ");
                }
            }
        } while (FindNextFileW(hfind, &FindFileData) != 0);  // Usar FindNextFileW
        FindClose(hfind);
    }
}

void ConfigurarDPI() {
    // 2. TENTA CARREGAR A DLL (LoadLibrary)
    // Tenta abrir a "Shcore.dll" (presente no Windows 8.1, 10, 11).
    // Se estiver no Windows 7, isso retorna NULL e o programa não trava.
    HMODULE hShcore = LoadLibraryW(L"Shcore.dll");

    if (hShcore) {
        // 3. BUSCA A FUNÇÃO (GetProcAddress)
        // Procura pelo nome exato da função dentro da DLL.
        // Nota: GetProcAddress sempre usa nomes em ANSI (texto normal), não Unicode.
        SetProcessDpiAwarenessProc setDpiAware = 
            (SetProcessDpiAwarenessProc)GetProcAddress(hShcore, "SetProcessDpiAwareness");

        // 4. EXECUTA A FUNÇÃO
        if (setDpiAware) {
            setDpiAware(KOSMOS_PER_MONITOR_DPI_AWARE);
        }

        // 5. LIBERA A MEMÓRIA
        // Como já configuramos, não precisamos mais da DLL aberta.
        FreeLibrary(hShcore);
    } 
    else {
        // --- FALLBACK PARA WINDOWS 7 / VISTA ---
        // Se a Shcore.dll não existir, tentamos a função antiga da user32.dll
        HMODULE hUser32 = LoadLibraryW(L"user32.dll");
        if (hUser32) {
            // Define o tipo da função antiga (SetProcessDPIAware) que não recebe argumentos
            typedef BOOL (STDAPICALLTYPE *SetProcessDPIAwareProc)(void);
            
            SetProcessDPIAwareProc setDpi = 
                (SetProcessDPIAwareProc)GetProcAddress(hUser32, "SetProcessDPIAware");
            
            if (setDpi) {
                setDpi();
            }
            FreeLibrary(hUser32);
        }
    }
}

// Adicione no kosmos.h / kosmos.c
void kSetWidgetFont(KWidget widget, const WCHAR* fontName, int size, BOOL bBold, BOOL bItalic, BOOL bUnderline) {
    
    // Define o peso da fonte baseado no parâmetro bBold
    int fontWeight = bBold ? FW_BOLD : FW_NORMAL;

    HFONT hFont = CreateFontW(
        size,                   // Altura (Tamanho)
        0, 0, 0, 
        fontWeight,             // Peso da fonte (Negrito ou Normal)
        bItalic,                // Itálico (TRUE/FALSE)
        bUnderline,             // Sublinhado (TRUE/FALSE)
        FALSE,                  // Riscado (Strikeout)
        DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,      // Garante que a fonte fique lisinha e sem pixels estourados
        DEFAULT_PITCH | FF_DONTCARE, 
        fontName                // Nome da fonte (Ex: L"Audiowide")
    );
    
    if (hFont != NULL) {
        SendMessageW(widget, WM_SETFONT, (WPARAM)hFont, TRUE);
    }
}

void kComboAdd(KWidget combo, const WCHAR* texto) {
    SendMessageW(combo, CB_ADDSTRING, 0, (LPARAM)texto);
}

void kComboClear(KWidget combo) {
    SendMessageW(combo, CB_RESETCONTENT, 0, 0);
}

int kComboGetIndex(KWidget combo) {
    return (int)SendMessageW(combo, CB_GETCURSEL, 0, 0);
}

void kCommandLinkSetTitle(KWidget widget, const WCHAR* titulo) {
    if (widget != NULL) {
        SetWindowTextW(widget, titulo);
    }
}

void kCommandLinkSetNote(KWidget widget, const WCHAR* nota) {
    if (widget != NULL) {
        SendMessageW(widget, BCM_SETNOTE, 0, (LPARAM)nota);
    }
}

void kComboGetText(KWidget combo, WCHAR* buffer) {
    int index = kComboGetIndex(combo);
    if (index != CB_ERR) {
        SendMessageW(combo, CB_GETLBTEXT, (WPARAM)index, (LPARAM)buffer);
    } else {
        buffer[0] = L'\0'; // Retorna string vazia se não houver seleção
    }
}

BOOL KCustomFont(const WCHAR* fontPath) {
    // Carrega a fonte de forma privada para o processo atual
    int result = AddFontResourceExW(fontPath, FR_PRIVATE, NULL);
    
    if (result > 0) {
        // Avisa todas as janelas do sistema que uma nova fonte está disponível
        PostMessageW(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
        return TRUE;
    }
    return FALSE;
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
    return CreateDialogW(instancia, MAKEINTRESOURCE(idDialogo), NULL, (DLGPROC)procedimento);
}

void LoopMsg(HWND dialogo) {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void EndKosmos(HWND dialogo) {
    EndDialog(dialogo, 0);
    PostQuitMessage(117);
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
