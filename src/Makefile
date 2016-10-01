CC=hipcc
CLANG=~/llvm/bin/clang
OBJ_FLAGS=-x assembler -target amdgcn--amdhsa -mcpu=fiji
CO_FLAGS=-target amdgcn--amdhsa
all: hipScatter

hipScatter:
        $(CC) hipScatter.cpp -o hipScatter
        $(CLANG) $(OBJ_FLAGS) -c -o scatter.o scatter.s
        $(CLANG) $(CO_FLAGS) scatter.o -o scatter.co

clean:
        rm scatter.o scatter.co hipScatter
