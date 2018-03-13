@echo off

reg add HKCR\*\shell\CTViewer /f
reg add HKCR\*\shell\CTViewer /ve /d "Open with CTViewer" /f
reg add HKCR\*\shell\CTViewer /v icon /t REG_SZ /d "%~dp0resource\item.ico" /f
reg add HKCR\*\shell\CTViewer\command /f
reg add HKCR\*\shell\CTViewer\command /ve /d "%~dp0VTK_demo.exe \"%%1\"" /f

reg add HKLM\SOFTWARE\Classes\Directory\shell\CTViewer /f
reg add HKLM\SOFTWARE\Classes\Directory\shell\CTViewer /ve /d "Open with CTViewer" /f
reg add HKLM\SOFTWARE\Classes\Directory\shell\CTViewer /v icon /t REG_SZ /d "%~dp0resource\item.ico" /f
reg add HKLM\SOFTWARE\Classes\Directory\shell\CTViewer\command /f
reg add HKLM\SOFTWARE\Classes\Directory\shell\CTViewer\command /ve /d "%~dp0VTK_demo.exe \"%%1\"" /f
