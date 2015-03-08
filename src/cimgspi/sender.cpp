#include <iostream>
#include <string.h>
#include "jpeg_func.h"
#include "RpiServerMain.h"

#include <list>
#include <algorithm>
#include "sobel.h"
#include "gauss.h"
#include "dilate.h"
#include "erode.h"
#include "hyst.h"
#include "output.h"

#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <time.h>
#include <linux/ioctl.h>

#include "logilib.h"

using namespace std;

// definitions of constants
#define REG_ADDR 0x0800
#define FIFO_CMD_ADDR 0x0200

#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240

#define LINE_BURST 1
#define FILTER_SIZE 6

// variables

jbyteArray finalResult;
int inputImageSize;
jobject result_listener;

//////////////////////////////////////////////////////////////////////////////
// prototype methods
//////////////////////////////////////////////////////////////////////////////

unsigned char* as_unsigned_char_array(JNIEnv *env, jbyteArray array);
jbyteArray as_byte_array(JNIEnv *env, unsigned char* buf, int len);

unsigned short int *filtering_chain_builder(list<Filter*> input_filter);
void print_array(unsigned short int *array, int size);
void place(list<Filter*> filtering_chain, Filter *new_filter, const char *source);

unsigned char *apply_filter_logifpga(unsigned char *in_image, char **command);
char **to_array(JNIEnv *env, jstring command_message);

//////////////////////////////////////////////////////////////////////////////
// implementation
//////////////////////////////////////////////////////////////////////////////

// JNI FUNCTION
JNIEXPORT void JNICALL Java_RpiServerMain_sendImageForProcessing(JNIEnv *env, jclass cls, jbyteArray image, jstring command){
  cout<<"So far so good... I'm in C++"<<endl;
  char **filters = to_array(env, command);

  //prepare to read future result
  unsigned char* buf = as_unsigned_char_array(env, image);
  unsigned char *buf_res;

  // process image in FPGA
  buf_res = apply_filter_logifpga(buf, filters);  
  // fetch result 
  finalResult = as_byte_array(env, buf_res, 320*240); // full size image

  //prepare class to return
  jclass listener_class = env->GetObjectClass(result_listener);
  jmethodID method = env->GetMethodID(listener_class, "onMessageRecieved", "(Ljava/lang/String;)V");
  jstring string = env->NewStringUTF("Hello");

  env->CallVoidMethod(result_listener, method, string);

  //cleanup
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

unsigned char * apply_filter_logifpga(unsigned char *in_image, char **command){

  unsigned char * inputImage ;
  long start_time, end_time ;
  double diff_time ;
  struct timespec cpu_time ;
  FILE * jpeg_fd ;
  FILE * raw_file ;
  int i,j, res ;
  unsigned int pos = 0 ;
  
  unsigned short int cmd_buffer[4];
  static unsigned char image_buffer[(320*240)] ; //monochrome frame buffer

  unsigned short fifo_state, fifo_data ;
  if(logi_open() < 0){
    printf("Error openinglogi \n");
    return NULL;  
  }

  // uncomment to save image locally 
  /*
  jpeg_fd  = fopen("./grabbed_frame.jpg", "w");

  if(jpeg_fd == NULL){
    perror("Error opening output file");
    exit(EXIT_FAILURE);
  }
  */

  // read the image in local memory & decompress
  res = read_jpeg_mem(in_image, inputImageSize, &inputImage);
  
  // create filter order
  list<Filter*> input_filter;

  for(int i = 0; i < 5 - 2; i++){
    if(command[i] == NULL) break;
    else if(strcmp(command[i], "sobel") == 0)
      input_filter.push_back(new Sobel());
    else if (strcmp(command[i], "gauss") == 0)
      input_filter.push_back(new Gauss());
    else if (strcmp(command[i], "erode") == 0)
      input_filter.push_back(new Erode());
    else if (strcmp(command[i], "dilate") == 0)
      input_filter.push_back(new Dilate());
    else if (strcmp(command[i], "hyst") == 0)
      input_filter.push_back(new Hyst());
  }  

  unsigned short int *reg_buffer = filtering_chain_builder(input_filter);

  // uncomment to view command buffer
  //print_array((unsigned short int*)reg_buffer, FILTER_SIZE);

  //sometimes it goes unsigned int* ?!?
  logi_write((unsigned char*)reg_buffer, 12, REG_ADDR);
  cout<<"Issuing reset to fifo"<<endl;

  cmd_buffer[1] = 0; 
  cmd_buffer[2] = 0 ;
  
  logi_write((unsigned char*)cmd_buffer, 6, FIFO_CMD_ADDR);
  logi_read((unsigned char*)cmd_buffer, 6, FIFO_CMD_ADDR);
  cout<<"fifo size: "<<cmd_buffer[0]<<", free: "<<cmd_buffer[1]<<", available: "<<cmd_buffer[2]<<endl;
    
  clock_gettime(CLOCK_REALTIME, &cpu_time);
  start_time = cpu_time.tv_nsec ;

  for(i = 0 ; i < IMAGE_HEIGHT ; i +=LINE_BURST){
    logi_write((unsigned char*)&inputImage[(i*IMAGE_WIDTH)], IMAGE_WIDTH*LINE_BURST, 0x0000);
    
    do{
      logi_read((unsigned char*)cmd_buffer, 6, FIFO_CMD_ADDR);
    }while((cmd_buffer[2]*2) < IMAGE_WIDTH*LINE_BURST);
    
    logi_read((unsigned char*)&image_buffer[(i*IMAGE_WIDTH)], IMAGE_WIDTH*LINE_BURST, 0x0000);
  }
  
  clock_gettime(CLOCK_REALTIME, &cpu_time);
  end_time = cpu_time.tv_nsec ;
  diff_time = end_time - start_time ;
  diff_time = diff_time/1000000000 ;
  
  printf("transffered %d bytes in %f s : %f B/s \n", IMAGE_WIDTH * IMAGE_HEIGHT, diff_time, (IMAGE_WIDTH * IMAGE_HEIGHT)/diff_time);
  
  // uncomment to save image locally
  //write_jpegfile(image_buffer, 320, 240, jpeg_fd, 100);
  //fclose(jpeg_fd);

  logi_close();

  return image_buffer;
  
}


void place(list<Filter*> filtering_chain, Filter *new_filter, const char *source){
  list<Filter*>::iterator current = filtering_chain.begin();
  for(; current != filtering_chain.end(); current++){
    if(strcmp((*current)->getName(), new_filter->getName()) == 0){
      (*current)->set_source_id(source);
    }
  }
}

unsigned short int *filtering_chain_builder(list<Filter*> input_filter){

  static unsigned short int buffer[FILTER_SIZE] = {0};
  list<Filter*> filtering_chain;

  // build the filtering chain
  filtering_chain.push_back(new Gauss());
  filtering_chain.push_back(new Sobel());
  filtering_chain.push_back(new Erode());
  filtering_chain.push_back(new Dilate());
  filtering_chain.push_back(new Hyst());
  filtering_chain.push_back(new Output());

  list<Filter*>::iterator current = input_filter.begin();
  list<Filter*>::iterator previous = current++;


  // set initial to fifo
  place(filtering_chain, *previous, "fifo");

  for(current; current != input_filter.end(); current++, previous++){
    place(filtering_chain, *current, (*previous)->getName());
  }
  //set final to output
  place(filtering_chain, new Output(), (*previous)->getName());

  // print result
  current = filtering_chain.begin();
  int j = 0;
  for(current; current != filtering_chain.end(); current++){
    buffer[j++] = (*current)->get_source();
  }
  
  // build array
  return buffer;
}

void print_array(unsigned short int *array, int size){
  for(int i = 0; i < size; i++){
    cout<<i<<". Element: "<<array[i]<<endl;
  }
}

char **to_array(JNIEnv *env, jstring command_message){
  const char *command_string_const = env->GetStringUTFChars(command_message, JNI_FALSE);
  char* command_string = new char[strlen(command_string_const)+1];
  strcpy(command_string, command_string_const);
  // split by tokens
  char* tmp = strtok(command_string, " ");
  int array_size = 0;

  static char *result[5] = {NULL};

  // count number of commands
  while(tmp != NULL){
    result[array_size] = tmp;
    tmp = strtok(NULL, "");
    array_size++;
  }

  return result;

}
