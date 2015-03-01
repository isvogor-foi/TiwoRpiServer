#include <iostream>
#include "jpeg_func.h"
#include "RpiServerMain.h"

using namespace std;
unsigned char* as_unsigned_char_array(JNIEnv *env, jbyteArray array);
jbyteArray as_byte_array(JNIEnv *env, unsigned char* buf, int len);

void image_processor(unsigned char *in_image);
jbyteArray finalResult;
int inputImageSize;
jobject result_listener;

// JNI FUNCTION
JNIEXPORT void JNICALL Java_RpiServerMain_sendImageForProcessing(JNIEnv *env, jclass cls, jbyteArray image, jstring command){
  cout<<"So far so good... I'm in C++"<<endl;
  cout<<"I recieved string: "<<command<<endl;  

  //prepare to read future result
  unsigned char* buf = as_unsigned_char_array(env, image);
  finalResult =  as_byte_array(env, buf, env->GetArrayLength(image));

  // process image in FPGA
  image_processor(buf);
  // done call listener...

  // call listener
  
  jclass listener_class = env->GetObjectClass(result_listener);

  jmethodID method = env->GetMethodID(listener_class, "onMessageRecieved", "(Ljava/lang/String;)V");
  jstring string = env->NewStringUTF("Hello");

  env->CallVoidMethod(result_listener, method, string);
  env->DeleteGlobalRef(result_listener);

}

// JNI FUNCTION
JNIEXPORT jbyteArray JNICALL Java_RpiServerMain_getProcessedImage(JNIEnv *env, jclass cls){
    return finalResult;
}

JNIEXPORT void JNICALL Java_RpiServerMain_setImageReadyListener(JNIEnv *env, jclass cls, jobject listener){
    result_listener = env->NewGlobalRef(listener);
}

// verified - works fine
unsigned char* as_unsigned_char_array(JNIEnv *env, jbyteArray array) {
    int len = env->GetArrayLength (array);
    inputImageSize = len;
    cout<<"Array size: "<<len<<endl;
    unsigned char* buf = new unsigned char[len];
    env->GetByteArrayRegion (array, 0, len, reinterpret_cast<jbyte*>(buf));

    return buf;
}


// verified - works fine
jbyteArray as_byte_array(JNIEnv *env, unsigned char* buf, int len) {
    jbyteArray array = env->NewByteArray (len);
    env->SetByteArrayRegion (array, 0, len, reinterpret_cast<jbyte*>(buf));
    return array;
}

void image_processor(unsigned char *in_image){
    unsigned char *inputImage ;
    unsigned char image_buffer[(320*240)] ;
    FILE * jpeg_fd ;

    cout<<"Started fine..."<<endl;

    jpeg_fd  = fopen("./output.jpg", "w");
    if(jpeg_fd == NULL){
        perror("Error opening output file");
        return;
    }
    
    printf("output file openened \n");

    //int res = read_jpeg_file("./cimgs/beagle_qvga.jpg", &inputImage);
    int res = read_jpeg_mem(in_image, inputImageSize, &inputImage);


    // this works now...
    /*
    int res = read_jpeg_file("./cimgs/beagle_qvga.jpg", &inputImage);
    if(res < 0){
        perror("Error opening input file");
    }

    cout<<"Opened fine... Saving...";
    */
    write_jpegfile(inputImage, 320, 240, jpeg_fd, 100);
    fclose(jpeg_fd);
    
    cout<<"Done!"<<endl;
}