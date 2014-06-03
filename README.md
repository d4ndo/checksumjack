
ChecksumJack
============

ChecksumJack is a software to calculate a files hash digest. It runs on multiple platform and is distributed under the GPL. You are welcome to download it for free.

http://www.ChecksumJack.org

Supported hash: 

* MD5, 
* SHA-1, 
* SHA224, 
* SHA256, 
* SHA384, 
* SHA512, 
* RIPEMD,
* Whirlpool.

# INSTALL

libraries needed:
* QT > 5.x
* openSSL -> libSSL-dev

## LINUX 

buildtools needed: 
* make, 
* gcc or clang

```bash
cd src
qmake -o Makefile checksumJack.pro
make
make install
```

## WINDOWS

Get installer from http://www.ChecksumJack.org

