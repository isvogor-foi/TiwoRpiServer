# TiwoRpiServer
Server side for Tiwo FPGA image processing

Make sure you loaded .bit file to the FPGA on RaspberryPi, and then, run the following command:

sudo java -Djava.library.path=/home/pi/java/workspace/TiwoRpiServer/src/cimgspi/ -jar jartest.jar  (this is the name of the jar which runs in background waiting for processing orders) 
