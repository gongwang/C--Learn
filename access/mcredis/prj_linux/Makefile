
MAKE := make --no-print-directory

all :
	@$(MAKE) -f Makefile_debug clean;
	@$(MAKE) -f Makefile_debug 1>../../../10-common/version/compileinfo/mcredis_linux_d.txt 2>&1;
	@$(MAKE) -f Makefile_debug clean;
	@$(MAKE) -f Makefile_release clean;
	@$(MAKE) -f Makefile_release 1>../../../10-common/version/compileinfo/mcredis_linux_r.txt 2>&1;
	@$(MAKE) -f Makefile_release clean;
#create make object respectly
clean:
	@$(MAKE) -f Makefile_debug clean;
	@$(MAKE) -f Makefile_release clean;
