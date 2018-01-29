echo "clean target file ..."
make clean

echo "compile C library ..."
make

echo "compile Java ..."
javac JavaToC.java

echo "Run Java ..."
java JavaToC
