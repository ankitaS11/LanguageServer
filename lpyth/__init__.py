import sys

from .server import LPythonServer
from .jsonrpc import ReadWriter, JSONRPC2Connection

def main():
    if len(sys.argv) <= 1:
        path = "/home/ankita/Documents/git/practice/hello.py"
    else:
        path = sys.argv[1]
    stdin, stdout = sys.stdin.buffer, sys.stdout.buffer
    LPythonServer(
        conn=JSONRPC2Connection(ReadWriter(stdin,  stdout)),
    ).run(path)