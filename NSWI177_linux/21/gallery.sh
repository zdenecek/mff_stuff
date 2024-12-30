#!/bin/bash

set -ueo pipefail

die() {
    local exit_code="$1"
    shift

    echo "[gallery.sh] Fatal error:" "$@" >&2
    exit "${exit_code}"
}

debug_msg() {
    echo "[gallery.sh $(date '+%Y-%m-%d %H:%M:%S' || true )]" "$@" >&2
}

# Extract image name from a given filename
get_name_from_image() {
    basename "$1" .jpg | tr -cd 'a-zA-Z0-9 .-_'
}

# Prints parameters as a JSON dictionary (no nesting).
# Expects parameter pairs, e.g.
# print_simple_json_dictionary key1 "value 1" key2 "value 2"
#
# Does not handle special characters
print_simple_json_dictionary() {
    echo '{'
    while [ $# -gt 0 ]; do
        echo " \"$1\": \"$2\""
        echo ,
        shift 2
    done | sed '$d'
    echo '}'
}

# Get list of albums.
# We make the names well-behaving: we want to copy them to a webserver
# and spaces and other special characters do not mix that well with URLs.
# Hence we skip anything that does not look like a valid directory name.
get_album_list() {
    find albums \
        -mindepth 1 \
        -maxdepth 1 \
        -iregex '.*/[-a-zA-Z0-9_.]+' \
        -type d \
        -exec basename {} \; \
        | sort
}


# Prepare images for one album: copy them to public_html and ensure
# they are named correctly.
prepare_album() {
    local album_dir="$1" # album directory name
    local source_dir="$2" # source path
    local dest_dir="$3" # output path
    local title="${4}"
    local album_thumb="${5:-}"

    ${debug} "Preparing album ${album_dir} (${title})."

    mkdir -p "${dest_dir}"
    (
        echo '{'
        printf '"title": "%s",\n' "${title}"
        echo '"images": ['
    ) >"${dest_dir}/.details"


    

    # check if front_image (album thumb) was provided, then set value to compare to full path
    local album_thumb_full_path=""
    if [ -n "$album_thumb" ] ; then
        album_thumb_full_path="$source_dir/$album_thumb"
    fi

    find "${source_dir}" -type f -iname '*.jpg' -print0 | sort -z | (
        counter=1
        while IFS='' read -r -d $'\0' source_image; do

            # copy image, make names
            image_name="$( get_name_from_image "${source_image}" )"
            dest_image="$( printf "%08d.jpg" "${counter}" )"
            thumbnail_name="thumb.$dest_image"
            cp -f "${source_image}" "${dest_dir}/${dest_image}"

            # check if this is the front image (album thumb)
            if [ "${source_image}" -ef "${album_thumb_full_path}" ] ; then
                echo "${dest_image}" >"${dest_dir}/front_image_temp"
            fi

            # generate thumbnail
            #local original_dims="$(identify -format "%wx%h" ${source_image})"
            #local original_width="${original_dims#*x}" # removes shortest match from the beginning
            #local original_height="${original_dims%x*}" # the same but from end
            #local thumb_width="$(( $original_width * $thumb_height / $original_height ))"
            thumb_dest="${dest_dir}/${thumbnail_name}"
            convert "${source_image}" -resize "$thumbnail_size" "${thumb_dest}"
            thumb_dims=$(identify -format "%wx%h" "${thumb_dest}")
            thumb_width="${thumb_dims%x*}" # removes shortest match from the beginning
            thumb_height="${thumb_dims#*x}" # the same but from end
            
            json_args=()

            # find exif metadata, if they exist
            image_time_data=$(identify -format "%[EXIF:DateTimeOriginal]" "${source_image}" 2>/dev/null)
            if [ -n "${image_time_data}" ] ; then
                json_args+=("date_time" "${image_time_data}")
            fi

            # print details to json
            json_args+=("filename" "${dest_image}")  
            json_args+=("thumbnail" "${thumbnail_name}")
            json_args+=("name" "${image_name}")
            json_args+=("thumb_width" "${thumb_width}")
            json_args+=("thumb_height" "${thumb_height}")
            (
                print_simple_json_dictionary "${json_args[@]}"
                echo ','
            ) >>"${dest_dir}/.details"

            counter=$(( counter + 1 ))
        done
    )

    if [ -f "${dest_dir}/front_image_temp" ] ; then
        front_image=$(<"${dest_dir}/front_image_temp")
        rm -f "${dest_dir}/front_image_temp"
    else
        front_image="00000001.jpg"
    fi
    
    echo "${album_dir} ${front_image} ${title}" >"${dest_dir}/.meta"
    

    sed -e '$s/.*/]}/' "${dest_dir}/.details" | "${json_reformat}" >"${dest_dir}/.details.json"
    rm -f "${dest_dir}/.details"
}

# Load global configuration, if available
if [ -f gallery.rc ]; then
    . gallery.rc
fi

# Setup debugging
if "${debug:-false}"; then
    debug=debug_msg
else
    debug=:
fi


# Parse options

opts_short="d:t:"
opts_long="publish-dir:,theme-dir:" 

# Check for bad usage first (notice the ||)
getopt -Q -o "$opts_short" -l "$opts_long" -- "$@" || exit 1

# Actually parse them (we are here only if they are correct)
eval set -- "$( getopt -o "$opts_short" -l "$opts_long" -- "$@" )"

while [ $# -gt 0 ]; do
    case "$1" in
        -d|--publish-dir)
            publish_dir="$2"
            shift 2
            ;;
        -t|--theme-dir)
            theme_dir="$2"
            shift 2
            ;;
        --)
            shift
            break
            ;;

        *)
            echo "Unknown option $1" >&2
            exit 1
            ;;
    esac
    shift
done


# Check tool availability
json_reformat="$( command -v json_reformat || echo "cat" )"
${debug} "Will use ${json_reformat} for reformatting JSON data."

pandoc="$( command -v pandoc || true )"
[ -z "${pandoc}" ] && die 1 "pandoc executable not found, cannot continue."
${debug} "Found Pandoc executable at ${pandoc}."


# Default configuration values
site_title="${site_title:-My photo gallery}"

# Default publish dir
publish_dir="${publish_dir:-public_html}"
${debug} "Target publish dir ${publish_dir}."


thumbnail_size="${thumbnail_size:-200x200}"

# Setup path to template files (replace the following with something like
# /usr/local/share/nswi177-gallery when installing system wide).

data_files_dir="${data_files_dir:-}"
if [ -z "${data_files_dir}" ]; then
    data_files_dir="$( dirname "$( realpath "${BASH_SOURCE[0]}" )" )"
fi
${debug} "Loading data files from ${data_files_dir}"

theme_dir="${theme_dir:-}"
if [ -z "${theme_dir}" ]; then
    theme_dir="${data_files_dir}"
fi
${debug} "Loading template files from theme directory: ${theme_dir}"


# Get list of albums (see get_album_list to understand why this is
# safe to be used in for loops).
if ! [ -d "albums" ]; then
    die 2 "No albums/ directory found."
fi
albums="$( get_album_list )"
${debug} "Found albums: $( echo "${albums}" | paste -s -d ' ' )."


# Prepare images for each album first
# We do not prepare the HTML pages yet as we have not yet discovered all
# the albums and we do not have their meta information (that might be
# needed for menu, for example).
for album in ${albums}; do
    unset front_image
    title=${album}
    if [ -f "albums/${album}/album.rc" ]; then
    # This does not work: shellcheck disable=SC1090
    # As this is dynamic source, there is no way to make it work. I could not find why the disable 
    # does not work.
    # shellcheck source=/dev/null
    . "albums/${album}/album.rc" 
    fi

    front_image=${front_image:-}

    prepare_album "${album}" "albums/${album}" "$publish_dir/${album}" "${title}" "${front_image}"
done


# Create meta JSON for the whole site. This meta file is then added to
# each generated page so that each album page can create global menu of
# albums etc.
${debug} "Preparing global meta JSON file."
cat "${publish_dir}"/*/.meta | (
    echo '{'
    echo '  "site": {'
    printf '    "title": "%s",\n' "${site_title}"
    echo '    "albums": ['
    while read -r album_dir album_front_image album_title; do
        print_simple_json_dictionary \
            "dir" "${album_dir}" \
            "title" "${album_title}" \
            "image" "${album_front_image}"
        echo ','
    done
) | sed -e '$s/.*/]}}/' | "${json_reformat}" >"${publish_dir}/.meta.json"


# Generate the actual HTML page for each album. Notice that we pass
# two metadata files so that we allow album.tpl.html access both
# information about the current album as well as the overview information
# about the whole site.
for album in ${albums}; do
    ${debug} "Generating HTML page for album ${album}."
    ( cat "albums/${album}/HEADER.md" 2>/dev/null || true ) \
        | "${pandoc}" \
            --template "${theme_dir}/album.tpl.html" \
            --metadata-file="$publish_dir/${album}/.details.json" \
            --metadata-file="$publish_dir/.meta.json" \
            >"$publish_dir/${album}/index.html"
done

# Generate the index page. This one needs only the overview meta information
${debug} "Generating index page."
( cat HEADER.md 2>/dev/null || true ) \
    | "${pandoc}" \
            --template "${theme_dir}/index.tpl.html" \
            --metadata-file="$publish_dir/.meta.json" \
            "--metadata=title:${site_title}" \
            >"$publish_dir/index.html"

${debug} "Will try to copy CSS files."
if [ -d "${data_files_dir}/assets" ]; then
    cp "${data_files_dir}/assets/"* "${publish_dir}/"
fi
