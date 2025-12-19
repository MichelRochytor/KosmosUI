# Kosmos Framework 🌌

Um framework leve, moderno e multiplataforma em C puro para desenvolvimento de interfaces nativas.

O Kosmos é uma camada de abstração (wrapper) sobre a WinAPI, projetada para remover a verbosidade e complexidade do desenvolvimento nativo no Windows, mantendo a performance extrema e o tamanho minúsculo dos executáveis em C.

Além de simplificar a WinAPI, o Kosmos oferece um ecossistema de build moderno que permite compilar e empacotar aplicações para Linux (via AppImage com Wine embutido) diretamente do seu ambiente de desenvolvimento.

✨ Funcionalidades

API Simplificada: Crie janelas e trate eventos com poucas linhas de código, sem WNDCLASS, RegisterClass ou loops de mensagens manuais.

Visual Moderno: Suporte nativo a High DPI (4K), temas visuais do Windows 10/11 (Flat Design) e renderização de fontes ClearType.

Editor de Código Embutido: Controles de edição de texto otimizados com suporte a indentação (Tab) e fontes monoespaçadas.

Renderização Híbrida: Suporte fácil para integração com OpenGL para desenhar gráficos de alta performance dentro de janelas nativas.

Cross-Platform Build:

Gera .exe nativo para Windows.

Gera .AppImage autossuficiente para Linux (com Wine e configurações de tema embutidos).

Zero Dependências: O núcleo é apenas C e bibliotecas do sistema (User32, GDI32).

🛠️ Instalação e Configuração

Pré-requisitos

Windows:

Visual Studio Code.

MSYS2 (MinGW-w64) instalado em C:\msys64.

Linux:

Visual Studio Code.

Pacotes: mingw-w64, wine64, wget, squashfs-tools.

Instalação Automática

Baixe o repositório e execute o script de configuração:

# No terminal (Windows ou Linux)
python setup_kosmos.py


Isso irá verificar as dependências, baixar as ferramentas necessárias (como o Runtime do AppImage) e configurar o VS Code.

🚀 Como Usar

1. Criar um Novo Projeto

Use a nossa ferramenta de linha de comando para gerar a estrutura:

kosmos -create MeuApp
cd MeuApp
code .


2. Exemplo: Hello World (src/main.c)

#include "kosmos.h"
#include "resource.h"

// Função de Callback da Janela
KosmosWindow(JanelaPrincipal) {
    switch (msg) {
        case WINDOW_INIT:
            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK) {
                MessageBox(hDlg, L"Olá, Universo!", L"Kosmos", MB_OK);
            }
            return TRUE;

        case WM_CLOSE:
            EndKosmos(hDlg); // Fecha a aplicação corretamente
            break;
    }
    return FALSE;
}

// Ponto de Entrada
KosmosMain() {
    // Inicializa controles modernos
    InitCommonControlsEx(&(INITCOMMONCONTROLSEX){sizeof(INITCOMMONCONTROLSEX), ICC_STANDARD_CLASSES});

    KWINDOW janela = KCreateWindow(hInst, IDD_DIALOG1, JanelaPrincipal);
    ShowWindow(janela, 1);
    LoopMsg(janela);
    return 0;
}


3. Compilar e Rodar

No VS Code, pressione Ctrl+Shift+B.

Windows: Compila e roda o .exe.

Linux: Compila o .exe, gera o .AppImage, configura o Wine silenciosamente e roda o AppImage final.

📦 Estrutura do Projeto

MeuApp/
├── src/
│   └── main.c           # Código da sua aplicação
├── resource/
│   ├── resource.rc      # Definição visual (Botões, Menus)
│   ├── resource.h       # IDs dos controles
│   ├── icon.png         # Ícone do App
│   ├── fonts/           # Fontes .ttf (Opcional, para embutir no Linux)
│   └── icons/           # Ícones de sistema .ico (Opcional)
├── tools/
│   ├── kosmos/          # Código fonte do Framework (kosmos.c, kosmos.h)
│   └── package_linux.sh # Script de geração do AppImage
└── output/              # Onde os binários são gerados


🐧 Suporte Linux (AppImage)

O Kosmos utiliza uma tecnologia inovadora de empacotamento. Ao compilar no Linux, ele cria um AppImage que contém:

O seu executável Windows (.exe).

Uma versão portátil e otimizada do Wine (Staging).

Configurações de Registro para Temas Flat, Fontes Suaves e DPI Awareness.

Isso garante que o seu aplicativo rode em qualquer distribuição Linux (Ubuntu, Fedora, Arch, Steam Deck) com um visual consistente e sem exigir que o usuário configure o Wine manualmente.

🤝 Contribuindo

Contribuições são bem-vindas! Se você tem ideias para melhorar a API, novos widgets ou correções para o script de build:

Faça um Fork do projeto.

Crie uma Branch (git checkout -b feature/NovaFeature).

Commit suas mudanças (git commit -m 'Adiciona suporte a Abas').

Push para a Branch (git push origin feature/NovaFeature).

Abra um Pull Request.

📄 Licença

Este projeto está sob a licença MIT.
Você é livre para usar, modificar e distribuir o Kosmos em projetos pessoais, acadêmicos ou comerciais.

Copyright (c) 2025 [Seu Nome Aqui].
