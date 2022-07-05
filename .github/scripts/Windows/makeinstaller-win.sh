echo "------------------------------------------------------------------"
echo "Making Installer ..."

if [[ "$PROCESSOR_ARCHITECTURE" == "x86"* ]]; then
"$PROGRAMFILES\Inno Setup 6\iscc" /Q ".github\scripts\Windows\PlugData.iss"
cp ".\installer\PlugData Installer.exe" ".\PlugData-Win32.exe"
else
"%ProgramFiles(x86)%\Inno Setup 6\iscc" /Q ".github\scripts\Windows\PlugData.iss"
cp ".\installer\PlugData Installer.exe" ".\PlugData-Win64.exe"
fi

# - Codesign Installer for Windows 8+
# -"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin\signtool.exe" sign /f "XXXXX.p12" /p XXXXX /d "PlugData Installer" ".\installer\PlugData Installer.exe"

# -if %1 == 1 (
# -copy ".\installer\PlugData Installer.exe" ".\installer\PlugData Demo Installer.exe"
# -del ".\installer\PlugData Installer.exe"
# -)