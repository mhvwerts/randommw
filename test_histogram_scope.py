"""
Execute 'test_histogram.exe' and plot the histogram several times (scope)

This is a quick-and-dirty approach, which works on x86-64 gcc system and
may work on other systems, if all elements (zignormw code and Python stack)
have been compiled by similar compilers on one an the same system.

To make it more portable, we should be more explicit about the exact integer
sizes and double format. But it's OK for now
"""
import subprocess
import time

import numpy as np
import matplotlib.pyplot as plt

fp = "histogram.bin"
fo = "histogram{0:03d}.png"


for i in range(20):
    process = subprocess.run(['./test_histogram.exe',
                              '500000',
                              str(time.time_ns())], 
                             stdout=subprocess.PIPE, 
                             universal_newlines=True)
    # Launching test.histogram several times per second
    # leads to several runs with same seed if default
    # seed method is used. Therefore, we seed using
    # the time_ns value from Python
    
    print(process.stdout)
    
    assert process.returncode == 0, "Error running ./test_histogram.exe"
    
    
    with open(fp, "rb") as f:
        Nbins = np.fromfile(f, dtype=np.uint32, count=1)[0]
        HV = np.fromfile(f, dtype=np.float64, count = Nbins) 
        H = np.fromfile(f, dtype=np.uint64, count = Nbins)
    
    
    plt.figure(1)
    plt.clf()
    plt.plot(HV, H, '.')
    plt.xlim(-5,5)
    plt.ylim(0,4400)
    plt.savefig(fo.format(i), dpi = 150)
    
