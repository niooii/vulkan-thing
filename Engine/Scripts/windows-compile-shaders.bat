REM def no copied from chatgpt

@echo off

IF "%~2"=="" (
    echo Usage: %0 ^<input_directory^> ^<output_directory^>
    exit /b 1
)

SET INPUT_DIR=%1
SET OUTPUT_DIR=%2

mkdir "%OUTPUT_DIR%" 2>nul

REM Compile vertex shaders
for %%v in ("%INPUT_DIR%\*.vert") do (
    SET vert_shader_name=%%~nxv
    SET vert_output="%OUTPUT_DIR%\!vert_shader_name:.vert=.vert.spv!"
    %VULKAN_SDK%\bin\glslc.exe "%%v" -o "!vert_output!"
    IF %ERRORLEVEL% EQU 0 (
        echo Compiled %%v to !vert_output!
    ) ELSE (
        echo Error compiling %%v
    )
)

for %%f in ("%INPUT_DIR%\*.frag") do (
    SET frag_shader_name=%%~nxf
    SET frag_output="%OUTPUT_DIR%\!frag_shader_name:.frag=.frag.spv!"
    %VULKAN_SDK%\bin\glslc.exe "%%f" -o "!frag_output!"
    IF %ERRORLEVEL% EQU 0 (
        echo Compiled %%f to !frag_output!
    ) ELSE (
        echo Error compiling %%f
    )
)