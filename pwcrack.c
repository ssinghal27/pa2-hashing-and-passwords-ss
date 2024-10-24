#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <openssl/sha.h>
#include <string.h>

uint8_t hex_to_byte(unsigned char h1, unsigned char h2) {
    uint8_t x = 0;
    uint8_t y = 0;

    // Convert h1 to a decimal value
    if (h1 >= '0' && h1 <= '9') {
        x += h1 - '0';
    }
    else if (h1 >= 'a' && h1 <= 'f') {
        x += h1 - 'a' + 10;
    }

    // Convert h2 to a decimal value
    if (h2 >= '0' && h2 <= '9') {
        y += h2 - '0';
    }
    else if (h2 >= 'a' && h2 <= 'f') {
        y += h2 - 'a' + 10;
    }
    // TODO: Determine what the function should return
    return (x<<4) + y;
}

void hexstr_to_hash(char hexstr[], unsigned char hash[32]){
	int i =0;
	for (i = 0; i < 32; i++) {
        // Convert two hex characters to a byte (8 bits)
        unsigned char high = hexstr[i * 2];
        unsigned char low = hexstr[i * 2 + 1];

        // Convert '0'-'9', 'a'-'f' to their decimal equivalents
        if (high >= '0' && high <= '9') {
            high -= '0';
        } else if (high >= 'a' && high <= 'f') {
            high = high - 'a' + 10;
        }

        if (low >= '0' && low <= '9') {
            low -= '0';
        } else if (low >= 'a' && low <= 'f') {
            low = low - 'a' + 10;
        }

        // Combine high and low to form a single byte
        hash[i] = (high << 4) | low;
    }
}

int8_t check_password(char password[], unsigned char given_hash[32]){
    unsigned char computed_hash[32];
    SHA256((unsigned char*)password, strlen(password), computed_hash);
   
    int i;
    for (i = 0; i < 32; i++) {
        if (computed_hash[i] != given_hash[i]) {
            return 0; 
        }
    }
    return 1;	
}


int8_t crack_password(char password[], unsigned char given_hash[]){
    if (check_password(password, given_hash)) return 1;
    
    int len = strlen(password);
    int i;
    for (i = 0; i < len; i++) {
        if (password[i] >= 'a' && password[i] <= 'z') {
            password[i] -= 32; 
            if (check_password(password, given_hash)) return 1;
            password[i] += 32; 
        } else if (password[i] >= 'A' && password[i] <= 'Z') {
            password[i] += 32; 
            if (check_password(password, given_hash)) return 1;
            password[i] -= 32; 
        }
    }
    return 0;
}

    




void test_hex_to_byte(){
	assert(hex_to_byte('a','2') == 162);
	assert(hex_to_byte('c', '8') == 200);
	assert(hex_to_byte('0', '3') == 3);
	assert(hex_to_byte('0', 'a') == 10);
	assert(hex_to_byte('1', '0') == 16); 
}

void test_hexstr_to_hash() {
  char hexstr[64] = "a2c3b02cb22af83d6d1ead1d4e18d916599be7c2ef2f017169327df1f7c844fd";
  unsigned char hash[32];
  hexstr_to_hash(hexstr, hash);
  int i = 0;
  for (i = 0; i < 32; i++) {
        printf("%02x", hash[i]);
  }
  assert(hash[0] == 0xa2);
  assert(hash[31] == 0xfd);
}

int main(int argc, char** argv) {
    unsigned char given_hash[32];
    if (argc < 2) {
        printf("Usage: %s <hash>\n", argv[0]);
        return 1;
    }
    
    hexstr_to_hash(argv[1], given_hash);
    
    char password[256];
    while (fgets(password, sizeof(password), stdin)) {
        password[strcspn(password, "\n")] = 0; 
        if (crack_password(password, given_hash)) {
            printf("Found password: SHA256(%s) = %s\n", password, argv[1]);
            return 0;
        }
    }
    
    printf("Did not find a matching password\n");
    return 1;
}





