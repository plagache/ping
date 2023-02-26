#!/usr/bin/python3

import subprocess
# import time

make = subprocess.run(["make", "re"], stdout=subprocess.PIPE)
                          # "sudo", 
# print(make.stdout)

ping = subprocess.run(["timeout", "4", "../ft_ping", "8.8.8.8"], stdout=subprocess.PIPE)
real_ping = subprocess.run(["timeout", "4", "ping", "8.8.8.8"], stdout=subprocess.PIPE)
print("\n")
print(ping.stdout)
print("\n")
print(real_ping.stdout)
