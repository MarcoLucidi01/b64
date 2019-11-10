/*
 * MIT License
 *
 * Copyright (c) 2019 Marco Lucidi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE   "usage: b64 [-d]\n" \
                "  base64 encode/decode standard input to standard output\n"

#define ARRAYLEN(a)     (sizeof((a)) / sizeof((a)[0]))
#define INVALIDDECVAL   (ARRAYLEN(enctable))

static void     builddectable(void);
static void     die(const char *reason);
static void     encode(void);
static void     decode(void);
static int      getcharskipn(void);
static int      isvalid(int c);

static const unsigned char enctable[] =
{
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
};

static unsigned char dectable[UCHAR_MAX + 1];

int main(int argc, char **argv)
{
        builddectable();

        if (argc == 1)
                encode();
        else if (argc == 2 && strcmp(argv[1], "-d") == 0)
                decode();
        else
                die(USAGE);

        return 0;
}

static void builddectable(void)
{
        unsigned int i;

        for (i = 0; i < ARRAYLEN(dectable); i++)
                dectable[i] = INVALIDDECVAL;

        for (i = 0; i < ARRAYLEN(enctable); i++)
                dectable[enctable[i]] = i;

        dectable['='] = 0;      /* pad character has a decoded value equal to 0 */
}

static void die(const char *reason)
{
        fprintf(stderr, reason);
        exit(EXIT_FAILURE);
}

static void encode(void)
{
        int b1, b2, b3;
        unsigned long g;        /* group of 4 6-bit indices for enctable built using 3 input bytes */

        while ((b1 = getchar()) != EOF) {
                b2 = getchar();
                b3 = getchar();

                g = b1;
                g = (g << 8) | (b2 == EOF ? 0 : b2);
                g = (g << 8) | (b3 == EOF ? 0 : b3);

                putchar(enctable[(g >> 18) & 0x3F]);
                putchar(enctable[(g >> 12) & 0x3F]);
                putchar(b2 == EOF ? '=' : enctable[(g >> 6) & 0x3F]);
                putchar(b3 == EOF ? '=' : enctable[g & 0x3F]);
        }
}

static void decode(void)
{
        int c1, c2, c3, c4;
        unsigned long g;        /* group of 3 bytes built using dectable indexed by 4 input characters */

        while ((c1 = getcharskipn()) != EOF) {
                c2 = getcharskipn();
                c3 = getcharskipn();
                c4 = getcharskipn();

                if ( ! isvalid(c1) || c1 == '='
                ||   ! isvalid(c2) || c2 == '='
                ||   ! isvalid(c3)
                ||   ! isvalid(c4))
                        die("b64: invalid input\n");

                g = dectable[c1];
                g = (g << 6) | dectable[c2];
                g = (g << 6) | dectable[c3];
                g = (g << 6) | dectable[c4];

                putchar((g >> 16) & 0xFF);
                if (c3 != '=')
                        putchar((g >> 8) & 0xFF);
                if (c4 != '=')
                        putchar(g & 0xFF);
        }
}

static int getcharskipn(void)
{
        int c;

        if ((c = getchar()) == '\n')
                return getchar();

        if (c == '\r') {
                if ((c = getchar()) == '\n')
                        return getchar();
                ungetc(c, stdin);
                return '\r';
        }

        return c;
}

static int isvalid(int c)
{
        return c != EOF && c >= 0 && c <= UCHAR_MAX && dectable[c] != INVALIDDECVAL;
}
