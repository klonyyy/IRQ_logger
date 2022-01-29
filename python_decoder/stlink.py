
import subprocess
from typing import List
import win32com.client
import sys

class STLink():
    
    def check(self):
        try:
            stlink_output = subprocess.run(
                ["ST-LINK_CLI.exe", "-List"],
                check=False,
                stdout=subprocess.PIPE).stdout.decode().splitlines()
        except FileNotFoundError:
            print('ST-LINK_CLI.exe is missing! Put in the same directory or add its location to the PATH')
            return 2
        if 'No ST-LINK detected!' in stlink_output:
            return 1
        else:
            return 0

    def dump(self, probe: int = 0, memaddr: int = 0x20000000, memsize: int = 0x100, file_path = "./test.bin"):
        try:
            output = subprocess.check_output(
                [
                    'ST-LINK_CLI.exe',
                    '-c',
                    'ID=' + str(probe),
                    'SWD',
                    'HOTPLUG',
                    '-Q',
                    '-NoPrompt',
                    '-Dump',
                    hex(memaddr),
                    hex(memsize),
                    file_path,
                ],
                stderr=subprocess.STDOUT,
                stdin=subprocess.PIPE).decode().splitlines()
        except subprocess.CalledProcessError as err:
            output = err.output.decode().splitlines()
            print("Error during dump memory attempt! ST-LINK CLI output: " + output)
            sys.exit(1)

    def writeByte(self, probe: int = 0, memaddr: int = 0x20000000, data: int = 0x00):
        try:
            output = subprocess.check_output(
                [
                    'ST-LINK_CLI.exe',
                    '-c',
                    'ID=' + str(probe),
                    'SWD',
                    'HOTPLUG',
                    '-Q',
                    '-NoPrompt',
                    '-w8',
                    hex(memaddr),
                    hex(data),
                ],
                stderr=subprocess.STDOUT,
                stdin=subprocess.PIPE).decode().splitlines()
        except subprocess.CalledProcessError as err:
            output = err.output.decode().splitlines()
            print("Error during write byte attempt! ST-LINK CLI output: " + output)
            sys.exit(1)
