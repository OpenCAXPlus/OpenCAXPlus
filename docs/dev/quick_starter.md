(dev_doc)=
# Quick starter

## Dev environment

Need to have golang and cmake installed.

```sh
sudo apt install golang-go cmake
```

## Initial run

```sh
git clone https://github.com/OpenCAXPlus/OpenCAXPlusSDK.git
cd OpenCAXPlusSDK
cmake --preset="linux-gnu-Debug" -S "."
cmake --build --preset="linux-gnu-Debug"
```

The initial run will build the OpenCAXPlus command line tool "ocp", you can use the ocp command to manage the SDK from now on.

## Following runs

```
./ocp build
```

To build a starter, such as the BasicConsole starter

```
./ocp build -p starter/BasicConsole
```
