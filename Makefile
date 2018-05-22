export CXXFLAGS := -std=c++14 -O3 -flto -Wall
# CXXFALGS_DEBUG=${CXXFLAGS} -g

all:
	$(MAKE) -C ./source

diversity: all

clean:
	$(MAKE) -C ./source clean
