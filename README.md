# Distributed-Mandelbulb

###Description
This was done for a final project in Dr. Ned Nedialkov's Distributed Computing course at McMaster University. The project was to take some given serial code which generates Mandelbulb images, and parallelize it to be run on the GPU with openACC. We were to create a 4 minute video showing our results. 

![Mandalbulb Image](https://github.com/inikdom/Distributed-Mandelbulb/blob/auto_path/image.jpg "Mandalbulb")

###Timing

The timing results are as follows:

GPU | Resolution | Time for 1 frame
----|------------|------------------
Titan X | 1080p | 0.113s

* Note time was obtained by finding average time to generate 1 out of 7200 frames.
* These times were obtained using a ssd, on a hdd it takes about 20% longer

###Results

Video [link](https://youtu.be/fnS-nIbzeyI). 
