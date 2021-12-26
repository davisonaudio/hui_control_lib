#ifndef hui
#define hui

#if (ARDUINO >=100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class USB_HUI {
  public:
    //Initialising
    USB_HUI(byte MIDI_Channel); //DONE

    //Reading incoming MIDI
    bool read_MIDI();

    bool get_button_state(byte zone, byte port);


    //Variables holding states
    bool mute_states[8];
    bool solo_states[8];
    bool select_states[8];
    bool recarm_states[8];

    //Sending MIDI buttons
    void button_press(byte zone, byte port); //DONE
    void button_release(byte zone, byte port); //DONE

    void send_button(byte zone, byte port); //DONE
    
    void send_mute(byte channel); //DONE
    void send_solo(byte channel); //DONE
    void send_recarm(byte channel); //DONE
    void send_select(byte channel); //DONE

    void bank_up(); //DONE
    void bank_down(); //DONE
    void channel_up(); //DONE
    void channel_down(); //DONE


    //Sending rotaries
    void send_pan(byte channel, int delta);
    void send_jog(int delta);





  private:
    void HUI_sense(); //DONE
    void write_rotary(byte byte2, byte delta_val); //DONE
    void HUI_button(byte zone, byte port, byte press_rel); //DONE
    byte _MIDI_channel;
    bool button_states[0x1D][0x8];
    midiEventPacket_t MIDI_packets[4];



};


#endif
