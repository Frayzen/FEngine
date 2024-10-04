#!/bin/sh

cd $(dirname $0)

ENV_NAME="fengine"
ENV_YML="env.yml"

eval "$(conda shell.bash hook)"
conda env list | grep $ENV_NAME
if [ $? -eq 1 ]; then
  echo "CREATING ENV"
  echo "USING ENV '$ENV_YML'"
  conda env create -n $ENV_NAME -f $ENV_YML 
fi


conda activate $ENV_NAME

mkdir deps 2>/dev/null
cd deps
if [ ! -d "$(pwd)/kokkos/" ]; then
  echo "CLONING KOKKOS..."
  git clone https://github.com/kokkos/kokkos/ --depth=1 -b master
fi
cd kokkos
echo "/!\\ You will be shown the kokkos configuration, please modify it according to your needs"
echo "Do NOT change the 'CMAKE_INSTALL_PATH' value"
echo "Press enter to continue"
read _
# rm -rf build
ccmake . -B build -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX

if [ $? -ne 0 ]; then
  echo "Aborting because of a bad configuration..."
  exit 1
fi

cd build
make -j12 install

if [ $? -ne 0 ]; then
  echo "Aborting because the build crashed..."
  exit 1
fi

cd $(dirname $0)


cmake . -B build
rm -rf compile_commands.json
ln -s ./build/compile_commands.json compile_commands.json
