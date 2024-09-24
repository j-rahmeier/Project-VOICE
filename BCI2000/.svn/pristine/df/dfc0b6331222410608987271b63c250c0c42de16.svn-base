#ifndef XIPP_STIM_CMD_H
#define XIPP_STIM_CMD_H

////////////////////////////////////////////////////
// Defines for Stim control

// Stim Commands consist of arrays of 32-bit entries that are queued as they are received
// (The entries of a stim command are always stored internally with a terminating zero entry.
// Sent commands may include terminating entries, or one will be appended when received.)

#define GVSTIM_CMD_MAXLEN   249         // max total valid 32-bit entries in a command
#define GVSTIM_CMD_ALLOC    256         // internal buffer length (must be >= (MAXLEN+1))
#define GVSTIM_CMD_BUFCNT   6           // total number of FIFO queue command buffers.

// The first entry [0] contains bit fields with specific command execution instructions,
// including the action, minimum period of execution, and the number of times command is cycled.

#define GVSTIM_CMD_ACTION   0xF0000000  // Action to perform with the command
#define GVSTIM_CMD_PERIOD   0x0FFFF000  // Minimum period of each command cycle
#define GVSTIM_CMD_CYCLES   0x00000FFF  // Number of times command is cycled

// The remaining 32-bit entries of a command are 16-bit word pairs that specify the GV stim
// hardware channel control bits (lower word), and the number of samples that the hw control is
// held (upper word).  The entries are serially executed for the number of cycles of the command.
//
// A control value of zero performs no action for its hold time; any other control value will
// be sent to the hardware and cause stim voltage waveforms to be captured during those samples.
//
// An entry with zero hold time is not executed and terminates entry execution for the cycle.
//
// After all of the entries are executed in the cycle, the channel is held in a no action state
// (control bits = zero) for the remainder of the cycle until the PERIOD value is reached.

#define GVSTIM_GET_CTRLBITS(ENTRY)  ((ENTRY) & 0xFFFF)
#define GVSTIM_GET_HOLDTIME(ENTRY)  ((ENTRY) >> 16)

// Actions are as described below.  IMMED clobbers all other pending queued commands, and executes
// immediately after PERIOD samples into any ongoing command cycle.  If no other commands are being
// executed, the IMMED command begins immedately with PERIOD long cycles.  All other commandss are
// are FIFO queued and execute based on their first entry command action.

#define GVSTIM_ACT_IMMED    0x00000000  // Immediately begin PERIOD into existing command cycle
#define GVSTIM_ACT_CURCYC   0x10000000  // After current cycle of previous command completes
#define GVSTIM_ACT_ALLCYC   0x20000000  // After all cycles of previous command are completed
#define GVSTIM_ACT_TRIGGER  0x30000000  // Enter triggered state, repeat command each trigger
#define GVSTIM_ACT_AT_TIME  0x40000000  // Instead of period, at absolute time matching

#define GVSTIM_GET_ACTION(ENTRY)  ((ENTRY) & GVSTIM_CMD_ACTION)         // Mask action bits
#define GVSTIM_GET_PERIOD(ENTRY) (((ENTRY) & GVSTIM_CMD_PERIOD) >> 12)  // returns period
#define GVSTIM_GET_CYCLES(ENTRY)  ((ENTRY) & GVSTIM_CMD_CYCLES)         // returns cycles

#define GVSTIM_TST_ALLCYC(ENTRY)  ((ENTRY) & 0x60000000)  // check if actions waits for all cycles

// A TRIGGER command will wait for all previous command cycles to complete, then cause the
// channel to enter a triggered state where there the command will be executed each time the
// trigger is received.  The triggered state is ended by receipt of an IMMED, AT_TIME, or another
// TRIGGER command.  CURCYC and ALLCYC may be queued after a TRIGGER command and will end the
// triggered state and execute following the next execution of the TRIGGERED command.
//
// An AT_TIME command will be executed once when the lower 16 bits of the system time match the
// value in the period field.
//
// SPECIAL COMMAND CASES :
//
// A single entry with GVSTIM_ACT_IMMED and CYCLES=0 (entry=0) immediately shuts down the channel.
//
// A single entry with GVSTIM_ACT_CURCYC and CYCLES=0 stops the channel after current cycle.
//
// A command with a single entry with CYCLES > 0 can be used to compactly repeat the control
// bit/hold entries of the previous command.  For example, this can be used to frequency modulate
// an existing pattern by varying the PERIOD in the single entry, or it can be used with AT_TIME
// to replay a previous pattern at multiple future times.
//
// A command with CYCLES=0 specified in its first entry is not executed, but if it includes
// control bit/hold entries, they are fully received and loaded.  These can be copied and executed
// by sending subsequent single-entry commands with non-zero cycle counts.  The control bit/hold
// entries are cleared out by any command with CYCLES=0 and no entries.
//
// Queued commands require at least one sample time each to process and complete.  This does not
// limit normal use cases, but can come up in unusual situations such as several sequential single
// entry commands with CYCLES=0 and PERIOD=0, or sequential cases with no entries.
//

#endif // XIPP_STIM_CMD_H
