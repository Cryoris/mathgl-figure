FLAGS=-std=gnu++11 -lmgl
FIG_DIR=~/dev/cpp/figure/v2/
FIG_SRC=~/dev/cpp/figure/v2/figure.cpp
FIG_HDR=~/dev/cpp/figure/v2/figure.hpp


all : plot.eps

main : main.cpp ${FIG_SRC} ${FIG_HDR}
	g++ $^ ${FLAGS} -o $@

plot.eps : main
	./$< 

clean : 
	rm main plot.eps
