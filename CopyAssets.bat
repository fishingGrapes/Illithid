set destination="empty"
if exist "binaries" (goto CopyAssets)
goto End

:CopyAssets
if exist "binaries\Debug-windows-x86_64" ( set destination="binaries\Debug-windows-x86_64\Sandbox\Assets" )
if exist "binaries\Release-windows-x86_64" ( set destination="binaries\Release-windows-x86_64\Sandbox\Assets" )
if exist "binaries\Dist-windows-x86_64" ( set destination="binaries\Dist-windows-x86_64\Sandbox\Assets" )

if exist %destination% ( rmdir %destination% /q /s )
mkdir %destination%
xcopy "Sandbox\Assets" %destination% /s

:End
