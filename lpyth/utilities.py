class Logger:
    def __init__(self):
        self.path = "/home/ankita/Documents/git/python_logs.log"
    
    def log(self, msg):
        with open(self.path, "a") as _file:
            _file.write(msg)
