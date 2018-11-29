@echo off
del %~dp0\LaMulanaClassic3DS.3dsx
make
if exist %~dp0\out\LaMulanaClassic3DS.3dsx (
	@echo "hi"
    start C:\Users\User\AppData\Local\Citra\nightly-mingw\citra-qt.exe %~dp0\out\LaMulanaClassic3DS.3dsx
)