
MAKE := make --no-print-directory

all :
	@$(MAKE) -f makefile_debug clean;
	@$(MAKE) -f makefile_debug 1>../../../10-common/version/compileinfo/topicmngctrl_linux_d.txt 2>&1;
	@$(MAKE) -f makefile_debug clean;
	@$(MAKE) -f makefile_release clean;
	@$(MAKE) -f makefile_release 1>../../../10-common/version/compileinfo/topicmngctrl_linux_r.txt 2>&1;
	@$(MAKE) -f makefile_release clean;
#create make object respectly
clean:
	@$(MAKE) -f makefile_debug clean;
	@$(MAKE) -f mkefile_release clean;
