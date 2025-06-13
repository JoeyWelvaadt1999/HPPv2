@echo off
setlocal enabledelayedexpansion

:: Get program name from command line
set PROGRAM=%1

:: Define the number of processes to run and the number of threads to use
set PROCESSES=1 2 4 8 16 32 64 128 256
set THREADS=16

set OUTPUT_FILE=benchmark_results_%PROGRAM%.csv
echo Number Of Processes;Number Of Threads;Frames;Time; > %OUTPUT_FILE%

for %%p in (%PROCESSES%) do (
    for %%t in (%THREADS%) do (
        set "TEMP_FILE=tmp_output_%%p_%%t.txt"
        echo Running: %%p processes with %%t threads
        echo "mpiexec -np %%p %PROGRAM% %%t"
        mpiexec -np %%p %PROGRAM% %%t > "!TEMP_FILE!"

        ping -n 2 127.0.0.1 > nul
        
        set "resultline="

        if exist "!TEMP_FILE!" (
            for /f "usebackq delims=" %%L in ("!TEMP_FILE!") do (
                echo %%L > tmp_result.txt
            )
            if exist "tmp_result.txt" (
                set /p resultline=<tmp_result.txt
                >> %OUTPUT_FILE% echo !resultline!
                del tmp_result.txt
            ) else (
                >> %OUTPUT_FILE% echo ^| %%p ^| %%t ^| error ^| no_result ^|
            )
        ) else (
            >> %OUTPUT_FILE% echo ^| %%p ^| %%t ^| error ^| file_missing ^|
        )

        del "!TEMP_FILE!" > nul 2>&1
    )
)

type %OUTPUT_FILE%
endlocal