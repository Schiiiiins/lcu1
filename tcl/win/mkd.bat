@echo off
rem RCS: @(#) $Id: mkd.bat,v 1.1.1.1 2005/09/27 07:38:07 reemtsl Exp $

if exist %1\tag.txt goto end

if "%OS%" == "Windows_NT" goto winnt

md %1
if errorlevel 1 goto end

goto success

:winnt
md %1
if errorlevel 1 goto end

:success
echo TAG >%1\tag.txt
echo created directory %1

:end
