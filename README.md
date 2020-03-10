.STJ is a video file format created by STMicroelectronics around 2004. It is produced by devices based around the STV0674 chip.
http://pdf.dzsc.com/74-/STV0674-100TQFP_1134827.pdf

This chip is obsolete, but between 2004 and 2006 it was used in a series of “eDVR 5 in 1” USB cameras. Known devices include:

eDVR 5-in 1
ST Labs 5 in 1
Verbatim Store N'Go eDVR 5 in 1
Sunrich Technology's USB 5 in 1 eDVR
Sunsway 5 in 1 DVR 
Octave Multipod
Sahara 5 in 1 Flash Memory

.STJ is not designed to be played but just stored.

Inside the file is an MJPEG stream peppered with u8 raw audio at either 8000, 11025 or 22050 khz. Generally video was shot at 320x240 and 30 or 15 FPS. 

The MJPEG stream is just a standard FF D8 and FF D9 start / stop format. The audio however has no pattern as to how often it appears, so JPEG will be corrupted and filled with audio data if you were to extract them from the STJ file

Audio data is always 512 bits in length and features FF FF at the start followed by 510 bits of data.

The header features the FourCC of STAV (ST Microelectronics Audio Video). The header info is 36 bits long and then padded with 0's till it reaches 512 bits. At 512 bits the first video frame starts and at 1024 the first audio sample is saved.

A rough layout of the file format is below.

byte          function
0-3           FourCC STAV
4 -5          01 - Format version, only one exsists.
6 - 14        Unknown
15 - 16       Video Width (byte 15 * 256 + byte 16)
17 - 18       Video Height (byte 17 * 256 + byte 18)
19 - 20       Video no of frames (byte 19 * 256 + byte 20)
21 - 22       Video FPS ((byte 21 * 256 + byte 22) / 2)
29 - 30       Audio Freq (byte 29 * 256 + byte 30)
31 - 32       Audio no of channels (byte 31 * 256 + byte 32)
33 - 34       Audio bits per sample (byte 33 * 256 + byte 34)
33 - 511      Padding till first video frame
512 - 1023    Start of first video frame
1024 - 1536   First slice of audio
1537 - EOF    Audio and video data

Demuxstj is a small command line util to extract the data in the STJ file and provide an MJPEG and WAV file to be muxed by FFmpeg. It’ll provide you with an FFMpeg command to proceed.

Hopefully this’ll help anyone who has any remaining STJ files convert them to a format that works.

If anyone has any STJ files they are willing to share and test the program out with, please let me know.
