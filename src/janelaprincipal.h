#include "kosmos.h"
#include "resource.h"
#include "auxinterface.h"
#include "base.h"


#define LARGURA_MINIMA 1000
#define ALTURA_MINIMA  500

HBRUSH g_hBrushFundo; 
KosmosWindow(JanelaPrincipal){
    switch (msg_param) {
        case WM_INITDIALOG:{
            // 2. Aplicar FIX de renderização (WS_CLIPCHILDREN)
            LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
            SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CLIPCHILDREN);
            g_hBrushFundo = CreateSolidBrush(RGB(24, 24, 24)); 
            AdicionarFonte();
            CarregarObjetos(hwnd);
            ColocarFontes();
            ApresentacaoPrincipal();
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;
        }
        return TRUE;
        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam;
            RECT rc;
            GetClientRect(hwnd, &rc);
            FillRect(hdc, &rc, g_hBrushFundo);
            return 1; // Retorna 1 = "Eu já pintei, não faça nada padrão"
        }
        case WM_CTLCOLORBTN:{
            hdc = (HDC)wParam;
            SetBkColor(hdc, RGB(31, 31, 31));
            return (INT_PTR)CreateSolidBrush(RGB(31, 31, 31));
            break;
        }
        return TRUE;
        case WM_CTLCOLORLISTBOX:{
            hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(204, 204, 193));
            SetBkColor(hdc, RGB(31, 31, 31));
            return (INT_PTR)CreateSolidBrush(RGB(31, 31, 31));
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
            SetBkColor(hdc, RGB(0, 0, 0));
            return (INT_PTR)CreateSolidBrush(RGB(0, 0, 0));
            break;
        }
        return TRUE;
        case WM_CTLCOLORSTATIC:{
            hdc = (HDC)wParam;
            if ((HWND)lParam == label_pasta) {
                SetBkColor(hdc, RGB(0, 200, 0));
                SetTextColor(hdc, RGB(255, 255, 255));
                //HRGN hrgn = CreateRoundRectRgn(0, 0, 230, 25, 20, 20);

                // Seleciona a regi�o no dispositivo de contexto
                //SelectClipRgn(hdc, hrgn);
                return (INT_PTR)CreateSolidBrush(RGB(0, 200, 0));
            }
            if ((HWND)lParam == labelnome_arquivo) {
                //HRGN hrgn = CreateRoundRectRgn(0, 0, janela1 - 345, 50, 30, 30);

                // Seleciona a regi�o no dispositivo de contexto
                //SelectClipRgn(hdc, hrgn);
                SetBkColor(hdc, RGB(0, 200, 0));
                SetTextColor(hdc, RGB(255, 255, 255));
                return (INT_PTR)CreateSolidBrush(RGB(0,200,0));
                
            }else{
                SetTextColor(hdc, RGB(255, 255, 255));
                SetBkColor(hdc, RGB(32, 32, 32));
                return (INT_PTR)CreateSolidBrush(RGB(32, 32, 32));
            }
            break;

        }
        return TRUE;
        case WM_SIZE:{
            janela1 = LOWORD(lParam);
            janela2 = HIWORD(lParam);
            SetWindowPos(label_registradores, HWND_BOTTOM, janela1 - 150, 0, 150, 25, SWP_NOSENDCHANGING);
            SetWindowPos(labelnome_arquivo, HWND_BOTTOM, 270, 30, janela1 - 345, 50, SWP_NOSENDCHANGING);
            SetWindowPos(label_pasta, HWND_BOTTOM, 0, 0, 250, 25, SWP_NOZORDER);
            SetWindowPos(list_arquivos, HWND_BOTTOM, 0, 25, 250, janela2 - 25, SWP_NOZORDER);
            SetWindowPos(caixa_texto, HWND_BOTTOM, 270, 100, janela1 - 345, janela2 - 125, SWP_NOZORDER);
            SetWindowPos(salvar, NULL, 300 + janela1 * 0.165, 30, 75, 35, SWP_NOZORDER);
            SetWindowPos(list_registradores, HWND_BOTTOM, janela1 - 150, 25, janela1 - 150, janela2 - 25, SWP_NOZORDER);
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;
        }
        return TRUE;
        case WM_GETMINMAXINFO:{
            tamanho = (MINMAXINFO*)lParam;
            tamanho->ptMinTrackSize.x = 1000;
            tamanho->ptMinTrackSize.y = 500;
            break;
        }
        return TRUE;
        case WM_COMMAND:{
            switch (LOWORD(wParam)) {
                case ID_ABRIRPASTA:{
                    SendMessage(list_arquivos, LB_RESETCONTENT, 0, 0);
                    bi.lpszTitle = L"Selecione uma pasta ";
                    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
                    if (pidl != NULL) {
                        SHGetPathFromIDList(pidl, pasta);
                        SHGetPathFromIDList(pidl, diretoriocriar);
                        list(pasta);
                        nome_pasta = PathFindFileName(pasta);
                        OrganizarListaArquivos(hwnd, list_arquivos, lParam);
                    }
                    break;
			    }
                case LISTAARQUIVO:{
                    if (HIWORD(wParam) == LBN_SELCHANGE) {
                        WCHAR arquivoselecionado[150];
                        selecionado = SendMessage(list_arquivos, LB_GETCURSEL, 0, 0);
                        SendMessage(list_arquivos, LB_GETTEXT, selecionado, (LPARAM)arquivoselecionado);
                        if (selecionado != LB_ERR) {
                            SendMessage(labelnome_arquivo, WM_SETTEXT, 0, (LPARAM)arquivoselecionado);
                            Diretorio(pasta, arquivoselecionado);
                            carregarArquivo(hwnd);
                            Mostrar(caixa_texto);
                            Mostrar(labelnome_arquivo);
                        }
                    }
                    break;
                }
                case ID_CRIARARQUIVO:{
                    ShowWindow(janelaCriar, 1);
                    break;
                }
                case ID_SALVARARQUIVO:{
                    SalvarArquivo();
                    break;
                }

                case ID_COMPILAR:{
                    Compilar(hwnd);

                    STARTUPINFO si;
                    PROCESS_INFORMATION pi;
                    ZeroMemory(&si, sizeof(si));
                    si.cb = sizeof(si);
                    ZeroMemory(&pi, sizeof(pi));

                    // Substitua "Caminho\\para\\seu\\programa.exe" pelo caminho do seu execut�vel
                    if (!CreateProcess(L"compilar\\output\\compilar.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                        return 1;
                    }

                    // Aguarde at� que o processo filho termine.
                    WaitForSingleObject(pi.hProcess, INFINITE);
                    // Fecha as al�as do processo e do thread ap�s o t�rmino do processo filho.
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                    ShowWindow(telaConsole, 1);
                    break;
                }
                case ID_COMPILARPASSOS:{
                    Compilar(hwnd);
                    STARTUPINFO si;
                    PROCESS_INFORMATION pi;
                    ZeroMemory(&si, sizeof(si));
                    si.cb = sizeof(si);
                    ZeroMemory(&pi, sizeof(pi));

                    // Substitua "Caminho\\para\\seu\\programa.exe" pelo caminho do seu execut�vel
                    if (!CreateProcess(L"compilar\\output\\compilarPasso.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                        return 1;
                    }

                    // Aguarde at� que o processo filho termine.
                    WaitForSingleObject(pi.hProcess, INFINITE);
                    // Fecha as al�as do processo e do thread ap�s o t�rmino do processo filho.
                    CloseHandle(pi.hProcess);
                    CloseHandle(pi.hThread);
                    lerResultadoRegistradores(hwnd);
                    ShowWindow(telaConsole, 1);
                    break;
                }
            }
            break;
        }

        case KOSMOS_CLOSE:{
            EndKosmos(hwnd);
        }
        return TRUE;

        case WM_DESTROY:{
		    PostQuitMessage(117);
		    break;
        }
        return TRUE;
    
    }
    return FALSE;
}