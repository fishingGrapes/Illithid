@echo OFF

echo "Cleanig Binaries.."
call "CleanBinaries.bat"

echo "Cleaning Project Files.."
call "CleanProjects.bat"

echo "Genarting Project Files.."
call "GenerateProjects.bat"