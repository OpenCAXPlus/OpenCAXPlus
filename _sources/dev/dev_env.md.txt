# Development environment

## Recommended setup

### Code editor
vscode

### Compilers
gnu, intel oneapi
```
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 12
sudo update-alternatives --config g++
```

### Dependencies

Need to have golang and cmake installed.

::::{tab-set}

:::{tab-item} Linux
```sh
sudo apt install golang-go cmake
```
:::

:::{tab-item} Mac
```sh
brew install go cmake
```
:::

:::{tab-item} Windows
Content 1
:::

::::
