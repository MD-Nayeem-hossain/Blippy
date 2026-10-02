; Inno Setup Script for Blippy

#define MyAppName "Blippy"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Blippy Team"
#define MyAppURL "https://github.com/blippy/blippy"
#define MyAppExeName "Blippy.exe"

[Setup]
AppId={{F7D2B9A5-3E8C-4F42-9B7A-D6E5C8F4A3B2}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=..\release
OutputBaseFilename=Blippy-Setup-{#MyAppVersion}
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "autoStart"; Description: "Launch Blippy at startup"; GroupDescription: "Startup"; Flags: unchecked

[Files]
Source: "..\release\*"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{autohome}\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\{#MyAppName}.lnk"; Filename: "{app}\{#MyAppExeName}"; Tasks: autoStart

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{app}\settings"
Type: filesandordirs; Name: "{app}\data"
