#!/bin/bash

# ================= CONFIGURAÇÃO =================
APP_NAME="KosmosApp"
EXE_PATH="output/MeuApp.exe"
ICON_PATH="resource/icon.png"
OUTPUT_DIR="output/linux"
APP_DIR="$OUTPUT_DIR/$APP_NAME.AppDir"

# Wine 10.0 Staging Portátil (Kron4ek Builds)
WINE_URL="https://github.com/Kron4ek/Wine-Builds/releases/download/10.0/wine-10.0-staging-amd64.tar.xz"

# Runtime "Continuous"
RUNTIME_URL="https://github.com/AppImage/AppImageKit/releases/download/continuous/runtime-x86_64"
# ================================================

# Função para verificar erros
check_error() {
    if [ $? -ne 0 ]; then
        echo "[ERRO] $1"
        exit 1
    fi
}

echo "[1/7] Limpando build anterior..."
rm -rf "$OUTPUT_DIR"
mkdir -p "$APP_DIR/usr/bin"
mkdir -p "$APP_DIR/usr/lib"

echo "[2/7] Copiando executável Windows..."
if [ ! -f "$EXE_PATH" ]; then
    echo "ERRO: output/MeuApp.exe não encontrado!"
    exit 1
fi
cp "$EXE_PATH" "$APP_DIR/usr/bin/$APP_NAME.exe"

# Copia ícone
if [ -f "$ICON_PATH" ]; then
    cp "$ICON_PATH" "$APP_DIR/$APP_NAME.png"
else
    wget -q https://upload.wikimedia.org/wikipedia/commons/8/83/Circle-icons-dev.svg -O "$APP_DIR/$APP_NAME.svg"
fi

echo "[3/7] Verificando Wine-Staging 10.0 Portátil..."
if [ ! -f "tools/wine-portable.tar.xz" ] || [ ! -s "tools/wine-portable.tar.xz" ]; then
    echo "Baixando Wine 10.0..."
    wget -q --show-progress "$WINE_URL" -O "tools/wine-portable.tar.xz"
    check_error "Falha ao baixar o Wine."
fi

echo "Extraindo Wine..."
tar -xf "tools/wine-portable.tar.xz" -C "$APP_DIR/usr/" --strip-components=1
check_error "Falha ao extrair o Wine."

echo "[4/7] Criando AppRun Profissional (Visual Windows 11 + Integração Linux)..."

cat > "$APP_DIR/AppRun" <<EOF
#!/bin/bash

HERE="\$(dirname "\$(readlink -f "\${0}")")"
export PATH="\$HERE/usr/bin:\$PATH"
export LD_LIBRARY_PATH="\$HERE/usr/lib:\$HERE/usr/lib64:\$LD_LIBRARY_PATH"
export WINEPREFIX="\$HOME/.wine_kosmos_pro"
mkdir -p "\$WINEPREFIX"

export WINEDEBUG=-all
export WINEARCH=win64

# Inicialização pesada (apenas na primeira vez)
if [ ! -f "\$WINEPREFIX/configured" ]; then
    echo "Inicializando prefixo Wine..."
    "\$HERE/usr/bin/wineboot" -u
    touch "\$WINEPREFIX/configured"
fi

# --- APLICAR TEMA WINDOWS 11 FLAT (SEMPRE) ---
# CORREÇÃO: Voltamos Managed="Y" para integrar com a barra de tarefas do Linux
echo "Aplicando tema moderno..."
cat <<REG > "\$WINEPREFIX/config.reg"
REGEDIT4

[HKEY_CURRENT_USER\Control Panel\Desktop]
"LogPixels"=dword:00000078
"FontSmoothing"="2"
"FontSmoothingType"=dword:00000002
"ForegroundLockTimeout"=dword:00000000

[HKEY_CURRENT_USER\Software\Wine\X11 Driver]
"Decorated"="Y"
"Managed"="Y"
"UseTakeFocus"="N"

[HKEY_CURRENT_USER\Control Panel\Colors]
"ActiveBorder"="200 200 200"
"ActiveTitle"="255 255 255"
"AppWorkspace"="255 255 255"
"Background"="255 255 255"
"ButtonAlternateFace"="255 255 255"
"ButtonDkShadow"="160 160 160"
"ButtonFace"="243 243 243"
"ButtonHilight"="255 255 255"
"ButtonLight"="243 243 243"
"ButtonShadow"="200 200 200"
"ButtonText"="0 0 0"
"GradientActiveTitle"="255 255 255"
"GradientInactiveTitle"="243 243 243"
"GrayText"="120 120 120"
"Hilight"="0 120 215"
"HilightText"="255 255 255"
"HotTrackingColor"="0 102 204"
"InactiveBorder"="243 243 243"
"InactiveTitle"="243 243 243"
"InactiveTitleText"="120 120 120"
"InfoText"="0 0 0"
"InfoWindow"="255 255 255"
"Menu"="255 255 255"
"MenuBar"="243 243 243"
"MenuHilight"="230 230 230"
"MenuText"="0 0 0"
"Scrollbar"="243 243 243"
"TitleText"="0 0 0"
"Window"="255 255 255"
"WindowFrame"="200 200 200"
"WindowText"="0 0 0"

[HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\FontSubstitutes]
"MS Shell Dlg"="Segoe UI"
"MS Shell Dlg 2"="Segoe UI"
"Tahoma"="Segoe UI"
"Arial"="Segoe UI"
REG

"\$HERE/usr/bin/regedit" /S "\$WINEPREFIX/config.reg"
# --------------------------------------

"\$HERE/usr/bin/wine" "\$HERE/usr/bin/$APP_NAME.exe" "\$@"
EOF

chmod +x "$APP_DIR/AppRun"

# 5. Criar .desktop
cat > "$APP_DIR/$APP_NAME.desktop" <<EOF
[Desktop Entry]
Name=$APP_NAME
Exec=AppRun
Icon=$APP_NAME
Type=Application
Categories=Development;
EOF

echo "[6/7] Verificando Runtime do AppImage..."
if [ ! -f "tools/runtime-x86_64" ] || [ ! -s "tools/runtime-x86_64" ]; then
    echo "Baixando Runtime..."
    rm -f "tools/runtime-x86_64"
    wget --no-check-certificate --show-progress -L "$RUNTIME_URL" -O "tools/runtime-x86_64"
    if [ $? -ne 0 ]; then
        echo "Wget falhou. Tentando Curl..."
        curl -L "$RUNTIME_URL" -o "tools/runtime-x86_64"
    fi
    if [ ! -s "tools/runtime-x86_64" ]; then
        echo "[ERRO] Falha fatal ao baixar o Runtime."
        exit 1
    fi
fi
chmod +x "tools/runtime-x86_64"

echo "[7/7] Empacotando AppImage..."
if ! command -v mksquashfs &> /dev/null; then
    echo "ERRO: mksquashfs não encontrado. Instale: sudo apt install squashfs-tools"
    exit 1
fi

mksquashfs "$APP_DIR" "$OUTPUT_DIR/fs.squashfs" -root-owned -noappend -comp xz -b 1M
check_error "Falha ao criar SquashFS."

cat "tools/runtime-x86_64" "$OUTPUT_DIR/fs.squashfs" > "$OUTPUT_DIR/$APP_NAME-x86_64.AppImage"
check_error "Falha na fusão do AppImage."

chmod +x "$OUTPUT_DIR/$APP_NAME-x86_64.AppImage"
rm -f "$OUTPUT_DIR/fs.squashfs"

echo "======================================================"
echo "SUCESSO! AppImage COM WINE 10.0 (Managed=Y) criado:"
echo "$OUTPUT_DIR/$APP_NAME-x86_64.AppImage"
echo "======================================================"