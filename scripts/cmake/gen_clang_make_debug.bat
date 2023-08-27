del /S /Q compile_commands.json
cmake -B BUILD/DEBUG/CLANG -DCMAKE_CXX_COMPILER=clang -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
mklink /H compile_commands.json BUILD\DEBUG\CLANG\compile_commands.json
