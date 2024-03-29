echo "Compiling site"

mkdir site/out
emcc -I ./inc/ src/*.cpp test/main.cpp -o site/out/main.html --shell-file site/html_template/shell_minimal.html

echo "Copying site to website repo"
cp site/out/* ../AdrianSiwiec.github.io/sci/
