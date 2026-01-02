#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/*  
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */
#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct EncodeInfo
{
    /* Source Image info */
    char *src_image_fname; //source file name
    FILE *fptr_src_image;// source file pointer
    uint image_capacity;// image capacity
    char image_data[MAX_IMAGE_BUF_SIZE];//a buffer for image data

    /* Secret File Info */
    char *secret_fname;//secret file name
    FILE *fptr_secret;//pointer pointing to secretfile 
    char extn_secret_file[MAX_FILE_SUFFIX];//extension of secret file
    char secret_data[MAX_SECRET_BUF_SIZE];//secret file data
    long size_secret_file;//size of the secret file
    char magic_string[50];//the magic string data

    /* Stego Image Info */
    char *stego_image_fname;//stego image file name
    FILE *fptr_stego_image;//stego imagefile pointer

} EncodeInfo;


/*------------------------------- Encoding function prototype -----------------------------------*/

/* Check operation type */
OperationType check_operation_type(char **argv);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(EncodeInfo *);

/* Store Magic String */
Status encode_magic_string(char * magic_string , int size,EncodeInfo *encInfo);

/* Encode extension of Magic String */
Status encode_extn_size(int size,  EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size,EncodeInfo *encInfo);//for characters

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);// for integers

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/* Encode the size to lsb of src file */
Status encode_size_to_lsb(int size,EncodeInfo *encInfo);

#endif
//------------------------------ EOF --------------------------------------