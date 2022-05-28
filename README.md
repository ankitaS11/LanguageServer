# `lpyth` Language Server

This repo contains an implementation of simple Langage Server Protocol (LSP) for Python.

All code editors (VS Code, Atom, Sublime Text, (Neo)Vim) that supports LSP can integrate with `lpyth`.

## Features:
  1. Draws a line when document is saved.
  ![image](https://user-images.githubusercontent.com/68434944/170823254-8e391875-8325-45e4-95d2-58d9fd48b88b.png)

  2. Symbol Lookup. 
  ![image](https://user-images.githubusercontent.com/68434944/170823314-590190b2-842c-4bf9-8999-829458a4977f.png)


Note: Meta data is being hardcoded.

## Installation

``` bash
git clone https://github.com/ankitaS11/LanguageServer
cd LanguageServer
python3 setup.py develop
```

## Clean project:

```bash
pip uninstall lpyth
# and also remove folders if they exist (build, dist)
rm -rf build dist
```

