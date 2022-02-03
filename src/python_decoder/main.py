import numpy as np
import matplotlib.pyplot as plt
import struct
import sys
import time
from collections import Counter
from elftools.elf.elffile import ELFFile
from turtle import width
from cursor import Cursor
from stlink import STLink

# EDIT THIS SECTION ACCORDINGLY
ELF_FILE_PATH   = 'C:/Users/klonyyy/PROJECTS/IRQ_logger/examples/IRQ_logger_STM32G474CCU6/Debug/IRQ_logger_STM32G474CCU6.elf'
ENABLE_VAR_NAME = 'logging_enabled'             # name of the variable that triggers logging
DEBUG_BUFF_VAR_NAME = "IRQ_logger_buff"         # name of the log buffer
LOG_FILE_PATH = "test.bin"                      # name of the file where binary log will be saved (later you can reuse previously recoded data)
COLLECT_TIME = 1                                # approximate time needed for the buffer to be filled up
LABELS = ["IRQ1 (TIM16 highest)", "IRQ2 (TIM6 medium)", "IRQ3 (TIM7 lowest)", "IRQ4 (systick)"]  
# END OF USER DEFINED SECTION #

ENTRY_SIZE = 6 # single log entry size in bytes

# open *.elf file 
elf = ELFFile(open(ELF_FILE_PATH, 'rb'))
# Get the symbol table entry for the respective symbol
symtab = elf.get_section_by_name('.symtab')
if not symtab:
    print('No symbol table available in the specified *.elf file!')
    sys.exit(1)
# check if the defined symbols can be found in the *.elf file 
try:
    sym_enable = symtab.get_symbol_by_name(ENABLE_VAR_NAME)[0]
    enable_addr = sym_enable['st_value']
except:
    print('Symbol ' + ENABLE_VAR_NAME + ' not found!')
    sys.exit(1)
try:
    sym_debug_buff = symtab.get_symbol_by_name(DEBUG_BUFF_VAR_NAME)[0]
    debug_buff_addr = sym_debug_buff['st_value']
    debug_buff_size = sym_debug_buff['st_size']
except:
    print('Symbol ' + DEBUG_BUFF_VAR_NAME + ' not found!')
    sys.exit(1)

print('Found defined symbols:\r\n - enable address:' + hex(enable_addr) + '\r\n - debug_buff address:' + hex(debug_buff_addr) + ' debug_buff size:' + str(debug_buff_size))
# calculate numer of entries based on buffer size from elf file 
MAX_ENTRIES = int(debug_buff_size/ENTRY_SIZE)

stlink = STLink()
# if ST-Link is not available, just read data from the file
if stlink.check() == 0:
    # write enable byte to 1 and wait for the buffer to fill with data 
    stlink.writeByte(memaddr=enable_addr,data = 0x01)
    time.sleep(COLLECT_TIME)
    # dump the filled buffer to the file
    stlink.dump(memaddr=debug_buff_addr,memsize=debug_buff_size,file_path=LOG_FILE_PATH);
    time.sleep(COLLECT_TIME)
    print('ST-LINK found!')
else:
    print("ST-LINK not found! Using data from available log file")

# open log file
f = open(LOG_FILE_PATH, "rb+")

mybytearray = bytearray()

IRQs = []
data = []
# write bytes to data vector and fill IRQs with all the available IRQ labels
for i in range(MAX_ENTRIES-1):
    data.append(struct.unpack("HHBB", f.read(ENTRY_SIZE)))
    IRQs.append(data[i][2])

# find all the IRQ IDs - these are the ones specified in the dbg_addIRQ() function
IRQLabels = list(Counter(IRQs).keys())
IRQLabels.sort()
print("Found the following IRQ labels: " + str(IRQLabels))

IRQList = []
timeList = []
IRQStates = []
# append lists to the IRQList and first states to IRQStates
for i in range(len(IRQLabels)):
    l = []
    IRQList.append(l)
    # the i*2 is to separate the plots vertically
    IRQStates.append(int(i*2))

# iterate through every entry, modify states in IRQStates[], add each entry twice to IRQList[] so that it forms rectangles on the plot
for i in range(MAX_ENTRIES-1):
    timeList.append(data[i][0]*0.001 + data[i][1]*0.000001)
    for j in range(len(IRQLabels)):
        IRQList[j].append(IRQStates[j])
        if data[i][2] == IRQLabels[j]:
            if data[i][3] == 0:
                IRQStates[j] = 1 + 2*j
            else:
                IRQStates[j] = 0 + 2*j
        IRQList[j].append(IRQStates[j])
    timeList.append(data[i][0]*0.001 + data[i][1]*0.000001)

# create plot, add cursors
fig, ax = plt.subplots()
cursor = Cursor(ax,timeList,IRQList[0])
cid1=  plt.connect('motion_notify_event', cursor.mouse_move)
cid2 = plt.connect('button_press_event', cursor.on_click)

# plot it!
fig.suptitle('IRQ Logger', fontsize=14, fontweight='bold')
for i in range(len(IRQLabels)):
    ax.plot(timeList,IRQList[i],linewidth=2, label=LABELS[i])
ax.set_ylim([-1, len(IRQLabels)*2 + 1])
ax.legend()
ax.grid()
plt.show()
