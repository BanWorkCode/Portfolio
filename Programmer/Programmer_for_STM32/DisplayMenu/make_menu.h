#ifndef _makeMenu_H
#define _makeMenu_H
//
//
//=====================================================================================================================================
//
#include "displayMenu.h"
#include "sd_card.h"
#include "bootloader.h"
#include "esp8266_lib.h"
#include "Ftp.h"
//
//
//=====================================================================================================================================
//
#define NULL_ENTRY Null_Menu
//
//
//=====================================================================================================================================
//
//MAIN
//MAKE_MENU(Name, 					Next, 					Previous, 				Parent, 				Child, 		Select, 				Text)
MAKE_MENU ( Boot_MAIN,  		SDcard_MAIN,   	NULL_ENTRY,  			NULL_ENTRY, 		init_BOOT, 0, ( void* ) 0, "Boot" );
MAKE_MENU ( SDcard_MAIN,  	WIFI_MAIN,    	Boot_MAIN,  			NULL_ENTRY, 		getList_SD, 0, ( void* ) 0, "SD card" );
MAKE_MENU ( WIFI_MAIN,  		FTP_MAIN,    		SDcard_MAIN,  		NULL_ENTRY, 		NULL_ENTRY, 0, ESP8266_ShowNetworksList, "WI-FI" );
MAKE_MENU ( FTP_MAIN,  			FIRMWARE_MAIN,  WIFI_MAIN,  			NULL_ENTRY, 		List_FTP, 0, ( void* ) 0, "FTP" );
MAKE_MENU ( FIRMWARE_MAIN,  About_MAIN,  		FTP_MAIN,  				NULL_ENTRY, 		NULL_ENTRY, 0, SD_SelectFileForUpdateFirmware, "Firmware update" );
MAKE_MENU ( About_MAIN,  		DEINIT,    			FIRMWARE_MAIN,  	NULL_ENTRY, 		NULL_ENTRY, 0, DISPLAY_AboutVersion, "About the app" );
MAKE_MENU ( DEINIT,  				INIT,    				About_MAIN,  			NULL_ENTRY, 		NULL_ENTRY, 0, ESP8266_DEinit_uart, "Deinit" );
MAKE_MENU ( INIT,  					NULL_ENTRY,    	DEINIT,  					NULL_ENTRY, 		NULL_ENTRY, 0, ESP8266_init_uart, "init" );





//BOOT
//MAKE_MENU(Name, 					Next, 					Previous, 				Parent, 				Child, 			Select, 			Text)
MAKE_MENU ( init_BOOT,  		erase_BOOT,    	NULL_ENTRY,  			Boot_MAIN, 			NULL_ENTRY, 			0, PROGRAMMER_InitBootloader, 							"Init" );
MAKE_MENU ( erase_BOOT,  		Upload_BOOT, 		init_BOOT,  			Boot_MAIN, 			NULL_ENTRY, 			0, PROGRAMMER_EraseMemory, 									"Full Erase" );
MAKE_MENU ( Upload_BOOT,  	run_BOOT,    		erase_BOOT,  			Boot_MAIN, 			FullErase_UPLOAD, 0, ( void* ) 0, 														"Upload Firmware" );
MAKE_MENU ( run_BOOT,  			NULL_ENTRY,    	Upload_BOOT,  		Boot_MAIN, 			NULL_ENTRY, 			0, PROGRAMMER_RunProgramm, 									"Run programm" );
//MAKE_MENU ( test,  					NULL_ENTRY,    	run_BOOT,  				Boot_MAIN, 			NULL_ENTRY, 			0, PROGRAMMER_GetAddressesOfSectorsToCleanUp, 			"Test" );


//Upload Firmware
//MAKE_MENU(Name, 									Next, 											Previous, 					Parent, 				Child, 		Select, 				Text)
MAKE_MENU ( FullErase_UPLOAD,  			EraseSectors_UPLOAD,    		NULL_ENTRY,  				init_BOOT, 			NULL_ENTRY, 0, PROGRAMMER_UploadFirmwareAfterFullErase, 	"Full Erase" );
MAKE_MENU ( EraseSectors_UPLOAD,  	NULL_ENTRY,    							FullErase_UPLOAD,  	init_BOOT, 			NULL_ENTRY, 0, PROGRAMMER_UploadFirmwareAfterEraseSectors, 	"Erase Sectors" );

//SDCard
//MAKE_MENU(Name, 					Next, 					Previous, 				Parent, 				Child, 		Select, 				Text)
MAKE_MENU ( getList_SD,  		NULL_ENTRY,    	NULL_ENTRY,  			Boot_MAIN, 			NULL_ENTRY, 1, SD_OpenCard, "Open SD card" );


//WIFI Удалить это старое меню!!!
//MAKE_MENU(Name, 					Next, 					Previous, 				Parent, 				Child, 					Select, 					Text)
MAKE_MENU ( Init_WIFI,  		Networks_WIFI,  NULL_ENTRY,  			Boot_MAIN, 			NULL_ENTRY, 1, ESP8266_CheckConnection, "WI-FI init" );
MAKE_MENU ( Networks_WIFI,  CustCommand,    Init_WIFI,  			Boot_MAIN, 			NULL_ENTRY, 1, ESP8266_ShowNetworksList, "WI-FI networks" );
MAKE_MENU ( CustCommand,  	Keyboard_WIFI,  Networks_WIFI,  	Boot_MAIN, 			NULL_ENTRY, 1, ESP8266_CheckConnection, "Custom Command" );
//MAKE_MENU(GetListFiles_WIFI,  Keyboard_WIFI,    	CustCommand,  				Boot_MAIN, 			NULL_ENTRY, 1, ESP8266_ShowListOfFiles, "Get LIST FTP");
MAKE_MENU ( Keyboard_WIFI,  NULL_ENTRY,    	CustCommand,  		Boot_MAIN, 			NULL_ENTRY, 1, KeyboardMenu, "Keyboard" );

//FTP
//MAKE_MENU(Name, 					Next, 					Previous, 				Parent, 				Child, 					Select, 					Text)
MAKE_MENU ( List_FTP,  			New_FTP,  		NULL_ENTRY,  				Boot_MAIN, 			NULL_ENTRY, 1, FTP_ShowFtpList, "List FTP" );
MAKE_MENU ( New_FTP,  			NULL_ENTRY,  			List_FTP,  			Boot_MAIN, 			Adsress_NewFTP, 1, ( void* ) 0, "New FTP connection" );


//NewFTP
//MAKE_MENU(Name, 					Next, 					Previous, 							Parent, 				Child, 					Select, 					Text)
MAKE_MENU ( Adsress_NewFTP,  		Port_NewFTP,  	NULL_ENTRY,  				New_FTP, 			NULL_ENTRY, 1, FTP_AddNewFtpAdress, "Address" );
MAKE_MENU ( Port_NewFTP,  			Login_NewFTP,  	Adsress_NewFTP,  		New_FTP, 			NULL_ENTRY, 1, FTP_AddNewFtpPort, "Port" );
MAKE_MENU ( Login_NewFTP,  			Password_NewFTP,Port_NewFTP,  			New_FTP, 			NULL_ENTRY, 1, FTP_AddNewFtpLogin, "Login" );
MAKE_MENU ( Password_NewFTP,  	Path_NewFTP,  	Login_NewFTP,  			New_FTP, 			NULL_ENTRY, 1, FTP_AddNewFtpPassword, "Password" );
MAKE_MENU ( Path_NewFTP,  			Enter_NewFTP,  	Password_NewFTP,  	New_FTP, 			NULL_ENTRY, 1, FTP_AddNewFtpPath, "Path for save" );
MAKE_MENU ( Enter_NewFTP,  			Info_NewFTP,  	Path_NewFTP,  			New_FTP, 			NULL_ENTRY, 1, FTP_ConfirmInputNewFtpServer, "Confirm" );
MAKE_MENU ( Info_NewFTP,  			Delete_NewFTP,  Enter_NewFTP,  			New_FTP, 			NULL_ENTRY, 1, FTP_InfoAboutConnection, "Info" );
MAKE_MENU ( Delete_NewFTP,  		NULL_ENTRY,  		Info_NewFTP,  			New_FTP, 			NULL_ENTRY, 1, FTP_DeleteCurrent, "Delete" );
//
//
//=====================================================================================================================================
//
#endif
