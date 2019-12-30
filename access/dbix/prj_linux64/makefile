MAKE := make --no-print-directory
ver_type=cloud2

all :
	@$(MAKE) -f makefile_debug clean;
	@$(MAKE) -f makefile_debug 1>../../../10-common/version/compileinfo/ismp_$(ver_type)_linux64_cbb_dbix_d.txt 2>&1;
	@$(MAKE) -f makefile_debug clean;
	@$(MAKE) -f makefile_release clean;
	@$(MAKE) -f makefile_release 1>../../../10-common/version/compileinfo/ismp_$(ver_type)_linux64_cbb_dbix_r.txt 2>&1;
	@$(MAKE) -f makefile_release clean;
#create make object respectly
clean:
	@$(MAKE) -f makefile_debug clean;
	@$(MAKE) -f makefile_release clean;
