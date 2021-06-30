# Ethernet descriptor

This is example on how to transfer data from Ethernet to PL (FPGA logic). FPGA is running linux, and has 32k BRAM on 0x4000_0000 address.

This is then used to open socket on. This is example of echo server.

``` bash
$ ncat (ip)[192.168.1.2] (port)[8888]
```