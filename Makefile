CPPC=clang++
CPPOPT=-std=c++14 -O3 -flto -Wall
# CPPOPT_DEBUG=${CPPOPT} -g

all:
	$(MAKE) -C ./source

diversity: all

clean:
	$(MAKE) -C ./source clean
