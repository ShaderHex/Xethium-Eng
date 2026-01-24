@echo off

cd engine
call build.bat
cd ..
cd testbed
call build.bat
cd ..