CFLAGS = -g

objects = main.o queue.o p_queue.o read_file.o gen.o

sim : $(objects)
	cc -o sim $(objects)

main.o : queue.h p_queue.h read_file.h gen.h

.PHONY : clean
clean :
	rm sim $(objects)
