# CXXFLAGS_DEBUG=$(CXXFLAGS) -g

diversity: main.o Splitter.o PlainStringInputStream.o StringInputStream.o StringOutputStream.o StringStream.o StringUtils.o
	$(CXX) $(CXXFLAGS) -o ../diversity main.o Splitter.o PlainStringInputStream.o StringInputStream.o StringOutputStream.o StringStream.o StringUtils.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp
PlainStringInputStream.o:
	$(CXX) $(CXXFLAGS) -c PlainStringInputStream.cpp
Splitter.o:
	$(CXX) $(CXXFLAGS) -c Splitter.cpp
StringInputStream.o:
	$(CXX) $(CXXFLAGS) -c StringInputStream.cpp
StringOutputStream.o:
	$(CXX) $(CXXFLAGS) -c StringOutputStream.cpp
StringStream.o:
	$(CXX) $(CXXFLAGS) -c StringStream.cpp
StringUtils.o:
	$(CXX) $(CXXFLAGS) -c StringUtils.cpp

clean:
	rm ../diversity main.o Splitter.o PlainStringInputStream.o StringInputStream.o StringOutputStream.o StringStream.o StringUtils.o

