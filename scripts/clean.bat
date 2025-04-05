@echo off
cd ..
if exist build (
    del /q build\*
    for /d %%x in (build\*) do rmdir /s /q "%%x"
)
if exist bin (
    del /q bin\*
    for /d %%x in (bin\*) do rmdir /s /q "%%x"
)