; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Calculator"
#define MyAppVersion "0.0.1.1"
#define MyAppPublisher "shangjiaxuan"
#define MyAppURL "https://github.com/shangjiaxuan/Calculator"
#define MyAppExeName "Host.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{067AC5D2-2000-49A0-AB59-E9F6674C54C6}
AppName=Calculator
AppVersion=0.0.1.1
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher=shangjiaxuan
AppPublisherURL=https://github.com/shangjiaxuan
AppSupportURL=https://github.com/shangjiaxuan/Calculator/issues
AppUpdatesURL=https://github.com/shangjiaxuan/Calculator/releases
DefaultDirName={userdesktop}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=..\LICENSE
InfoBeforeFile=Before.rtf
InfoAfterFile=After.rtf
OutputDir=..\Build\x86-Release
OutputBaseFilename=setup-x86
Compression=lzma/ultra
SolidCompression=yes
PrivilegesRequired=lowest
ShowUndisplayableLanguages=True
VersionInfoVersion=0.0.1.1
VersionInfoProductName=Calculator
VersionInfoProductVersion=0.0.1.1
InternalCompressLevel=ultra
CompressionThreads=2

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"; InfoBeforeFile: "Before.rtf"; InfoAfterFile: "After.rtf"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "..\Build\x86-Release\Host.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build\x86-Release\Calculator.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build\x86-Release\SwapEnc.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build\x86-Release\Calculator caller.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build\x86-Release\Calculator.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build\x86-Release\SwapEnc.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
