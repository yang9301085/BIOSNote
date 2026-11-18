```
Displays all EFI NVRAM variables.
DMPSTORE [-b] [-d] [Variable]
DMPSTORE [Variable] -s file
DMPSTORE [Variable] -l file
    -b       - Display one screen at a time
    Variable - Display the specified variable name
    -d       - Delete variables
    -s       - Save variables to file
    -l       - Load and set variables from file
Note:
    1. If the 'variable' parameter is not specified, all variables will be
       displayed.
    2. The variable name is not case sensitive.
Examples:
* To display all EFI NVRAM variables:
  Shell> dmpstore
  Dump NVRAM
  Variable RT+BS 'Efi:BootCurrent' DataSize = 2
   00000000: FF FF                                           *..*
  Variable NV+RT+BS 'Efi:LangCodes' DataSize = 2A
   00000000: 65 6E 67 65 6E 6D 61 6E-67 63 68 69 7A 68 6F 64 *engenmangchizhod*
   00000010: 65 75 67 65 6D 67 65 72-67 6D 68 67 6F 68 66 72 *eugemgergmhgohfr*
   00000020: 61 66 72 65 66 72 6D 66-72 6F                   *afrefrmfro*
  Variable NV+RT+BS 'Efi:Lang' DataSize = 3
   00000000: 65 6E 67                                        *eng*
  ...
  Variable NV+BS 'ShellAlias:copy' DataSize = 6
   00000000: 63 00 70 00 00 00                                *c.p...*
  Variable NV+BS 'SEnv:path' DataSize = 4
   00000000: 2E 00 00 00                                      *....*
* To display the Boot0000 EFI NVRAM variable:
  Shell> dmpstore Boot0000
  Dump Variable Boot0000
  Variable NV+RT+BS 'Efi:Boot0000' DataSize = 48
  00000000: 01 00 00 00 30 00 42 00-6F 00 6F 00 74 00 30 00  *....0.B.o.o.t.0.*
  00000010: 30 00 30 00 30 00 00 00-01 04 14 00 B1 18 C5 58  *0.0.0..........X*
  00000020: F3 76 D4 11 BC EA 00 80-C7 3C 88 81 01 04 18 00  *.v.......<......*
  00000030: 2F A9 95 0C 06 A0 D4 11-BC FA 00 80 C7 3C 88 81  */............<..*
  00000040: 00 00 00 00 7F FF 04 00-                         *........*
```