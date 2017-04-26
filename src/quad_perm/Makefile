LLVM_PATH=~/rocm/hcc/compiler/bin
LLC=$(LLVM_PATH)/llc
CLANG=$(LLVM_PATH)/clang

TARGET=amdgcn--amdhsa
ARCH=amdgcn
CPU=fiji
HIPCC=hipcc

all: qp1 qp2

qp1:
	$(LLC) -mtriple $(TARGET) -march=$(ARCH) -mcpu=$(CPU) qp1.ll -o qp1.s
	$(CLANG) -x assembler -target $(TARGET) -mcpu=$(CPU) -c qp1.s -o qp1.o
	$(CLANG) -target $(TARGET) qp1.o -o qp1.co
	$(HIPCC) hipQP1.cpp -o hipQP1.out

qp2:
	$(LLC) -mtriple $(TARGET) -march=$(ARCH) -mcpu=$(CPU) qp2.ll -o qp2.s
	$(CLANG) -x assembler -target $(TARGET) -mcpu=$(CPU) -c qp2.s -o qp2.o
	$(CLANG) -target $(TARGET) qp2.o -o qp2.co
	$(HIPCC) hipQP2.cpp -o hipQP2.out

qp3:
	$(LLC) -mtriple $(TARGET) -march=$(ARCH) -mcpu=$(CPU) qp3.ll -o qp3.s
	$(CLANG) -x assembler -target $(TARGET) -mcpu=$(CPU) -c qp3.s -o qp3.o
	$(CLANG) -target $(TARGET) qp3.o -o qp3.co
	$(HIPCC) hipQP3.cpp -o hipQP3.out



clean:
	rm -rf *.out *.o *.s *.co
