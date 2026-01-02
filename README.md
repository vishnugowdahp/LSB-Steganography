📋 Project Overview
A sophisticated C-based steganography application that securely hides secret messages within BMP images using Least Significant Bit (LSB) encoding. This tool allows users to embed confidential data into images and extract it later while maintaining the image's visual integrity.

✨ Key Features
🔒 Encoding (Data Hiding)
Secret Embedding: Hide text files within BMP images

Magic String Protection: Optional custom passphrase for security

Automatic Capacity Check: Verifies image has sufficient space

Extension Preservation: Maintains original file extensions

Header Integrity: Preserves BMP image headers

🔓 Decoding (Data Extraction)
Secure Extraction: Retrieve hidden data with proper authentication

Magic String Verification: Validates passphrase before extraction

File Reconstruction: Automatically recreates original files

Error Handling: Graceful failure for corrupted/invalid images

🛡️ Security & Validation
Input Validation: Ensures proper file formats (.bmp, .txt)

Capacity Verification: Checks if image can store the secret

Magic String Authentication: Customizable security layer

Data Integrity: Maintains original data structure
