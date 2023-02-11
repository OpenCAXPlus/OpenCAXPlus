# Build system

Now you understand the [core four parts](folders/#core-folders) of our SDK, we will discuss how the cmake build system is configured.

:::{admonition} Important: Modern cmake
:class: important

We use modern cmake, you should also do so if you want to contribute your code to the SDK. The key is always think from the cmake target's perspective.
:::

## Target names

Following are the available targets in our build system and their purposes.

- `OCP::Interface`, an interface library target for all header files in the framework/interface folder.
- lifecycle targets, here we use the BasicConsole as an example
    - `${USER_PROGRAM}`, user defined name for the final executable target,
- toolkit targets, here we use the faspsolver library as an example
    - `OCP::external_faspsolver`, an imported interface library that links to the external library.
    - `OCP::toolkit_faspsolver`, a library target that contains source code for wrappers of the external library, it also links to the OCP::external_faspsolver target.
- Cli target: `cli` ALL target that build the golang cli tool.
- docs target: we have `html` and `latexpdf` EXCLUDE_FROM_ALL target for building the web page and pdf document. 

## Target relationships

There are two types of build when developing for the SDK:
1. build the SDK itself
2. build the starter template

:::{admonition} Why we have two builds
:class: important

It is the outcome of the following factors:
1. we want to distribute the toolkit source code and let the SDK users to build the used toolkits when they use them for maximum cross platform/compiler support and flexibility
2. cmake does not allow adding the same directory twice
:::

### Build the SDK/toolkits
To build the SDK, you just run `ocp build` command from the root of the SDK repository. This will build the CMake project located in the root folder.

Below, we illustrate the targets dependencies for building the SDK itself. All libraries in the toolkit are compiled, and the lifecycle main programs are built. Notice the starter template codes are not included in this build

[![](https://mermaid.ink/img/pako:eNqtlNtu2zAMhl9F0HaZpmi8A6IBA-IlGwZsaJF2V3FRKDK1aJUlQ5bRBUHefXRt5SDDuVquGPLnT32U7R0VNgfKqNT2RWy48-QhzQzBn9C8quYgiRXla0yk0pq9kVJ-nE4_RSL468EZrjtR_h7WcBOL6gpccEn4h-m7ToATlMF-yQVcXX321upn5dtaVa9_O15uiHS8gBfrnsnqawgf-_2rjH4PcZaZ2y93jB0T9JExFoD-Q3NDxMsS-34oCWIrdNP3dvfrfrF8ulveflvOfu6HGrVaY-O8Loptu5rmLi6IcTXdvDYNJo921G2OoO1DG44I1_o1XxHugKxrpT3OaBtz5UB4Zc3h1sM93qwW4UZLZ_-gisyag4U6ngXPeFAfVnnsPxzhenYdsmGn4f_T7By3de0ojqYhceoZrLpa7HROM-nTpMM0kx7N5GRyOkyTXqCZxDRnnjFNeokm6dOMx-NhnqTHk5zMxtZhIixeYEpipsg3puq5nTzHmaEjWoAruMrxe7RrShn1GyjwlWAY5iB53Ty7mdmjtC5z7mGRK28dZZLrCkaU197eb42gzLsagmiuOL4eRafa_wMCyrf6?type=png)](https://mermaid.live/edit#pako:eNqtlNtu2zAMhl9F0HaZpmi8A6IBA-IlGwZsaJF2V3FRKDK1aJUlQ5bRBUHefXRt5SDDuVquGPLnT32U7R0VNgfKqNT2RWy48-QhzQzBn9C8quYgiRXla0yk0pq9kVJ-nE4_RSL468EZrjtR_h7WcBOL6gpccEn4h-m7ToATlMF-yQVcXX321upn5dtaVa9_O15uiHS8gBfrnsnqawgf-_2rjH4PcZaZ2y93jB0T9JExFoD-Q3NDxMsS-34oCWIrdNP3dvfrfrF8ulveflvOfu6HGrVaY-O8Loptu5rmLi6IcTXdvDYNJo921G2OoO1DG44I1_o1XxHugKxrpT3OaBtz5UB4Zc3h1sM93qwW4UZLZ_-gisyag4U6ngXPeFAfVnnsPxzhenYdsmGn4f_T7By3de0ojqYhceoZrLpa7HROM-nTpMM0kx7N5GRyOkyTXqCZxDRnnjFNeokm6dOMx-NhnqTHk5zMxtZhIixeYEpipsg3puq5nTzHmaEjWoAruMrxe7RrShn1GyjwlWAY5iB53Ty7mdmjtC5z7mGRK28dZZLrCkaU197eb42gzLsagmiuOL4eRafa_wMCyrf6)

### Build the starter template
To build a specific starter template, you can use the command `ocp build -p starter/starter_Name` from the root of the SDK repository. The `-p` option will set the path to the CMake project to be built.

The build dependencies for a starter template project are shown below. Only the necessary libraries within the toolkit are built, and the starter code is included.

[![](https://mermaid.ink/img/pako:eNqtVMtu2zAQ_BWC7dFBYPcFs0AAqXGDAi0S2O7JNAJaXNZsKFKgKKSG4X_PKiL9kJOcqtNqObM7w11pSwsngTKqjHss1sIHMs-5JfgURtT1NSjiiuo5Jkobw94ppb6Mx197IPgXwFthIkh-ghUM-6CmBp-qfBCfxx8jADtoi3wlCri4uArOmQcdurO6Wf3xoloT5UUJj84_kMX3FC7P-QtOf6SYc3v77Y6xQ4IuGWPJ0H8gt45EVSHvp1ZQbArT8t5vf88m0_u76e3NNPu1e4kIVvb8RdcEa827cECcNRtSg4EigHxG1FisI0rtMa2d3U8szWC4mKRpVN79RRTJWgXpHK8YxezR-2s48PcSLrPLlE33kd7vs1NfXdXoYniqaHSuKH9d0ehM0ehIUf66ovwNRYeiSeKxy1QqnvW99bknevrc_M15RxRqqwN-buB7axCzZDHrguVh0YxeYd-YJwHKyohwtG_zbHozmcd1awm91rEGto5ryy23dEBL8KXQEv8C2xbIaVhDievOMJSgRGMCp9zuENpUEltOpA7OU6aEqWFARRPcbGMLyoJvIIGutUBHZUTtngD7vYiF?type=png)](https://mermaid.live/edit#pako:eNqtVMtu2zAQ_BWC7dFBYPcFs0AAqXGDAi0S2O7JNAJaXNZsKFKgKKSG4X_PKiL9kJOcqtNqObM7w11pSwsngTKqjHss1sIHMs-5JfgURtT1NSjiiuo5Jkobw94ppb6Mx197IPgXwFthIkh-ghUM-6CmBp-qfBCfxx8jADtoi3wlCri4uArOmQcdurO6Wf3xoloT5UUJj84_kMX3FC7P-QtOf6SYc3v77Y6xQ4IuGWPJ0H8gt45EVSHvp1ZQbArT8t5vf88m0_u76e3NNPu1e4kIVvb8RdcEa827cECcNRtSg4EigHxG1FisI0rtMa2d3U8szWC4mKRpVN79RRTJWgXpHK8YxezR-2s48PcSLrPLlE33kd7vs1NfXdXoYniqaHSuKH9d0ehM0ehIUf66ovwNRYeiSeKxy1QqnvW99bknevrc_M15RxRqqwN-buB7axCzZDHrguVh0YxeYd-YJwHKyohwtG_zbHozmcd1awm91rEGto5ryy23dEBL8KXQEv8C2xbIaVhDievOMJSgRGMCp9zuENpUEltOpA7OU6aEqWFARRPcbGMLyoJvIIGutUBHZUTtngD7vYiF)

All <span style="background-color:#fff799">yellow boxed targets</span> are managed by the SDK, <span style="background-color:#d5ebe1">blue boxed targets</span> are external open source projects, <span style="background-color:#f3a694">red boxed target</span> is the code that SDK users write (in this case, you as the SDK developer pretent to be a user). Only the used toolkit and lifecycle is built by cmake.