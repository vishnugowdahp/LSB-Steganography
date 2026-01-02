#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

//---------------------------Bmp file size extraction function---------------------
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 *              and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

//------------------------------- Open files function------------------------------
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
        sleep(1);
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
        sleep(1);
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
        sleep(1);
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    } 

    // No failure return e_success
    return e_success;
}


//---------------------------- Read and validate function-----------------------------
/* Check the passed files
 * Input: argv[] and struct encInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function compares the argv[2],argv[3] and argv[4](if given)           
 *               with the .bmp , .txt and .bmp respectively 
 */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")!= NULL)
    {
        printf("Status : .bmp file found...\n");
        sleep(1);
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("Oops..! Upload a .bmp file...!\n");
        sleep(1);
        return e_failure;
    }
    if(strstr(argv[3],".")!=NULL)
    {
        printf("Status : .txt file found...\n");
        sleep(1);
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, strstr(argv[3],"."));
    }
    else
    {
        printf("Oops..! Upload a .txt file...!\n");
        sleep(1);
        return e_failure;
    }
    if(argv[4]!=NULL)
    {
        if(strstr(argv[4],".bmp")!=e_success)
        {
            printf("Status : Output file detected...");
            sleep(1);
            encInfo->stego_image_fname=argv[4];
        }
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }
    return e_success;
}

//------------------ do_encoding function--------------------------------------
/* Calling of all required functions
 * Input:  struct encInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function calls all functions required for 
 *              encoding and plays the main role in hiding the data
 */
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf("Status : File opened successfully...\n");
        sleep(1);
    }
    else
    {
        printf("Error : File open failed...\n");
        sleep(1);
        return e_failure;
    }

    if(check_capacity(encInfo) == e_success)
    {
        printf("Status : Capacity check successfull and sufficient space detected...!\n");
        sleep(1);
    }
    else
    {
        printf("Oops... The capacity check failed with less space detection...!\n");
        sleep(1);
        return e_failure;
    }

    if(copy_bmp_header(encInfo) == e_success)
    {
        printf("Status : Header copied successfully...\n");
        sleep(1);
    }
    else
    {
        printf("Oops... The header copy process failed...!\n");
        sleep(1);
        return e_failure;
    }
    
    int i=0;
    while(i++<3)
    {
        char op;
        printf("Do you want to explicity enter the magic string...?(Y/N)\n");
        scanf(" %c",&op);
        if(op=='Y' || op=='y')
        {
            printf("Enter the magic string :\n");
            scanf(" %[^\n]",encInfo->magic_string);
            break;
        }
        else if(op=='N' || op=='n')
        {
            strcpy(encInfo->magic_string,MAGIC_STRING);
            break;
        }
        else
        {
            printf("Error : Invalid input..!\nPlease enter only Y/N..\n");
        }
    }
    int length = strlen(encInfo->magic_string);
    if(encode_magic_string(encInfo->magic_string, length, encInfo) == e_success)
    {
        printf("Status : Magic string encoded successfully...!\n");
        sleep(1);
    }
    else
    {
        printf("Oops... Magic string encoding failed...!\n");
        sleep(1);
        return e_failure;
    }
    
    if(encode_extn_size(strlen(encInfo->extn_secret_file),encInfo) == e_success)
    {
        printf("Status : Extention size encoded successfully...!\n");
        sleep(1);
    }
    else 
    {
        printf("Oops... Extention size encoding failed...!\n");
        sleep(1);
        return e_failure;
    }
    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo)==e_success)
    {
        printf("Status : Extention encoded successfully...!\n");
        sleep(1);
    }
    else
    {
        printf("Oops... Extention encoding failed...!\n");
        sleep(1);
        return e_failure;
    }
    if( encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
    {
        printf("Status : Secret file size encoded Successfully...!\n");
        sleep(1);
    }
    else
    {
        printf("Oops... Secret file size encoding failed...!\n");
        sleep(1);
        return e_failure;
    }
    if(encode_secret_file_data(encInfo)==e_success)
    {
        printf("Status : Secret file data encoded Successfully...!\n ");
        sleep(1);
    } 
    else
    {
        printf("Oops... Secret file data encoding failed...!\n ");
        sleep(1);
        return e_failure;
    }
    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
        printf("Status : Rest of the data encoded Successfully...!\n ");
        sleep(1);
    }
    else
    {
        printf("Oops... Rest of the data encoding failed...!\n ");
        sleep(1);
        return e_failure;
    }
    
    return e_success;  
}

//-------------------------------Encode the extension size------------------------------------
/* Get the extension size
 * Input: size and struct encInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function extracts the extension size and encodes the extension in stego.bmp file
 */ 
Status encode_extn_size(int size, EncodeInfo *encInfo)
{
    if (encode_size_to_lsb(size, encInfo) == e_success)
    {
        printf("Status : Extension size encoded successfully\n");
        sleep(1);
        return e_success;
    }
    else
    {
        fprintf(stderr, "Oops...Failed to encode extension size...!\n");
        sleep(1);
        return e_failure;
    }
}

//-------------------------------Check Capacity------------------------------------
/* Verify the capacity of stego.bmp
 * Input: struct encInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function checks the size of secret file and size of src.bmp file 
 *              And compares if the size of the .bmp is more or equal to required file size
 */
Status check_capacity(EncodeInfo *encInfo)
{

    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    long required_size = 16 + 32 + 32 + 32 + (encInfo->size_secret_file * 8);
    if (encInfo->image_capacity >= required_size)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

//-------------------------------Get the file size------------------------------------
/* Obtain the file size
 * Input: File pointer
 * Output: status(e_success, e_failure,etc..)
 * Description: This function moves the file pointer to end and extracts the file size
 */
uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

//-------------------------------Copy the header------------------------------------
/* Copy src header to stego header
 * Input: Struct EncodeInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function copies the header of src.bmp to 
 *              a buffer of size 54 and copies the same to stego.bmp
 */
Status copy_bmp_header(EncodeInfo *encInfo)
{
    rewind(encInfo -> fptr_src_image);
    char buffer[54];
    fread(buffer,54,1, encInfo->fptr_src_image);
    fwrite(buffer,54,1, encInfo->fptr_stego_image);
    return e_success;
}
   
//-------------------------------Encode the magic string------------------------------------
/* Hide the magic string
 * Input: The data , it's size and Struct EncodeInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function calls the function to hide the magic string in the image
 */
Status encode_magic_string(char *data, int size, EncodeInfo *encInfo)
{
    if (encode_data_to_image(data, size, encInfo) == e_success)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

//-------------------------------Encode the data to image------------------------------------
/* Hide the data in image
 * Input: The data , it's size and Struct EncodeInfo
 * Output: status(e_success, e_failure,etc..)
 * Description: This function hides the data in the image by calling a function that hides and returns the encoded bytes
 */
Status encode_data_to_image(char *data, int size,EncodeInfo *encInfo)
{
    for(int i = 0; i<size; i++)
    {
        fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(data[i],encInfo->image_data);
        fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

//-------------------------------Encode the byte to lsb------------------------------------
/* Encode bit by bit
 * Input: The data and a image buffer
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that hides and returns the encoded bytes using bitwise operations
 */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        int bit = (data >> (7 - i)) & 1;
        image_buffer[i] = (image_buffer[i] & 0xFE)|bit;
    }
    return e_success;
}

//-------------------------------Encode the size to lsb------------------------------------
/* Encode the size of file bit by bit
 * Input: The size and Encode Info
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that hides the size of file using bitwise operations
 */
Status encode_size_to_lsb(int size, EncodeInfo *encInfo)
{
    char buffer[32];
    if (fread(buffer, 32, 1, encInfo->fptr_src_image) != 1)
    {
        return e_failure;
    }
    for (int i = 0; i<32; i++)
    {
        int bit = (size >> (32-1-i)) & 1; 
        buffer[i] = (buffer[i] & 0xFE)|bit;
    }
    if (fwrite(buffer,32, 1, encInfo->fptr_stego_image) != 1)
    {
        return e_failure;
    }
    return e_success;
}

//-------------------------------Encode the extension to lsb------------------------------------
/* Encode the size of secret file extension bit by bit
 * Input: The file extension and Encode Info
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that hides the secret file extn
 *              by calling function that enocodes data in image
 */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn, strlen(file_extn),encInfo);
    return e_success;
}

//-------------------------------Encode the size to lsb------------------------------------
/* Encode the size of secret file bit by bit
 * Input: The size and Encode Info
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that hides the size of secret 
 *              file using bitwise operations
 */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
   encode_size_to_lsb(file_size,encInfo);
   return e_success;

}

//-------------------------------Encode the secret message to lsb------------------------------------
/* Encode the secret file message bit by bit
 * Input: The Encode Info
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that extracts the size of secret file and 
 *              reads those many bytes and encodes to image
 */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char str[encInfo->size_secret_file];
    fseek(encInfo->fptr_secret,0,SEEK_SET);
    fread(str,encInfo->size_secret_file,1,encInfo->fptr_secret);
    encode_data_to_image(str,encInfo->size_secret_file,encInfo);
    return e_success;
}

//-------------------------------Copy the remaining data------------------------------------
/* Copy rest of the data
 * Input: The file pointer of src and stego files
 * Output: status(e_success, e_failure,etc..)
 * Description: This function that copies remaining data from src.bmp to stego.bmp
 */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,1,1,fptr_src)>0)
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}
//-------------------------------------- EOF -----------------------------------------------