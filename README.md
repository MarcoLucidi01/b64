b64
===

I wanted to understand how [base64](https://en.wikipedia.org/wiki/Base64)
encoding (and decoding) works so I implemented this tool.

usage examples
==============

install it in `/usr/local/bin` with:

    # make install

encode `stdin` to `stdout`:

    $ printf "foo" | b64
    Zm9v

encode `stdin` to file:

    $ printf "foo" | b64 > foo.enc
    $ more foo.enc
    Zm9v

decode file to `stdout`:

    $ b64 -d < foo.enc
    foo

decode `stdin` to `stdout`:

    $ printf "Zm9v" | b64 -d
    foo

if you want nice 80 characters (or whatever) lines use [fold](https://linux.die.net/man/1/fold):

    $ head -c 512 /dev/urandom | b64 | fold
    IlrfOJcW+4fZvza6GiCgd5a1ZN6wUd26F3ir8y185+i1q48gFayMWG1kQnCOxNnuqj8h/ds9VCPlanRS
    JRHecpN9i73gOMDbOu5YH9oU9M1FfijhuLwFN4h5kRU7KL8jUTYGHM9z3ASvwT7z9ckIcoDoHHpzPIk6
    2JLMnDVrm+cXAbzNGFbpZa6ACZzR0BzOudJu213RR7okB219mMrySEGKTo375Fx9ZzWPn9kr8LEYBHyc
    BhuKXZ/KOlmoclkNoPPVES/c0RCILvvbNsOqa4DJR0QCXAJYIYABtrCSopcIJ9IC3SwQa5HFd9tlBKVJ
    zKWhMjRQX/7WiYUWi6xOFFl4IaleMS+Ji/i5oYXDwBoOa70ZMiXkArfb8ZX9mHTs0XmvUDNGUBCpr3dI
    /2MTNKlVOmVwwHDlM5wGpY0ambGPWhjH0pO/BtITBVHUMWKlkchvtj9Edj1Uk66PPlPNbwTm7YlHxTDS
    +tqbw2pijy+s0rhJ5XAVtORocdznMYMyGbuNudI61aZTUfgRDyxfAd4kDb1bj4V8aWyU0r25sR3QUY2Y
    626IHv3FT2TKw6txfiZhLuZ+EZsGNyeHtYIplqCKPaQM76VMaG8GxBNHzKY94Irhg+9s2IIkFezLQ/On
    l00nUNqIP27spokBG/l8fQh6sZF5JqJ0fs07/LMwZKE=
