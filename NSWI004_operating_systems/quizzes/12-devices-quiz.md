# Devices

This is a graded quiz, do not forget to commit and push your responses in time (deadline is end of January 4).


# Device Drivers

**[Q1]** In _Arpaci-Dusseau Section 36 I/O Devices_, recommended for the last two self study modules,
you could see the code of a simple IDE (disk) driver. The code calls `sleep` and `wakeup` functions,
describe briefly what is the semantics of these functions and explain why the `ide_rw` function
calls `sleep` with `ide_lock` held.

**[A1]** Sleep - OS issues a request, calling process is put to sleep and waits for interrupt. OS can switch to another task.
Wakeup - When a hardware interrupt is raised the CPU jumps jumps into OS at ISR, which then wakes the process waiting for I/O which can then proceed.
Sleep is called with ide_lock held because if lock was not held the output could be damaged during writing by other writer, who currently holds
the ide_lock. Sleeping with ide_lock held prevents this, because the ide_rw function is the only one currently able to write. **[/A1]**


# Devices

**[Q2]** Still in _Arpaci-Dusseau Section 36 I/O Devices_, is there a limit on the size of the disk
that the simple IDE (disk) driver can handle, and if so, what is it ? Explain how you found out.

**[A2]** The maximum disk size for an IDE interface is typically limited by the number of addressable sectors that can be addressed by the disk controller. In the case of the IDE interface, the maximum number of addressable sectors is 2^28, or 268,435,456 sectors. Each sector is typically 512 bytes, so the maximum size of a disk that can be used with the IDE interface is approximately 137 GB.
The documentation of motherboard specifies the maximum number of addressable sectors and the size of each sector, which can be used to calculate the maximum disk size.  **[/A2]**


**[Q3]** On the `lab` computer, which device on the PCI bus uses the largest memory window ?
What do you think it could be used for (back your hypothesis with some explanation) ?

The `lspci` command is your friend here.

**[A3]** VGA compatible controller: Cirrus Logic GD 5446. It could be used for the video output of a virtual machine,  the system has a Cirrus Logic GD 5446 graphics controller installed, which is being used to generate and display graphics on the system's display. **[/A3]**


## Ask Your Question

**[Q0]** Ask a single question concerning the topic of the last self study module.

**[A0]** What are some benefits of using a journaling filesystem over a non-journaling one? **[/A0]**
