ssh_scan: ssh_scan.o
	gcc -lssh ssh_scan.o -o ssh_scan

ssh_scan.o: ssh_scan.c
	gcc -c ssh_scan.c -o ssh_scan.o

clean: 
	rm -f ssh_scan.o ssh_scan

