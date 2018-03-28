CPPC=clang++
CPPOPT=-std=c++14 -O3 -flto -Wall
# CPPOPT_DEBUG=${CPPOPT} -g

diversity: main.o Splitter.o PlainStringInputStream.o StringInputStream.o StringOutputStream.o StringStream.o StringUtils.o
	${CPPC} ${CPPOPT} -o diversity main.o Splitter.o PlainStringInputStream.o StringInputStream.o StringOutputStream.o StringStream.o StringUtils.o

main.o: main.cpp stats.h
	${CPPC} ${CPPOPT} -c main.cpp
PlainStringInputStream.o:
	${CPPC} ${CPPOPT} -c PlainStringInputStream.cpp
Splitter.o:
	${CPPC} ${CPPOPT} -c Splitter.cpp
StringInputStream.o:
	${CPPC} ${CPPOPT} -c StringInputStream.cpp
StringOutputStream.o:
	${CPPC} ${CPPOPT} -c StringOutputStream.cpp
StringStream.o:
	${CPPC} ${CPPOPT} -c StringStream.cpp
StringUtils.o:
	${CPPC} ${CPPOPT} -c StringUtils.cpp

clean:
	rm diversity main.o Splitter.o PlainStringInputStream.o StringInputStream.o StringOutputStream.o StringStream.o StringUtils.o

