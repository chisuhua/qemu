/*
 * QEMU memory pci emulation (PCI to ISA bridge)
 *
 */

#include "qemu/osdep.h"
#include "hw/hw.h"
#include "hw/i386/pc.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_host.h"
#include "hw/isa/isa.h"
#include "hw/sysbus.h"
// #include "hw/timer/i8254.h"
// #include "hw/audio/pcspk.h"


#define MEM_PCI_VENDOR_ID	0x1234
#define	MEM_PCI_DEVICE_ID	0x5678
#define	MEM_PCI_REVISION_ID	0x73

#define	PCI_MEM_SIZE		0x00000010

typedef struct MEMPCIState {
    MemoryRegion mem;
} MEMPCIState;

typedef struct PCIMEMPCIState {
    PCIDevice pci_dev;
    // uint32_t mem_pci_base_uint32;
    hwaddr mem_pci_base;
    MEMPCIState state;
} PCIMEMPCIState;

static const VMStateDescription vmstate_mem_pci = {
    .name = "mem_pci",
    .version_id = 0,
    .minimum_version_id = 0,
    .fields = (VMStateField[]) {
        VMSTATE_PCI_DEVICE(pci_dev, PCIMEMPCIState),
        VMSTATE_END_OF_LIST()
    },
};


typedef unsigned char  byte;
typedef unsigned short int uint16;
typedef unsigned int 	uint32;


static void mem_pci_write(void *opaque, hwaddr addr,
                             uint64_t value, unsigned int size)
{
	fprintf(stderr,"INFO: FUNC MEM_PCI_WRITE called\n");
	fprintf(stderr,"INFO: ARG  addr=%lx\n",addr);
	fprintf(stderr,"INFO: ARG  value=%lx\n",value);
	fprintf(stderr,"INFO: ARG  size=%d\n",size);
   	void  	*pci_mem_addr;
   	// int32_t  	pci_mem_addr_int;
   	int 	temp,region_size;
	byte	buff[8];

	// pci_mem_addr	= (void*)((PCIMEMPCIState *)opaque)->mem_pci_base_uint32;
	pci_mem_addr	= (void*)((PCIMEMPCIState *)opaque)->mem_pci_base;


	fprintf(stderr,"mem_pci_base=%p\n",pci_mem_addr);

	pci_mem_addr	= ((char *)pci_mem_addr) + addr;
	// pci_mem_addr_int	= pci_mem_addr_int + addr;
	region_size	= (int)memory_region_size( &((PCIMEMPCIState *)opaque)->state.mem);

	fprintf(stderr,"INFO:       pci_mem_addr=%p\n",pci_mem_addr);
	fprintf(stderr,"INFO:       region_size=%x\n",region_size);

	if(addr > region_size)
		return ;
	switch (size) {
	    	case 1:
			// sprintf((char*)buff,"%02llx",value);
			sprintf((char*)buff,"%02lx",value);
			sscanf((char*)buff,"%x",&temp);
			*((byte*)pci_mem_addr)	= (byte)temp;
			break;
	    	case 2:
			// sprintf((char*)buff,"%04llx",value);
			sprintf((char*)buff,"%04lx",value);
			sscanf((char*)buff,"%x",&temp);
			*((uint16*)pci_mem_addr)= (uint16)temp;
			break;
	    	case 4:
			// sprintf((char*)buff,"%08llx",value);
			sprintf((char*)buff,"%08lx",value);
			sscanf((char*)buff,"%x",&temp);
			*((uint32*)pci_mem_addr)= (uint32)temp;
			break;
	}
	fprintf(stderr,"INFO:       write value is %x\n",temp);
	fprintf(stderr,"INFO: FUNC MEM_PCI_WRITE leave\n");
}

static uint64_t mem_pci_read(void *opaque, hwaddr addr,
                                unsigned int size)
{
	fprintf(stderr,"INFO: FUNC MEM_PCI_READ called\n");
	fprintf(stderr,"INFO: ARG  addr=%lx\n",addr);
	fprintf(stderr,"INFO: ARG  size=%d\n",size);
	void  	*pci_mem_addr;
	// int32_t  	pci_mem_addr;
	int 	temp,region_size;
	// byte	buff[8];
	// pci_mem_addr	= (void*)((PCIMEMPCIState *)opaque)->mem_pci_base_uint32;
	pci_mem_addr	= (void*)((PCIMEMPCIState *)opaque)->mem_pci_base;
	fprintf(stderr,"INFO:       mem_pci_base=%p\n",pci_mem_addr);

	pci_mem_addr	= ((char *)pci_mem_addr) + addr;
	// pci_mem_addr	= pci_mem_addr + addr;
	region_size	= memory_region_size(&((PCIMEMPCIState *)opaque)->state.mem);

	fprintf(stderr,"INFO:       pci_mem_addr=%p\n",pci_mem_addr);
	fprintf(stderr,"INFO:       region_size=%x\n",region_size);

	if(addr > region_size)
		return 0;
	switch (size) {
		case 1:
			temp = *((byte *)pci_mem_addr);
            fprintf(stderr,"INFO:       readout is %d\n",temp);
	        fprintf(stderr,"INFO: FUNC MEM_PCI_READ leave\n");
			return ((byte)temp);
		case 2:
			temp = *((uint16 *)pci_mem_addr);
            fprintf(stderr,"INFO:       readout is %d",temp);
	        fprintf(stderr,"INFO: FUNC MEM_PCI_READ leave\n");
			return ((uint16)temp);
		case 4:
			temp = *((uint32 *)pci_mem_addr);
            fprintf(stderr,"INFO:       readout is %d",temp);
	        fprintf(stderr,"INFO: FUNC MEM_PCI_READ leave\n");
			return ((uint32)temp);
	}
    fprintf(stderr,"INFO:       readout is none");
    fprintf(stderr,"INFO: FUNC MEM_PCI_READ leave\n");
    return 0;
}

static const MemoryRegionOps mem_pci_ops = {
    .read = mem_pci_read,
    .write = mem_pci_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};


static Property mem_pci_properties[] = {
    // DEFINE_PROP_HEX32("membase", PCIMEMPCIState, mem_pci_base, 0xc0000000),
    // DEFINE_PROP_UINT32("membase", PCIMEMPCIState, mem_pci_base_uint32, 0xc0000000),
    DEFINE_PROP_UINT64("membase", PCIMEMPCIState, mem_pci_base, 0xc0000000),
    DEFINE_PROP_END_OF_LIST()
};


static void mem_pci_init(PCIDevice *dev, Error **errp)
{
// #define ES1370(obj) 
// OBJECT_CHECK(ES1370State, (obj), TYPE_ES1370)
    PCIMEMPCIState *pci = DO_UPCAST(PCIMEMPCIState, pci_dev, dev);
    MEMPCIState *s = &pci->state;

    // pci->mem_pci_base_uint32 	= (uint32_t)malloc(PCI_MEM_SIZE);
    pci->mem_pci_base 	= (hwaddr)malloc(PCI_MEM_SIZE);
    // memory_region_init_io(&s->mem, &mem_pci_ops, pci, "mem-pci", PCI_MEM_SIZE);
    memory_region_init_io(&s->mem, OBJECT(s), &mem_pci_ops, pci, "mem-pci", PCI_MEM_SIZE);
    pci_register_bar(dev, 1, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mem);

}

static void mem_pci_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    // schi hack change k->init = mem_pci_init;
    k->realize = mem_pci_init;
    k->vendor_id = MEM_PCI_VENDOR_ID;
    k->device_id = MEM_PCI_DEVICE_ID;
    k->revision  = MEM_PCI_REVISION_ID;

    dc->vmsd = &vmstate_mem_pci;
    dc->props = mem_pci_properties;
}

static const TypeInfo mem_pci_info = {
    .name = "mem_pci",
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(PCIMEMPCIState),
    .class_init = mem_pci_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    },
};

static void mem_pci_register_types(void)
{
    type_register_static(&mem_pci_info);
}

type_init(mem_pci_register_types)


