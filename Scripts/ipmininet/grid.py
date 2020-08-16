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
        +-----+     +-----+                 +-----+     +-----+     +-----+
        | h00 +-----+ r00 +-----[ ... ]-----+r0n-2+-----+r0n-1+-----+h0n-1|
        +-----+     +--+--+                 +--+--+     +--+--+     +-----+
                       |           |           |           |
                       |           |           |           |
                       |           |           |           |
                       |           |           |           |
                       |           |           |           |
                       |           |           |
        [ ... ]-----[ ... ]-----[ ... ]-----[ ... ]-----[ ... ]-----[ ... ]
                       |           |           |           |
                       |           |           |           |
                       |           |           |           |
                       |           |           |           |
                       |           |           |           |
                       |           |           |           |
        +-----+     +-----+        |       +---+---+   +---+---+  +-------+
        |hm-20+-----+rm-20+-----[ ... ]----+rm-2n-3+---+rm-2n-2+--+rm-2n-1|
        +-----+     +--+--+        |       +---+---+   +---+---+  +-------+
                       |           |           |           |
                       |           |           |           |
                       |           |           |           |
                       |           |           |           |
                       |           |           |           |
        +-----+     +-----+        |       +---+---+   +---+---+  +-------+
        |hm-10+-----+rm-10+-----[ ... ]----+rm-1n-3+---+rm-1n-2+--+rm-1n-1|
        +-----+     +-----+                +-------+   +-------+  +-------+
        """

        r = []
        h = []
        for i in range(M):
            r.append([])
            for j in range(N):
                r[i].append(self.addRouter_v6("r{}".format(i*N+j)))
            for j in range(0, N, N-1):
                hi = self.addHost("h{}{}".format(i,j))
                self.addLink(hi, r[i][j])
                self.addSubnet(nodes=[r[i][j], hi], subnets=["2042:{}{}:{}{}::/64".format(i, j, i, j)])
                h.append(hi)

        l = []
        for i in range(M):
            for j in range(N-1):
                li = self.addLink(r[i][j], r[i][j+1], igp_metric=1)
                li[r[i][j]].addParams(ip="2042:{}{}:{}{}::{}{}/64".format(i, j, i, j+1, i, j))
                li[r[i][j+1]].addParams(ip="2042:{}{}:{}{}::{}{}/64".format(i, j, i, j+1, i, j+1))
                l.append(li)
        for i in range(M-1):
            for j in range(N):
                li = self.addLink(r[i][j], r[i+1][j], igp_metric=1)
                li[r[i][j]].addParams(ip="2042:{}{}:{}{}::{}{}/64".format(i, j, i+1, j, i, j))
                li[r[i+1][j]].addParams(ip="2042:{}{}:{}{}::{}{}/64".format(i, j, i+1, j, i+1, j))
                l.append(li)

        for i in range(M):
            for j in range(N):
                r[i][j].addDaemon(RIPng)

        super(RIPngNetwork, self).build(*args, **kwargs)

    def addRouter_v6(self, name):
        return self.addRouter(name, use_v4=False, use_v6=True,
                              config=RouterConfig)


N = int(sys.argv[1]) if len(sys.argv) > 1 else 4
#M = int(sys.argv[2]) if len(sys.argv) > 2 else 4
M = N
path = sys.argv[2]
c = True

net = IPNet(topo=RIPngNetwork(), allocate_IPs=False)  # Disable IP auto-allocation
try:
    net.start()
    IPCLI(net)
    while c:
        for i in range(0, N*N):
            if not os.path.isfile(path+'/r'+str(i)+'.out'):
                c = True
                break
            c = False
        time.sleep(1)
finally:
    net.stop()
