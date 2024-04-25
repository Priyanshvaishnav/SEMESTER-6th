/*
    Lab Assignment: 04 Introduction To Cyptography And Network Security (CS 364)
    Name: Priyansh Vaishnav       Student ID: 202151120 
*/


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define uc unsigned char
#define ull unsigned long long
#define ui unsigned int

const ui P = 1021;
const ui a = 449;
const ui b = 233;

ui Total_points()
{
    ui count = 0;
    for (ui i = 1; i < P; i++)
    {
        for (ui j = 1; j < P; j++)
        {
            ui LHS = (j * j) % P;
            ui RHS = ((i * i * i) + a * i + b) % P;
            if (LHS == RHS)
                count++;
        }
    }

    return count;
}

void POint_on_curve(ui totalPoints, ui points[totalPoints][2])
{
    ui index = 0;
    for (ui i = 1; i < P; i++)
    {
        for (ui j = 1; j < P; j++)
        {
            ui LHS = (j * j) % P;
            ui RHS = ((i * i * i) + a * i + b) % P;
            if (LHS == RHS)
            {
                points[index][0] = i;
                points[index][1] = j;
                index++;
            }
        }
    }
}

const ui X_theta = 0;
const ui Y_theta = 1;

ui alpha[2];

ui k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

int extendedEuclidean(int a, int b, int *p, int *q)
{
    if (a == 0)
    {
        *p = 0;
        *q = 1;
        return b;
    }
    int p1, q1;
    int gcd = extendedEuclidean(b % a, a, &p1, &q1);
    *p = q1 - (b / a) * p1;
    *q = p1;
    return gcd;
}
ui makeInversePositive(int a)
{
    while (a < 0)
        a += P;
    return (ui)(a % P);
}

void addPoints(ui p1[2], ui p2[2], ui p3[2])
{
    ui m;
    if (p1[0] == 0 || p2[0] == 0)
    {
        if (p1[0] == 0)
        {
            p3[0] = p2[0];
            p3[1] = p2[1];
        }
        else
        {
            p3[0] = p1[0];
            p3[1] = p1[1];
        }
        return;
    }
    else if ((p1[0] == p2[0]) && (p1[1] == P - p2[1]))
    {

        p3[0] = X_theta;
        p3[1] = Y_theta;
        return;
    }
    else if ((p1[0] == p2[0]) && (p1[1] == p2[1]))
    {
        m = (3 * p1[0] * p1[0] + a) % P;
        int p, q;
        extendedEuclidean(2 * p1[1], P, &p, &q);
        m = (m * makeInversePositive(p)) % P;
    }
    else
    {
        ui numerator = (p2[1] + P - p1[1]) % P;
        ui denominator = (p2[0] + P - p1[0]) % P;
        int p = 0, q = 0;
        extendedEuclidean(denominator, P, &p, &q);
        m = (numerator * makeInversePositive(p)) % P;
    }

    p3[0] = (m * m + (P - p1[0]) + (P - p2[0])) % P;
    p3[1] = P - ((p1[1] + m * (p3[0] + P - p1[0])) % P);
}
void pointTimesN(ui n, ui point[2], ui result[2])
{
    result[0] = X_theta;
    result[1] = Y_theta;
    ui temp[2] = {point[0], point[1]};
    while (n > 0)
    {
        if (n & 1)
        {
            ui t[2];
            addPoints(result, temp, t);
            result[0] = t[0];
            result[1] = t[1];
        }
        ui t[2];
        addPoints(temp, temp, t);
        temp[0] = t[0];
        temp[1] = t[1];
        n = n >> 1;
    }
}

ui paddedLength(ui l)
{

    ui x = 0;

    if (l % 16 == 0)
        x = l / 16;
    else
        x = l / 16 + 1;
    if (l % 16 == 14 || l % 16 == 15 || l % 16 == 0)
        x++;

    return x * 16;
}

ui rightRotate(ui x, ui bits)
{
    return ((x >> bits) | (x << (32 - bits)));
}

void SHA256(ui length, ui m[length], ui hash[8])
{

    ui paddedLen = paddedLength(length);
    ui paddedMsg[paddedLen];
    for (ui i = 0; i < paddedLen; i++)
        paddedMsg[i] = 0;

    for (ui i = 0; i < length; i++)
        paddedMsg[i] = m[i];
    paddedMsg[length] = ((ui)1) << 31;

    paddedMsg[paddedLen - 1] = 32 * length;

    ui h0 = 0x6a09e667;
    ui h1 = 0xbb67ae85;
    ui h2 = 0x3c6ef372;
    ui h3 = 0xa54ff53a;
    ui h4 = 0x510e527f;
    ui h5 = 0x9b05688c;
    ui h6 = 0x1f83d9ab;
    ui h7 = 0x5be0cd19;

    for (ui j = 0; j < paddedLen / 16; j++)
    {
        ui words[64] = {0};
        for (ui i = 0; i < 16; i++)
            words[i] = paddedMsg[16 * j + i];
        for (ui i = 16; i < 64; i++)
        {
            ui x = rightRotate(words[i - 15], 7) ^ rightRotate(words[i - 15], 18) ^ (words[i - 15] >> 3);
            ui y = rightRotate(words[i - 2], 17) ^ rightRotate(words[i - 2], 19) ^ (words[i - 2] >> 10);
            words[i] = words[i - 16] + x + words[i - 7] + y;
        }
        ui a = h0, b = h1, c = h2, d = h3, e = h4, f = h5, g = h6, h = h7;
        for (ui i = 0; i < 64; i++)
        {
            ui x = rightRotate(e, 6) ^ rightRotate(e, 11) ^ rightRotate(e, 25);
            ui ch = (e & f) ^ ((~e) & g);
            ui temp1 = h + x + ch + k[i] + words[i];
            ui y = rightRotate(a, 2) ^ rightRotate(a, 13) ^ rightRotate(a, 22);
            ui maj = (a & b) ^ (a & c) ^ (b & c);
            ui temp2 = y + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
        h5 = h5 + f;
        h6 = h6 + g;
        h7 = h7 + h;
    }
    hash[0] = h0;
    hash[1] = h1;
    hash[2] = h2;
    hash[3] = h3;
    hash[4] = h4;
    hash[5] = h5;
    hash[6] = h6;
    hash[7] = h7;
}

// void generateMAC(ui key[8], uc M[16], ui MAC[8])
// {
//     ui inp[32];
//     for (ui i = 0; i < 7; i++)
//     {
//         inp[i] = key[i] ^ 125;
//     }
//     // inp[7] = key[7] ^ 125;
//     ui mac1[8];
//     SHA256(16, inp, mac1);

//     for (ui i = 0; i < 7; i++)
//     {
//         inp[i] = key[i];
//     }
//     inp[7] = key[7] ^ 215;
//     for (ui i = 8; i < 16; i++)
//     {
//         inp[i] = M[i - 8];
//     }
//     for (ui i = 16; i < 32; i++)
//         inp[i] = mac1[i - 16];

//     SHA256(32, inp, MAC);
// }

#include <string.h>

void convertToChar(uint32_t hash[8], uc key[32])
{
    uc z = 0xff; // mask
    for (uint32_t i = 0; i < 8; i++)
    {                                         // for each integer in integer array, convert it into 4 succesive characters on char array
        key[i * 4] = hash[i] >> 24;           // 1 to 8 bits from left
        key[i * 4 + 1] = (hash[i] >> 16) & z; // 9 to 16 bits from left
        key[i * 4 + 2] = (hash[i] >> 8) & z;  // 17 to 24 bits from left
        key[i * 4 + 3] = hash[i] & z;         // 25 to 32 bits from left
    }
}
void generateMAC(ui *KA, uc *MA, uc *MACA)
{
    // Convert KA to char array
    uc KA_char[32];
    convertToChar(KA, KA_char);

    // XOR KA_char with 125
    uc KA_XOR_125_char[32];
    for (int i = 0; i < 32; i++)
    {
        KA_XOR_125_char[i] = KA_char[i] ^ 125;
    }

    // Calculate SHA-256 hash of KA_XOR_125_char
    ui hash1[8];
    SHA256(32, KA_XOR_125_char, hash1);

    // XOR KA with 215
    ui KA_XOR_215[8];
    for (int i = 0; i < 8; i++)
    {
        KA_XOR_215[i] = KA[i] ^ 215;
    }

    // Convert KA_XOR_215 to char array
    uc KA_XOR_215_char[32];
    convertToChar(KA_XOR_215, KA_XOR_215_char);

    // Calculate SHA-256 hash of KA_XOR_215_char
    ui hash2[8];
    SHA256(32, KA_XOR_215_char, hash2);

    // Concatenate KA_XOR_125_char, hash2, and MA
    uc concatenated[72];
    memcpy(concatenated, KA_XOR_125_char, 32);
    memcpy(concatenated + 32, hash2, 32);
    memcpy(concatenated + 64, MA, 8);

    // Calculate SHA-256 hash of concatenated
    SHA256(72, concatenated, MACA);
}


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
    0x36};

// To perform Galois Field multiplication
uc galMUL(uc a, uc b)
{
    uc p = 0, carry;
    for (int i = 0; i < 8; ++i)
    {
        if (b & 1)
        {
            p ^= a;
        }
        carry = a & 0x80;
        a <<= 1;
        if (carry)
        {
            a ^= 0x1b;
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
        rnKeys[(i * 4) + 1] = key[(i * 4) + 1];
        rnKeys[(i * 4) + 2] = key[(i * 4) + 2];
        rnKeys[(i * 4) + 3] = key[(i * 4) + 3];
        rnKeys[(i * 4) + 0] = key[(i * 4) + 0];
        i++;
    }

    i = key_cols;
    while (i < st_cols * (total_rounds + 1))
    {
        j = 0;
        while (j < 4)
        {
            tmp[j] = rnKeys[(i - 1) * 4 + j];
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
            rnKeys[i * 4 + k] = rnKeys[(i - key_cols) * 4 + k] ^ tmp[k];
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
        st[i] = galMUL((st[i] ^ 39), 39);
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
        tmp[0] = st[i * 4];
        tmp[1] = st[i * 4 + 1];
        tmp[2] = st[i * 4 + 2];
        tmp[3] = st[i * 4 + 3];

        st[i * 4] = galMUL(tmp[0], 0x01) ^ galMUL(tmp[1], 0x04) ^ galMUL(tmp[2], 0x04) ^ galMUL(tmp[3], 0x05);
        st[i * 4 + 1] = galMUL(tmp[0], 0x05) ^ galMUL(tmp[1], 0x01) ^ galMUL(tmp[2], 0x04) ^ galMUL(tmp[3], 0x04);
        st[i * 4 + 2] = galMUL(tmp[0], 0x04) ^ galMUL(tmp[1], 0x05) ^ galMUL(tmp[2], 0x01) ^ galMUL(tmp[3], 0x04);
        st[i * 4 + 3] = galMUL(tmp[0], 0x04) ^ galMUL(tmp[1], 0x04) ^ galMUL(tmp[2], 0x05) ^ galMUL(tmp[3], 0x01);

        i++;
    }
}

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

        st[i * 4] = galMUL(tmp[0], 0xA5) ^ galMUL(tmp[1], 0x07) ^ galMUL(tmp[2], 0x1A) ^ galMUL(tmp[3], 0x73);
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

void triple_aes(const uc *msg, const ui *ka, uc *cptext)
{
    uc temp_ciphertext[16];
    uc k1[16];
    uc k2[16];

    for (int i = 0; i < 16; i++)
    {
        k1[i] = ka[i / 4] >> (24 - 8 * (i % 4)) & 0xFF;
    }

    for (int i = 0; i < 16; i++)
    {
        k2[i] = ka[i / 4 + 4] >> (24 - 8 * (i % 4)) & 0xFF;
    }

    uc rnKeys[176]; // 11 * 16 Bytes

    ex_key(k1, rnKeys);
    enc(msg, rnKeys, temp_ciphertext);
    ex_key(k2, rnKeys);
    enc(temp_ciphertext, rnKeys, cptext);
    ex_key(k1, rnKeys);
    enc(cptext, rnKeys, cptext);
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

    int r = total_rounds - 1;

    round_keys(msg, rnKeys + 10 * 16);
    inv_shROW(msg);
    inv_subByte(msg);

    while (r >= 1)
    {
        round_keys(msg, rnKeys + r * 16);
        INV_mixCol(msg);
        inv_shROW(msg);
        inv_subByte(msg);
        r--;
    }

    // Final r (without MixColumns)
    round_keys(msg, rnKeys);
}

void triple_des(const uc *cptext, const ui *ka, uc *msg)
{
    const int total_rounds = 10;
    uc temp_decrypted[16];
    uc k1[16];
    uc k2[16];

    for (int i = 0; i < 16; i++)
    {
        k1[i] = ka[i / 4] >> (24 - 8 * (i % 4)) & 0xFF;
    }

    for (int i = 0; i < 16; i++)
    {
        k2[i] = ka[i / 4 + 4] >> (24 - 8 * (i % 4)) & 0xFF;
    }

    uc rnKeys[176]; // 11 * 16 Bytes
    ex_key(k1, rnKeys);
    dec(cptext, rnKeys, temp_decrypted);
    ex_key(k2, rnKeys);
    dec(temp_decrypted, rnKeys, msg);
    ex_key(k1, rnKeys);
    dec(msg, rnKeys, temp_decrypted);
    memcpy(msg, temp_decrypted, 16);
}

void print(ui length, uc s[length])
{
    for (int i = 0; i < length; i++)
    {
        printf("%02x ", s[i]);
    }
    printf("\n");
}

int main()
{
    printf("\n");
    int totalPoints = Total_points();
    ui points[totalPoints][2];
    POint_on_curve(totalPoints, points);

    ui index;
    printf("Enter a number between 1 and %d  For alpha: ", totalPoints);
    do
    {
        scanf("%u", &index);
        if (index < 1 || index > totalPoints)
        {
            printf("Error, enter a number in the given range.\n");
            printf("Enter a number between 1 and %d for alpha: ", totalPoints);
        }
    } while (index > totalPoints || index < 1);

    alpha[0] = points[index - 1][0];
    alpha[1] = points[index - 1][1];

    printf("The point alpha is: (%u, %u)\n", alpha[0], alpha[1]);
    printf("\n");

    ui nA, nB;
    printf("Enter Alice's Private Key (Integer between 1 and 330 (both inclusive)): ");
    do
    {
        scanf("%u", &nA);
        if (nA < 1 || nA > 330)
        {
            printf("!!!!!!Error!!!!!.\n");
            printf("Enter Alice's Private Key (an integer between 1 and 330 (both inclusive)): ");
        }
    } while (nA > 330 || nA < 1);

    printf("Enter Bob's Private Key (an integer between 1 and 330 (both inclusive)): ");
    do
    {
        scanf("%u", &nB);
        if (nB < 1 || nB > 330)
        {
            printf("!!!!!!Error!!!!!!.\n");
            printf("Enter Alice's Private Key (an integer between 1 and 330 (both inclusive)): ");
        }
    } while (nB > 330 || nB < 1);

    ui naAlpha[2];
    pointTimesN(nA, alpha, naAlpha);
    ui nbAlpha[2];
    pointTimesN(nB, alpha, nbAlpha);
    ui naNbAlpha[2];
    ui nbNaAlpha[2];

    pointTimesN(nA, nbAlpha, naNbAlpha);
    pointTimesN(nB, naAlpha, nbNaAlpha);
    printf("\n");

    printf("The Shared Secret key between Alice and Bob is: (%u, %u)\n", nbNaAlpha[0], nbNaAlpha[1]);

    ui kA[8] = {0};
    ui msgA[2] = {naNbAlpha[0], naNbAlpha[1]};
    SHA256(2, msgA, kA);

    ui kB[8] = {0};
    ui msgB[2] = {nbNaAlpha[0], nbNaAlpha[1]};
    SHA256(2, msgB, kB);

    printf("\n");
    printf("kA: ");
    for (int i = 0; i < 8; i++)
        printf("%08x ", kA[i]);

    printf("\n\n");

    printf("kB: ");
    for (int i = 0; i < 8; i++)
        printf("%08x ", kB[i]);
    printf("\n\n");

    uc MA[16];
    printf("Enter 128-bit Plaintext(msg) of Alice (pair separated by spaces): ");
    for (int i = 0; i < 16; i++)
    {
        scanf("%hhx", &MA[i]);
    }

    printf("\n");
    printf("mA: ");
    print(16, MA);

    uc cp[16];
    triple_aes(MA, kA, cp);
    printf("Ciphertext of Alice: ");
    print(16, cp);

    // uc MACA[8];
    // generateMAC(kA, MA, MACA);
    // printf("MACA: ");
    // print(8, MACA);   

    triple_des(cp, kB, MA);
    printf("Decrypted Ciphertext of Alice: ");
    print(16, MA);

    uc MACA[8];
    generateMAC(kA, MA, MACA);
    uc MACB[8];
    generateMAC(kB, cp, MACB);
    printf("MACA: ");
    for(int i = 0; i < 8; i++)
        printf("%02x ", MACB[i]);
    printf("\nMACB: ");
    for(int i = 0; i < 8; i++)
        printf("%02x ", MACB[i]);

    return 0;
}