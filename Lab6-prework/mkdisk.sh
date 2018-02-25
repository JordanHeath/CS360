# mkdisk

touch mydisk
sudo mkfs mydisk 1440
sudo mount -o loop /mnt
(cd /mnt; sudo mkdir a b c d; sudo touch f1 f2 f3;)
ls -l /mnt
sudo umount /mnt
