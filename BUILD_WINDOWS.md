# Blippy - Windows Build Instructions

## Option 1: Build on Windows (Recommended)

### Prerequisites
1. **Install Qt 6.11+** (MinGW 11.2 64-bit)
   - Download from: https://www.qt.io/download-qt-installer
   - Select: "Qt 6.11+" → "MinGW 11.2 64-bit"
   - Default install: `C:\Qt\6.11.2\mingw_64`

2. **Install MinGW** (comes with Qt installer)

3. **Install Git** (optional, for cloning)

### Build Steps
```cmd
# Clone or copy project
git clone <repo-url> Blippy
cd Blippy

# Build
C:\Qt\6.11.2\mingw_64\bin\qmake.exe Blippy.pro
mingw32-make.exe -j4

# Run
release\Blippy.exe
```

### Create Installer (Optional)
```cmd
# Using Qt Installer Framework
C:\Qt\Tools\QtInstallerFramework\4.6\bin\binarycreator.exe -c config\config.xml -p packages BlippyInstaller.exe
```

---

## Option 2: Cross-Compile from Linux (Advanced)

### Install MinGW on Linux
```bash
# Arch/Manjaro
sudo pacman -S mingw-w64-gcc mingw-w64-qt6

# Ubuntu/Debian
sudo apt install g++-mingw-w64-x86-64 qt6-base-dev-tools

# Fedora
sudo dnf install mingw64-gcc mingw64-qt6-qtbase
```

### Cross-Compile
```bash
cd Blippy

# Configure for cross-compilation
x86_64-w64-mingw32-qmake-qt6 Blippy.pro
make -j$(nproc)

# Result: release/Blippy.exe
```

### Required Qt6 MinGW Packages (Linux)
```bash
# Arch
pacman -S mingw-w64-qt6-base mingw-w64-qt6-declarative mingw-w64-qt6-quickcontrols2 mingw-w64-qt6-svg

# Ubuntu
apt install qt6-base-dev qt6-declarative-dev qt6-quickcontrols2-dev qt6-svg-dev
# Note: Cross-compile packages may need manual setup
```

---

## Option 3: GitHub Actions (Automated)

Create `.github/workflows/windows.yml`:

```yaml
name: Windows Build

on:
  push:
    tags: ['v*']
  workflow_dispatch:

jobs:
  build:
    runs-on: windows-latest
    
    steps:
    - uses: actions/checkout@v4
    
    - name: Install Qt
      uses: jurplel/install-qt-action@v3
      with:
        version: '6.11.2'
        target: 'desktop'
        arch: 'win64_mingw'
        modules: 'qtdeclarative qtquickcontrols2 qtsvg'
    
    - name: Build
      run: |
        qmake Blippy.pro
        mingw32-make -j4
    
    - name: Upload Artifact
      uses: actions/upload-artifact@v4
      with:
        name: Blippy-Windows
        path: release/Blippy.exe
        retention-days: 30
```

---

## Option 4: Quick Test with Pre-built Dependencies

If you just want to test without full Qt install:

1. **Download Qt 6.11 MinGW binaries** from Qt's CI
2. **Set PATH**: `set PATH=C:\Qt\6.11.2\mingw_64\bin;%PATH%`
3. **Run**: `qmake && mingw32-make`

---

## Project Structure for Windows

```
Blippy/
├── Blippy.pro              # qmake project (works on Windows)
├── src/
│   ├── main.cpp            # Entry point
│   ├── Blippy.qrc          # Resources (QML, icons)
│   ├── core/               # Core C++ modules
│   ├── models/             # Data models
│   ├── storage/            # Storage/import
│   ├── network/            # Sync/update
│   └── ui/qml/             # QML UI
├── config/                 # Installer config (optional)
└── BUILD_WINDOWS.md        # This file
```

---

## Windows-Specific Notes

### .pro File Already Has Windows Config:
```pro
win32 {
    LIBS += -luser32 -lpsapi
    DEFINES += WIN32_LEAN_AND_MEAN
}
```

### Required Qt Modules (in .pro):
```pro
QT += core gui widgets network qml quick quickcontrols2 svg
```

### Deployment (for distribution):
```cmd
# Use windeployqt to copy DLLs
C:\Qt\6.11.2\mingw_64\bin\windeployqt.exe --release release\Blippy.exe
```

---

## Troubleshooting

| Issue | Solution |
|-------|----------|
| `qmake` not found | Add Qt bin to PATH |
| `mingw32-make` not found | Use `make` or full path |
| Missing DLLs | Run `windeployqt` |
| QML not loading | Check `qrc:/` paths in qrc file |
| Theme singleton error | Ensure `qmlRegisterSingletonType` uses function provider |

---

## Quick Start for Testing

1. Install Qt 6.11+ with MinGW on Windows
2. Open `Blippy.pro` in Qt Creator
3. Configure kit: "Desktop Qt 6.11.2 MinGW 64-bit"
4. Build & Run (Ctrl+R)

The project is configured for cross-platform and should build on Windows without code changes.