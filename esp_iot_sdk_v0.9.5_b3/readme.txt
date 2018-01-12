1¡¢compile command:
    ./gen_misc.sh
    then choose one according to your needs.. 


2¡¢compile options

(1) COMPILE
    Possible value: gcc
    Default value: 
    If not set, use xt-xcc by default.

(2) BOOT
    Possible value: old/new
      old: use boot_v1.1
      new: use boot_v1.2
    Default value: old

(3) APP
    Possible value: 0/1/2
      0: original mode, generate eagle.app.v6.flash.bin and eagle.app.v6.irom0text.bin
      1: generate user1
      2: generate user2
    Default value: 0

(4) FLASH
    Possible value: 512/1024
      512:  use 512KB SPI Flash
      1024: use 1024KB SPI Flash
    Default value: 512

For example:
    make COMPILE=gcc BOOT=new APP=1 FLASH=1024