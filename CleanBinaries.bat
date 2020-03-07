@echo OFF
SET "mypath=%~dp0binaries"

if EXIST %mypath% RD /S /Q %mypath% 
