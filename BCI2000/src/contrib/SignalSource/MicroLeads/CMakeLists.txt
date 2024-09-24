###########################################################################
## Authors: schalkadmin@DESKTOP-OTQRNP6.wucon.wustl.edu
## Description: Build information for the MicroLeads source module
###########################################################################

# Use the bci2000_include macro if you need to link with frameworks from /src/extlib:
# bci2000_include( MATH )

bci2000_add_signal_source_module(
  MicroLeads NOBUNDLE
  MicroLeadsADC.cpp
  basestation_SOH.cpp
  powerpad_SOH.cpp
  record_disable.cpp
  record_enable.cpp
  record_init.cpp
  open_data_port.cpp
  send_tcp.cpp
  send_udp.cpp
  socket_init.cpp
  stim_enable.cpp
  MicroLeads_Protocol.h
  variables.h
)
