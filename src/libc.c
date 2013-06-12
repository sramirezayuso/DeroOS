#include "../include/kc.h"
#include "../include/defs.h"
#include "../include/kernel.h"
#include "../include/kasm.h"
#include "../include/stdio.h"

/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion
*	 Derechos de acceso del segmento
*	 Cero
****************************************************************/



void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}


void lspci()
 {
	int bus, slot;
	unsigned short vendor, device;
	for(bus = 0; bus < 256; bus++){
		for(slot = 0; slot < 32; slot++){
			vendor = pciConfigReadWord(bus,slot,0,0);
			if(vendor != 0xFFFF){
				device = pciConfigReadWord(bus,slot,0,2);
				printf("%d", vendor);
				putc('c');
				printf("%d", device);
			}
		}
	}
}

 unsigned short pciConfigReadWord (unsigned short bus, unsigned short slot,
				unsigned short func, unsigned short offset)
 {
    unsigned long address;
    unsigned long longBus = (unsigned long)bus;
    unsigned long longSlot = (unsigned long)slot;
    unsigned long longFunc = (unsigned long)func;
    unsigned short tmp = 0;

    address = (unsigned long)((longBus << 16) | (longSlot << 11) | (longFunc << 8)
				| (offset & 0xfc) | ((unsigned long)0x80000000));

    _outl(0x0CF8, address);

    tmp = (unsigned short)((_inl(0x0CFC) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
 }

 int get_update_in_progress_flag() {
      _outb((unsigned char *)CMOS_ADDRESS, 0x0A);
      return (_inb(CMOS_DATA) & 0x80);
}

unsigned char ans[9];
unsigned char * read_rtc() {
    unsigned char hour, minute, second;
    unsigned char registerB;


    // Note: This uses the "read registers until you get the same values twice in a row" technique
    //       to avoid getting dodgy/inconsistent values due to RTC updates

    while (get_update_in_progress_flag());
    _outb((unsigned char *) CMOS_ADDRESS, 0x00);
    second = _inb(CMOS_DATA);
    _outb((unsigned char *)CMOS_ADDRESS, 0x02);
    minute = _inb(CMOS_DATA);
    _outb((unsigned char *)CMOS_ADDRESS, 0x04);
    hour = _inb(CMOS_DATA);

    _outb((unsigned char *)CMOS_ADDRESS, 0x0B);
    registerB = _inb(CMOS_DATA);

    if (!(registerB & 0x04)) {
        second = (second & 0x0F) + ((second / 16) * 10);
        minute = (minute & 0x0F) + ((minute / 16) * 10);
        hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
    }

    if (!(registerB & 0x02) && (hour & 0x80)) {
        hour = ((hour & 0x7F) + 12) % 24;
    }
	
	ans[0] = (hour/10) + '0';
    ans[1] = (hour%10) + '0';
	ans[2] = ':';
	ans[3] = (minute/10) + '0';
    ans[4] = (minute%10) + '0';
	ans[5] = ':';
	ans[6] = (second/10) + '0';
    ans[7] = (second%10) + '0';
	ans[8] = 0;
	return ans;
}

unsigned char ansTemp[5];
unsigned char * read_temp(){
    dword maxTemp = _read_msr(MSR_IA32_TEMPERATURE_TARGET);
    maxTemp = (maxTemp & 0x0007F000) >> 16;
	maxTemp = maxTemp==0?85:maxTemp;
    dword temp = _read_msr(MSR_IA32_THERM_STATUS);
    temp = (temp & 0x0007F000) >> 16;
    temp = maxTemp - temp;
	
	if(temp < 0) {
		ansTemp[0] = 'E';
		ansTemp[1] = 'R';
		ansTemp[2] = 'R';
		ansTemp[3] = ' ';
	} else {
		ansTemp[0] = ((temp/10) + '0' )=='0'?' ':(temp/10) + '0';
		ansTemp[1] = (temp%10) + '0';
		ansTemp[2] = ' ';
		ansTemp[3] = 'C';
	}
	ansTemp[4] = 0;
	return ansTemp;
}
