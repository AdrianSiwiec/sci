mkdir site/out
# echo "Compiling site"
# emcc -I ./inc/ src/*.cpp test/main.cpp -o site/out/main.html --shell-file site/html_template/shell_minimal.html

echo "Compiling emscripten_lib.cpp"
emcc -O3 -I ./inc/ src/*.cpp site/emscripten_lib.cpp -o site/out/sci.html -sEXPORTED_FUNCTIONS=_SolveFormulas -sEXPORTED_RUNTIME_METHODS=ccall,cwrap --shell-file site/html_template/shell_minimal.html -s MINIFY_HTML=0 -s ALLOW_MEMORY_GROWTH=1


echo "Copying site to website repo"
cp site/out/* ../AdrianSiwiec.github.io/sci/
