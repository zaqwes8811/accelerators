OBJS=compact.o scan.o

all: a.bin


a.bin: $(OBJS)
	nvcc -o a.bin $(OBJS)

compact.o: compact.cu float_ops.h
	nvcc compact.cu -o compact.o -c

scan.o: scan.cu float_ops.h
	nvcc scan.cu -o scan.o -c

clean:
	rm -f *.o *.bin