# Encrypted Cloud File Storage System

## Project Overview
A secure cloud-based file storage system implemented in C that provides user authentication, file encryption using XOR cipher, and complete file management capabilities. Each user has isolated storage with password-protected access.

## Features
- **User Authentication**: Secure login and account creation system
- **File Encryption**: XOR-based encryption for secure file storage
- **File Upload**: Upload files with automatic encryption
- **File Download**: Download and decrypt files with correct key
- **File Deletion**: Password-protected file deletion
- **File Listing**: View all uploaded files with metadata
- **User Isolation**: Each user has separate storage space

## Project Structure
```
/
├── src/                    # Source files (.c)
│   ├── main.c             # Main program entry point
│   ├── cloud_operations.c # User management and file operations
│   ├── encryption.c       # XOR encryption/decryption
│   └── file_manager.c     # Metadata and file ID management
├── include/               # Header files (.h)
│   ├── cloud_operations.h
│   ├── encryption.h
│   └── file_manager.h
├── docs/                  # Documentation
│   └── ProjectReport.pdf
└── README.md             # This file
```

## Compilation Instructions

### For Linux/Mac:
```bash
gcc src/*.c -o main -Iinclude
```

### For Windows (MinGW):
```bash
gcc src/*.c -o main.exe -Iinclude
```

## How to Run
```bash
./main          # Linux/Mac
main.exe        # Windows
```

## Usage Guide

### 1. First Time Setup
- Choose option 2 to create a new account
- Enter a username and password
- Then login with option 1

### 2. Upload a File
- Select option 1 from main menu
- Enter the filename to upload
- Provide an encryption key (remember this!)
- Note down the File ID generated

### 3. Download a File
- Select option 2 from main menu
- Enter the File ID
- Provide the correct encryption key
- Specify output filename

### 4. List Files
- Select option 4 to view all your uploaded files
- Shows File ID and original filename

### 5. Delete a File
- Select option 3 from main menu
- Enter File ID
- Confirm with your account password

## Technical Details

### Encryption Algorithm
- **Method**: XOR Cipher
- **Key Type**: User-defined string
- **Security**: Symmetric encryption (same key for encrypt/decrypt)

### File Storage
- Files are stored in `cloud_storage/<username>/` directory
- Each file is encrypted and stored with a unique ID
- Metadata tracked in `metadata.txt` per user

### Security Features
- Password hashing using DJB2 algorithm
- User-specific storage isolation
- Key verification before decryption
- Password confirmation for file deletion

## System Requirements
- GCC Compiler (or MinGW for Windows)
- C Standard Library
- Minimum 10MB free disk space

## Author
MD EJAZ

## Limitations
- XOR encryption is basic (suitable for learning, not production)
- No network functionality (local storage only)
- Single-machine deployment

## Future Enhancements
- Implement AES encryption for stronger security
- Add file sharing between users
- Network-based cloud storage
- File compression before encryption
- Multiple encryption key support
