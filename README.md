# Distributed-Mandelbulb

###Description
This was done for a final project in Dr. Ned Nedialkov's Distributed Computing course at McMaster University. The project was to take some given serial code which generates Mandelbulb images, and parallelize it to be run on the GPU with openACC. We were to create a 4 minute video showing our results. 

![Mandelbulb Image](https://github.com/inikdom/Distributed-Mandelbulb/blob/auto_path/image.jpg "Mandelbulb")

###Timing

The timing results are as follows:

 Device | render 1 frame @ 1080p | render 1 frame @ 4k | speedup (for 4k)
----|------------|----------------------|---------------
 1 core  | 30.05s |  124.5s | -
 2 cores | 15.44s | 61.71s | 2.02x
 4 cores  | 8.31s | 33.72s | 3.69x
 8 cores  | 4.92s | 19.64s | 6.34x
Titan X  | 0.113s | 0.364s | 341.76x
 
* the CPU used was a AMD FX-9590 Vishera 8-Core 4.7 GHz, on a machine with 16gb of RAM.
* CPU parallelization was acheived using openMP
* Time was obtained by finding average time to generate 1 out of 7200 frames on GPU, and 1 out of 10 on CPU.
* These times were obtained using a ssd, on a hdd it takes about 20% longer

###Results

4K Video [link](https://youtu.be/-fkmAzlNoks). 
