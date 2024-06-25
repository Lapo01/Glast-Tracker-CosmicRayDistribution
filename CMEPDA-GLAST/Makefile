all: CMEPDA-GLAST/SharedLibs/libGeneralUtils.so CMEPDA-GLAST/SharedLibs/libUtilsCreateTree.so CMEPDA-GLAST/SharedLibs/libUtilsRetina.so


clean:
	rm CMEPDA-GLAST/SharedLibs/libGeneralUtils.so
	rm CMEPDA-GLAST/SharedLibs/libUtilsCreateTree.so
	rm CMEPDA-GLAST/SharedLibs/libUtilsRetina.so



CMEPDA-GLAST/SharedLibs/GeneralUtils.o:
	g++ -c -fPIC CMEPDA-GLAST/LibrariesSources/GeneralUtils.cpp `root-config --libs --cflags` -I./CMEPDA-GLAST/Headers -o CMEPDA-GLAST/SharedLibs/GeneralUtils.o



CMEPDA-GLAST/SharedLibs/UtilsCreateTree.o:
	g++ -c -fPIC CMEPDA-GLAST/LibrariesSources/UtilsCreateTree.cpp `root-config --libs --cflags` -I./CMEPDA-GLAST/Headers -o CMEPDA-GLAST/SharedLibs/UtilsCreateTree.o



CMEPDA-GLAST/SharedLibs/UtilsRetina.o:
	g++ -c -fPIC CMEPDA-GLAST/LibrariesSources/UtilsRetina.cpp `root-config --libs --cflags` -I./CMEPDA-GLAST/Headers -o CMEPDA-GLAST/SharedLibs/UtilsRetina.o







CMEPDA-GLAST/SharedLibs/libGeneralUtils.so: CMEPDA-GLAST/SharedLibs/GeneralUtils.o
	g++ -shared -fPIC `root-config --libs --cflags` -o CMEPDA-GLAST/SharedLibs/libGeneralUtils.so CMEPDA-GLAST/SharedLibs/GeneralUtils.o
	rm CMEPDA-GLAST/SharedLibs/GeneralUtils.o

CMEPDA-GLAST/SharedLibs/libUtilsCreateTree.so: CMEPDA-GLAST/SharedLibs/UtilsCreateTree.o
	g++ -shared -fPIC `root-config --libs --cflags` -o CMEPDA-GLAST/SharedLibs/libUtilsCreateTree.so CMEPDA-GLAST/SharedLibs/UtilsCreateTree.o
	rm CMEPDA-GLAST/SharedLibs/UtilsCreateTree.o


CMEPDA-GLAST/SharedLibs/libUtilsRetina.so: CMEPDA-GLAST/SharedLibs/UtilsRetina.o
	g++ -shared -fPIC `root-config --libs --cflags` -o CMEPDA-GLAST/SharedLibs/libUtilsRetina.so CMEPDA-GLAST/SharedLibs/UtilsRetina.o
	rm CMEPDA-GLAST/SharedLibs/UtilsRetina.o
