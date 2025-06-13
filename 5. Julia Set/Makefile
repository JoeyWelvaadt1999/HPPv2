CXX = mpic++
CXXFLAGS = -O2 --std=c++23
OMPFLAGS = -fopenmp

all: julia

ifeq ($(UNAME), Darwin)
	LDFLAGS = -L/opt/homebrew/opt/libomp/lib -lomp
endif

ffmpeg:
	if test ! -e ffmpeg -a ! -h ffmpeg; then if which ffmpeg > /dev/null; then ln -s $$(which ffmpeg) ./ffmpeg; else curl https://johnvansickle.com/ffmpeg/releases/ffmpeg-release-amd64-static.tar.xz | tar xJ --absolute-names --no-anchored --transform='s:.*/::' ffmpeg-6.1-amd64-static/ffmpeg; fi; fi

%.o: %.cc %.h consts.h
	g++ -O2 $(LDFLAGS) -c -o $@ $<

julia-mpi: main-mpi.cc frame.o animation.o consts.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o julia-mpi main-mpi.cc frame.o animation.o

julia-mpi-5000: main-mpi.cc frame.o animation.o consts.h
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o julia-mpi-5000 main-mpi.cc frame.o animation.o

julia-omp: main-omp.cc frame.o animation.o consts.h
	$(CXX) $(CXXFLAGS) $(OMPFLAGS) $(LDFLAGS) -o julia-omp main-omp.cc frame.o animation.o

julia-omp-mpi: main-omp-mpi.cc frame.o animation.o consts.h
	$(CXX) $(CXXFLAGS) $(OMPFLAGS) $(LDFLAGS) -o julia-omp-mpi main-omp-mpi.cc frame.o animation.o

julia-omp-mpi-5000: main-omp-mpi.cc frame.o animation.o consts.h
	$(CXX) $(CXXFLAGS) $(OMPFLAGS) $(LDFLAGS) -o julia-omp-mpi-5000 main-omp-mpi.cc frame.o animation.o

run: ffmpeg julia
	mpiexec -np $(or $(NP),4) ./julia

help:
	@echo "Use \`make\` to build and \`make run\` to execute on 4 processes (default). Use \`make run NP=2\` to choose the number of processes."

clean:
	rm *.o julia
