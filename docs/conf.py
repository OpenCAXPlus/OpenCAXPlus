# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))
from datetime import date


# -- Project information -----------------------------------------------------

project = 'OpenCAXPlusSDK'
copyright = f"{date.today().year}, OpenCAXPlus Org."
author = 'OpenCAXPlux Org.'
master_doc = "index"


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    # ,'breathe', 'exhale'
    'myst_parser', "sphinx_design", "sphinxcontrib.mermaid", 'sphinxcontrib.bibtex'
]


source_suffix = ['.rst', '.md']


# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', 'node_modules']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_book_theme'
html_theme_options = {
    "home_page_in_toc": True,
    "github_url": "https://github.com/OpenCAXPlus/OpenCAXPlusSDK",
    "repository_url": "https://github.com/OpenCAXPlus/OpenCAXPlusSDK",
    "use_repository_button": True,
    "repository_branch": "main",
    "path_to_docs": "docs",
    "use_edit_page_button": True,
}
html_logo = "_static/ocp.png"
html_favicon = "_static/logo.png"
html_title = "OpenCAXPlus SDK"
# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']


myst_enable_extensions = [
    "dollarmath",
    "amsmath",
    "deflist",
    "fieldlist",
    "html_admonition",
    "html_image",
    "colon_fence",
    "smartquotes",
    "replacements",
    "strikethrough",
    "substitution",
    "tasklist",
    "attrs_image",
]
myst_number_code_blocks = ["typescript"]
myst_heading_anchors = 2
myst_footnote_transition = True
myst_dmath_double_inline = True


# # Setup the breathe extension
# breathe_projects = {
#     "PREDICT": "./_doxygen/xml"
# }
# breathe_default_project = "PREDICT"

# # Setup the exhale extension
# exhale_args = {
#     # These arguments are required
#     "containmentFolder":     "./api",
#     "rootFileName":          "library_root.rst",
#     "doxygenStripFromPath":  "..",
#     # Heavily encouraged optional argument (see docs)
#     "rootFileTitle":         "程序",
#     # Suggested optional arguments
#     "createTreeView":        True,
#     # TIP: if using the sphinx-bootstrap-theme, you need
#     # "treeViewIsBootstrap": True,
#     "exhaleExecutesDoxygen": True,
#     "exhaleDoxygenStdin":    "INPUT = ../src/"
# }

# # Tell sphinx what the primary language being documented is.
# primary_domain = 'cpp'

# # Tell sphinx what the pygments highlight language should be.
# highlight_language = 'cpp'

# numfig = True

bibtex_bibfiles = ['references/refs.bib']
