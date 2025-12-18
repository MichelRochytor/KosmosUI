#include "kosmos.h"
#include "resource.h"

HINSTANCE hInst;
HWND carregando_barra;
HWND botao_exemplo;
HWND upar;
HWND idc_confirmar;
HWND list_registradores;
HWND list_arquivos;
HWND caixa_texto;
HWND label_pasta;
HWND compilar;
HWND idc_console;
HWND excluir;
HWND labelnome_arquivo;
HWND label_registradores;
HWND logo;
HWND fundo;
HFONT fonte3;
HWND labelspasta;
HWND salvar;
HWND list_selecao;
HWND edit_resultado;
HWND janelaCriar;
HWND edit_criar;
HBRUSH hBrush;
HDC hdc;
HWND telaConsole;
HFONT fonte_listbox;
HFONT fonte_pasta;
MINMAXINFO* tamanho;
DRAWITEMSTRUCT* dis;
BROWSEINFOW bi;  // Mudado para BROWSEINFOW
LONG style;
int janela1, janela2;
WCHAR path[5000];  // Mudado para WCHAR
int selecionado;
LPCWSTR nome_pasta;  // Mudado para LPCWSTR
WCHAR pasta[5000];   // Mudado para WCHAR
WCHAR arquivoselecionado[150];  // Mudado para WCHAR

HICON hIcon;

void Esconder(HWND objeto) {
    ShowWindow(objeto,SW_HIDE);
}
void Mostrar(HWND objeto) {
    ShowWindow(objeto,SW_SHOW);
}

// Remove this duplicate definition of OrganizarListaArquivos to fix redefinition error.
void OrganizarListaArquivos(HWND hwnd,HWND list_arquivos,LPARAM lParam) {
	HWND label_pasta = GetDlgItem(hwnd, LABELPASTA);
	WCHAR exibir_formatado[50] =  L"  > " ;
	wcscat(exibir_formatado, nome_pasta);
	Mostrar(label_pasta);
	Mostrar(list_arquivos);
	SendMessage(label_pasta, WM_SETTEXT,0, (LPARAM)exibir_formatado);
}

void CarregarObjetos(HWND hwnd) {
    list_arquivos = GetDlgItem(hwnd, LISTAARQUIVO);
	//salvar = GetDlgItem(hwnd, SALVAR);
	caixa_texto = GetDlgItem(hwnd, CAIXADETEXTO);
	label_pasta = GetDlgItem(hwnd, LABELPASTA);
	labelnome_arquivo = GetDlgItem(hwnd, LABELNOME_ARQUIVO);
	//label_registradores = GetDlgItem(hwnd, LABEL_REGISTRADORES);
}

void AdicionarFonte() {  // Removido o 'B' errante
    fonte_listbox = CreateFontW(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS
        , CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Courier New");  // Usar CreateFontW e L""
    
    fonte_pasta = CreateFontW(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS
        , CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Courier New");  // Usar CreateFontW e L""
    
    fonte3 = CreateFontW(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS
        , CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Courier New");  // Usar CreateFontW e L""
}

void ColocarFontes() {
    SendMessageW(list_registradores, WM_SETFONT, (WPARAM)fonte_listbox, TRUE);  // Usar SendMessageW
    SendMessageW(label_pasta, WM_SETFONT, (WPARAM)fonte_pasta, TRUE);
    SendMessageW(list_arquivos, WM_SETFONT, (WPARAM)fonte_listbox, TRUE);
    SendMessageW(caixa_texto, WM_SETFONT, (WPARAM)fonte_listbox, TRUE);
    SendMessageW(list_selecao, WM_SETFONT, (WPARAM)fonte_listbox, TRUE);
    SendMessageW(labelnome_arquivo, WM_SETFONT, (WPARAM)fonte_listbox, TRUE);
}

void list(LPCWSTR pasta) {  // Mudado para LPCWSTR
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

void ApresentacaoPrincipal(){
    Esconder(label_pasta);
    Esconder(caixa_texto);
    Esconder(list_arquivos);
    Esconder(labelnome_arquivo);
}