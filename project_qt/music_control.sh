#!/bin/bash

case "$1" in
	# Case: Start a new song
	-s)
	echo start $2
	madplay $2 -r 44100 --output=wave:- | aplay -D d80 & > output
	;;

	# Case: Stop the current song
	-k)
	echo stop $2
	killall madplay
	;;

	# Case: Continue playing the current song
	-g)
	echo play $2
	killall -CONT madplay
	;;

	# Case: Pause the current song
	-p)
	echo pause $2
	killall -STOP madplay
	;;
	
	# Case: Help menu default
	*)
	echo "Usage: ./music_control.sh [-command] [song name]"
	echo "[-command] can be -s for start, -k for stop,"
	echo "		-g for play, -p for pause"
	echo "[song name] must be full path or begin with ../mp3/"
	;;
esac
