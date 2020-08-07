        #include <SdFat.h> // SdFat V "2.0.0"
        const char *SDFAT_ver = SD_FAT_VERSION;

        // Use built-in SD for SPI modes on Teensy 3.5/3.6.
        // Teensy 4.0 use first SPI port.
        // SDCARD_SS_PIN is defined for the built-in SD on some boards.
        #ifndef SDCARD_SS_PIN
        const uint8_t SD_CS_PIN = SS;
        #else  // SDCARD_SS_PIN
        // Assume built-in SD is used.
        const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
        #endif  // SDCARD_SS_PIN

        // SD_FAT_TYPE = 0 for SdFat/File as defined in SdFatConfig.h,
        // 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
        #define SD_FAT_TYPE 2

        SdFat sd; // SD
        // SdFat SD;

        // The file to be parsed.
        File m_pSource;

        const int DIR_max = 150; // [# of Directory - max]
        const int max_DIR_name_lenght = 100;
        char CURDIRname[max_DIR_name_lenght] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Elements with missing values will be initialized to 0;
        uint32_t DIRcount = 1; // COUNT of all DIRs on SD CARD
        char DIR_list_array[DIR_max][max_DIR_name_lenght]; // [# of DIRs max][max DIR name lenght];

        // uint32_t TOTAL_PMF_FILE_counter = 0;

        void setup() {
          // put your setup code here, to run once:
          Serial.begin(38400); // Teensy ignores this and use full USB speed:  T3.x USB 1.1/Full-Speed: 12 Mbps, T4.x Hi-Speed: 480 Mbps.
          byte i = 0;
          while (!Serial && millis() < 5000) { // wait for Arduino Serial Monitor
          }
          if (!sd.begin(SdioConfig(FIFO_SDIO))) { // SdFat.h Ver 2.0.0 Beta
            Serial.println("m.SD initialization failed!");
          } else {
            Serial.println("m.SD initialization OK");
            LISTDIRs();
            m_pSource = sd.open("/"); // ROOT
            PrintFiles(m_pSource);
            m_pSource.close();
          }

        #if defined(SdFat_h)
          if (!strcmp("2.0.0", SDFAT_ver)) { // ver 2.0.0
            Serial.print( "SdFat version "), Serial.println( SD_FAT_VERSION );
          } else if ((SDFAT_ver[0] == '2') && (SDFAT_ver[1] == '.')) { // ver 2.
            Serial.println("Warning: MTP Library was only tested on SdFat Version 2.0.0 beta");
            Serial.print("SdFat Version currently installed "), Serial.println( SD_FAT_VERSION );
          } else { // ver 1
            Serial.print("Wrong SdFat Version "), Serial.println( SD_FAT_VERSION );
          }
        #endif

        }

        void loop() {
          // put your main code here, to run repeatedly:

        }

        // # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
        //============================================================================
        //------------ LIST ALL DIRs ON SD CARD & SAVE IN DIR_list_array -------------
        //============================================================================
        void LISTDIRs() {
          Serial.println("\t**List Directories**");
          m_pSource = sd.open("/"); // ROOT
          char dirname[max_DIR_name_lenght]; // SDFAT // max_DIR_name_lenght
          if (m_pSource.isDirectory()) {
            strcpy(DIR_list_array[DIRcount], "/"); // array

            Serial.print("ROOT# ");
            Serial.print(DIRcount);
            Serial.print("\t");
            Serial.println(DIR_list_array[DIRcount]); // SDFAT LIB
            DIRcount++;
          }
          while (true) {
            File DIR = m_pSource.openNextFile();

            if (! DIR) {
              // no more files
              Serial.println("\t **no more Directories** ");
              DIR.close();
              break;
            }
            DIR.getName(dirname, sizeof(dirname));
            if (DIR.isDirectory()) {
              //DIRcount++;
              strcpy(DIR_list_array[DIRcount], dirname); // array
              strcat(DIR_list_array[DIRcount], "/");
              Serial.print("DIR# ");
              Serial.print(DIRcount);
              Serial.print("\t");
              Serial.println(DIR_list_array[DIRcount]); // SDFAT LIB
              DIRcount++;
            }
            DIR.close();
          }
          DIRcount --;
          Serial.print("\t **Directory count ");
          Serial.print(DIRcount), Serial.println(" **"), Serial.println();
          m_pSource.close();
        }
        //----------------------------------------------------------------------------

        //============================================================================
        // Print Files in Cur. Directory
        //============================================================================
        void PrintFiles(File dir) {
          Serial.println("\t**List Files in Cur. Directory**");
          char filename[max_DIR_name_lenght]; // SDFAT

          int len = strlen(filename);
          char *extension = filename + len - 4;

          while (true) {

            File entry = dir.openNextFile();
            if (! entry) {
              // no more files
              Serial.println("\t **no more files**");
              entry.close();

              // Serial.print("\t TOTAL PMF_FILE_counter: "), Serial.println(TOTAL_PMF_FILE_counter), Serial.println();
              break;
            }
            entry.getName(filename, sizeof(filename));

            if (entry.isDirectory()) {
              // skip
            } else {
              Serial.print(filename); // SDFAT LIB
              // files have sizes, directories do not
              Serial.print("       \t");
              Serial.print(entry.size(), DEC);

              Serial.print("\t dirIndex  "), Serial.println(entry.dirIndex());

              // PMF File Format counter
              /*
                len = strlen(filename);
                extension = filename + len - 4;
                if (!strcmp(".pmf", extension) || !strcmp(".PMF", extension)) {
                TOTAL_PMF_FILE_counter++;
                }
              */
            }
            entry.close();
          }
        }
        //----------------------------------------------------------------------------

    Reply With Quote Reply With Quote
    06-09-2020, 01:13 AM #16
    Silverlock
    Silverlock is offline
    Member

    Join Date
        Oct 2019
    Location
        Calgary
    Posts
        64  

        Quote Originally Posted by marcelodantas View Post
        If you don't mind, can you zip the changes and send over? I did the changes above and am still getting some errors when compiling.
        I think I must not create SD as :
        SdFat SD;
        When using the 4.1?
        Unfortunately, it's not that easy... I've done all this on my own "gone way out in left field" fork of RunCPM. ZCPR3.3, ZSDOS, serial and parallel port (yeah, it's me, mecparts).

        Let me start over with a current mainline copy and I'll get back to you. As I said, it only took me an hour to figure out the first time (and that included getting my SdFat extensions for setting file attributes and timestamps migrated to SdFat-beta), so it shouldn't take me too long. 

    Reply With Quote Reply With Quote
    06-09-2020, 01:22 AM #17
    marcelodantas
    marcelodantas is offline
    Junior Member

    Join Date
        Oct 2019
    Posts
        14  

        Quote Originally Posted by Silverlock View Post
        Unfortunately, it's not that easy... I've done all this on my own "gone way out in left field" fork of RunCPM. ZCPR3.3, ZSDOS, serial and parallel port (yeah, it's me, mecparts).

        Let me start over with a current mainline copy and I'll get back to you. As I said, it only took me an hour to figure out the first time (and that included getting my SdFat extensions for setting file attributes and timestamps migrated to SdFat-beta), so it shouldn't take me too long.
        Cool, thanks.
        I feel that I am really close. But it has been a long day so it is hard for me to concentrate. 

    Reply With Quote Reply With Quote
    06-09-2020, 01:33 AM #18
    Silverlock
    Silverlock is offline
    Member

    Join Date
        Oct 2019
    Location
        Calgary
    Posts
        64  

        Quote Originally Posted by marcelodantas View Post
        Cool, thanks.
        I feel that I am really close. But it has been a long day so it is hard for me to concentrate.
        No trouble at all. I'd just finished a 22km bike ride before sitting down at the computer so my mind is fresh even if my knees are complaining.

        And right now, grabbing a couple extra 4.1s as soon as DigiKey had them in stock is about to pay off. I don't even need to disturb the "TeensyBoard/Z80". 

    Reply With Quote Reply With Quote
    06-09-2020, 01:42 AM #19
    marcelodantas
    marcelodantas is offline
    Junior Member

    Join Date
        Oct 2019
    Posts
        14  

        I got it to work.

        Looks like I will have to do a major cleanup, but I will probably upload a new version to GitHub soon.

        Thanks a lot for the tips. 

    Reply With Quote Reply With Quote
    06-09-2020, 01:45 AM #20
    Silverlock
    Silverlock is offline
    Member

    Join Date
        Oct 2019
    Location
        Calgary
    Posts
        64  

        And I got my changes to work on the mainline code as well. If you want to check anything, I've pushed my changes to https://github.com/mecparts/RunCPM. It's up and running on one of the spare T4.1s. I'm just going to double check again a "clean but renamed" copy of SdFat-beta (i.e. without my file set attribute additions) but I don't expect any trouble.

        Edit: checked out okay with a "clean but renamed" copy of SdFat-beta. 

        Last edited by Silverlock; 06-09-2020 at 01:58 AM. 

    Reply With Quote Reply With Quote
