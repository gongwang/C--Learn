
MAKE := make --no-print-directory
ver_type=cloud2

all :
	@$(MAKE) -f Makefile_debug clean;
	@$(MAKE) -f Makefile_debug 1>../../../10-common/version/compileinfo/ismp_$(ver_type)_linux32_cbb_mqkafka_d.txt 2>&1;
	@$(MAKE) -f Makefile_debug clean;
	@$(MAKE) -f Makefile_release clean;
	@$(MAKE) -f Makefile_release 1>../../../10-common/version/compileinfo/ismp_$(ver_type)_linux32_cbb_mqkafka_r.txt 2>&1;
	@$(MAKE) -f Makefile_release clean;
#create make object respectly
clean:
	@$(MAKE) -f Makefile_debug clean;
	@$(MAKE) -f Makefile_release clean;
