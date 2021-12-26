#include "USB_HUI.h"

//CLASS INITIATION
USB_HUI::USB_HUI(byte MIDI_channel = 1) {
  //Convert MIDI channel from 1-16 to 0-15
  _MIDI_channel = MIDI_channel - 1;
}


//MIDI Read - returns true if changes
bool USB_HUI::read_MIDI() {
  MIDI_packets[0] = MidiUSB.read();

  //check for correct MIDI channel
  if ((MIDI_packets[0].byte1 && 0x0F) != MIDI_channel) {
    return;
  }

  //Check for HUI sensing message
  if (MIDI_packets[0].byte1 == 0x90 && e.byte2 == 0x00 && e.byte3 == 0x00) {
    HUI_sense();
    return;
  }
  else if (e.header == 0x0A && e.byte1 == 0xA0 && e.byte2 < 0x08 && (e.byte3 & 0x0F)<0x0D && ((e.byte3 & 0xF0)>>4)<2){
    int meter_channel = e.byte2;
    int meter_side = ((e.byte3 & 0xF0)>>4);
    int meter_level = (e.byte3 & 0x0F);





}

//BUTTON SENDING
void USB_HUI::button_press(byte zone, byte port) {
  HUI_button(byte zone, port, 0)
}
void USB_HUI::button_release(byte zone, byte port) {
  HUI_button(zone, port, 1)
}

void USB_HUI::send_button(byte zone, byte port) {
  button_press(zone, port);
  button_release(zone, port);
}

void USB_HUI::send_mute(byte channel) {
  channel = channel - 1; //Convert 1-8 to 0-7
  if (channel < 8) { //Check in range
    send_button(channel, 2);
  }
}
void USB_HUI::send_solo(byte channel) {
  channel = channel - 1; //Convert 1-8 to 0-7
  if (channel < 8) { //Check in range
    send_button(channel, 3);
  }
}
void USB_HUI::send_recarm(byte channel) {
  channel = channel - 1; //Convert 1-8 to 0-7
  if (channel < 8) { //Check in range
    send_button(channel, 7);
  }
}
void USB_HUI::send_select(byte channel) {
  channel = channel - 1; //Convert 1-8 to 0-7
  if (channel < 8) { //Check in range
    send_button(channel, 1);
  }
}

void USB_HUI::bank_up() {
  send_button(0x0A, 3);
}
void USB_HUI::bank_down() {
  send_button(0x0A, 1);
}
void USB_HUI::channel_up() {
  send_button(0x0A, 2);
}
void USB_HUI::channel_down() {
  send_button(0x0A, 0);
}



//PRIVATE FUNCTIONS

void USB_HUI::HUI_sense() {
  MidiUSB.sendMIDI({0x09, 0x90 | _MIDI_channel, 0, 0x7F});
  MidiUSB.flush();
}

void USB_HUI::write_rotary(byte byte2, byte delta_val) {
  int midi_rot;
  if (delta_val == 0) {
    return;
  }
  else if (delta_val > 0) {
    midi_rot = 0x40 + delta_val;
  }
  else if (delta_val < 0) {
    midi_rot = 0 - delta_val;
  }
  MidiUSB.sendMIDI({0x0B, 0xB0 | _MIDI_channel, byte2, midi_rot});
  MidiUSB.flush();
}


void USB_HUI::HUI_button(byte zone, byte port, byte press_rel) {
  int num_rel;
  if (press_release == 0) {
    num_rel = 0x40;
  }
  else {
    num_rel = 0;
  }
  MidiUSB.sendMIDI({0x0B, 0xB0 | _MIDI_channel, 0x0F, zone});
  MidiUSB.flush();
  MidiUSB.sendMIDI({0x0B, 0xB0 | _MIDI_channel, 0x2F, num_rel + port});
  MidiUSB.flush();
}
