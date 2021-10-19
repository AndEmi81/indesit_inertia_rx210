rem Set PROJ_PATH (path to the project) with the first value from the command line 
set PROJ_PATH=%1


rem Set LINT_PATH (path to Pc-Lint) with the 2nd value from the command line 
set LINT_PATH=%2


rem Set WZD_PATH (path to the local Platform2 folder) with the 3rd value from the command line
set WZD_PATH=%3

set ECLIPSE_PATH=%4

set GNURX_PATH=%5

rem Execute Pc-Lint with its options on the whole project
%LINT_PATH%/lint-nt.exe   %PROJ_PATH%/plt2_options.lnt %PROJ_PATH%/plt2_include.lnt %PROJ_PATH%/platform2.lnt  

