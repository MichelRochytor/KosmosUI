#ifndef KOSMOS_H
#define KOSMOS_H

#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <mmsystem.h>
#include <wingdi.h>

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
typedef INT_PTR (CALLBACK* KosmosWindowProc)(HWND, UINT, WPARAM, LPARAM);

typedef HWND KWidget;
typedef UINT KEvent;
typedef WPARAM Kid;
typedef LPARAM KData;
typedef INT_PTR KResult;

typedef HFONT KFONT;

#define kcontroller INT_PTR CALLBACK
#define kGetWidget(janela, id) GetDlgItem(janela, id)

#define MsgWindow() switch (msg_param)
// Macros
#define KosmosWindow(nome) INT_PTR CALLBACK nome(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
#define KosmosMain() int WINAPI wWinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPWSTR pCmdLine,int nCmdShow)

#define kevents(msg) switch(msg)
// --- EXTRAÇÃO DE DADOS (Substituindo HIWORD e LOWORD) ---
// Retorna o ID do componente (Botão, Menu, Lista)
#define kGetId(dado)     LOWORD(dado)
// Retorna o tipo de ação (ex: clique duplo, mudança de seleção)
#define kGetAction(dado) HIWORD(dado)

#define KInit          WM_INITDIALOG
#define KCommand       WM_COMMAND
#define KResize        WM_SIZE
#define KResizing      WM_SIZING
#define KPreResize     WM_WINDOWPOSCHANGING
#define KErase         WM_ERASEBKGND
#define KPaint         WM_PAINT
#define KClose         WM_CLOSE 
#define KDestroy       WM_DESTROY

#define KColors        WM_CTLCOLORBTN: \
                       WM_CTLCOLORLISTBOX: \
                       WM_CTLCOLORDLG: \
                       WM_CTLCOLOREDIT: \
                       WM_CTLCOLORSTATIC

#define WINDOW_INIT 1
#define KOSMOS_COMMAND 2

// kosmos.h

// Altera o título principal (o texto grande em negrito) do Command Link
void kCommandLinkSetTitle(KWidget widget, const WCHAR* titulo);

// Altera a nota secundária (o texto explicativo menor de baixo) do Command Link
void kCommandLinkSetNote(KWidget widget, const WCHAR* nota);

// Protótipos
void kSetWidgetFont(KWidget widget, const WCHAR* fontName, int size, BOOL bBold, BOOL bItalic, BOOL bUnderline);
BOOL KCustomFont(const WCHAR* fontPath);
int kGetText(HWND widget, LPWSTR buffer, int tamanho);
WINBOOL kSetText(HWND widget, LPCWSTR texto);

void kComboAdd(KWidget combo, const WCHAR* texto);
void kComboClear(KWidget combo);
int kComboGetIndex(KWidget combo);
void kComboGetText(KWidget combo, WCHAR* buffer);

void ConfigurarDPI();
KFONT CriarFontePersonalizada(const wchar_t* nomeFonte, int tamanho, int peso);
HWND KCreateWindow(HINSTANCE instancia, int idDialogo, KosmosWindowProc procedimento);
void LoopMsg(HWND dialogo);
void DefinirFonteDialogo(HWND dialogo, HFONT fonte);
void ConfigurarMenuOwnerDraw(HWND hwnd, HMENU menu, const EstiloVisual* estilo);
INT_PTR TratarMensagemMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, const EstiloVisual* estilo);
void EndKosmos(HWND dialogo);


#endif // KOSMOS_H