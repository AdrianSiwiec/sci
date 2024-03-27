echo "Compiling site"

mkdir site/out
emcc -I ./inc/ src/*.cpp test/main.cpp -o site/out/main.html --shell-file site/html_template/shell_minimal.html
