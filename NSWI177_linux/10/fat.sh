#!/bin/bash

# tohle by mohlo fungovat

scp tomisz@linux.ms.mff.cuni.cz:~/lab10.fat.img lab10.fat.img || mkdir mnt || sudo mount lab10.fat.img mnt || gzip -kcd mnt/31a78560.gz >fat.txt || sudo umount mnt || rm -rf mnt || rm lab10.fat.img