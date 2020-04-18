@echo OFF

echo "Cleaning Binaries.."
call "CleanBinaries.bat"

echo "Cleaning Project Files.."
call "CleanProjects.bat"

echo "Generating Project Files.."
call "GenerateProjects.bat"