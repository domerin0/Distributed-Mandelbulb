ffmpeg -framerate 30 -loop 1 -i title.bmp -c:v libx264 -t 5 1.mp4
ffmpeg -framerate 30 -i frame_%04d.bmp -i conc.mp3 -c:v libx264 -c:a copy -shortest 2.mp4

ffmpeg -i 'concat:1.mp4|2.mp4' -codec copy output.mp4