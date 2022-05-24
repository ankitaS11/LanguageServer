# lp_server

Note that this now supports `LanguageClient` with VSCode Extension. :)

## Build project:

```bash
python3 setup.py develop
```

## Clean project:

```bash
pip uninstall lpyth
# and also remove folders if they exist (build, dist)
rm -rf build dist
```

## Usage:

~Go to any other folder except this, and do: `lpyth <file_path>`~ (this will no longer work, it only expects request from the VSCode extension)
