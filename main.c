/* 
*   Author : Vishnu Gowda H P
*   Date : 23/09/25
*   Description : The program that help user to hide the secret in an image(encoding) and decode it to extract the secret messages 
*   Prerequisits : Arrays, Structures, Functions, Files concepts, Enums, Encryption - Decryption and 2D array concepts
*   Sample input : ./a.out -e beautiful.bmp secret.txt
*    Sample output : Status : Encoding is chosen for action
*                    Encoding started...
*                    Status : .bmp file found...
*                    Status : .txt file found...
*                    Reading and Validation compeleted successfully...
*                    Status : File opened successfully...
*                    Status : Capacity check successfull and sufficient space detected...!
*                    Status : Header copied successfully...
*                    Do you want to explicity enter the magic string...?(Y/N)
*                    Y
*                    Enter the magic string :
*                    Emertxe
*                    Status : Magic string encoded successfully...!
*                    Status : Extension size encoded successfully
*                    Status : Extention size encoded successfully...!
*                    Status : Extention encoded successfully...!
*                    Status : Secret file size encoded Successfully...!
*                    Status : Secret file data encoded Successfully...!
*                    Status : Rest of the data encoded Successfully...!
*                    Yay!!!...Encoding is successfully Completed...!
*                    Status : Decoding is chosen for action
*    Sample input : ./a.out -d stego.bmp
*    Sample output : Status : Decoding started...
*                    Status : Reading and Validation Completed successfully...
*                    Status : File opened successfully...
*                    Please enter the magic string : Emertxe
*                    Status : Magic string is getting decoded...
*                    Status : Data is decoded from image..!
*                    Status : Magic string decoded successfully...!
*                    Status : Magic string Matching...!
*                    Status : Data is decoded from image..!
*                    Here you go the Secret File name -> decode.txt !!!
*                    Status : The Secret file extension decoded successfully...!
*                    Status : Data is decoded from image..!
*                    Status : Secret file data decoded successfully ...!
*                    Yay!!!...Decoding is successfully Completed...!
*    Sample Input :  ./a.out -e 
*    Sample Outnput: Oops.., For encoding these many arguements aren't enough..!
*                    (Hint : For Encoding try - ./a.out -e <image.bmp> <secret.txt> <stego.bmp>(optional))
*    Sample Input :  ./a.out
*    Sample Outnput: Oops.., No operation specified!
*                    (Hint : For Encoding try - ./a.out -e <image.bmp> <secret.txt> <stego.bmp>(optional)
*                            For Decoding try - ./a.out -d <stego.bmp> <output.txt>(optional))
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "decode.h"
#include "encode.h"
#include "types.h" 

OperationType check_operation_type(char *argv[]);
//------------------------------ Main function ------------------------------------------------------------
/* 
 * Main function calls the functio to check the operation type
 * Any error or warnings will be prompted throughmain function
 * Checks the arguments required for process
 */
int main(int argc,char *argv[])
{
    if(argc < 2)
    {
        printf("Oops.., No operation specified!\n(Hint : For Encoding try - ./a.out -e <image.bmp> <secret.txt> <stego.bmp>(optional)\n        For Decoding try - ./a.out -d <stego.bmp> <output.txt>(optional)\n");
        return e_failure;
    }
    if(argc < 3)
    {
        if(strcmp(argv[1],"-e")==0)
        {
            printf("Oops.., For encoding these many arguements aren't enough..!\n(Hint : For Encoding try - ./a.out -e <image.bmp> <secret.txt> <stego.bmp>(optional)\n");
            return e_failure;
        }
        else if(strcmp(argv[1],"-d")==0)
        {
            printf("Oops.., For decoding these many arguements aren't enough..!\n(Hint :  For Decoding try - ./a.out -d <stego.bmp> <output.txt>(optional))\n");
            return e_failure;
        }
        else
        {
            printf("Oops.., Something went wrong...!\n(Hint : For Encoding try - ./a.out -e <image.bmp> <secret.txt> <stego.bmp>(optional)\n        For Decoding try - ./a.out -d <stego.bmp> <output.txt>(optional)\n");
            return e_failure;
        }
    }

    if(argc >= 3)
    {
        OperationType res = check_operation_type(argv);
        if(res == e_encode)
        {
            if(argc==3)
            {
                
                printf("Oops.., Did you forget secret messege file..Provide a secret messege in a file\n(Hint : For Encoding try - ./a.out -e <image.bmp> <secret.txt> <stego.bmp>(optional)\n");
                return e_failure;
            }
            printf("Encoding started...\n");
            sleep(1);
            EncodeInfo enc;
            if(read_and_validate_encode_args(argv, &enc)==e_success)
            {
                printf("Reading and Validation compeleted successfully...\n");
                sleep(1);
                if(do_encoding(&enc)== e_success)
                {
                    printf("Yay!!!...Encoding is successfully Completed...!\n");
                }
                else
                {
                    printf("Oops.. Unfortunately encoding failed...!\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Oops.. Reading and Validation Failed...\n");
            }
        }
        else if( check_operation_type(argv) == e_decode)
        {
            printf("Status : Decoding is chosen for action\n");
            sleep(1);
            printf("Status : Decoding started...\n");
            sleep(1);
            if(argc ==  2)
            {
                printf("Oops... Arguements isn't enough...\nTry again...\n");
                return e_failure;
            }
            DecodeInfo dec;
            if(read_and_validate_decode_args(argv, &dec)==e_success)
            {
                printf("Status : Reading and Validation Completed successfully...\n");
                sleep(1);
                if(do_decoding(&dec)== e_success)
                {
                    printf("Yay!!!...Decoding is successfully Completed...!\n");
                    sleep(1);
                }
                else
                {
                    printf("Oops.. Unfortunately decoding failed...!\n");
                    sleep(1);
                    return e_failure;
                }
            }
            else
            {
                printf("Oops.. Reading and Validation Failed...\n");
                sleep(1);
            } 
        }
    }
    
}



//----------------------- Check Operation Type-----------------------------
/* Get input and stores as arguments
 * Input: argv[1]
 * Output: status(e_success, e_failure,etc..)
 * Description: This function compares the argv[1] with -e/-d and decides            
 *              whether user as chosen encoding or decoding
 */
OperationType check_operation_type(char *argv[])
{
    if(!(strcmp(argv[1],"-e")))
    {
        printf("Status : Encoding is chosen for action\n");
        sleep(1);
        return e_encode;
    }
    else if(!(strcmp(argv[1],"-d")))
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
//------------------------------------ EOF--------------------------------------------------------