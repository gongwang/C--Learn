
AUTOMAKE_OPTIONS = subdir-objects

#CPPFLAGS    C/C++ preprocessor flags, e.g. -I and -D
#CXXFLAGS    C++ compiler flags

lib_LIBRARIES = libdbix.a
libdbix_a_CPPFLAGS = -D_LINUX_ \
                 -I../../../b0-common/include/ \
				 -I../../../b0-common/include/cms/dbix \
                 -I../code \
				 -I../code/utils \
				 -I../../../10-common/include \
                 -I../../../10-common/include/cbb/system
libdbix_a_CXXFLAGS = -Wall
#libdir = /root
libdbix_a_SOURCES = ../../../b0-common/include/cms/dbix/dbresult_callback_imp4osptask.cpp \
                    ../code/utils/inner_util.cpp \
					../code/connect_pool.cpp \
					../code/dbcmd.cpp \
					../code/dbentity.cpp \
					../code/dbi_layer.cpp \
					../code/dbix.cpp \
					../code/dbix_debug.cpp \
					../code/sql_layer.cpp \
					../code/task_queue.cpp \
					../code/thread_pool.cpp
