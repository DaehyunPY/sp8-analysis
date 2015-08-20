// LMInfo.h
// Informations file to control List-Mode Read

#define LM_FILE_VERSION		0x74656

#define LM_READFILE			-0x00000000
#define LM_NOFILEOPEN		-0x00000001
#define LM_CLOSEFILE		-0x00000002
#define LM_STARTFILE		-0x00000004
#define LM_HOLDFILE			-0x00000008
#define LM_ENDFILE			-0x00000010
#define LM_REWINDFILE		-0x00000020

#define LM_HARDWARE			0x00000001
#define LM_FILEREAD			0x00000002
#define LM_HARDWAREWRITE	0x00000004
#define LM_SKIPINSERT		0x00000008

#define LMS_HRUNNING		0x00000004
#define LMS_FRUNNING		0x00000003
#define LMS_RUNNING			0x00000002
#define LMS_PAUSED			0x00000001
#define LMS_NONE			0x00000000

#define LMW_NOTSET			0x00000000
#define LMW_EOF				0x00000001
#define LMW_TIME			0x00000002
#define LMW_COUNTS			0x00000003

// Data types possible in DAQ/DAN
#define DT_NOTSET			0x00000000
#define DT_CHANNEL			0x00000001
#define DT_TIMEINPS			0x00000010
#define DT_TIMEINNS			0x00000011

#define DAQ_ID_RAW			0x000000		// for RAW (no Data)
#define DAQ_ID_HM1			0x000001		// for HM1 (single)
#define DAQ_ID_TDC8			0x000002		// for TDC8/ISA/PCI
#define DAQ_ID_CAMAC		0x000003		// for CAMAC
#define DAQ_ID_2HM1			0x000004		// for 2 HM1
#define DAQ_ID_2TDC8		0x000005		// for 2 TDC8
