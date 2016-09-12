
all : 
	$(MAKE) -C simcc
	$(MAKE) -C 3rdparty

test : all
	$(MAKE) -C test

3rdparty :
	$(MAKE) -C 3rdparty

check : all
	$(MAKE) check -C test

clean:
	$(MAKE) clean -C simcc
	$(MAKE) clean -C test
	$(MAKE) clean -C 3rdparty


.PHONY: all test check clean 3rdparty
