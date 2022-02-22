#ifndef TRIGGER_h
#define TRIGGER_h

class TRIGGER {
  private:
    int pitch;
    bool active, trig;
    SEQUENCE seq;

  public:
    void init (int p) {
      active = false;
      pitch = p;
    }

    void release () {
      if (active) {
        active = false;
        usbMIDI.sendNoteOff(pitch, 0, 1);
      }
    }

    void next (bool t) {
      trig = t;
      release();
    }

    void trigger (bool t) {
      if (t) {
        active = true;
        trig = false;
        usbMIDI.sendNoteOn(pitch, 100, 1);
      }
    }
};

#endif
