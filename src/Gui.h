#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QString>
#include <QNetworkAccessManager>
#include <QProcess>
#include "Settings.h"
#include "Device.h"
#include "Console.h"

#ifdef _WIN32
#include <windows.h>
#include <QWinTaskbarButton>
#else
#include <unistd.h>
#endif
#include <stdint.h>
#include <stdio.h>

#define LOW 0
#define HIGH 1
#define false 0
#define true 1

#include "rs232/rs232.h"

#include "ReadFlashThread.h"
#include "ReadRamThread.h"
extern int cport_nr;
extern int bdrate;

#define CART_MODE 'C'
#define GB_MODE 1
#define GBA_MODE 2

// GB/GBC defines/commands
#define SET_START_ADDRESS 'A'
#define READ_ROM_RAM 'R'
#define WRITE_RAM 'W'
#define SET_BANK 'B'
#define GB_CART_MODE 'G'

// GBA defines/commands
#define EEPROM_NONE 0
#define EEPROM_4KBIT 1
#define EEPROM_64KBIT 2

#define SRAM_FLASH_NONE 0
#define SRAM_FLASH_256KBIT 1
#define SRAM_FLASH_512KBIT 2
#define SRAM_FLASH_1MBIT 3

#define NOT_CHECKED 0
#define NO_FLASH 1
#define FLASH_FOUND 2
#define FLASH_FOUND_ATMEL 3

#define GBA_READ_ROM 'r'
#define GBA_READ_ROM_256BYTE 'j'
#define GBA_READ_SRAM 'm'
#define GBA_WRITE_SRAM 'w'
#define GBA_WRITE_ONE_BYTE_SRAM 'o'
#define GBA_CART_MODE 'g'

#define GBA_SET_EEPROM_SIZE 'S'
#define GBA_READ_EEPROM 'e'
#define GBA_WRITE_EEPROM 'p'

#define GBA_FLASH_READ_ID 'i'
#define GBA_FLASH_SET_BANK 'k'
#define GBA_FLASH_4K_SECTOR_ERASE 's'
#define GBA_FLASH_WRITE_BYTE 'b'
#define GBA_FLASH_WRITE_ATMEL 'a'

// Flash Cart commands
#define GB_FLASH_WE_PIN 'P'
    #define WE_AS_AUDIO_PIN 'A'
    #define WE_AS_WR_PIN 'W'

#define GB_FLASH_PROGRAM_METHOD 'E'
    #define GB_FLASH_PROGRAM_555 0
    #define GB_FLASH_PROGRAM_AAA 1
    #define GB_FLASH_PROGRAM_555_BIT01_SWAPPED 2
    #define GB_FLASH_PROGRAM_AAA_BIT01_SWAPPED 3
    #define GB_FLASH_PROGRAM_5555 4

#define GB_FLASH_WRITE_BYTE 'F'
#define GB_FLASH_WRITE_64BYTE 'T'
#define GB_FLASH_WRITE_256BYTE 'X'
#define GB_FLASH_WRITE_BUFFERED_32BYTE 'Y'

#define GB_FLASH_BANK_1_COMMAND_WRITES 'N'

#define GBA_FLASH_CART_WRITE_BYTE 'n'
#define GBA_FLASH_WRITE_64BYTE_SWAPPED_D0D1 'q'
#define GBA_FLASH_WRITE_256BYTE_SWAPPED_D0D1 't'
#define GBA_FLASH_WRITE_256BYTE 'f'
#define GBA_FLASH_WRITE_INTEL_64BYTE 'l'
#define GBA_FLASH_WRITE_INTEL_64BYTE_WORD 'u'

// General commands
#define CART_MODE 'C'
#define SET_INPUT 'I'
#define SET_OUTPUT 'O'
#define SET_OUTPUT_LOW 'L'
#define SET_OUTPUT_HIGH 'H'
#define READ_INPUT 'D'
#define RESET_COMMON_LINES 'M'
#define READ_FIRMWARE_VERSION 'V'
#define READ_PCB_VERSION 'h'

#define VOLTAGE_3_3V '3'
#define VOLTAGE_5V '5'

#define RESET_AVR '*'
#define RESET_VALUE 0x7E5E1

// PCB versions
#define PCB_1_0 1
#define PCB_1_1 2
#define PCB_1_3 4
#define PCB_GBXMAS 90
#define PCB_Mini 100

// Common vars
#define READ_BUFFER nullptr

extern uint8_t gbxcartFirmwareVersion;
extern uint8_t gbxcartPcbVersion;
extern uint8_t readBuffer[257];
extern uint8_t writeBuffer[257];

extern char gameTitle[17];
extern uint16_t cartridgeType;
extern uint32_t currAddr;
extern uint32_t endAddr;
extern uint16_t romSize;
extern uint32_t romEndAddr;
extern uint16_t romBanks;
extern int ramSize;
extern uint16_t ramBanks;
extern uint32_t ramEndAddress;
extern int eepromSize;
extern uint16_t eepromEndAddress;
extern int hasFlashSave;
extern uint8_t cartridgeMode;
extern int flashCartType;
extern uint8_t flashID[10];

class Gui:public QWidget
{
  Q_OBJECT Settings * settings;
  Console *console;
  Device *device;
  QGridLayout *grid;
  QVBoxLayout *left;
  QVBoxLayout *right;
  QVBoxLayout *center;
  QHBoxLayout *down;
  #if defined (_WIN32)
    QWinTaskbarButton *winTaskbar;
  #endif
  QLabel *image;
  QPixmap *logo;
  QPushButton *cancel_btn;
  QPushButton *status_btn;
  QPushButton *rflash_btn;
  QPushButton *wflash_btn;
  QPushButton *rram_btn;
  QPushButton *wram_btn;
  ReadFlashThread *thread_RFLA;
  ReadRamThread *thread_RRAM;
  //WriteFlashThread *thread_WFLA;
  //WriteRamThread * thread_WRAM;
  QString path;
  QString file_name;

public:
    Gui (QWidget * parent = nullptr);
  int port_type;


  QProgressBar *progress;
  public slots:void startup_info (void);
  void setEnabledButtons (bool stan);
  void setRamButtons ();
  void write_flash (void);
  void write_ram (void);
  void show_info (void);
  void setProgress (int ile, int max);
  void read_config(void);
  void write_config(void);
  void load_cart_ram_info(void);
  void write_cart_ram_info(void);
  static void delay_ms(uint16_t ms);
  char read_one_letter(void);
  void com_wait_for_ack (void);
  static void com_read_stop(void);
  static void com_read_cont(void);
  uint8_t com_test_port(void);
  static uint16_t com_read_bytes(FILE *file, int count);
  void com_write_bytes_from_file(uint8_t command, FILE *file, int count);
  static void set_mode (char command);
  static void set_number (uint32_t number, uint8_t command);
  uint8_t read_cartridge_mode (void);
  static uint8_t request_value (uint8_t command);
  static void set_bank (uint16_t address, uint8_t bank);
  static void mbc2_fix (void);
  void read_gb_header (void);
  uint8_t gba_check_rom_size (void);
  uint8_t gba_test_sram_flash_write (void);
  uint8_t gba_check_sram_flash (void);
  void flash_4k_sector_erase (uint8_t sector);
  uint8_t gba_check_eeprom (void);
  void gba_read_gametitle(void);
  void read_gba_header (void);
  void write_flash_config(int number);
  void read_config_flash(void);
  void wait_for_flash_sector_ff(uint16_t address);
  void wait_for_flash_chip_erase_ff(uint8_t printProgress);
  void gb_flash_pin_setup(char pin);
  void gb_flash_program_setup(uint8_t method);
  void gb_flash_write_address_byte (uint16_t address, uint8_t byte);
  void gb_check_change_flash_id (uint8_t flashMethod);
  void gba_flash_write_address_byte (uint32_t address, uint16_t byte);
  void send_hex_wait_ack (uint16_t hex);
  void print_error (int err);
  void exit();
  void gba_specify_ram_size(int memoryType, int flashType, int size);
  void read_rom();
  void read_ram();
};

