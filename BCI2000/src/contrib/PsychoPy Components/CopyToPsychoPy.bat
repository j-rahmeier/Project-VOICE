@setlocal enableextensions
@cd /d "%~dp0"
echo .
echo This script assumes that you have your Psychopy installation from the standalone version from here
echo Make sure to run this script as administrator
echo .

xcopy /s "." "C:\Program Files\PsychoPy\Lib\site-packages\psychopy\experiment\components"