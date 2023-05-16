mount:
	mkdir test
	./wadfs/wadfs /home/reptilian/sample1.wad test
destroyMount:
	fusermount -u test
	rmdir test
test:
	c++ -o main main.cpp -L ./libWad -lWad
