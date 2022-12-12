---
sd_hide_title: true
---

# Home
````{only} html

::::{grid}
:gutter: 3 4 4 4
:margin: 1 2 1 2

:::{grid-item}
:columns: 12 3 3 3

```{image} _static/logo.png
:width: 150px
:class: sd-m-auto
```

:::

:::{grid-item}
:columns: 12 9 9 9
:child-align: justify
:class: sd-fs-5

```{rubric} Some slogan
```
We are ...

We are ...

We are not ...

We are not ...

:::

::::


::::{grid} 2

:::{grid-item-card} {octicon}`rocket;1.5em;sd-mr-1` I want to use the SDK
:link: user_manual
:link-type: ref

I want to develop a new CAD, CAE, CAM, CAI, etc. application using the SDK.

+++
[Learn how to use»](user/quick_starter.md)
:::

:::{grid-item-card} {octicon}`plug;1.5em;sd-mr-1` I want to contribute to the SDK
:link: dev_doc
:link-type: ref

I want to add new things to the SDK, such as,
 create a new starter template, add a new toolkit library, improve the docs, etc.

+++
[Learn how to contribute »](dev/quick_starter.md)
:::

::::

````

```{toctree}
:hidden:

overview.md
user/index.md
dev/index.md
```

```{toctree}
:hidden:
:caption: Reference

references/index.md
```

[commonmark]: https://commonmark.org/
[github-ci]: https://github.com/executablebooks/MyST-Parser/workflows/continuous-integration/badge.svg?branch=master
[github-link]: https://github.com/executablebooks/MyST-Parser
[codecov-badge]: https://codecov.io/gh/executablebooks/MyST-Parser/branch/master/graph/badge.svg
[codecov-link]: https://codecov.io/gh/executablebooks/MyST-Parser
[rtd-badge]: https://readthedocs.org/projects/myst-parser/badge/?version=latest
[rtd-link]: https://myst-parser.readthedocs.io/en/latest/?badge=latest
[black-badge]: https://img.shields.io/badge/code%20style-black-000000.svg
[pypi-badge]: https://img.shields.io/pypi/v/myst-parser.svg
[pypi-link]: https://pypi.org/project/myst-parser
[conda-badge]: https://anaconda.org/conda-forge/myst-parser/badges/version.svg
[conda-link]: https://anaconda.org/conda-forge/myst-parser
[black-link]: https://github.com/ambv/black
[github-badge]: https://img.shields.io/github/stars/executablebooks/myst-parser?label=github
[markdown-it-py]: https://markdown-it-py.readthedocs.io/
[markdown-it]: https://markdown-it.github.io/
[rst-to-myst]: https://rst-to-myst.readthedocs.io
[mystjs]: https://github.com/executablebooks/mystjs
