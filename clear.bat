@echo off
SETLOCAL

REM Set working directory to script location
cd /d "%~dp0"

echo *********************************************************************************************************************
echo ******************************************** Clean Unreal Engine Project ********************************************
echo *********************************************************************************************************************
echo.

:PROMPT
SET /P AREYOUSURE=Are you sure to delete all Binaries, Build, Intermediate, Saved and DerivedDataCache folders in your project and plugins (y/[N])? 
IF /I "%AREYOUSURE%" NEQ "y" GOTO END

echo.
echo Cleaning your project, please wait...

FOR /d %%d IN (Binaries Build Intermediate Saved DerivedDataCache) DO (
    IF EXIST "%%d" (
        echo Deleting %%d
        rd /s /q "%%d"
    )
)

REM Also clean plugin folders recursively
FOR /R %%p IN (.) DO (
    FOR %%d IN (Binaries Build Intermediate Saved DerivedDataCache) DO (
        IF EXIST "%%p\%%d" (
            rd /s /q "%%p\%%d"
        )
    )
)

del /q *.sln

echo.
echo ✅ Your project was cleaned. Now regenerate Visual Studio project files!
pause

:END
ENDLOCAL
