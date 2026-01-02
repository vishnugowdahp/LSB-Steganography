#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "types.h"
#include "decode.h"

//---------------------------- Read and validate function-----------------------------
/* Check the passed files
 * Input: argv[] and struct decInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function compares the argv[2] and argv[3](if given)           
 *               with the .bmp and .txt respectively 
 */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
  if(strstr(argv[2],".bmp")!=NULL)
  {
    decInfo->stego_image_fname=argv[2];
  }
  else
  {
    printf("Oops..!.bmp file isn't found...!\n");
    return e_failure;
  }

  if(argv[3]!=NULL)
  {
    strtok(argv[3], ".");
    strcpy(decInfo->secret_fname,argv[3]);
  }
  else
  {
    strcpy(decInfo->secret_fname,"decode");
  }
  return e_success;

}

//------------------ do_decoding function--------------------------------------
/* Calling of all required functions
 * Input:  struct decInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function calls all functions required for 
 *              decoding and plays the main role in hiding the data
 */
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_files_decode(decInfo)==e_success)
    {
      printf("Status : File opened successfully...\n");
      sleep(1);
    }
    else
    {
      printf("Oops... File couldn't be accessed..!\n");
      sleep(1);
      return e_failure;
    }
    
    printf("Please enter the magic string : ");
    scanf("%s",decInfo->magic_string);

    char mag_str[50];
    if(decode_magic_string( mag_str, strlen(decInfo->magic_string),decInfo) !=e_success)
    {
      return e_failure;
    }
    if(strcmp(mag_str,decInfo->magic_string) == 0)
    {
      printf("Status : Magic string Matching...!\n");
      sleep(1);
    }
    else
    {
      printf("Oops.. The magic string isn't matching...\n");
      sleep(1);
      return e_failure;
    }
    if(decode_secret_file_extn( decInfo->fptr_stego_image,decInfo )==e_success)
    {
      printf("Status : The Secret file extension decoded successfully...!\n");
      sleep(1);
    }
    else
    {
      printf("Oops... The secret file extension decoding failed..!\n");
      sleep(1);
      return e_failure;
    }
    if(decode_secret_file_data(decInfo) == e_success)
    {
      printf("Status : Secret file data decoded successfully ...!\n");
      sleep(1);
    }
    else
    {
      printf("Status : Secret file data decoding failed..!\n");
      sleep(1);
      return e_failure;
    }
    return e_success;
}

//------------------------------- Open files function------------------------------
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: decInfo
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files_decode(DecodeInfo *decInfo)
{
  
  decInfo->fptr_stego_image = fopen( decInfo->stego_image_fname,"rb");
  if(decInfo->fptr_stego_image == NULL)
  {
    perror("fopen");
    return e_failure;
  }
  return e_success;
}

//-------------------------------Encode the magic string------------------------------------
/* Decode the magic string
 * Input: The magic string , it's size and Struct DecodeInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function calls the function to decode the magic string from the image
 */
Status decode_magic_string(char * magic_string , int size,DecodeInfo *decInfo)
{
  printf("Status : Magic string is getting decoded...\n");
  sleep(1);
  fseek(decInfo->fptr_stego_image,54,SEEK_SET);
  if(decode_data_from_image(magic_string, size,decInfo)==e_success)
  {
    printf("Status : Magic string decoded successfully...!\n");
    sleep(1);
    magic_string[size] = '\0';
    // printf("Here you go the Magic String --> %s\n", magic_string);
    sleep(1);
  }
  else
  {
    printf("Oops...! Decoding the magic string was unsuccessful..!\n");
    sleep(1);
    return e_failure;
  }
  return e_success;
}

//-------------------------------Decode the data from image------------------------------------
/* Decode the data in image
 * Input: The data , it's size and Struct DecodeInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function recovers the data from the image by calling a function that recovers and returns the decoded bytes
 */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
  
  char buffer[8];

  for(int i=0;i<size;i++)
  {
    fread(buffer,8,1,decInfo->fptr_stego_image);
    data[i]= decode_byte_from_lsb( buffer);
  }
  printf("Status : Data is decoded from image..!\n");
  sleep(1);
  return e_success;
}

//-------------------------------Decode from byte from lsb------------------------------------
/* Decode bit by bit
 * Input: The image buffer
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that recovers and returns the decoded bytes using bitwise operations
 */
char decode_byte_from_lsb( char *image_buffer)
{
  char data=0;
  for(int i=0;i<8;i++)
  {
    data = (data<<1) |(image_buffer[i] & 1) ;
  }
  return data;
}

//-------------------------------Decode the extension from lsb------------------------------------
/* Decode the secret file extension bit by bit
 * Input: The file extension and DecodeInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that recovers the secret file extn
 *              by calling function that decodes data from image
 */
Status decode_secret_file_extn( FILE*file_extn,DecodeInfo *decInfo )
{
    
  int extension = decode_exten_size(decInfo->fptr_stego_image, decInfo);
  decode_data_from_image(decInfo->secret_file_extn, extension , decInfo);
  decInfo->secret_file_extn[extension]='\0';

  strcat(decInfo->secret_fname, decInfo->secret_file_extn);
  decInfo->fptr_secret = fopen( decInfo->secret_fname,"w");
  printf("Here you go the Secret File name -> %s !!!\n", decInfo->secret_fname);
  sleep(1);
  return e_success;
}

//-------------------------------Decode the size of extn from lsb------------------------------------
/* Decode the size of extn bit by bit
 * Input: The DecodeInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that extracts the size of secret file extn and 
 *              reads those many bytes and decodes from image
 */
int decode_exten_size(FILE *file_extn, DecodeInfo *decInfo )
{
  int size;
  size = decode_size_from_lsb(file_extn);
  return size;
}
  

//-------------------------------Decode the size from lsb------------------------------------
/* Decode the size of file bit by bit
 * Input: The size and Decode Info
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that recovers the size of file using bitwise operations
 */
int decode_size_from_lsb(FILE *file_extn)
{
  char buffer[32];
  fread(buffer , 32 , 1 ,file_extn);
  int size=0;
  for(int i=0;i<32;i++)
  {
    size = (size<<1) |(buffer[i] & 1);
  }
  return size;
}

//-------------------------------Decode the secret message from lsb------------------------------------
/* Decode the secret file message bit by bit
 * Input: The DecodeInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that extracts the size of secret file and 
 *              reads those many bytes and decodes from the image
 */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
  
  int size = decode_size_from_lsb(decInfo->fptr_stego_image);
     
  char *buffer = (char *)malloc(size);
  if(buffer == NULL)
  {
    return e_failure;
  }
      
  decode_data_from_image(buffer, size, decInfo);
  FILE *fptr = fopen(decInfo->secret_fname, "w");
  if(fptr == NULL)
  {
    perror("");
    free(buffer);
    return e_failure;
  }
  fwrite(buffer,size,1,fptr);
  fclose(fptr);
  free(buffer);
  return e_success;
}