
# Drone Simulator by BOMBA STUDIOS™

  

## Installation Guide

### Linux

#### Installing Raylib

```
sudo apt install cmake
sudo apt install build-essential git
sudo apt install libwayland-dev
sudo apt install libxkbcommon-dev
sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```

Then in your home directory

```
sudo git clone https://github.com/raysan5/raylib.git raylib

cd raylib/src/

sudo make PLATFORM=PLATFORM_DESKTOP

sudo make install
```

If you've done all of the steps above correctly, you have successfully installed raylib!

#### Compilation

```
cd build/

cmake ..

make
```

After compilation if you want to run the project

```
make run
```

And we are done!

### Mac

#### Installing Raylib

If you already have brew just skip this step

#### Installing brew

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

After installing brew

```
brew install cmake 

brew install raylib
```

If you’ve done all of the steps above correctly, you have successfully installed raylib!

#### Compilation

```
cd build/

cmake ..

make
```

After compilation if you want to run the project

```
make run
```

And we are done!