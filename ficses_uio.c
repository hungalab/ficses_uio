//---------------------------------------------------------
// FiCSES Userspace I/O Driver
// nyacom (C) 2019.11 <kzh@nyacom.net>
//---------------------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/pci.h>

#define FICSES_PCI_VENID    0x10ee     // Xilinx
#define FICSES_PCI_DEVID    0x8028     // KCU1500
#define FICSES_PCI_SUBDEVID 0x0007     // KCU1500
#define DRIVER_NAME         "ficses_uio"
#define DRIVER_VERSION      "0.1"

static int ficses_pci_probe(struct pci_dev *dev, 
							const struct pci_device_id *id)
{
	printk(KERN_INFO "[ficses_uio] FiCSES KCU1500 found (%04x:%04x)\n", dev->vendor, dev->device);

	struct uio_info *info;
	info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);

	if (info == NULL) {
		return -ENOMEM;
	}

	if (pci_enable_device(dev)) {
		goto _free;
	}

	if (pci_request_regions(dev, "ficses")) {
		goto _disable;
	}

	// BA0
	//info->mem[0].name = 'lvds_if';
	info->mem[0].addr = pci_resource_start(dev, 0);
	if (!info->mem[0].addr) {
		goto _release;
	}
	info->mem[0].size = pci_resource_len(dev, 0);
	info->mem[0].memtype = UIO_MEM_PHYS;

	// BA1
	//info->mem[1].name = 'spio_fic';
	info->mem[1].addr = pci_resource_start(dev, 1);
	if (!info->mem[1].addr) {
		goto _release;
	}
	info->mem[1].size = pci_resource_len(dev, 1);
	info->mem[1].memtype = UIO_MEM_PHYS;

	info->name = DRIVER_NAME;
	info->version = DRIVER_VERSION;

	info->irq = UIO_IRQ_NONE;

	//info->irq = dev->irq;
	//info->irq_flags = UIO_IRQ_NONE

	if (uio_register_device(&dev->dev, info)) {
		//goto _unmap;
	}

	pci_set_drvdata(dev, info);

	printk(KERN_INFO "[ficses_uio] FiCSES loaded\n");

	return 0;

_release:
	pci_release_regions(dev);

_disable:
	pci_disable_device(dev);

_free:
	kfree(info);

	printk(KERN_INFO "[ficses_uio] FiCSES failed\n");

	return -ENODEV;
}

static void ficses_pci_remove(struct pci_dev *dev) 
{
	struct uio_info *info = pci_get_drvdata(dev);

    printk(KERN_INFO "[ficses_uio] DEBUG %d\n", __LINE__ );

	uio_unregister_device(info);
	pci_release_regions(dev);
	pci_disable_device(dev);
	iounmap(info->mem[0].internal_addr);

	kfree(info);
}

static struct pci_device_id ficses_pci_ids[] = {
	{
		.vendor =    PCI_VENDOR_ID_XILINX,
		.device =    FICSES_PCI_DEVID,
		.subvendor = PCI_VENDOR_ID_XILINX,
		.subdevice = FICSES_PCI_SUBDEVID,
	},
	{ 0, }
};

static struct pci_driver ficses_pci_driver = {
	.name     = "ficses",
	.id_table = ficses_pci_ids,
	.probe    = ficses_pci_probe,
	.remove   = ficses_pci_remove,
};

module_pci_driver(ficses_pci_driver);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("K.Hironaka <kzh@nyacom.net>");
MODULE_DESCRIPTION("FiCSES Userspace I/O Driver");
MODULE_DEVICE_TABLE(pci, ficses_pci_ids);
