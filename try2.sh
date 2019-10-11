#!/bin/bash

cd ~
# скачиваем код с github и устанавливаем основные модули
git clone https://github.com/Bitmessage/PyBitmessage PyBitmessage
sudo -root apt-get install python openssl libssl-dev git python-msgpack python-gt4
sudo apt-get install python-msgpack python-qt4

# проверяем правильно ли установилось
cd PyBitmessage
python checkdeps.py 

sudo -root apt-get install python-pyopencl python-setuptools build-essential libssl-dev python-pip
pip install .[prctl]
sudo -root apt-get install libcap-dev python-prctl

# собираем и запускаем проект
python setup.py 
src/bitmessagemain.py