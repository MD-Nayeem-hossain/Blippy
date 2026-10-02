# Blippy Windows Installer (.exe)

Since we can't build a Windows .exe on Linux without cross-compilation, here's the easiest way to get a ONE-PACKAGE INSTALLER (.exe) for Windows:

## Recommended: GitHub Actions (Free, Builds the .exe Installer Automatically)

This will build a proper `Blippy-Setup-1.0.0.exe` installer using Inno Setup (just like commercial apps).

### Steps:

1. **Create a GitHub repo** (if you haven't)
   ```bash
   # On GitHub.com: Create new repo "blippy" (public/private)
   ```

2. **Push the code to GitHub**
   ```bash
   cd /home/nerdy_taco/Work/Blippy
   git init
   git add .
   git commit -m "Initial commit: Blippy text expander"
   git branch -M main
   git remote add origin https://github.com/YOUR_USERNAME/blippy.git
   git push -u origin main
   ```

3. **Create a release tag to trigger installer build**
   ```bash
   git tag v1.0.0
   git push origin v1.0.0
   ```

4. **Download the installer**
   - Go to: `https://github.com/YOUR_USERNAME/blippy/actions`
   - Click "Windows Installer" workflow (it should be running)
   - Wait ~10-15 minutes for build to complete
   - Click on the completed run → Download `Blippy-Windows-Installer` artifact
   - Extract the ZIP → You'll get `Blippy-Setup-1.0.0.exe` (single-file installer)

5. **Install on Windows**
   - Run `Blippy-Setup-1.0.0.exe` → Next, Next, Install
   - Creates Start Menu shortcut, optional desktop/startup
   - Fully self-contained (includes all Qt DLLs)

---

## Alternative: Source Build + Manual Installer

If you want to build locally on Windows:

1. Download `Blippy-Windows-Source-Full.zip` (80KB) - contains `installer/blippy.iss`
2. Extract to Windows
3. Install Qt 6.11.2 + MinGW 64-bit
4. Open `Blippy.pro` in Qt Creator → Build Release
5. Run `windeployqt.exe --release release\Blippy.exe`
6. Install [Inno Setup](https://jrsoftware.org/isinfo.php) (free)
7. Right-click `installer/blippy.iss` → "Compile" → Creates `release/Blippy-Setup-1.0.0.exe`

---

## What You Get

The installer (.exe) will:
- Install Blippy to `C:\Program Files\Blippy\` (or user choice)
- Include ALL required Qt DLLs (self-contained, no Qt install needed)
- Create Start Menu shortcuts + optional Desktop/Startup
- Add proper Windows uninstaller (Control Panel → Add/Remove Programs)
- Launch Blippy after install (optional)

---

## Files Available

- `Blippy-Windows-Source-Full.zip` (80KB) - Complete source + installer script + GitHub Actions
- Location: `/home/nerdy_taco/Work/Blippy-Windows-Source-Full.zip`

This is the cleanest way to get a proper Windows installer. The GitHub Actions approach is zero-cost and reliable.