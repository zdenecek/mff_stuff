# usage: override device and runs script

device=ns1
ssh -t $device "sudo mv /var /@var; sudo btrfs subvolume create /var; sudo mv /@var/* /var/;"
# tmp is tmpfs
ssh -t $device "sudo pacman -Syu snapper;sudo snapper -c config create-config /;"
# default settings are all right, no need to override config

# however, I would for what is my life worth never run pacman over ssh and risk breaking everything.