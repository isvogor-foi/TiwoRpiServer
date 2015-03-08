//#include <jpeglib.h>

int write_jpegfile(unsigned char * frame, unsigned short width, unsigned short height, FILE * fd, int quality);
int write_jpegmem(char * frame, unsigned short width, unsigned short height, unsigned short nbChannels, unsigned char **outbuffer, long unsigned int *outlen, int quality);
int read_jpeg_file( char *filename, unsigned char ** buffer);
int read_jpeg_mem(unsigned char *in_image, int image_size, unsigned char ** buffer);

//void jpeg_mem_src (j_decompress_ptr cinfo, void* buffer, long nbytes);
