#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
// need random
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

// #define BUFFER_SIZE 1024
#define SALT_SIZE 8
#define KEY_LENGTH 32
#define BLOCK_SIZE 16
#define IV_SIZE 16

// command line storage
typedef struct {
    int encrypt;                    // 1 -> encrypt, 0 -> decrypt
    int use_salt;                   // 1 -> salt, 0 -> no salt
    int print_key_iv_salt;          // 1 -> print, 0 -> do not print
    char *key;                      // password string
    char *infile;                   // input filename
    char *outfile;                  // output filename
} Opts; //options_t

// int argv_cnt = 1;

// iv --> should be random (for CBC mode !!!)

// helper function to handle any erros (using openssl library)
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

void print_help(char **argv)
{
    printf("use: %s -e|-d -k key -in input -out output\n", argv[0]);
	exit(1);
}


// calling the salt function

/////// Encrypting the message //////////
/*  Stages of encryption
    1. setting up a context
    2. initializing the encyrption operation
    3. providing plaintext bytes to be encrypted
    4. Finalising the encyrption operation
*/

// EVP cipher object --> AES_256_cbc() 

/*
    what we need for the ciphertext (encryption text)
int encrytp(plaintext, len of plaintext, key to be used, iv)
*/
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext)
 {
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Creaet and initialize the context*/
    if (!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
        Initilaize the encyrption operation. IMPORANT - ensure you use a key
        and iv size appropriate for your cipher
        we are using 256 bit AES (256 bit key).
        The iv size for most modes is the same as the block size. 
        For AES this is 128 bit
    */
   if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
        Provide the message to be encrypted, and obtain the encrypted output.
        EVP_EncryptUpdate can be called multiple times if necessary 
    */
   if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
        Finalise the encryption. Further ciphertext bytes may be written at this stage
    */
   if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}


////// decryption ///////////

/*  decryption conssist of the folloiwng stages
    1. Setting up a context
    2. intializing the decryption operation
    3. providing ciphertext bytes to be decrypted
    4. finalizing the decyrption operation
*/

/*
    we need:
    int decrypt(ciphertext, len of ciphertext, key, iv)
*/

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;
    
    int plaintext_len;

    /* Create and initialize the context*/
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}


// If we want to add salt or no salt
int derive_key_iv(const char *password, unsigned char *salt, unsigned char *key, unsigned char *iv, int use_salt) {
    const EVP_CIPHER *cipher = EVP_aes_256_cbc();


    // EVP_md5 is a 'digest algorith' --> it produces a 128 bit output from a given input
    const EVP_MD *dgst = EVP_md5();


    if (!use_salt) {
        salt = NULL; // EVP_BytesToKey will treat this as no salt
    }

    // EVP_BytesToKey --> function to derive a key and iv from a passowrd and salt.
    // EVP_BytesToKey(EVP_CIPHER, EVP_MD, salt/nosalt, password, passowrd length, iteration length, out buffer key, out buffer iv )
    // reinterpret_cast --> it converts string into a const uunsigned char*
    // the reason it coverts it is the format that OpenSSL function EVP_BytesToKey is excpecting to have
    int key_iv_len = EVP_BytesToKey(cipher,dgst,salt,(const unsigned char *)password,strlen(password),1,key,iv);


    if (key_iv_len != 32) {
        fprintf(stderr, "Key size is not 256 bits!\n");
        return 0;
    }
    return 1;

}

// command line arguments
int parse_args(int argc, char *argv[], Opts *opts) {
    // Set defaults
    opts->encrypt = -1; // -1 = not set yet
    opts->key = NULL;
    opts->infile = NULL;
    opts->outfile = NULL;
    opts->use_salt= 0; // default = nosalt
    opts->print_key_iv_salt = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            opts->encrypt = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            opts->encrypt = 0;
        } else if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
            opts->key = argv[++i];
        } else if (strcmp(argv[i], "-in") == 0 && i + 1 < argc) {
            opts->infile = argv[++i];
        } else if (strcmp(argv[i], "-out") == 0 && i + 1 < argc) {
            opts->outfile = argv[++i];
        } else if (strcmp(argv[i], "-salt") == 0) {
            opts->use_salt = 1;
        } else if (strcmp(argv[i], "-nosalt") == 0) {
            opts->use_salt = 0;
        } else if (strcmp(argv[i], "-p") == 0) {
            opts->print_key_iv_salt = 1;
        } else {
            fprintf(stderr, "This (%s) is not an option type 'help' for assistance!\n", argv[i]);
            return 0;
        }
    }
    // cchecking if the args are valid
    if (opts->encrypt == -1 || !opts->key || !opts->infile || !opts->outfile) {
        fprintf(stderr, "Missing required arguments\n");
        return 0;
    }
    return 1;
}


int main(int argc, char *argv[]) {
    // calling options
    Opts opts = {0};
    if (!parse_args(argc, argv, &opts)) {
        print_help(argv);
        return 1;
        // using opts.key, opts.infile, opts.outfile, opts.encrypt
    }
    /*
        Set up the key and iv. Do I need to say to not hard code these in
        a real application?
    */
   /* A 256 bit key */
    unsigned char key[KEY_LENGTH];

    /* A 128 bit IV */
    unsigned char iv[IV_SIZE];
    // generating salf if needed

    unsigned char salt[SALT_SIZE] = {0};
    if (opts.encrypt) {
        // Generate salt and IV if using salt
        if (opts.use_salt) {
            if (!RAND_bytes(salt, SALT_SIZE)) {
                fprintf(stderr, "Error generating salt.\n");
                return 1;
            }
        }

        if (!derive_key_iv(opts.key, salt, key, iv, opts.use_salt)) {
            return 1;
        }

        if (opts.print_key_iv_salt) {
            printf("Key: ");
            for (int i = 0; i < KEY_LENGTH; i++) printf("%02x", key[i]);
            printf("\nIV: ");
            for (int i = 0; i < IV_SIZE; i++) printf("%02x", iv[i]);
            if (opts.use_salt) {
                printf("\nSalt: ");
                for (int i = 0; i < SALT_SIZE; i++) printf("%02x", salt[i]);
            }
            printf("\n");
        }

        FILE *in_fp = fopen(opts.infile, "rb");
        if (!in_fp) {
            perror("Input file open error");
            return 1;
        }

        fseek(in_fp, 0, SEEK_END);
        long plaintext_len = ftell(in_fp);
        rewind(in_fp);

        unsigned char *plaintext = malloc(plaintext_len);
        fread(plaintext, 1, plaintext_len, in_fp);
        fclose(in_fp);

        unsigned char *ciphertext = malloc(plaintext_len + BLOCK_SIZE);
        int ciphertext_len = encrypt(plaintext, plaintext_len, key, iv, ciphertext);

        FILE *out_fp = fopen(opts.outfile, "wb");
        if (!out_fp) {
            perror("Output file open error");
            return 1;
        }

        if (opts.use_salt) {
            fwrite("Salted__", 1, 8, out_fp);
            fwrite(salt, 1, 8, out_fp);
        }

        fwrite(ciphertext, 1, ciphertext_len, out_fp);
        fclose(out_fp);

        free(plaintext);
        free(ciphertext);
    } else {
        FILE *in_fp = fopen(opts.infile, "rb");
        if (!in_fp) {
            perror("Input file open error");
            return 1;
        }

        unsigned char salt_hdr[8];
        if (opts.use_salt) {
            fread(salt_hdr, 1, 8, in_fp);
            if (memcmp(salt_hdr, "Salted__", 8) != 0) {
                fprintf(stderr, "Invalid salt header.\n");
                fclose(in_fp);
                return 1;
            }

            fread(salt, 1, 8, in_fp);
        }

        if (!derive_key_iv(opts.key, salt, key, iv, opts.use_salt)) {
            return 1;
        }

        if (opts.print_key_iv_salt) {
            printf("Key: ");
            for (int i = 0; i < KEY_LENGTH; i++) printf("%02x", key[i]);
            printf("\nIV: ");
            for (int i = 0; i < IV_SIZE; i++) printf("%02x", iv[i]);
            if (opts.use_salt) {
                printf("\nSalt: ");
                for (int i = 0; i < SALT_SIZE; i++) printf("%02x", salt[i]);
            }
            printf("\n");
        }

        fseek(in_fp, 0, SEEK_END);
        long file_len = ftell(in_fp);
        rewind(in_fp);

        long ciphertext_offset = opts.use_salt ? 16 : 0;
        fseek(in_fp, ciphertext_offset, SEEK_SET);
        long ciphertext_len = file_len - ciphertext_offset;

        unsigned char *ciphertext = malloc(ciphertext_len);
        fread(ciphertext, 1, ciphertext_len, in_fp);
        fclose(in_fp);

        unsigned char *plaintext = malloc(ciphertext_len); // max possible
        int plaintext_len = decrypt(ciphertext, ciphertext_len, key, iv, plaintext);

        FILE *out_fp = fopen(opts.outfile, "wb");
        if (!out_fp) {
            perror("Output file open error");
            return 1;
        }
        fwrite(plaintext, 1, plaintext_len, out_fp);
        fclose(out_fp);

        free(ciphertext);
        free(plaintext);
    }

    return 0;
}