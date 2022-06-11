## Instructions

Use this command to compile: (you should be in this cpp folder)

```bash
g++ JSONRPC2Connection.cpp LPythonServer.cpp main.cpp -I. -o lpythcpp\
```

Now it will generate a binary/executable call `lpythcpp`, you can run it using:

```bash
./lpythcpp
```

Copy this executable to `/usr/bin`:

```bash
sudo cp lpythcpp /usr/bin
```

Now you can run it from anywhere.