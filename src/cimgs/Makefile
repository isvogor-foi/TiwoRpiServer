#Compiler to use
#fCC=g++
#Cross-compile for ARM (Raspberry)
ARMCPP=g++
#Compiler Flags
#Linker Flags
CFLAGS = -c
LDFLAGS= -L/usr/lib -ljpeg 


all: libSenderImpl
#JNI

#image_demo: image_demo.o jpeg_func.o
#	$(CC) -o image_demo image_demo.o jpeg_func.o $(LDFLAGS) 
	
#image_demo.o: image_demo.cpp
#	$(CC) $(CFLAGS) image_demo.cpp

#jpeg_func.o: jpeg_func.cpp#
#	$(CC) $(CFLAGS) jpeg_func.cpp

# for raspberry

#main: main.so jpeg_func.o
#	$(ARMCPP) -o libSenderImpl.so main.so jpeg_func.o $(LDFLAGS) 
#	cp libSenderImpl.so ./

#main.so: main.cpp
#	$(ARMCPP) -o main.so -lc -shared -I/usr/lib/jvm/default-java/include -I/usr/lib/jvm/default-java/include/linux main.cpp -fPIC

#jpeg_func.o: jpeg_func.cpp
#	$(ARMCPP) $(CFLAGS) jpeg_func.cpp

#libSenderImpl.so -lc -shared -I/usr/lib/jvm/default-java/include -I/usr/lib/jvm/default-java/include/linux sender.cpp -fPIC

libSenderImpl: jpeg_func.o
	$(ARMCPP) -o libSenderImpl.so jpeg_func.o $(LDFLAGS) -fPIC -shared -I/usr/lib/jvm/default-java/include -I/usr/lib/jvm/default-java/include/linux sender.cpp 


jpeg_func.o: jpeg_func.cpp
	$(ARMCPP) $(CFLAGS) jpeg_func.cpp -fPIC


# clean

clean:
	rm -rf *.o main
	rm -rf *.so main
	rm -rf *.out main