#include <iostream>
#include <stdio.h>
#include "jpeg_func.h"
#include "../Sender.h"
using namespace std;

unsigned char* as_unsigned_char_array(JNIEnv *env, jbyteArray array);
jbyteArray as_byte_array(JNIEnv *env, unsigned char* buf, int len);

JNIEXPORT void JNICALL Java_Sender_Sender(JNIEnv *env, jobject obj, jbyteArray image, jint check){
  cout<<"So far so good... I'm in C++"<<endl;
  cout<<"I recieved integer: "<<check<<endl;  

  unsigned char* buf = as_unsigned_char_array(env, image);

  cout<<"Done"<<endl;
}


unsigned char* as_unsigned_char_array(JNIEnv *env, jbyteArray array) {
    int len = env->GetArrayLength (array);
    unsigned char* buf = new unsigned char[len];
    env->GetByteArrayRegion (array, 0, len, reinterpret_cast<jbyte*>(buf));
    return buf;
}

jbyteArray as_byte_array(JNIEnv *env, unsigned char* buf, int len) {
    jbyteArray array = env->NewByteArray (len);
    env->SetByteArrayRegion (array, 0, len, reinterpret_cast<jbyte*>(buf));
    return array;
}
/*
int  main(int argc, char *argv[]){
	unsigned char * inputImage ;
	unsigned char image_buffer[(320*240)] ;
	FILE * jpeg_fd ;

	cout<<"Started fine..."<<endl;

	jpeg_fd  = fopen("./grabbed_frame.jpg", "w");
	if(jpeg_fd == NULL){
		perror("Error opening output file");
		return 1;
	}
	
	printf("output file openened \n");
	printf("loading input file : %s \n", argv[1]);

	int res = read_jpeg_file( argv[1], &inputImage);
	if(res < 0){
		perror("Error opening input file");
	}

	cout<<"Opened fine... Saving...";

	write_jpegfile(inputImage, 320, 240, jpeg_fd, 100);

	cout<<"Done!"<<endl;

	return 0;
}
*/



