import torch
import torch.nn as nn
from torch.autograd import Variable

from mfc.modules.noop import NoopEncoder
from mfc.modules.noop import NoopDecoder


class MyNetwork(nn.Module):
    def __init__(self):
        super(MyNetwork, self).__init__()
        self.noop_encoder = NoopEncoder()
        self.noop_decoder = NoopDecoder()

    def forward(self, inp):
        encoded = self.noop_encoder(inp)
        decoded = self.noop_decoder(encoded)
        return decoded

model = MyNetwork()

x = torch.arange(0, 4000).view(10, 4, 10, 10)

inp = Variable(x)

print(inp)
out = model(inp)
print(out)

print('noop success:', (inp == out).all())