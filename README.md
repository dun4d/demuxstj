STJ is a video file format created by STMicroelectronics around 2004. It is produced by devices based around the STV0674 chip.
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

Demuxstj is a small command line util to extract the data in the STJ file and provide an MJPEG and WAV file to be muxed by FFmpeg. It’ll provide you with an FFMpeg command to proceed.

Hopefully this’ll help anyone who has any remaining STJ files convert them to a format that works.

If anyone has any STJ files they are willing to share and test the program out with, please let me know.
