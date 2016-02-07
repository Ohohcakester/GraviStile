rm -r _build
mkdir _build
g++ -std=c++11 -c src/*.cpp
g++ -std=c++11 *.o -o _build/GraviStile -lsfml-graphics -lsfml-window -lsfml-system
cp *.ttf _build
cp -r img _build
rm *.o
