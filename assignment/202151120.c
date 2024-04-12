/*
Lab Assignment: 03 
Name: Priyansh Vaishnav             Student ID: 202151120 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>


#define uc unsigned char
#define ull unsigned long long 


// AES S-box
 uc SBOX[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

// AES inverse S-box
 uc INV_SBOX[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

// AES r constants
 uc rn[10] = {
    0x01, 
    0x02, 
    0x04, 
    0x08, 
    0x10, 
    0x20, 
    0x40, 
    0x80, 
    0x1B, 
    0x36
};

// To perform Galois Field multiplication
uc galMUL(uc a, uc b)
{
    uc p = 0, carry;
    for (int i = 0; i < 8; ++i)
    {
        if (b&1)
        {
            p ^= a;
        }
        carry = a&0x80;
        a <<= 1;
        if (carry)
        {
            a^=0x1b;
             // This is the irreducible polynomial for AES
        }
        b >>= 1;
    }
    return p;
}

// Key schedule operation
void KEYSch(uc *w, uc itr)
{
    // Rotate left
    uc tmp = w[0];
    w[0] = w[1];
    w[1] = w[2];
    w[2] = w[3];
    w[3] = tmp;

    // Substitute bytes
    w[0] = SBOX[w[0]];
    w[1] = SBOX[w[1]];
    w[2] = SBOX[w[2]];
    w[3] = SBOX[w[3]];

    // XOR with r constant
    w[0] ^= rn[itr];
}

// Key expansion (Key schedule)
void ex_key(const uc *key, uc *rnKeys)
{
    const int key_cols = 4, st_cols = 4, total_rounds = 10;
    int i, j, k;
    uc tmp[4];

i = 0;
while (i < key_cols)
{
    rnKeys[(i*4)+1] = key[(i*4)+1];
    rnKeys[(i*4)+2] = key[(i*4)+2];
    rnKeys[(i*4)+3] = key[(i*4)+3];
    rnKeys[(i*4)+0] = key[(i*4)+0];
    i++;
}


i = key_cols;
while (i < st_cols * (total_rounds + 1))
{
    j = 0;
    while (j < 4)
    {
        tmp[j] = rnKeys[(i-1)*4+j];
        j++;
    }

    if (i % key_cols == 0)
    {
        // Rotate the w
        uc t = tmp[0];
        tmp[0] = tmp[1];
        tmp[1] = tmp[2];
        tmp[2] = tmp[3];
        tmp[3] = t;

        // Substitute bytes
        tmp[0] = SBOX[tmp[0]];
        tmp[1] = SBOX[tmp[1]];
        tmp[2] = SBOX[tmp[2]];
        tmp[3] = SBOX[tmp[3]];

        // XOR with r constant
        tmp[0] ^= rn[i / key_cols - 1];
    }
    else if (key_cols > 6 && i % key_cols == 4)
    {
        // Substitute bytes
        tmp[1] = SBOX[tmp[1]];
        tmp[2] = SBOX[tmp[2]];
        tmp[3] = SBOX[tmp[3]];
        tmp[0] = SBOX[tmp[0]];
    }

    k = 0;
    while (k < 4)
    {
        rnKeys[i*4+k] = rnKeys[(i-key_cols)*4+k]^tmp[k];
        k++;
    }

    i++;
}

}

// SubBytes transformation
void subbytes(uc *st)
{
    int i = 0;
    while (i < 16)
    {
        st[i] = SBOX[galMUL(st[i], 201) ^ 39];
        i++;
    }
}


// Inverse SubBytes transformation
void inv_subByte(uc *st)
{
    int i = 0;
    while (i < 16)
    {
        st[i] = INV_SBOX[st[i]];
        st[i] = galMUL((st[i]^39), 39);
        i++;
    }
}

// ShiftRows transformation
void shift_rows(uc *st)
{
    uc tmp;

    // Rotate row 1 by 1 position left
    tmp = st[1];
    st[1] = st[5];
    st[5] = st[9];
    st[9] = st[13];
    st[13] = tmp;

    // Rotate row 2 by 2 positions left
    tmp = st[2];
    st[2] = st[10];
    st[10] = tmp;
    tmp = st[6];
    st[6] = st[14];
    st[14] = tmp;

    // Rotate row 3 by 3 positions left
    tmp = st[3];
    st[3] = st[15];
    st[15] = st[11];
    st[11] = st[7];
    st[7] = tmp;
}

// Inverse ShiftRows transformation
void inv_shROW(uc *st)
{
    uc tmp;

  // Rotating rows by correspondin position right
    tmp = st[13];
    st[13] = st[9];
    st[9] = st[5];
    st[5] = st[1];
    st[1] = tmp;


    tmp = st[2];
    st[2] = st[10];
    st[10] = tmp;
    tmp = st[6];
    st[6] = st[14];
    st[14] = tmp;


    tmp = st[3];
    st[3] = st[7];
    st[7] = st[11];
    st[11] = st[15];
    st[15] = tmp;
}

// MixColumns transformation
void mix_columns(uc *st)
{
    uc tmp[4];
    int i = 0;
    while (i < 4)
    {
        tmp[0] = st[i*4];
        tmp[1] = st[i*4+1];
        tmp[2] = st[i*4+2];
        tmp[3] = st[i*4+3];

        st[i*4] = galMUL(tmp[0], 0x01) ^ galMUL(tmp[1], 0x04) ^ galMUL(tmp[2], 0x04) ^ galMUL(tmp[3], 0x05);
        st[i*4+1] = galMUL(tmp[0], 0x05) ^ galMUL(tmp[1], 0x01) ^ galMUL(tmp[2], 0x04) ^ galMUL(tmp[3], 0x04);
        st[i*4+2] = galMUL(tmp[0], 0x04) ^ galMUL(tmp[1], 0x05) ^ galMUL(tmp[2], 0x01) ^ galMUL(tmp[3], 0x04);
        st[i*4+3] = galMUL(tmp[0], 0x04) ^ galMUL(tmp[1], 0x04) ^ galMUL(tmp[2], 0x05) ^ galMUL(tmp[3], 0x01);

        i++;
    }
}

// Inverse MixColumns transformation
void INV_mixCol(uc *st)
{
    uc tmp[4];
    int i = 0;
    while (i < 4)
    {
        tmp[0] = st[i * 4];
        tmp[1] = st[i * 4 + 1];
        tmp[2] = st[i * 4 + 2];
        tmp[3] = st[i * 4 + 3];

        st[i * 4]     = galMUL(tmp[0], 0xA5) ^ galMUL(tmp[1], 0x07) ^ galMUL(tmp[2], 0x1A) ^ galMUL(tmp[3], 0x73);
        st[i * 4 + 1] = galMUL(tmp[0], 0x73) ^ galMUL(tmp[1], 0xA5) ^ galMUL(tmp[2], 0x07) ^ galMUL(tmp[3], 0x1A);
        st[i * 4 + 2] = galMUL(tmp[0], 0x1A) ^ galMUL(tmp[1], 0x73) ^ galMUL(tmp[2], 0xA5) ^ galMUL(tmp[3], 0x07);
        st[i * 4 + 3] = galMUL(tmp[0], 0x07) ^ galMUL(tmp[1], 0x1A) ^ galMUL(tmp[2], 0x73) ^ galMUL(tmp[3], 0xA5);

        i++;
    }
}

// AddRoundKey transformation
void round_keys(uc *st, const uc *round_key)
{
    int i = 0;
    while (i < 16)
    {
        st[i] ^= round_key[i];
        i++;
    }
}



// AES enc function
void enc(const uc *msg, const uc *rnKeys, uc *cptext)
{
    uc st[16];

    // Copy msg to st
    int i = 0;
    while (i < 16)
    {
        st[i] = msg[i];
        i++;
    }

    // Initial r (AddRoundKey)
    round_keys(st, rnKeys);

    // Main rounds
    int r = 1;
    while (r < 10)
    {
        subbytes(st);
        shift_rows(st);
        mix_columns(st);
        round_keys(st, rnKeys + r * 16);
        r++;
    }

    // Final r (without MixColumns)
    subbytes(st);
    shift_rows(st);
    round_keys(st, rnKeys + 10 * 16);

    // Copy st to cptext
    i = 0;
    while (i < 16)
    {
        cptext[i] = st[i];
        i++;
    }
}




// AES dec function
void dec(const uc *cptext, const uc *rnKeys, uc *msg)
{
    const int total_rounds = 10; // Number of rounds

    // Copy cptext to st
    int i = 0;
    while (i < 16)
    {
        msg[i] = cptext[i];
        i++;
    }

    // Initial r (AddRoundKey)
    round_keys(msg, rnKeys + total_rounds * 16);

    // Main rounds
    int r = total_rounds - 1;
    while (r >= 1)
    {
        inv_shROW(msg);
        inv_subByte(msg);
        round_keys(msg, rnKeys + r * 16);
        INV_mixCol(msg);
        r--;
    }

    // Final r (without MixColumns)
    inv_shROW(msg);
    inv_subByte(msg);
    round_keys(msg, rnKeys);
}

// Function to create a random 256-bit x
void create_randomINPUT(uc *x)
{
    int i = 0;
    while (i < 32)
    {
        x[i] = rand() % 256;
        i++;
    }
}






// Compression function h
void com_fxn(const uc *x, uc *ans)
{
    // Divide the 256-bit x into two 128-bit equal parts
    uc m1[16];
    uc m2[16];
    memcpy(m1, x, 16);
    memcpy(m2, x + 16, 16);

    uc rnKeys[176]; // 11 * 16 Bytes
    ex_key(m2, rnKeys);
    enc(m1, rnKeys, ans);
}








void rkeys(const uc *rnKeys){
    for (int r = 0; r <= 10; ++r)
    {
        for (int i = 0; i < 16; ++i)
        {
            printf("%02x ", rnKeys[r * 16 + i]);
        }
    }
}




void SecondPreImage()
{
    // Generate a random x m1 || m2
    uc m1[16];
    uc m2[16];
    int i = 0;
    while (i < 16)
    {
        m1[i] = rand() % 256; // Random byte
        m2[i] = rand() % 256; // Random byte
        i++;
    }

    // Compute AES ans for m1 || m2
    uc ans[16];
    uc m1_m2[32];
    memcpy(m1_m2, m1, 16);
    memcpy(m1_m2 + 16, m2, 16);
    com_fxn(m1_m2, ans);

    // Generate a random primeof_m2
    uc primeof_m1[16];
    uc primeof_m2[16];
    i = 0;
    while (i < 16)
    {
        primeof_m2[i] = rand() % 256; // Random byte
        i++;
    }

    // Compute primeof_m1 such that h(primeof_m1 || primeof_m2) = h(m1 || m2)
    // Use AES dec to find primeof_m1
    
    // Buffer to hold r keys
    uc rnKeys[176]; // 11 * 16 bytes
    ex_key(primeof_m2, rnKeys);
    dec(ans, rnKeys, primeof_m1);

    // Compute AES ans for primeof_m1 || primeof_m2
    uc resPrime[16];
    uc m1_m2_prime[32];
    memcpy(m1_m2_prime, m1, 16);
    memcpy(m1_m2_prime + 16, m2, 16);
    com_fxn(m1_m2_prime, resPrime);

    // Print m1, primeof_m1, primeof_m2, h(m1 || m2), and h(primeof_m1 || primeof_m2)
    printf("\nm1: ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02x ", m1[i]);
    }
    printf("\n");

    printf("m2: ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02x ", m2[i]);
    }
    printf("\n");

    printf("h(m1 || m2): ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02x ", ans[i]);
    }
    printf("\n");

    printf("\nm'1: ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02x ", primeof_m1[i]);
    }
    printf("\n");

    printf("m'2: ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02x ", primeof_m2[i]);
    }
    printf("\n");

    printf("h(m'1 || m'2): ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02x ", resPrime[i]);
    }
    printf("\n");

    // Check if h(m1 || m2) == h(primeof_m1 || primeof_m2)
    int match = memcmp(ans, resPrime, 16);
    if (match == 0)
    {
        printf("\nSecond pre-image found!\n\n");
    }
    else
    {
        printf("\nSecond pre-image not found.\n\n");
    }
}


int main() {
    // Seed the random number generator
    srand(time(NULL));

    // User x for m1 and m2
    uc m1[16];
    uc m2[16];

    printf("\nEnter m1:-  ");
    for (int i = 0; i < 16; ++i) {
        scanf("%2hhx", &m1[i]);
    }

    printf("Enter m2:-  ");
    for (int i = 0; i < 16; ++i) {
        scanf("%2hhx", &m2[i]);
    }


    // Compute AES ans for m1 || m2
    uc ans[16];
    uc m1_m2[32];
    memcpy(m1_m2, m1, 16);
    memcpy(m1_m2 + 16, m2, 16);
    com_fxn(m1_m2, ans);

    // Print the ans of the compression
    printf("h(m1||m2)=  ");
    for (int i = 0; i < 16; ++i) {
        printf("%02x ", ans[i]);
        
    }
    
    printf("\nSecond Pre-Image Fxn:- ");
    SecondPreImage();

    return 0;
}