all: link_install

link_install:
	make -C src/builder link_install
	make -C src/python link_install


.PHONY: \
    link_install \
	all