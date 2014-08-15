# Linux installation guide

ChecksumJack depends on 

* Qt > 5.x
* openSSL

```bash
apt-get install libssl-dev qt5-default
```

Clone the repository from github:

```bash
git clone https://github.com/d4ndo/checksumjack.git
```

Compile:

```bash
cd checksumjack/src/
qmake -o Makefile checksumJack.pro
make
make install
```

IMPORTANT

The file `filter.xml` is necessary.

```bash
mkdir ~/.checksumJack
cp ../linux/filter.xml ~/.checksumJack/
```

Refresh mime-type, desktop and icon database:

```bash
sudo update-mime-database /usr/share/mime/
sudo update-icon-caches /usr/local/share/icons/
sudo update-desktop-database
```
