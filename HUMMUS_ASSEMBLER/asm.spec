
################################################################################
 _   _ _   _ __  __ __  __ _   _ ____  ____  _    _   _ ____  
| | | | | | |  \/  |  \/  | | | / ___||  _ \| |  | | | / ___| 
| |_| | | | | |\/| | |\/| | | | \___ \| |_) | |  | | | \___ \ 
|  _  | |_| | |  | | |  | | |_| |___) |  __/| |__| |_| |___) |
|_| |_|\___/|_|  |_|_|  |_|\___/|____/|_|   |_____\___/|____/ 
                                                              
 ____  ____  _____ ____ ___ _____ ___ ____    _  _____ ___ ___  _   _ ____  
/ ___||  _ \| ____/ ___|_ _|  ___|_ _/ ___|  / \|_   _|_ _/ _ \| \ | / ___| 
\___ \| |_) |  _|| |    | || |_   | | |     / _ \ | |  | | | | |  \| \___ \ 
 ___) |  __/| |__| |___ | ||  _|  | | |___ / ___ \| |  | | |_| | |\  |___) |
|____/|_|   |_____\____|___|_|   |___\____/_/   \_\_| |___\___/|_| \_|____/ 
                                                                            
################################################################################

 *  The assembly language will be changed a bit from its
    old Minecraft predecessor. This document will specify all
    the relevant changes.

 *  The assembly language will be a 32-bit assembly language,
    4 times larger, thus allowing us to improve and
    optimize the instruction set. (Fixed Size: 32 bits)

    0b_0000_0000_0000_0000_0000_0000_0000_0000

    Many staple features (or lack of features) of the old Hummus
    will be present in the new iteration as well. This includes the
    utilization of only two registers and a result register.

    Each addressable location will contain 4 bytes. Each Address 0xADDR
    represents a byte in memory. This means that we can (in theory)
    access 2^32 bytes in memory (4 Gibibytes). In order to actually
    not utilize that much memory, The simulator will dynamically
    allocate the memory as the assembly language needs it. Caution
    should still be taken for memory overflow.

    The key feature of the original Hummus was that the program and
    data memory were kept separate. This will not carry over to the 
    new language. Thus caution must be taken when specifying an address
    to avoid writing or running bad addresses. The VAR instruction is
    provided as a pseudo instruction that defines a memory location.
    It will occupy 32 bits.

################################################################################

MISC    0 0 0 0     The Miscellaneous Instruction:
                        Nothing particularly special about this instruction.
                        It performs a variety of different tasks based on
                        the given argument bits.
                            
                            Ex: MISC 0x0
                                [ PROGRAM END ]

SHFF    0 0 0 1     The Shift Forward Instruction: (+LABEL)
                        The instruction increments the Program Counter by an 
                        unsigned amount specified by the argument bits.
                            
                            Ex: SHFF 0x_F2F0_0A83
                                [ PC += unsigned(0x_F2F0_0A83); ]

SHFB    0 0 1 0     The Shift Backward Instruction: (+LABEL)
                        The instruction decrements the Program Counter by an
                        unsigned amount specified by the argument bits.
                            
                            Ex: SHFB 0x_0000_FFFF
                                [ PC -= unsigned(0x_0000_FFFF); ]

BROZ    0 0 1 1     The Branch on Zero Instruction: (+LABEL)
                        The instruction checks to see if the arg. register
                        has a value of zero. If it is, it adds the 2's
                        complement value of its argument bits to the program
                        counter. ($RX is the last written register)
                            
                            Ex: BROZ xFFFF_FF0F
                                [ if ($RX == 0) PC += signed(0xFF0F); ]

SVPC    0 1 0 0     The Save Program Counter Instruction:
                        The instruction saves the value of the program
                        counter into a register of choice. The remaining
                        argument bits can be used to specify any signed
                        offset.
                            
                            Ex: SVPC $RF 0x02
                                [ $RF = $PC + 0x02; ]

UDPC    0 1 0 1     The Update Program Counter Instruction:
                        The instruction saves the value of the specified
                        register into a the program counter while applying
                        any signed offset.
                            
                            Ex: REPC $R5 0x0007
                                [ $PC = $R5 + 0x0007; ]

LDMY    0 1 1 0     The Load via Memory Instruction: (+LABEL)
                        The instruction loads memory at the location of the
                        program current plus a signed offset specified by
                        the argument bits.
                            
                            Ex: LDMY $R3 0x5000
                                [ $R3 = Memory[$PC + 0x5000]] 

LDRG    0 1 1 1     The Load via Register Instruction:
                        The instruction loads memory into a register of the
                        data from the location specified by another register
                        and adds an offset of some value obtained by a third
                        register
                            
                            EX: LDRG $R2 $RE $R9
                                [ $R2 = Memory[$RE + $R9]; ]

CNST    1 0 0 0     The Constant Instruction:
                        The instruction stores the constant supplied by the
                        argument bits as a signed number into a specified
                        register.
                            
                            EX: CNST $R7 0xFF001
                                [ $R7 = 0xFF001; ]

BLSM    1 0 0 1     The Boolean State Machine Instruction:
                        This tells the boolean state machine what instruction
                        to do when it is actually called. It supports a variety
                        of different boolean instructions (16 kinds).
                            
                            Ex: BLSM 0x0002
                                [ $BL = 0x0002; ]

BOOL    1 0 1 0     The Boolean Instruction:
                        The instruction performs one of sixteen boolean actions.
                        The actions themselves are chosen based on the state
                        of the boolean state machine. It is configured via BLSM.
                        It performs the particular action on two specified
                        registers and stores it into another register.
                            
                            Ex: BOOL $R4 $R1 $R2
                                [ $R4 = bool($R1, $R2); ] 

ADDR    1 0 1 1     The Add Register Instruction:
                        The instruction adds two registers and store the result
                        in the final register.

                            Ex: ADDR $R0 $R1 $R2
                                [ $R0 = $R1 + $R2; ]

ADDC    1 1 0 0     The Add Constant Instruction:
                        The instruction adds a constant to a register and stores
                        it into another.

                            Ex: ADDC $R0 $R1 0x05
                                [ $R0 = $R1 + 0x05; ]

SUBC    1 1 0 1     THe Subtract Constant Instruction:
                        The instruction subtracts a constant to a register 
                        and stores it into another.

                            Ex: SUBC $R0 $R1 0x05
                                [ $R0 = $R1 - 0x05; ]

STMY    1 1 1 0     The Store via Memory Instruction: (+LABEL)
                        The instruction stores memory from a register at 
                        the location of the program counter plus a signed 
                        offset specified by the argument bits.

                            Ex: STMY $R2 0x5000
                                [ Memory[$PC + 0x5000] = $R2; ]

STRG    1 1 1 1     The Store via Register Instruction:
                        The instruction stores memory from a register into
                        the address stored in the second register and adds
                        an offset of some signed value found in the third
                        register.

                            Ex; STRG $R2 $RE $R9
                                [ Memory[$RE + $R9] = $R2; ]
