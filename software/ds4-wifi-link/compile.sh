
BUILD_ENVIRON="development"

while [[ $# -gt 0 ]]; do
  case "$1" in
    -p|--prod)
        BUILD_ENVIRON="production"
        shift
        ;;
    *)
        echo "Error: Unknown flag $1" >&2
        exit 1
        ;;
  esac
done

if [ ! -d "./build" ]; then
    mkdir build
fi
cd build || exit

rm -rf ./*
cmake --preset=default
export BUILD_ENVIRON=$BUILD_ENVIRON;
cmake ..
make
cd ..
