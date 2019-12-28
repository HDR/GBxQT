#include "ReadFlashThread.h"
#include "Settings.h"
#include "const.h"
#include <QFileDialog>
#include <stdio.h>
#include "Gui.h"


void
ReadFlashThread::run ()
{
    end = false;
    Gui::set_mode('0');
    FILE *romFile = fopen(filename.toLatin1(), "wb");
    if(romFile == nullptr)
    {
        return;
    }

    if(cMode == GB_MODE){
        uint32_t readBytes = 0;
        currAddr = 0x0000;
        endAddr = 0x7FFF;
        for (uint16_t bank =1; bank < romBanks; bank++){
            if (cartridgeType >= 5) { // MBC2 and above
                Gui::set_bank(0x2100, bank);
                if (bank >= 256) {
                    Gui::set_bank(0x3000, 1); // High bit
                }
            }
            else { // MBC1
                if ((strncmp(gameTitle, "MOMOCOL", 7) == 0) || (strncmp(gameTitle, "BOMCOL", 6) == 0)) { // MBC1 Hudson
                    Gui::set_bank(0x4000, bank >> 4);
                    if (bank < 10) {
                        Gui::set_bank(0x2000, bank & 0x1F);
                    }
                    else {
                        Gui::set_bank(0x2000, 0x10 | (bank & 0x1F));
                    }
                }
                else { // Regular MBC1
                    Gui::set_bank(0x6000, 0); // Set ROM Mode
                    Gui::set_bank(0x4000, bank >> 5); // Set bits 5 & 6 (01100000) of ROM bank
                    Gui::set_bank(0x2000, bank & 0x1F); // Set bits 0 & 4 (00011111) of ROM bank
                }
            }
            if (bank > 1) { currAddr = 0x4000; }

            // Set start address and rom reading mode
            Gui::set_number(currAddr, SET_START_ADDRESS);
            Gui::set_mode(READ_ROM_RAM);

            // Read data
            while (currAddr < endAddr) {
                if (!Gui::com_read_bytes(romFile, 64)) {
                    emit error(FILEERROR_W);
                }
                Gui::com_read_bytes(romFile, 64);
                currAddr += 64;
                readBytes += 64;

                // Request 256 bytes more
                if (currAddr < endAddr) {
                    RS232_cputs(cport_nr, "1");
                }

                uint8_t comReadBytes = Gui::com_read_bytes(romFile, 64);
                if (comReadBytes == 64) {
                    currAddr += 64;
                    readBytes += 64;

                    // Request 64 bytes more
                    if (currAddr < endAddr) {
                        Gui::com_read_cont();
                    }
                }
                else { // Didn't receive 64 bytes, usually this only happens for Apple MACs
                    fflush(romFile);
                    Gui::com_read_stop();
                    Sleep(500);
                    printf("Retrying\n");

                    // Flush buffer
                    RS232_PollComport(cport_nr, readBuffer, 64);

                    // Start off where we left off
                    fseek(romFile, readBytes, SEEK_SET);
                    Gui::set_number(currAddr, SET_START_ADDRESS);
                    Gui::set_mode(READ_ROM_RAM);
                }

                // Print progress
                emit set_progress(readBytes, romBanks * 16384);
            }
            Gui::com_read_stop(); // Stop reading ROM (as we will bank switch)
        }
        fclose(romFile);
        emit error (true);

    }
    if(cMode == GBA_MODE){
            // Set start and end address
         currAddr = 0x00000;
         endAddr = romEndAddr;
         Gui::set_number(currAddr, SET_START_ADDRESS);

         uint16_t readLength = 64;
         if (gbxcartPcbVersion != PCB_1_0) {
             Gui::set_mode(GBA_READ_ROM_256BYTE);
             readLength = 256;
         }
         else {
             Gui::set_mode(GBA_READ_ROM);
         }

         // Read data
         while (currAddr < endAddr) {
             int comReadBytes = Gui::com_read_bytes(romFile, readLength);
             if (comReadBytes == readLength) {
                 currAddr += readLength;

                 // Request 64 bytes more
                 if (currAddr < endAddr) {
                     Gui::com_read_cont();
                 }
             }
             else { // Didn't receive 256 bytes
                 fflush(romFile);
                 Gui::com_read_stop();
                 Sleep(500);
                 qDebug() << "Retrying";

                 // Flush buffer
                 RS232_PollComport(cport_nr, readBuffer, readLength);

                 // Start off where we left off
                 fseek(romFile, long(currAddr), SEEK_SET);
                 Gui::set_number(currAddr / 2, SET_START_ADDRESS);
                 if (gbxcartPcbVersion != PCB_1_0) {
                     Gui::set_mode(GBA_READ_ROM_256BYTE);
                 }
                 else {
                     Gui::set_mode(GBA_READ_ROM);
                 }
             }

             // Print progress
             //print_progress_percent(progress, currAddr, endAddr);
             emit set_progress(int(currAddr), int(endAddr));

             //if (cancelOperation == 1) {
             //    break;
             //}
        }
         Gui::com_read_stop(); // Stop reading
    }

    fclose(romFile);
    emit error (true);
}

void
ReadFlashThread::canceled (void)
{
    end = true;
}
