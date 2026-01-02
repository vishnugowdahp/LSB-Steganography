#ifndef DECODE_H
#define DECODE_H
#include "types.h"
#include <unistd.h>
/* Structure with decode members */
typedef struct Decodeinfo
{
    char *stego_image_fname;//stego file name
    FILE *fptr_stego_image;//stego file pointer

    char secret_fname[20];//secret file name
    FILE*fptr_secret;//secret file pointer
    char secret_file_extn[30];//secret file extension

    char magic_string[100]; //magic string
}DecodeInfo;

/* Check operation type */
OperationType check_operation_type(char **);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *[], DecodeInfo *);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *);

/* Perform the decoding */
Status do_decoding(DecodeInfo *);

/* Recover Magic String */
Status  decode_magic_string(char*, int,DecodeInfo*);

/* Decode function which performs decoding */
Status decode_data_from_image(char*, int,DecodeInfo*);

/* Decode a byte from LSB of image data array */
char decode_byte_from_lsb(char*);

/* Decode secret file extenstion */
int decode_exten_size(FILE*, DecodeInfo* );

/* Decode a byte from LSB of image data array */
int decode_size_from_lsb(FILE *);

/* Decode secret file extenstion */
Status decode_secret_file_extn( FILE*,DecodeInfo *);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *);

#endif