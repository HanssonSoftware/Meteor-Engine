@echo off
setlocal enabledelayedexpansion

rem Beállítások
set "header_file=%~dp0\..\Engine\Version.h"
set "temp_file=%~dp0\..\Engine\VersionTemp.h"

rem Dátum lekérése
for /f "tokens=1-3 delims=/ " %%a in ('date /t') do (
    set "BUILD_YEAR=%%c"
    set "BUILD_MONTH=%%a"
    set "BUILD_DAY=%%b"
)

rem A hónap és nap formázása 2 számjegyűre
if %BUILD_MONTH% LSS 10 set "BUILD_MONTH=0%BUILD_MONTH%"
if %BUILD_DAY% LSS 10 set "BUILD_DAY=0%BUILD_DAY%"

rem Dátum formázása: HH/ÉÉÉÉ formátumban (pont nélkül)
set "BUILD_DATE=L%BUILD_MONTH%/%BUILD_YEAR%"

rem BUILD_NUMBER olvasása és növelése
set "current_number=0"
for /f "tokens=3" %%a in ('findstr /r /c:"#define BUILD_NUMBER" "%header_file%"') do set "current_number=%%a"
set /a "new_number=current_number + 1"

rem Frissített tartalom írása
(
    for /f "delims=" %%i in ('type "%header_file%"') do (
        set "line=%%i"
        if "!line!"=="#define BUILD_NUMBER !current_number!" (
            echo #define BUILD_NUMBER !new_number!
        ) else if "!line!"=="#define BUILD_YEAR" (
            echo #define BUILD_YEAR %BUILD_YEAR%
        ) else if "!line!"=="#define BUILD_MONTH" (
            echo #define BUILD_MONTH %BUILD_MONTH%
        ) else if "!line!"=="#define BUILD_DAY" (
            echo #define BUILD_DAY %BUILD_DAY%
        ) else if "!line!"=="#define BUILD_DATE" (
            echo #define BUILD_DATE "%BUILD_DATE%"
        ) else (
            echo !line!
        )
    )

    rem Új makrók hozzáadása, ha nem léteznek
    findstr /c:"#define BUILD_YEAR" "%header_file%" >nul || echo #define BUILD_YEAR %BUILD_YEAR%
    findstr /c:"#define BUILD_MONTH" "%header_file%" >nul || echo #define BUILD_MONTH %BUILD_MONTH%
    findstr /c:"#define BUILD_DAY" "%header_file%" >nul || echo #define BUILD_DAY %BUILD_DAY%
    findstr /c:"#define BUILD_DATE" "%header_file%" >nul || echo #define BUILD_DATE "%BUILD_DATE%"
) > "%temp_file%"

rem Eredeti fájl cseréje
move /y "%temp_file%" "%header_file%"

endlocal
