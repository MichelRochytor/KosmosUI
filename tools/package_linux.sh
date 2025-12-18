#!/bin/bash

# ================= CONFIGURAÇÃO =================
APP_NAME="KosmosApp"
EXE_PATH="output/MeuApp.exe"
ICON_PATH="resource/icon.png"
OUTPUT_DIR="output/linux"
APP_DIR="$OUTPUT_DIR/$APP_NAME.AppDir"
# ================================================

# 1. Preparação
rm -rf "$OUTPUT_DIR"
mkdir -p "$APP_DIR/usr/bin"

echo "Copiando binários..."
if [ ! -f "$EXE_PATH" ]; then
    echo "[Erro] O arquivo $EXE_PATH não foi encontrado. Compile primeiro!"
    exit 1
fi
cp "$EXE_PATH" "$APP_DIR/usr/bin/$APP_NAME.exe"

# Ícone
if [ -f "$ICON_PATH" ]; then
    cp "$ICON_PATH" "$APP_DIR/$APP_NAME.png"
else
    wget -q https://upload.wikimedia.org/wikipedia/commons/8/83/Circle-icons-dev.svg -O "$APP_DIR/$APP_NAME.svg"
fi

# 2. Criar o AppRun Inteligente
echo "Criando AppRun (DPI, Fontes, Wine Auto-Install)..."

cat > "$APP_DIR/AppRun" <<EOF
#!/bin/bash

export WINEPREFIX="\$HOME/.wine_kosmos_app"
mkdir -p "\$WINEPREFIX"
export WINEDEBUG=-all

# --- FUNÇÃO: Instalar Wine (se faltar) ---
install_wine() {
    CMD=""
    if command -v apt-get &> /dev/null; then CMD="sudo apt update && sudo apt install -y wine64 wine32";
    elif command -v pacman &> /dev/null; then CMD="sudo pacman -S --noconfirm wine";
    elif command -v dnf &> /dev/null; then CMD="sudo dnf install -y wine";
    elif command -v zypper &> /dev/null; then CMD="sudo zypper install -y wine";
    fi

    for term in gnome-terminal konsole xterm terminator alacritty kitty; do
        if command -v \$term &> /dev/null; then
            \$term -e "bash -c 'echo [Instalador Kosmos]; echo Instalando Wine...; \$CMD; echo Feito. Pressione Enter.; read'"
            return
        fi
    done
}

if ! command -v wine &> /dev/null; then
    if command -v zenity &> /dev/null; then
        zenity --question --text="O Wine é necessário. Instalar?" && install_wine
    elif command -v kdialog &> /dev/null; then
        kdialog --yesno "Wine necessário. Instalar?" && install_wine
    fi
    if ! command -v wine &> /dev/null; then exit 1; fi
fi

# ==============================================================================
#  APLICAR TEMA VISUAL (MODO WINDOWS 10/11)
# ==============================================================================

# Cria um arquivo .reg temporário para aplicar tudo de uma vez
cat > "\$WINEPREFIX/theme.reg" <<REG
REGEDIT4

[HKEY_CURRENT_USER\Control Panel\Desktop]
"LogPixels"=dword:00000078
"FontSmoothing"="2"
"FontSmoothingGamma"=dword:00000000
"FontSmoothingOrientation"=dword:00000001
"FontSmoothingType"=dword:00000002

[HKEY_CURRENT_USER\Control Panel\Colors]
"ActiveBorder"="212 212 212"
"ActiveTitle"="255 255 255"
"AppWorkspace"="255 255 255"
"Background"="0 0 0"
"ButtonAlternateFace"="255 255 255"
"ButtonDkShadow"="160 160 160"
"ButtonFace"="240 240 240"
"ButtonHilight"="255 255 255"
"ButtonLight"="240 240 240"
"ButtonShadow"="160 160 160"
"ButtonText"="0 0 0"
"GradientActiveTitle"="0 120 215"
"GradientInactiveTitle"="240 240 240"
"GrayText"="109 109 109"
"Hilight"="0 120 215"
"HilightText"="255 255 255"
"HotTrackingColor"="0 102 204"
"InactiveBorder"="244 247 252"
"InactiveTitle"="240 240 240"
"InactiveTitleText"="0 0 0"
"InfoText"="0 0 0"
"InfoWindow"="255 255 255"
"Menu"="255 255 255"
"MenuBar"="240 240 240"
"MenuHilight"="0 120 215"
"MenuText"="0 0 0"
"Scrollbar"="240 240 240"
"TitleText"="0 0 0"
"Window"="255 255 255"
"WindowFrame"="160 160 160"
"WindowText"="0 0 0"

[HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\FontSubstitutes]
"MS Shell Dlg"="Segoe UI"
"MS Shell Dlg 2"="Segoe UI"
"Tahoma"="Segoe UI"
REG

# Aplica o registro silenciosamente
wine regedit /S "\$WINEPREFIX/theme.reg" 2> /dev/null

# ==============================================================================

# Roda o App
DIR="\$(dirname "\$(readlink -f "\${0}")")"
wine "\$DIR/usr/bin/$APP_NAME.exe" "\$@"
EOF

chmod +x "$APP_DIR/AppRun"

# 3. Criar .desktop
cat > "$APP_DIR/$APP_NAME.desktop" <<EOF
[Desktop Entry]
Name=$APP_NAME
Exec=AppRun
Icon=$APP_NAME
Type=Application
Categories=Development;
EOF

# 4. Gerar AppImage
if [ ! -f "tools/appimagetool" ]; then
    echo "Baixando appimagetool..."
    wget -q https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage -O tools/appimagetool
    chmod +x tools/appimagetool
fi

echo "Gerando AppImage final..."
ARCH=x86_64 ./tools/appimagetool "$APP_DIR" "$OUTPUT_DIR/$APP_NAME-x86_64.AppImage"

echo "Sucesso! AppImage criado em: $OUTPUT_DIR/$APP_NAME-x86_64.AppImage"