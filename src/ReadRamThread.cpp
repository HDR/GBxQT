#include "ReadRamThread.h"
#include "const.h"
#include <QFileDialog>
#include <stdio.h>
#include "Gui.h"

void ReadRamThread::run(){
    end = false;
    if (cMode == 1) {
        if (ramEndAddress > 0) {
            FILE *ramFile = fopen(filename.toLatin1(), "wb");
            Gui::mbc2_fix();
            if (cartridgeType <= 4) {Gui::set_bank(0x6000, 1);}
            Gui::set_bank(0x0000, 0x0A);
            if (cartridgeType == 252 && gbxcartFirmwareVersion == 1) {
                uint32_t readBytes = 0;
                for (uint8_t bank = 0; bank < ramBanks; bank++) {
                    uint16_t ramAddress = 0xA000;
                    Gui::set_bank(0x4000, bank);
                    Gui::set_number(ramAddress, SET_START_ADDRESS);
                    RS232_cputs(cport_nr, "M0");
                    sleep(5);
                    Gui::set_mode(GB_CART_MODE);
                    while (ramAddress < ramEndAddress) {
                        if (end == true){emit error(END); break;}
                        for (uint8_t x = 0; x < 64; x++) {
                            char hexNum[7];
                            sprintf(hexNum, "HA0x%x", ((ramAddress + x) >> 8));
                            RS232_cputs(cport_nr, hexNum);
                            RS232_SendByte(cport_nr, 0);
                            sprintf(hexNum, "HB0x%x", ((ramAddress + x) & 0xFF));
                            RS232_cputs(cport_nr, hexNum);
                            RS232_SendByte(cport_nr, 0);
                            RS232_cputs(cport_nr, "LD0x60");
                            RS232_SendByte(cport_nr, 0);
                            RS232_cputs(cport_nr, "DC");
                            RS232_cputs(cport_nr, "HD0x60");
                            RS232_SendByte(cport_nr, 0);
                            RS232_cputs(cport_nr, "LA0xFF");
                            RS232_SendByte(cport_nr, 0);
                            RS232_cputs(cport_nr, "LB0xFF");
                            RS232_SendByte(cport_nr, 0);
                        }
                        Gui::com_read_bytes(ramFile, 64);
                        ramAddress += 64;
                        readBytes += 64;
                        if (ramAddress < ramEndAddress) {RS232_cputs(cport_nr, "1");}
                        emit set_progress(readBytes, ramBanks * (ramEndAddress - 0xA000 + 1));
                    }
                    RS232_cputs(cport_nr, "0");
                }
                RS232_cputs(cport_nr, "M1");
            }
            else {
                uint32_t readBytes = 0;
                for (uint8_t bank = 0; bank < ramBanks; bank++) {
                    uint16_t ramAddress = 0xA000;
                    Gui::set_bank(0x4000, bank);
                    Gui::set_number(ramAddress, SET_START_ADDRESS);
                    Gui::set_mode(READ_ROM_RAM);
                    while (ramAddress < ramEndAddress) {
                        if (end == true){emit error(END); break;}
                        uint8_t comReadBytes = Gui::com_read_bytes(ramFile, 64);
                        if (comReadBytes == 64) {
                            ramAddress += 64;
                            readBytes += 64;
                            if (ramAddress < ramEndAddress) {Gui::com_read_cont();}
                        }
                        else {
                            fflush(ramFile);
                            Gui::com_read_stop();
                            sleep(500);
                            printf("Retrying\n");
                            RS232_PollComport(cport_nr, readBuffer, 64);
                            fseek(ramFile, readBytes, SEEK_SET);
                            Gui::set_number(ramAddress, SET_START_ADDRESS);
                            Gui::set_mode(READ_ROM_RAM);
                        }
                        emit set_progress(readBytes, ramBanks * (ramEndAddress - 0xA000 + 1));
                    }
                    Gui::com_read_stop();
                }
            }
            Gui::set_bank(0x0000, 0x00);
            fclose(ramFile);
            emit error (true);
        }
        else {
            emit error (false);
        }
    }

    if (cMode == 2) {
        if (ramEndAddress > 0 || eepromEndAddress > 0) {
            FILE *ramFile = fopen(filename.toLatin1(), "wb");
            if (ramEndAddress > 0) {
                uint32_t readBytes = 0;
                for (uint8_t bank = 0; bank < ramBanks; bank++) {
                    if (bank == 1) {Gui::set_number(1, GBA_FLASH_SET_BANK);}
                    currAddr = 0x00000;
                    endAddr = ramEndAddress;
                    Gui::set_number(currAddr, SET_START_ADDRESS);
                    Gui::set_mode(GBA_READ_SRAM);
                    while (currAddr < endAddr) {
                        if (end == true){emit error(END); break;}
                        uint8_t comReadBytes = Gui::com_read_bytes(ramFile, 64);
                        if (comReadBytes == 64) {
                            currAddr += 64;
                            readBytes += 64;
                            if (currAddr < endAddr) {Gui::com_read_cont();}
                        }
                        else {
                            fflush(ramFile);
                            Gui::com_read_stop();
                            sleep(500);
                            printf("Retrying\n");
                            RS232_PollComport(cport_nr, readBuffer, 64);
                            fseek(ramFile, currAddr, SEEK_SET);
                            Gui::set_number(currAddr, SET_START_ADDRESS);
                            Gui::set_mode(GBA_READ_SRAM);
                        }
                        emit set_progress(readBytes, ramBanks * endAddr);
                    }
                    Gui::com_read_stop();
                    if (bank == 1) {
                        Gui::set_number(0, GBA_FLASH_SET_BANK);
                    }
                }
            }
            else {
                Gui::set_number(eepromSize, GBA_SET_EEPROM_SIZE);
                currAddr = 0x000;
                endAddr = eepromEndAddress;
                Gui::set_number(currAddr, SET_START_ADDRESS);
                Gui::set_mode(GBA_READ_EEPROM);
                uint32_t readBytes = 0;
                while (currAddr < endAddr) {
                    if (end == true){emit error(END); break;}
                    Gui::com_read_bytes(ramFile, 8);
                    currAddr += 8;
                    readBytes += 8;
                    if (currAddr < endAddr) {RS232_cputs(cport_nr, "1");}
                    emit set_progress(readBytes, endAddr);
                }
                RS232_cputs(cport_nr, "0");
            }
            fclose(ramFile);
            emit error (true);
        }
        else {emit error (false);}
    }
}

void ReadRamThread::canceled(){end = true;}
