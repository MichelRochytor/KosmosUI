#ifndef KOSMOS_H
#define KOSMOS_H

#include <windows.h>
#include <commctrl.h>
#include <ShellScalingApi.h>

// Estruturas
typedef struct {
    wchar_t texto[256];
    HBITMAP icone;
} DadosItemMenu;

typedef struct {
    COLORREF corFundo;
    COLORREF corTexto;
    COLORREF corDestaque;
    COLORREF corDesativado;
    wchar_t fonteNome[32];
    int fonteTamanho;
    int fontePeso;
} EstiloVisual;

// Definições de tipos
typedef INT_PTR(CALLBACK* KosmosWindowProc)(HWND, UINT, WPARAM, LPARAM);
typedef HWND KWINDOW;
typedef HFONT KFONT;

// Macros
#define KosmosWindow(nome) INT_PTR CALLBACK nome(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
#define KosmosMain() int WINAPI wWinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPWSTR pCmdLine,int nCmdShow)
#define WINDOW_INIT WM_INITDIALOG

// Protótipos
void ConfigurarDPI();
KFONT CriarFontePersonalizada(const wchar_t* nomeFonte, int tamanho, int peso);
HWND KCreateWindow(HINSTANCE instancia, int idDialogo, KosmosWindowProc procedimento);
void LoopMsg(HWND dialogo);
void DefinirFonteDialogo(HWND dialogo, HFONT fonte);
void ConfigurarMenuOwnerDraw(HWND hwnd, HMENU menu, const EstiloVisual* estilo);
INT_PTR TratarMensagemMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, const EstiloVisual* estilo);
void EndKosmos(HWND dialogo);


#endif // KOSMOS_H