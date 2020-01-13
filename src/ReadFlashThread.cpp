#include "ReadFlashThread.h"
#include "const.h"
#include <QFileDialog>
#include <stdio.h>
#include "Gui.h"


void ReadFlashThread::run(){
    end = false;
    Gui::set_mode('0');
    FILE *romFile = fopen(filename.toLatin1(), "wb");
    if(cMode == GB_MODE){
        uint32_t readBytes = 0;
        currAddr = 0x0000;
        endAddr = 0x7FFF;
        for (uint16_t bank =1; bank < romBanks; bank++){
            if (cartridgeType >= 5) {
                Gui::set_bank(0x2100, bank);
                if (bank >= 256) {Gui::set_bank(0x3000, 1);}
            }
            else {
                if ((strncmp(gameTitle, "MOMOCOL", 7) == 0) || (strncmp(gameTitle, "BOMCOL", 6) == 0)) {
                    Gui::set_bank(0x4000, bank >> 4);
                    if (bank < 10) {Gui::set_bank(0x2000, bank & 0x1F);}
                    else {Gui::set_bank(0x2000, 0x10 | (bank & 0x1F));}
                }
                else {
                    Gui::set_bank(0x6000, 0);
                    Gui::set_bank(0x4000, bank >> 5);
                    Gui::set_bank(0x2000, bank & 0x1F);
                }
            }
            if (bank > 1) { currAddr = 0x4000; }
            Gui::set_number(currAddr, SET_START_ADDRESS);
            Gui::set_mode(READ_ROM_RAM);
            while (currAddr < endAddr) {
                if (end == true){emit error(END); break;}
                Gui::com_read_bytes(romFile, 64);
                currAddr += 64;
                readBytes += 64;
                if (currAddr < endAddr) {RS232_cputs(cport_nr, "1");}
                uint8_t comReadBytes = Gui::com_read_bytes(romFile, 64);
                if (comReadBytes == 64) {
                    currAddr += 64;
                    readBytes += 64;
                    if (currAddr < endAddr) {Gui::com_read_cont();}
                }
                else {
                    fflush(romFile);
                    Gui::com_read_stop();
                    sleep(500);
                    qDebug() << "Retrying\n";
                    RS232_PollComport(cport_nr, readBuffer, 64);
                    fseek(romFile, readBytes, SEEK_SET);
                    Gui::set_number(currAddr, SET_START_ADDRESS);
                    Gui::set_mode(READ_ROM_RAM);
                }
                emit set_progress(readBytes, romBanks * 16384);
            }
            Gui::com_read_stop();
        }
        fclose(romFile);
        emit error (true);

    }
    if(cMode == GBA_MODE){
         currAddr = 0x00000;
         endAddr = romEndAddr;
         Gui::set_number(currAddr, SET_START_ADDRESS);
         uint16_t readLength = 64;
         if (gbxcartPcbVersion != PCB_1_0) {
             Gui::set_mode(GBA_READ_ROM_256BYTE);
             readLength = 256;
         }
         else {Gui::set_mode(GBA_READ_ROM);}
         while (currAddr < endAddr) {
             if (end == true){emit error(END); break;}
             int comReadBytes = Gui::com_read_bytes(romFile, readLength);
             if (comReadBytes == readLength) {
                 currAddr += readLength;
                 if (currAddr < endAddr) {Gui::com_read_cont();}
             }
             else {
                 fflush(romFile);
                 Gui::com_read_stop();
                 sleep(500);
                 qDebug() << "Retrying";
                 RS232_PollComport(cport_nr, readBuffer, readLength);
                 fseek(romFile, long(currAddr), SEEK_SET);
                 Gui::set_number(currAddr / 2, SET_START_ADDRESS);
                 if (gbxcartPcbVersion != PCB_1_0) {Gui::set_mode(GBA_READ_ROM_256BYTE);}
                 else {Gui::set_mode(GBA_READ_ROM);}
             }
             emit set_progress(int(currAddr), int(endAddr));
        }
         Gui::com_read_stop();
    }
    fclose(romFile);
    emit error (true);
}

void ReadFlashThread::canceled (void){end = true;}
