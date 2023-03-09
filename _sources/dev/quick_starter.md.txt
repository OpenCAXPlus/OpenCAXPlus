(dev_doc)=
# Quick starter

:::{admonition} Important: Check environment
:class: important

Please check the [development environment](dev_env.md) requirement before running the following commands.
:::

## Step 1: Clone the source code

```sh
git clone https://github.com/OpenCAXPlus/OpenCAXPlusSDK.git
cd OpenCAXPlusSDK
```

## Step 2: Build the SDK
### Initial build


::::{tab-set}

:::{tab-item} Linux
```sh
cmake --preset="linux-gnu-Debug" -S "."
cmake --build --preset="linux-gnu-Debug"
export PATH=$PATH:/home/runner/code/OCP-SDK
```
:::

:::{tab-item} Mac
```sh
cmake --preset="darwin-gnu-Debug" -S "."
cmake --build --preset="darwin-gnu-Debug"
```
:::

:::{tab-item} Windows
```sh
cmake --preset="windows-gnu-Debug" -S "."
cmake --build --preset="windows-gnu-Debug"
```
:::

::::

Cmake will perform out of source build in the folder out/build/. You should also see an executable file **ocp** been created in the root of the repository.
This is the compiled executable of the golang OpenCAXPlus command line tool, you can use the **ocp** command to manage the SDK from now on.

### Install available toolkits

Here we are installing all toolkits, you can choose to install only some of them
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/opencaxplus/cache/lapack/3.11.0/install/lib
ocp build -i install_all
export PYTHONPATH=${PYTHONPATH}:$HOME/opencaxplus/cache/petsc/3.18.4/install/lib
```


### Following builds

We will use the **ocp** executable for all following builds. 
To make it easier using the **ocp** tool, you probably want to add the SDK repository root directory to your PATH variable.

```sh
echo "export PATH=\$PATH:$(pwd)" >> ~/.bashrc
```

Then, to build the SDK.
```sh
ocp build
```

To build a starter, you just need to pass the `-p` or `--path` option with the location to the starter, such as the BasicConsole starter
```sh
ocp build -p starter/BasicConsole
```

For more information of how the build system is setup, please read the [build system](build_system.md) section.

## Step 3: Add new toolkit

Not ready

Please read the [toolkit](toolkit.md) section for more guidance on how to work with toolkit development.
## Step 4: Add new starter

Not ready

Please read the [starter](starter.md) section for more guidance on how to work with starter development.

## Step 5: Build the doc

```sh
ocp build -t html
ocp build -t latexpdf
```

## Step 6: Package the SDK

```sh
ocp build -t package
```

Or `cmake --build . --target package` inside the build directory, or `cmake --build --preset="linux-gnu-Debug" --target package` from the project root directory.