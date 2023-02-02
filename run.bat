@echo OFF
SETLOCAL

if not exist .\debug\ mkdir debug
if not exist .\.cache\ mkdir .cache

if exist .\.cache\main.c (
    call fc .\main.c .\.cache\main.c > nul
    if errorlevel 1 (
        echo Building
        clang -o .\debug\program.exe main.c
        copy .\main.c .\.cache\main.c > nul
    )

) else (
    echo Building
    clang -o .\debug\program.exe main.c
    copy .\main.c .\.cache\main.c > nul
)

call .\debug\program.exe
