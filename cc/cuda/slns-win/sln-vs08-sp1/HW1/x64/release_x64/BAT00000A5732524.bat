@echo off
echo copy "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\bin\cudart*.dll" "D:\home\parallelize\cuda\sln-vs08-sp1\x64\release_x64"
copy "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v5.5\bin\cudart*.dll" "D:\home\parallelize\cuda\sln-vs08-sp1\x64\release_x64"

if errorlevel 1 goto VCReportError
goto VCEnd
:VCReportError
echo Project : error PRJ0019: A tool returned an error code from "Performing Post-Build Event..."
exit 1
:VCEnd