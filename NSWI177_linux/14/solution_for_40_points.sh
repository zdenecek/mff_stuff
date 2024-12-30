
LOGIN=tomisz

# nejdriv nainstalovat podmana

## COMMAND
podman pull registry.gitlab.com/mffd3s/nswi177/labs-2022-command:latest
podman run --interactive --tty registry.gitlab.com/mffd3s/nswi177/labs-2022-command:latest nswi177-task-command $LOGIN >14/command.txt


## VOLUME
# naklonovat repo
git clone git@gitlab.mff.cuni.cz:teaching/nswi177/2022/student-"$LOGIN".git ~/task14
podman pull registry.gitlab.com/mffd3s/nswi177/labs-2022-volume:latest
podman run --security-opt label=disable -v ~/task14/:/srv/nswi177/:rw registry.gitlab.com/mffd3s/nswi177/labs-2022-volume:latest nswi177-task-volume >14/volume.txt
rm -rf ~task/14


