

# cuda
  741  sudo apt-get install build-essential
  742  sudo apt-get install cuda
  743  sudo apt update
  744  ubuntu-drivers devices
  745  sudo ubuntu-drivers autoinstall
  746  nvidia-settings 
nvcc 
sudo apt install nvidia-cuda-toolkit
headers https://forums.developer.nvidia.com/t/nsight-cant-find-cuda-built-ins-or-items-from-cuda-modified-headers/64382

https://docs.nvidia.com/cuda/nsight-eclipse-plugins-guide/index.html

sudo apt-get install -y mpi
sudo apt install libopenmpi-dev

https://docs.nvidia.com/nsight-visual-studio-edition/2020.1/cuda-build-run/index.html

Eclipxe
https://on-demand.gputechconf.com/supercomputing/2012/presentation/SB006-Goodwin-CUDA-Development-Nsight.pdf


###################### Run
https://stackoverflow.com/questions/15966046/cudamemgetinfo-returns-same-amount-of-free-memory-on-both-devices-of-gtx-690

gpu-library-advisor  gpu-manager          
z@z:~/github/accelerators/cuda/book$ nvidia-settings -q GPUUtilization -q useddedicatedgpumemory

  Attribute 'GPUUtilization' (z:1[gpu:0]): graphics=0, memory=6, video=0, PCIe=0

  Attribute 'UsedDedicatedGPUMemory' (z:1[gpu:0]): 550.
    'UsedDedicatedGPUMemory' is an integer attribute.
    'UsedDedicatedGPUMemory' is a read-only attribute.
    'UsedDedicatedGPUMemory' can use the following target types: GPU.
https://stackoverflow.com/questions/13940600/cudamalloc-always-gives-out-of-memory

https://forums.developer.nvidia.com/t/cuda-out-of-memory-with-tons-of-memory-left/13391

################ Debug
!!! > 3.5
https://stackoverflow.com/questions/35292723/all-cuda-devices-are-used-for-display-can-not-debug-my-cuda-code-from-within-de

fatal:  All CUDA devices are used for display and cannot be used while debugging. (error code = CUDBG_ERROR_ALL_DEVICES_WATCHDOGGED(0x18)

Product Name: A78M-E35 (MS-7721)

https://us.msi.com/Motherboard/A78M-E35.html
	
3 Display Outputs

Graphics

• 1 x VGA port, supporting a maximum resolution of 1920x1200 @ 60Hz, 24bpp
• 1 x DVI-D port, supporting a maximum resolution of 2560x1600@60Hz, 24bpp/ 1920x1200 @ 60Hz, 24bpp
• 1 x HDMI port, supporting a maximum resolution of 4096x2160@24Hz, 36bpp*/ 3840x2160@30Hz, 36bpp*/ 1920x1200@120Hz, 36bpp and 1920x1200@60Hz, 36bpp
* Only support when using an FM2+ APU

z@z:~$ sudo /usr/sbin/dmidecode -t baseboard
dmidecode -t 4

https://www.cnet.com/products/msi-a78m-e35-motherboard-micro-atx-socket-fm2-plus-amd-a78/

TODO встроенные графические порты не заработали


####################### Profile


nvprof --metrics branch_efficiency ./ch1
==8307== Error: Internal profiling error 4277:1.