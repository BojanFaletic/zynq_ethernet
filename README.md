# Ethernet descriptor

This is example on how to transfer data from Ethernet to PL (FPGA logic). FPGA is running linux, and has 32k BRAM on 0x4000_0000 address.

This is then used to open socket on. This is example of echo server.

``` bash
$ ncat (ip)[192.168.1.2] (port)[8888]
```


# How to upload bitstream to FPGA

1) Build .bit file
``` bash
    bootgen -image Full_Bitstream.bif -arch zynq -process_bitstream bin
```

Example of .bif file for converting .bit into .bit.bin
``` c
all:
{
        design_1_wrapper.bit /* Bitstream file name */
}
```

Copy file to FPGA linux.


2) Set to full bitstream
``` bash
    echo 0 > /sys/class/fpga_manager/fpga0/flags
```


3) Upload to FPGA
``` bash
mkdir -p /lib/firmware
cp /media/design_1_wrapper.bit.bin /lib/firmware/
echo design_1_wrapper.bit.bin > /sys/class/fpga_manager/fpga0/firmware
```