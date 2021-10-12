polib /MACHINE:X86 /DEF:aura_sdk.def /OUT:aura_sdk.lib
cc /x /Tx86-coff /Os /Ze gak.c /ENTRY:wmain /SUBSYSTEM:CONSOLE /RELEASE /MACHINE:X86 /NODEFAULTLIB kernel32.lib aura_sdk.lib
