
xc: parser.tab.cpp scan.cpp
	g++ -o xc *.cpp

parser.tab.cpp: parser.y
	bison -d -o parser.tab.cpp parser.y

scan.cpp:
	flex -o scan.cpp scan.l


