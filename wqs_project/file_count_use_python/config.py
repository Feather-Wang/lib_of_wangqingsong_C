class Config(object):
    def __init__(self):
        self.lists = []
    def read_config(self, filepath):
        fp = open(filepath)
        line = fp.readline().strip('\n')
        while line:
            self.lists.append(line)
            line = fp.readline().strip('\n')
        fp.close()
