TAGLIB_CFLAGS=`taglib-config --cflags`
TAGLIB_LIBS=`taglib-config --libs`
BOOST_LIBS=-lboost_regex -lboost_filesystem -lboost_system
MAIN_CFLAGS=-I/Users/islam5/Applications/include

OBJS = convert.o curlparser.o metadata.o

metaread: metadata.o metaread.o
	g++ -g -Wall -pthread metadata.o metaread.o -o metaread -lmp4v2 \
	${TAGLIB_LIBS} ${BOOST_LIBS} -lz -lm
metawrite: metadata.o metawrite.o
	g++ -g -Wall -pthread metadata.o metawrite.o -o metawrite -lmp4v2 \
	${TAGLIB_LIBS} ${BOOST_LIBS} -lz -lm
metadata.o: metadata.cpp metadata.h
	g++ -g -Wall -c metadata.cpp -o metadata.o ${TAGLIB_CFLAGS} ${MAIN_CFLAGS}
metaread.o: metaread.cpp
	g++ -g -Wall -c metaread.cpp -o metaread.o ${TAGLIB_CFLAGS} ${MAIN_CFLAGS}
metawrite.o: metawrite.cpp
	g++ -g -Wall -c metawrite.cpp -o metawrite.o ${TAGLIB_CFLAGS} ${MAIN_CFLAGS}
clean:
	rm -f *.o metawrite metaread
