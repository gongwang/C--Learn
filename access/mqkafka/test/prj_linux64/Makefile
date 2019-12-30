MAKE := make --no-print-directory
ver_type=cloud2

all :
	@$(MAKE) -f Makefile_debug clean;
	@$(MAKE) -f Makefile_debug 2>&1;
	@$(MAKE) -f Makefile_debug clean;
	@$(MAKE) -f Makefile_release clean;
	@$(MAKE) -f Makefile_release 2>&1;
	@$(MAKE) -f Makefile_release clean;
#create make object respectly
clean:
	@$(MAKE) -f Makefile_debug clean;
	@$(MAKE) -f Makefile_release clean;
