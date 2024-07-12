obj-m += ipc_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -g -o Writer writer.c
	gcc -g -o Reader reader.c


clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean 
	rm *.o Writer Reader
