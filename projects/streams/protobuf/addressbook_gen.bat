@echo off
set SRC_DIR="."
set DST_DIR=..\\src\\serialize

rem protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%\\addressbook.proto
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%\\addressbook.proto
pause
