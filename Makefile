all: gatorLibrary

rbt: gatorLibrary.cpp
	g++ -o gatorLibrary gatorLibrary.cpp

clean:
	rm -f gatorLibrary
