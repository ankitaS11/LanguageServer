import os
import sys

from pathlib import Path

from .jsonrpc import JSONRPC2Connection, ReadWriter


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

    def on_save(self, request: dict):
        params: dict = request["params"]
        uri: str = params["textDocument"]["uri"]
        filepath = path_from_uri(uri)

        if filepath.split(".")[-1] != "py":
            # It's not a python file
            return filepath
        self.send_diagnostics(uri)

    def send_diagnostics(self, uri: str):
        diag_results = {
            "start_index": 14,
            "end_index": 24,
            "hover_message": "This is a sample error message starting from 14th character to 24th character.",
        }
        self.conn.send_notification(
            "textDocument/publishDiagnostics",
            {"uri": uri, "diagnostics": diag_results},
        )

    def run(self, path: str):
        # Run server
        while self.running:
            # request = self.conn.read_message()
            request = {
                "params": {
                    "textDocument": {
                        "uri": path,
                    },
                },
            }
            # {
            #   "method": "textDocument/onSave"
            # }

            # request["method"] -- -textDocument/onSave
            self.on_save(request)
            self.running = False
