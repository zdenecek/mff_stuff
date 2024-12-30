#!/bin/sh

die() {
    exit_code="$1"
    shift
    if [ "$#" -gt 0 ] ; then
        echo "$@" >&2
    fi
    exit "${exit_code}"
}

debug_msg() {
    if [ "$verbose" = 1 ] ; then
        echo "debug:" "$@"
    fi
}

show_help() {
    echo "This script generates a key and configures ssh connection to machines at the Mala strana building of Matfyz."
    echo "Options:"
    (   echo "-h           | Show this help."
        echo "-v           | Verbose mode."
        echo "-l,--login   | (required) User name used for ssh connection."
        echo "-f,--filename| (required) Filename where the keys should be stored."
        echo "-i,--include-config| File with ssh_config for the connection. Defaults to ~/.ssh/config.mff" ) | column -t -s'|' -l 2
    die 0
}

opts_short="hvl:f:i:"
opts_long="login:,filename:,include-config:" 
getopt -Q -o "$opts_short" -l "$opts_long" -- "$@" || exit 1
eval set -- "$( getopt -o "$opts_short" -l "$opts_long" -- "$@" )"

ssh_config_include="$HOME/.ssh/config.mff"
verbose=0

while [ $# -gt 0 ]; do
    case $1 in
        -l|--login)
            login="$2"
            shift 1
            ;;
        -f|--filename)
            filename="$2"
            shift 1
            ;;
        -i|--include-config)
            ssh_config_include="$2"
            shift 1
            ;;
        -h)
            show_help
            ;;
        -v)
            verbose=1
            shift
            ;;
        --)
            shift
            break
            ;;
        *)
            die 1 "Unknown option $1"
            ;;
    esac
    shift
done


mandatory_missing=""

if [ -z ${login+x} ]; then
    mandatory_missing="$mandatory_missing login"
fi ; if [ -z ${filename+x} ]; then
    mandatory_missing="$mandatory_missing filename"
fi ; if [ "${#mandatory_missing}" -gt 0 ]; then
    # mandatory opts are not cool but...
    die 1 "Following options are required:$mandatory_missing" 
fi

file_path="$( realpath "$filename" )"

colliding_files=""
colliding_files_count=0


debug_msg "checking for file collisions"

if [ -f "$filename" ]; then
    colliding_files="$colliding_files $filename"
    colliding_files_count=$((colliding_files_count+1))
fi; if  [ -f "${filename}.pub" ]; then
    colliding_files="$colliding_files ${filename}.pub"
    colliding_files_count=$((colliding_files_count+1))
fi; if [ "$colliding_files_count" = 1 ]; then
    die 1 "File already exists:$colliding_files"
    elif [ "$colliding_files_count" = 2 ]; then
    debug_msg "key already exists"
    else

    debug_msg "generating key files"

    ssh-keygen -f "$file_path" -t ed25519
    sudo chmod 600 "$file_path"
fi


debug_msg "adding key to ssh agent"

ssh-add "$file_path"

debug_msg "creating config in $ssh_config_include"

(echo "Host linux"
echo "    Hostname linux.ms.mff.cuni.cz "
echo "    User $login"
echo "    IdentityFile $file_path"
echo "" ) >"$ssh_config_include"

machines="10.10.50.7 10.10.50.9 10.10.50.10 10.10.50.11"

( for machine in $machines ; do
    echo "Host $machine"
    echo "    User $login"
    echo "    IdentityFile $file_path"
    echo "    ProxyJump linux"
    echo ""
done ) >>"$ssh_config_include"

sudo chmod 600 "$ssh_config_include"

debug_msg "appending generated config to ~/.ssh/config"

if !  grep -q "Include $ssh_config_include" <"$HOME/.ssh/config" ; then
    ( echo "Include $ssh_config_include"
    echo ""
    cat "$HOME/.ssh/config" ) > "$HOME/.ssh/config"
    else
    debug_msg "config was already appended"
fi

debug_msg "copying the new key to following machines: linux.ms.mff.cuni.cz $machines"


ssh-copy-id -i "$file_path.pub" "linux"
for machine in $machines ; do
    ssh-copy-id -i "$file_path.pub" "$machine"
done

