CXX = g++
CXXFLAGS = -std=c++14 -Wall -Werror -MMD -Werror=vla
EXEC = watan
OBJECTS = src/main.o src/manager.o src/board.o src/regularHex.o src/RegularHexTextDisplay.o src/ColourHexTextDisplay.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS} #includes the .d files, which are dependencies for objects

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
.PHONY: clean
