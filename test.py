import subprocess

make = subprocess.run(["sudo", "make re"], stdout=subprocess.PIPE)
print(make.stdout)
