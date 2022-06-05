import os
import sys
import traceback
from urllib.parse import unquote

from pathlib import Path

from .jsonrpc import JSONRPC2Connection, ReadWriter, path_from_uri
from .json_templates import symbol_json

class DiagnosticSeverity:
    Error = 1
    Warning = 2
    Information = 3
    Hint = 4


def path_from_uri(uri: str) -> str:
    # Convert file uri to path (strip html like head part)
    if not uri.startswith("file://"):
        return os.path.abspath(uri)
    if os.name == "nt":
        _, path = uri.split("file:///", 1)
    else:
        _, path = uri.split("file://", 1)
    return str(Path(unquote(path)).resolve())


class LPythonServer:
    def __init__(self, conn):
        self.conn = conn
        self.running = True
        self.post_messages = []

    def on_save(self, request: dict):
        params: dict = request["params"]
        uri: str = params["textDocument"]["uri"]
        filepath = path_from_uri(uri)

        if filepath.split(".")[-1] != "py":
            # It's not a python file
            return filepath
        self.send_diagnostics(uri)

    def serve_onSave(
        self, request: dict, did_open: bool = False, did_close: bool = False
    ):
        # Update workspace from file on disk
        params: dict = request["params"]
        uri: str = params["textDocument"]["uri"]
        filepath = path_from_uri(uri)
        start_line = 2
        start_column = 0
        end_line = 3
        end_column = 10
        msg = "msg: Hi"
        diag_results = []
        diag_results.append({
            'source': 'source: Ankita',
            'range': {
                'start': {
                    'line': start_line,
                    'character': start_column,
                },
                'end': {
                    'line': end_line,
                    'character': end_column,
                },
            },
            'message': msg,
            'severity': 2,
        })

        self.conn.send_notification(
            "textDocument/publishDiagnostics",
            {"uri": uri, "diagnostics": diag_results},
        )

    
    def serve_document_symbols(self, request: dict):

        # Get parameters from request
        params: dict = request["params"]
        uri: str = params["textDocument"]["uri"]
        path: str = path_from_uri(uri)
        with open("/home/ankita/Documents/log.txt", "a") as _file:
            _file.write("Data types: "+
                str(request) + " " + str(type(request)) + "\n"
            )
        test_output = [
            symbol_json(
                "Ankita",
                12,
                uri,
                0,  # start_line
                0,  # start_character
                10, # end_line
                10  # end_character
            )
        ]
        return test_output
            

    
    # def serve_onChange(self, request: dict):
    #     # Update workspace from file sent by editor
    #     params: dict = request["params"]
    #     uri: str = params["textDocument"]["uri"]
    #     path = path_from_uri(uri)
    #     file_obj = self.workspace.get(path)
    #     if file_obj is None:
    #         self.post_message(f"Change request failed for unknown file '{path}'")
    #         return
    #     else:
    #         # Update file contents with changes
    #         reparse_req = True
    #         if self.sync_type == 1:
    #             file_obj.apply_change(params["contentChanges"][0])
    #         else:
    #             try:
    #                 reparse_req = False
    #                 for change in params["contentChanges"]:
    #                     reparse_flag = file_obj.apply_change(change)
    #                     reparse_req = reparse_req or reparse_flag
    #             except:
    #                 self.post_message(
    #                     f"Change request failed for file '{path}': Could not apply"
    #                     " change",
    #                     1,
    #                     exc_info=True,
    #                 )
    #                 return


    def post_message(self, msg, severity = 3, exc_info = False):
        self.conn.send_notification(
            "window/showMessage",
            {"type": severity, "message": msg}
        )

    def handle(self, request: dict):
        def noop(request: dict):
            return None

        handler = {
            "initialize": self.serve_initialize,
            # "textDocument/didOpen": self.serve_onSave,
            "textDocument/didSave": self.serve_onSave,
            "textDocument/documentSymbol": self.serve_document_symbols,
            # "workspace/symbol": self.serve_workspace_symbol,
            # "textDocument/didChange": self.serve_onChange,
            # "initialized": noop,
            # "$/cancelRequest": noop,
            # "$/setTrace": noop,
            # "shutdown": noop,
            # "exit": self.serve_exit,
        }.get(request["method"], self.serve_default)

        if "id" not in request:
            try:
                handler(request)
            except:
                print("error got")
            return

        try:
            resp = handler(request)
        except JSONRPC2Error as e:
            self.conn.write_error(
                request["id"], code=e.code, message=e.message, data=e.data
            )
        except Exception as e:
            self.conn.write_error(
                request["id"],
                code=-32603,
                message=str(e),
                data={
                    "traceback": traceback.format_exc(),
                },
            )
        else:
            self.conn.write_response(request["id"], resp)


    def serve_default(self, request: dict):
        return JSONRPC2Error(
            code = -32601,
            message="found error",
        )

    def serve_initialize(self, request: dict):
        server_capabilities = {
            "textDocumentSync": 2,
            "documentSymbolProvider": True,
        }
        return {"capabilities": server_capabilities}

    def serve_exit(self, request: dict) -> None:
        self.running = False


    def run(self, path: str):
        # Run server
        while self.running:
            try:
                request = self.conn.read_message()
                self.handle(request)
            except EOFError:
                break
            except Exception as e:
                self.post_message(f"Unexpected error: {e}", exc_info=True)
                break
            else:
                for message in self.post_messages:
                    self.post_message(message[1], message[0])
                self.post_messages = []


class JSONRPC2Error(Exception):
    def __init__(self, code, message, data=None):
        self.code = code
        self.message = message
        self.data = data