#create title video
ffmpeg -framerate 30 -loop 1 -i title.bmp -c:v libx264 -t 3 a.mp4

#combine video frmaes to gether
ffmpeg -framerate 30 -i frame_%04d.bmp -c:v libx264 b.mp4

#join title to video
ffmpeg -f concat -i playlist.txt -c copy no_sound.mp4

#add aduio track, -shortest stops the video when the shortest stream is over (the audio track should be longer than 4min)
ffmpeg -i conc.mp3 -i no_sound.mp4 -c copy -shortest output.mp4

#clean up
rm a.mp4 b.mp4 no_sound.mp4