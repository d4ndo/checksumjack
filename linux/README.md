checksumJack depends on 

Qt > 5.x 
openSSL

How to compile :

clone the reopsitory from github.

```bash
git clone https://github.com/d4ndo/checksumjack.git
```

```bash
cd checksumjack/src/
qmake -o Makefile checksumJack.pro
make
make install
```

Prepare for desktop (gnome nautilus):

```bash
sudo cp /linux/checksumjack.xml /usr/share/mime/package/
sudo update-mime-database /usr/share/mime
sudo cp /src/icon/48.png .local/share/icons/
```

IMPORTANT

The config file is needed.

```bash
mkdir ~/.checksumJack
cp /linux/filter.xml ~/.checksumJack/
```
