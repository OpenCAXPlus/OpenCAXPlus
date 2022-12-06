# OpenCAXPlusSDK

The sdk documentation [site](https://sdk-dev.opencax.plus).
Our [landing page](https://opencax.plus).

## For SDK developer

toolkit use interface, lifecycle use toolkit libraries, starter use lifecycle, user defined application use starter.

### Dev environment

Need to have golang and cmake installed.

```
sudo apt install golang-go cmake
```

### Usage

Initial run

```
git clone https://github.com/OpenCAXPlus/OpenCAXPlusSDK.git
cd OpenCAXPlusSDK
cmake --preset="linux-gnu-Debug" -S "."
cmake --build --preset="linux-gnu-Debug"
```

The initial run will build the OpenCAXPlus command line tool "ocp", you can use the ocp command to manage the SDK from now on.
Following run

```
./ocp build
```

To build a starter, such as the BasicConsole starter

```
./ocp build -p starter/BasicConsole
```
