## About the Project

This project is a prototype for:

1. A language server written in Python which generates a sample meta data (please see the list below), and transfers it to the received jsonrpc client. The message is returned as a dictionary. The following details are expected to be returned from our linter, but for now they have been hard coded.
  a. Starting index (character): start_index (Number).
  b. Ending index (character): end_index (Number).
  c. Hover message: hover_message (string).

2. The jsonrpc code is taken from fortls repository (https://github.com/gnikit/fortls/blob/master/fortls/jsonrpc.py)
3. A sample VSCode Extension for the linter, which calls the language server when the document is saved and when the document is a python file. It then received the metadata and draws it on the active editor (if any) with a wavy underline.

The objectives for this stage were to write a sample language server, and understand how to communicate between the server and the extension.

## How to Install?

Please follow the commands below:

```bash
# First install the LPython Language Server
git clone <server_repo> && cd <server_repo> && python3 setup.py install

# Install the linter
git clone https://github.com/ankitaS11/custom-vscode-linter
cd custom-vscode-linter
npm install && npm run compile
```

## Usage

Once installation is done, enter the `custom-vscode-linter` folder, open VSCode and open the `extension.ts` file in the `src` folder. Now launch the extension using ctrl + F5 (select VSCode Extension Development if a dropdown appears).

Now create a new “python” file, just make sure it has more than 24 characters (as the metadata is hardcoded, so the line will be drawn from the 14th to 24th character). Save the file, and you’ll see the magic! :)

## Credits

This extension is WIP, but I would like to take this opportunity to thank a few people.

Thanks to Ondrej Certik for guiding throughout this project. As my manager, as well as a guide, he has been very kind and helpful.

Thanks to the creators of [custom-vscode-linter](https://github.com/hchiam/custom-vscode-linter) and [fortls](https://github.com/gnikit/fortls/tree/master/fortls) for open sourcing their code, as well as great instructions! I’ve learnt a lot from their codes, so thank you both!

