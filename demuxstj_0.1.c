// demuxstj - version 0.1 - March 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
	// Generate some needed vars
	FILE *STJ;
	FILE *TEMP;
	FILE *WAV;
	FILE *MJPEG;
	int y, z, loop = 0;
	int export = 0;
	int mode = 0;
	int frame_number = 0;
	int header[36];
	// Ignore. Silent option next release. Keep 0 for now.
	int silent = 0;
	
	// Generate the file names for output
	char *temp_filename = strtok(argv[argc -1], ".");
	STJ = fopen(strcat(temp_filename,".STJ"), "rb");
		
	temp_filename = strtok(argv[argc -1], ".");
	WAV = fopen(strcat(temp_filename,".RAW"), "wb+");
	
	temp_filename = strtok(argv[argc -1], ".");
	MJPEG = fopen(strcat(temp_filename,".MJPEG"), "wb+");
	
	temp_filename = strtok(argv[argc -1], ".");
	TEMP = fopen(strcat(temp_filename,".TEMP"), "wb+");
	
	
	// Load the hearder info into the program.
    while (loop <= 35) {
 	   header[loop] = fgetc(STJ);
 	   loop = loop + 1;
    };
	
	// Check if we have a valid STJ file (STAV01).
    if (header[0] == 83 && header[1] == 84 && header[2] == 65 && header[3] == 86 && header[4] == 48 && header[5] == 49) {
		printf("Valid STJ file\n");
    } else {
 		perror("Not a valid STJ file.\n");
    	exit(EXIT_FAILURE);
    };
	
	// Let's do some math to get the headers.
	int video_width = header[15] * 256 + header[16];
	int video_height = header[17] * 256 + header[18];
	int video_frames = header[19] * 256 + header[20];
	int video_fps = (header[21] * 256 + header[22]) / 2;
	int audio_freq = header[29] * 256 + header[30];
	int audio_chan = header[31] * 256 + header[32];
	int audio_bits = header[33] * 256 + header[34];
	
	// Let's show off what we have found
	if (silent == 0) {
		printf("VIDEO:\n");
		printf("Width: %i\n", video_width);
		printf("Height: %i\n", video_height);
		printf("No Of Frames: %i\n", video_frames);
		printf("FPS: %i\n\n", video_fps);
	
		printf("AUDIO\n");
		printf("Freq: %i\n", audio_freq);
		printf("Channels: %i\n", audio_chan);
		printf("Bits: %i\n\n", audio_bits);
	
		printf("Audio: Extracting...\n");
	};
	
	// Now onto the work. First, extract the audio.
	// Audio starts with FF FF and is always 512 character long including FF FF.
	while ((z = fgetc(STJ)) != EOF) {
		if (loop > 509) {
			export = 0;
			loop = 0;
		};	   
		
		if (export == 1) {
			putc(z,WAV);
			loop = loop + 1;
		};
		
		if (y == 255 && z == 255) {
			export = 1;
		};
		
		y = z;
	};

	// Reset the STJ and vars
	fseek(STJ, 0, SEEK_SET);
	y = 0;
	z = 0;
	export = 0;
	loop = 0;

	// Letting you know we are done with audio, now video.
	if (silent == 0) { 
		printf("Extacting video...\n");
	};
	
	// STJ video is MJPEG with segmants of audio chucked in for good messure. Messy.
	// First lets export all the JPEG frames into one big file
	// We'll remove the audio later
	while ((z = fgetc(STJ)) != EOF && frame_number != video_frames) {
		if (loop > 0){
			loop = loop + 1;
		};
	   
		if (loop > 511) {
			loop = 0;
		};

		if (y == 255 && z == 255) {
			loop = 1;
		};  
		
		if (y == 255 && z == 216 && loop == 0) {  
			export = 1;
			putc(y,MJPEG);
       };
	   
	   if (y == 255 && z == 217 && loop == 0) {
	       export = 0;
		   putc(z,TEMP);
	   };
       
	   if (export == 1) {
			putc(z,TEMP);
			frame_number++;
	   };
   	   y = z;
	};
	
	// Reset the Vars
	y = 0;
	z = 0;
	loop = 0;
	
	// Reset the STJ file
	fseek(STJ, 0, SEEK_SET);
	
	// Again, letting them know what's happening
	if (silent == 0) { 
		printf("Cleaning video...\n");
	};
	
	// Clean our temp file. Remove all audio.
	while ((z = fgetc(STJ)) != EOF) {
		if (y == 255 && z == 255 & loop == 0) {
				loop = 1;
				fseek(MJPEG, -1, SEEK_END);
		};
	
	    if (loop > 0 && loop <= 512) {
	    		loop = loop + 1;
	    };
		
		if (loop == 513) {
			loop = 0;
		};
		
		if (loop == 0) {
			putc(z,MJPEG);
		};
		y = z;
	};
	
	// We're done. Let's clean up
	temp_filename = strtok(argv[argc -1], ".");
	
	// Tell the user where to go from here
	printf("\nSource files exported, please use FFMpeg to remux:\n");
	printf("ffmpeg -f u%i -ar %i -ac %i -i %s.RAW -framerate %i -i %s.MJPEG -vcodec copy -acodec copy %s.MKV\n", audio_bits, audio_freq, audio_chan, temp_filename, video_fps, temp_filename, temp_filename);
	
	// Close our source files
	fclose(STJ);
    fclose(TEMP);
	fclose(WAV);
	fclose(MJPEG);
	
	// Delete the TEMP file
	temp_filename = strtok(argv[argc -1], ".");
	remove(strcat(temp_filename,".TEMP"));
	
	// We are done!
	return 0;
};