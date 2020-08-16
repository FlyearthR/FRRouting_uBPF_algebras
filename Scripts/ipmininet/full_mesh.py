"""This file contains a simple example of topology connected with RIPng"""

from ipmininet.iptopo import IPTopo
from ipmininet.router.config import RouterConfig
from ipmininet.router.config.ripng import RIPng
from ipmininet.ipnet import IPNet
from ipmininet.cli import IPCLI
import sys
import os.path
import time

class RIPngNetwork(IPTopo):

    def build(self, *args, **kwargs):
        """
                                  +-----+
                                  | h1  |
                                  +-----+
                                     |
                                     |
                                     |
                                     |
                                     |
          +--+--+     +-----+     +--+--+
          | h0  +-----+ r0  +-----+ r1  +
          +-----+     +--+--+     +--+--+
                         |   \   /   |
                         |    \ /    |
                         |     X     |
                         |    / \    |
                         |   /   \   |
                      +--+--+     +--+--+     +-----+
                      | rn-1+-----+ r2  +-----+ h2  |
                      +--+--+     +-----+     +-----+
                         |
                         |
                         |
                         |
                         |
                      +--+--+
                      | hn-1+
                      +-----+
        """

        r=[]
        h=[]
        for i in range(N):
            r.append(self.addRouter_v6("r{}".format(i)))
            h.append(self.addHost("h{}".format(i)))
            self.addLink(h[i], r[i])

        l=[]
        for i in range(N):
            for j in range(i+1, N):
                li = self.addLink(r[i], r[j], igp_metric=1)
                li[r[i]].addParams(ip="2042:{}{}::{}/64".format(i, j, i))
                li[r[j]].addParams(ip="2042:{}{}::{}/64".format(i, j, j))
                self.addSubnet(nodes=[r[i], h[i]], subnets=["2042:{}{}::/64".format(i, i)])
                l.append(li)
        for i in range(N):
            r[i].addDaemon(RIPng, update_timer=5, timeout_timer=3600)

        super(RIPngNetwork, self).build(*args, **kwargs)

    def addRouter_v6(self, name):
        return self.addRouter(name, use_v4=False, use_v6=True,
                              config=RouterConfig)

N = int(sys.argv[1]) if sys.argv[1] else 10
N = N*N
path = sys.argv[2]
c = True
net = IPNet(topo=RIPngNetwork(), allocate_IPs=False)  # Disable IP auto-allocation
try:
    net.start()
    IPCLI(net)
    while c:
        for i in range(0, N):
            if not os.path.isfile(path+'/r'+str(i)+'.out'):
                c = True
                break
            c = False
        time.sleep(1)
finally:
    net.stop()
