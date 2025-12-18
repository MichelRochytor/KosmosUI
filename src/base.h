#include "kosmos.h"
#include <stdio.h>
#include <stdlib.h>

WCHAR diretoriocriar[1000];
WCHAR diretorio[1000];
WCHAR nome_arquivo[50];

void lerResultadoRegistradores(HWND hwnd) {
    long tamanhoArquivo;
    char* codigo_texto;
    int i = 0;
    int letra;

    FILE* arquivo = fopen("registradores.txt", "r");
    if (arquivo == NULL) {
        MessageBoxW(hwnd, L"Erro ao abrir arquivo registradores.txt", L"Erro", MB_OK);
        return;
    }

    fseek(arquivo, 0, SEEK_END);
    tamanhoArquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    codigo_texto = (char*)malloc((2 * tamanhoArquivo + 1) * sizeof(char));
    while ((letra = fgetc(arquivo)) != EOF && i < 2 * tamanhoArquivo) {
        codigo_texto[i++] = letra;
    }
    codigo_texto[i] = '\0';
    fclose(arquivo);
    
    // Converter para wide char se necessário
    int wide_len = MultiByteToWideChar(CP_UTF8, 0, codigo_texto, -1, NULL, 0);
    WCHAR* wide_text = (WCHAR*)malloc(wide_len * sizeof(WCHAR));
    MultiByteToWideChar(CP_UTF8, 0, codigo_texto, -1, wide_text, wide_len);
    
    //SetWindowTextW(edit_resultado, wide_text);
    free(codigo_texto);
    free(wide_text);
}

void criarArquivo(HWND hwnd) {
    if (wcslen(diretoriocriar) > 0) {
        wcscat(diretoriocriar, L"\\");
        wcscat(diretoriocriar, nome_arquivo);
        FILE* file = _wfopen(diretoriocriar, L"w");
        if (file == NULL) {
            MessageBoxW(hwnd, L"Erro ao criar arquivo!", L"Erro", MB_OK);
            return;
        }
        fclose(file);
        MessageBoxW(hwnd, L"Arquivo criado com sucesso!", L"Sucesso", MB_OK);
    } else {
        MessageBoxW(hwnd, L"Diretório inválido!", L"Erro", MB_OK);
    }
}

void carregarArquivo(HWND hwnd) {
    long tamanhoArquivo;
    char *codigo_texto;
    int i = 0;
    int letra;

    FILE* arquivo = _wfopen(diretorio, L"r");
    if (arquivo == NULL) {
        MessageBoxW(hwnd, L"Erro ao abrir o arquivo!", L"Erro", MB_OK);
        return;
    }

    fseek(arquivo, 0, SEEK_END);
    tamanhoArquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    codigo_texto = (char*)malloc((2 * tamanhoArquivo + 1) * sizeof(char));
    while ((letra = fgetc(arquivo)) != EOF && i < 2 * tamanhoArquivo) {
        if (letra == '\n') {
            codigo_texto[i++] = '\r';
            codigo_texto[i++] = '\n';
        } else {
            codigo_texto[i++] = letra;
        }
    }
    codigo_texto[i] = '\0';
    fclose(arquivo);
    
    // Converter para wide char
    int wide_len = MultiByteToWideChar(CP_UTF8, 0, codigo_texto, -1, NULL, 0);
    WCHAR* wide_text = (WCHAR*)malloc(wide_len * sizeof(WCHAR));
    MultiByteToWideChar(CP_UTF8, 0, codigo_texto, -1, wide_text, wide_len);
    
    //SetWindowTextW(caixa_texto, wide_text);
    free(codigo_texto);
    free(wide_text);
}

void tirarEspaco(WCHAR arquivoselecionado[]) {
    int x = 0;
    int y = 0;
    WCHAR novoarquivo[30];
    while (arquivoselecionado[x] != L'\0') {
        if (x > 5 && x <= wcslen(arquivoselecionado)) {
            novoarquivo[y] = arquivoselecionado[x];
            y++;
            novoarquivo[y] = L'\0';
        }
        x++;
    }
    wcscpy(arquivoselecionado, novoarquivo);
}

void Diretorio(LPCWSTR pasta, WCHAR arquivoselecionado[]) {
    tirarEspaco(arquivoselecionado);
    wcscpy(diretorio, L"");
    wcscpy(diretorio, pasta);
    wcscat(diretorio, L"\\");
    wcscat(diretorio, arquivoselecionado);
}

void SalvarArquivo() {
    int tamanhoArquivo = GetWindowTextLengthW(caixa_texto);
    WCHAR* salvartext = (WCHAR*)malloc((tamanhoArquivo + 1) * sizeof(WCHAR));
    GetWindowTextW(caixa_texto, salvartext, tamanhoArquivo + 1);

    FILE* arquivo = _wfopen(diretorio, L"w");
    if (arquivo) {
         //Converter wide char para UTF-8 para salvar
        int utf8_len = WideCharToMultiByte(CP_UTF8, 0, salvartext, -1, NULL, 0, NULL, NULL);
        char* utf8_text = (char*)malloc(utf8_len);
        WideCharToMultiByte(CP_UTF8, 0, salvartext, -1, utf8_text, utf8_len, NULL, NULL);
        
        fputs(utf8_text, arquivo);
        fclose(arquivo);
        free(utf8_text);
    }
    free(salvartext);
}

void Compilar(HWND hwnd) {
    SalvarArquivo();
    FILE* arquivo = fopen("output\\diretorio.dat", "wb");
    if (arquivo) {
        // Salvar diretório como UTF-8
        int utf8_len = WideCharToMultiByte(CP_UTF8, 0, diretorio, -1, NULL, 0, NULL, NULL);
        char* utf8_dir = (char*)malloc(utf8_len);
        WideCharToMultiByte(CP_UTF8, 0, diretorio, -1, utf8_dir, utf8_len, NULL, NULL);
        
        fwrite(utf8_dir, sizeof(char), utf8_len, arquivo);
        fclose(arquivo);
        free(utf8_dir);
    }
}