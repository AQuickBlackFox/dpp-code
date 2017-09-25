all: conv1

conv1:
	llvm-mc -arch=amdgcn -mcpu=gfx900 conv1.s -filetype=obj -o conv1.o
	ld.lld -shared conv1.o -o conv1.co
	/opt/rocm/bin/hipcc conv1.cpp -o conv1

clean:
	rm conv1.co conv1.o conv1
