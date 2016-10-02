CC=hipcc
CLANG=~/llvm/bin/clang
OBJ_FLAGS=-x assembler -target amdgcn--amdhsa -mcpu=fiji
CO_FLAGS=-target amdgcn--amdhsa

all: hipScatter hipGather

hipScatter:
	$(CC) hipScatter.cpp -o hipScatter
	$(CLANG) $(OBJ_FLAGS) -c -o scatter.o scatter.s
	$(CLANG) $(CO_FLAGS) scatter.o -o scatter.co

hipGather:
	$(CC) hipGather.cpp -o hipGather
	$(CLANG) $(OBJ_FLAGS) -c -o gather.o gather.s
	$(CLANG) $(CO_FLAGS) gather.o -o gather.co

clean:
	rm scatter.o scatter.co hipScatter gather.o gather.co hipGather
