__author__ = 'Rostislav Varzar'

import os
import trik_protocol

# Init 12 V power in ARM controller
def init_power():
    pwr1 = "echo 1 > /sys/class/gpio/gpio62/value"
    os.system(pwr1)

