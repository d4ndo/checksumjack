Installation guide

ChecksumJack depends on 

* Qt > 5.x
* openSSL

```bash
apt-get install libssl-dev qt5
```

clone the repository from github:

```bash
git clone https://github.com/d4ndo/checksumjack.git
```

compile:

```bash
cd checksumjack/src/
qmake -o Makefile checksumJack.pro
make
make install
```

IMPORTANT

The config file filter.xml is needed:

```bash
mkdir ~/.checksumJack
cp ../linux/filter.xml ~/.checksumJack/
```

Prepare for desktop (gnome nautilus):

```bash
cp /icon/48.png .local/share/icons/
sudo cp ../linux/checksumjack.xml /usr/share/mime/packages/
sudo cp ../linux/checksumjack.desktop /usr/share/applications/
sudo update-mime-database /usr/share/mime
```