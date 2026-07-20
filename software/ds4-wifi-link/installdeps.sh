sudo apt update


echo "Installing development dependencies"
sudo apt install curl zip unzip tar
sudo apt install libx11-dev libxft-dev libxext-dev libwayland-dev libxkbcommon-dev libegl1-mesa-dev libibus-1.0-dev


echo "Setting up VCPKG"

# add setup conditional on if vcpkg is already insatlled

if [ ! -d "$HOME/vcpkg" ]; then
    cd "$HOME" || exit 1
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg || exit 1
    ./bootstrap-vcpkg.sh
    echo -e 'Add to .bashrc:\nexport VCPKG_ROOT="$HOME/vcpkg"\nexport PATH="$VCPKG_ROOT:$PATH"'
else
    echo "VCPKG already exists"
fi
